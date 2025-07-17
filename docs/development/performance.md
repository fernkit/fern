# Performance Guide

This guide covers performance optimization techniques, profiling, and best practices for developing efficient applications with Fern UI.

## Table of Contents
- [Performance Principles](#performance-principles)
- [Rendering Performance](#rendering-performance)
- [Memory Management](#memory-management)
- [Input Handling Performance](#input-handling-performance)
- [Widget Optimization](#widget-optimization)
- [Layout Performance](#layout-performance)
- [Profiling and Debugging](#profiling-and-debugging)
- [Platform-Specific Optimizations](#platform-specific-optimizations)
- [Best Practices](#best-practices)

## Performance Principles

### Core Performance Goals
- **60 FPS rendering**: Target 16.67ms per frame
- **Low memory footprint**: Minimize allocations and fragmentation
- **Responsive input**: Handle user interactions within 100ms
- **Efficient layouts**: Minimize layout recalculations
- **Smart redraws**: Only render what has changed

### Performance Mindset
```cpp
// ❌ Avoid: Creating objects in render loops
void MyWidget::render() {
    for (int i = 0; i < items_.size(); ++i) {
        std::string text = "Item " + std::to_string(i); // Allocation in hot path
        drawText(text, x, y + i * 20);
    }
}

// ✅ Better: Pre-compute or cache expensive operations
void MyWidget::updateItems() {
    itemTexts_.clear();
    for (int i = 0; i < items_.size(); ++i) {
        itemTexts_.push_back("Item " + std::to_string(i));
    }
}

void MyWidget::render() {
    for (size_t i = 0; i < itemTexts_.size(); ++i) {
        drawText(itemTexts_[i], x, y + i * 20);
    }
}
```

## Rendering Performance

### Minimize Draw Calls
```cpp
// ❌ Multiple individual draw calls
void render() {
    for (auto& widget : widgets_) {
        renderer->setColor(widget->getColor());
        renderer->drawRect(widget->getBounds());
    }
}

// ✅ Batch similar operations
void render() {
    // Group widgets by color and draw in batches
    std::map<uint32_t, std::vector<Rect>> colorGroups;
    for (auto& widget : widgets_) {
        colorGroups[widget->getColor()].push_back(widget->getBounds());
    }
    
    for (auto& [color, rects] : colorGroups) {
        renderer->setColor(color);
        renderer->drawRects(rects); // Batch draw
    }
}
```

### Dirty Rectangle Optimization
```cpp
class OptimizedCanvas {
private:
    std::vector<Rect> dirtyRegions_;
    bool needsFullRedraw_ = true;
    
public:
    void markDirty(const Rect& region) {
        if (needsFullRedraw_) return;
        
        // Merge overlapping dirty regions
        bool merged = false;
        for (auto& dirty : dirtyRegions_) {
            if (dirty.intersects(region)) {
                dirty = dirty.merge(region);
                merged = true;
                break;
            }
        }
        
        if (!merged) {
            dirtyRegions_.push_back(region);
        }
    }
    
    void render() {
        if (needsFullRedraw_) {
            renderFullScreen();
            needsFullRedraw_ = false;
        } else {
            for (const auto& region : dirtyRegions_) {
                renderRegion(region);
            }
        }
        dirtyRegions_.clear();
    }
};
```

### Text Rendering Optimization
```cpp
class TextCache {
private:
    struct CachedText {
        std::string text;
        uint32_t color;
        int size;
        TextureID textureId;
        int width, height;
    };
    
    std::unordered_map<std::string, CachedText> cache_;
    static constexpr size_t MAX_CACHE_SIZE = 256;
    
public:
    TextureID getTextTexture(const std::string& text, uint32_t color, int size) {
        std::string key = text + "|" + std::to_string(color) + "|" + std::to_string(size);
        
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second.textureId; // Cache hit
        }
        
        // Cache miss - render text to texture
        auto texture = renderTextToTexture(text, color, size);
        
        if (cache_.size() >= MAX_CACHE_SIZE) {
            evictLRU(); // Remove least recently used
        }
        
        cache_[key] = {text, color, size, texture, /* width/height */};
        return texture;
    }
};
```

## Memory Management

### Smart Pointer Usage
```cpp
// ✅ Use appropriate smart pointer types
class WidgetManager {
private:
    std::vector<std::shared_ptr<Widget>> widgets_;        // Shared ownership
    std::vector<std::weak_ptr<Widget>> weakReferences_;   // Non-owning references
    std::unique_ptr<Renderer> renderer_;                  // Exclusive ownership
    
public:
    void addWidget(std::shared_ptr<Widget> widget) {
        widgets_.push_back(widget);
    }
    
    void addWeakReference(std::weak_ptr<Widget> widget) {
        weakReferences_.push_back(widget);
    }
    
    void cleanupExpiredReferences() {
        weakReferences_.erase(
            std::remove_if(weakReferences_.begin(), weakReferences_.end(),
                [](const std::weak_ptr<Widget>& w) { return w.expired(); }),
            weakReferences_.end()
        );
    }
};
```

### Memory Pool for Frequent Allocations
```cpp
template<typename T, size_t PoolSize = 1024>
class ObjectPool {
private:
    std::array<T, PoolSize> pool_;
    std::vector<size_t> freeIndices_;
    size_t nextIndex_ = 0;
    
public:
    ObjectPool() {
        for (size_t i = 0; i < PoolSize; ++i) {
            freeIndices_.push_back(i);
        }
    }
    
    T* acquire() {
        if (freeIndices_.empty()) {
            return nullptr; // Pool exhausted
        }
        
        size_t index = freeIndices_.back();
        freeIndices_.pop_back();
        return &pool_[index];
    }
    
    void release(T* obj) {
        size_t index = obj - &pool_[0];
        if (index < PoolSize) {
            freeIndices_.push_back(index);
        }
    }
};

// Usage for frequent event objects
static ObjectPool<InputEvent> inputEventPool;
```

### RAII and Resource Management
```cpp
class ScopedRenderer {
private:
    Renderer& renderer_;
    uint32_t previousColor_;
    
public:
    ScopedRenderer(Renderer& renderer, uint32_t color) 
        : renderer_(renderer), previousColor_(renderer.getColor()) {
        renderer_.setColor(color);
    }
    
    ~ScopedRenderer() {
        renderer_.setColor(previousColor_); // Automatic cleanup
    }
};

// Usage
void renderColoredSection() {
    ScopedRenderer sr(renderer, Colors::Red);
    // Render red content
    // Color automatically restored when scope ends
}
```

## Input Handling Performance

### Efficient Event Propagation
```cpp
class InputManager {
private:
    std::vector<std::shared_ptr<Widget>> inputWidgets_;
    
public:
    bool handleInput(const InputState& input) {
        // Process in reverse order (top to bottom)
        for (auto it = inputWidgets_.rbegin(); it != inputWidgets_.rend(); ++it) {
            if ((*it)->isVisible() && (*it)->containsPoint(input.mouseX, input.mouseY)) {
                if ((*it)->handleInput(input)) {
                    return true; // Early exit - input consumed
                }
            }
        }
        return false;
    }
};
```

### Input Event Batching
```cpp
class InputBatcher {
private:
    std::vector<InputEvent> eventQueue_;
    std::chrono::milliseconds batchInterval_{16}; // ~60 FPS
    
public:
    void addEvent(const InputEvent& event) {
        eventQueue_.push_back(event);
    }
    
    void processBatch() {
        if (eventQueue_.empty()) return;
        
        // Combine similar events (e.g., mouse moves)
        auto combinedEvents = combineEvents(eventQueue_);
        
        for (const auto& event : combinedEvents) {
            dispatchEvent(event);
        }
        
        eventQueue_.clear();
    }
};
```

## Widget Optimization

### Lazy Widget Creation
```cpp
class LazyContainer {
private:
    std::function<std::shared_ptr<Widget>()> widgetFactory_;
    std::shared_ptr<Widget> widget_;
    bool isVisible_ = false;
    
public:
    LazyContainer(std::function<std::shared_ptr<Widget>()> factory)
        : widgetFactory_(factory) {}
    
    void setVisible(bool visible) {
        if (visible && !widget_) {
            widget_ = widgetFactory_(); // Create only when needed
        }
        isVisible_ = visible;
    }
    
    void render() override {
        if (isVisible_ && widget_) {
            widget_->render();
        }
    }
};
```

### Widget State Caching
```cpp
class CachedWidget : public Widget {
private:
    bool isDirty_ = true;
    Rect lastBounds_;
    uint32_t lastColor_;
    
public:
    void render() override {
        if (isDirty_ || boundsChanged() || colorChanged()) {
            renderInternal();
            updateCache();
            isDirty_ = false;
        } else {
            renderFromCache(); // Use cached rendering
        }
    }
    
    void markDirty() {
        isDirty_ = true;
    }
};
```

## Layout Performance

### Layout Caching and Invalidation
```cpp
class PerformantLayout {
private:
    bool layoutValid_ = false;
    Rect lastBounds_;
    std::vector<Rect> childBounds_;
    
public:
    void arrangeChildren() override {
        if (layoutValid_ && bounds_ == lastBounds_) {
            applyChildBounds(); // Use cached layout
            return;
        }
        
        // Recalculate layout
        calculateLayout();
        layoutValid_ = true;
        lastBounds_ = bounds_;
    }
    
    void invalidateLayout() {
        layoutValid_ = false;
        // Propagate to parent
        if (auto parent = getParent()) {
            parent->invalidateLayout();
        }
    }
};
```

### Efficient Flex Layout
```cpp
class FlexLayout {
public:
    struct FlexChild {
        std::shared_ptr<Widget> widget;
        int flex = 0;
        int minSize = 0;
        int maxSize = INT_MAX;
    };
    
private:
    void distributeSpace(const std::vector<FlexChild>& children, int availableSpace) {
        // Calculate total flex weight
        int totalFlex = 0;
        int fixedSpace = 0;
        
        for (const auto& child : children) {
            if (child.flex == 0) {
                fixedSpace += child.widget->getPreferredSize();
            } else {
                totalFlex += child.flex;
            }
        }
        
        int flexSpace = availableSpace - fixedSpace;
        if (flexSpace <= 0 || totalFlex == 0) return;
        
        // Distribute proportionally
        for (const auto& child : children) {
            if (child.flex > 0) {
                int size = (flexSpace * child.flex) / totalFlex;
                size = std::clamp(size, child.minSize, child.maxSize);
                child.widget->resize(size);
            }
        }
    }
};
```

## Profiling and Debugging

### Performance Profiler
```cpp
class Profiler {
public:
    struct Timer {
        std::chrono::high_resolution_clock::time_point start;
        std::string name;
        
        Timer(const std::string& n) : name(n) {
            start = std::chrono::high_resolution_clock::now();
        }
        
        ~Timer() {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            Profiler::getInstance().recordTime(name, duration.count());
        }
    };
    
    static Profiler& getInstance() {
        static Profiler instance;
        return instance;
    }
    
    void recordTime(const std::string& name, long microseconds) {
        times_[name].push_back(microseconds);
        if (times_[name].size() > 100) {
            times_[name].erase(times_[name].begin());
        }
    }
    
    void printStats() {
        for (const auto& [name, times] : times_) {
            if (times.empty()) continue;
            
            long total = std::accumulate(times.begin(), times.end(), 0L);
            long avg = total / times.size();
            long max = *std::max_element(times.begin(), times.end());
            
            std::cout << name << ": avg=" << avg << "μs, max=" << max << "μs\n";
        }
    }
    
private:
    std::unordered_map<std::string, std::vector<long>> times_;
};

#define PROFILE(name) Profiler::Timer timer(name)

// Usage
void Widget::render() {
    PROFILE("Widget::render");
    // rendering code...
}
```

### Memory Usage Tracking
```cpp
class MemoryTracker {
public:
    static void* allocate(size_t size, const char* file, int line) {
        void* ptr = malloc(size);
        
        std::lock_guard<std::mutex> lock(mutex_);
        allocations_[ptr] = {size, file, line};
        totalAllocated_ += size;
        
        return ptr;
    }
    
    static void deallocate(void* ptr) {
        if (!ptr) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = allocations_.find(ptr);
        if (it != allocations_.end()) {
            totalAllocated_ -= it->second.size;
            allocations_.erase(it);
        }
        
        free(ptr);
    }
    
    static void printReport() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Total allocated: " << totalAllocated_ << " bytes\n";
        std::cout << "Active allocations: " << allocations_.size() << "\n";
        
        if (!allocations_.empty()) {
            std::cout << "Leaked allocations:\n";
            for (const auto& [ptr, info] : allocations_) {
                std::cout << "  " << info.size << " bytes at " 
                         << info.file << ":" << info.line << "\n";
            }
        }
    }
    
private:
    struct AllocationInfo {
        size_t size;
        const char* file;
        int line;
    };
    
    static std::unordered_map<void*, AllocationInfo> allocations_;
    static size_t totalAllocated_;
    static std::mutex mutex_;
};
```

## Platform-Specific Optimizations

### Linux Optimizations
```cpp
// Use memory mapping for large resources
class LinuxResourceLoader {
public:
    static std::unique_ptr<uint8_t[]> loadFile(const std::string& path) {
        int fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) return nullptr;
        
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            close(fd);
            return nullptr;
        }
        
        void* mapped = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
        
        if (mapped == MAP_FAILED) return nullptr;
        
        auto data = std::make_unique<uint8_t[]>(sb.st_size);
        std::memcpy(data.get(), mapped, sb.st_size);
        munmap(mapped, sb.st_size);
        
        return data;
    }
};
```

### WebAssembly Optimizations
```cpp
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

class WebOptimizations {
public:
    // Use requestAnimationFrame for smooth rendering
    static void scheduleFrame() {
        emscripten_request_animation_frame_loop([](double time, void* userData) -> EM_BOOL {
            static_cast<Application*>(userData)->renderFrame();
            return EM_TRUE;
        }, getApplication());
    }
    
    // Minimize memory allocations
    static void optimizeForWeb() {
        // Disable features that don't work well in browser
        // Use simpler rendering paths
        // Reduce texture sizes
    }
};
#endif
```

## Best Practices

### Performance Checklist
- [ ] **Minimize allocations in hot paths** (render, input handling)
- [ ] **Use object pooling** for frequently created/destroyed objects
- [ ] **Cache expensive calculations** (text metrics, layout)
- [ ] **Batch similar operations** (draw calls, updates)
- [ ] **Implement dirty tracking** to avoid unnecessary work
- [ ] **Profile regularly** and measure actual performance
- [ ] **Test on target hardware** with realistic data sets
- [ ] **Use RAII** for automatic resource management
- [ ] **Prefer stack allocation** over heap when possible
- [ ] **Implement level-of-detail** for complex widgets

### Code Organization for Performance
```cpp
// Separate hot and cold paths
class Widget {
public:
    // Hot path - called every frame
    void render() {
        if (!visible_) return;
        renderImpl();
    }
    
    // Cold path - called infrequently
    void updateStyle(const Style& style) {
        style_ = style;
        invalidate();
    }
    
private:
    // Keep frequently accessed members together
    bool visible_ = true;
    bool dirty_ = true;
    Rect bounds_;
    
    // Less frequently accessed members
    Style style_;
    std::string id_;
};
```

### Performance Testing
```cpp
class PerformanceTest {
public:
    static void runWidgetPerformanceTest() {
        const int NUM_WIDGETS = 1000;
        const int NUM_FRAMES = 1000;
        
        // Setup
        std::vector<std::shared_ptr<Widget>> widgets;
        for (int i = 0; i < NUM_WIDGETS; ++i) {
            widgets.push_back(createRandomWidget());
        }
        
        // Measure rendering performance
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int frame = 0; frame < NUM_FRAMES; ++frame) {
            for (auto& widget : widgets) {
                widget->render();
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        double avgFrameTime = static_cast<double>(totalTime.count()) / NUM_FRAMES;
        double fps = 1000.0 / avgFrameTime;
        
        std::cout << "Average frame time: " << avgFrameTime << "ms\n";
        std::cout << "Approximate FPS: " << fps << "\n";
        
        // Verify performance targets
        assert(avgFrameTime < 16.67); // 60 FPS target
    }
};
```

## Conclusion

Performance optimization in Fern UI requires a systematic approach:

1. **Measure first** - Profile before optimizing
2. **Focus on hot paths** - Optimize the most frequently executed code
3. **Think about cache** - Both CPU cache and application-level caching
4. **Manage memory carefully** - Use appropriate smart pointers and pools
5. **Test on real hardware** - Development machines may not reflect user experience

Remember that premature optimization is often counterproductive. Focus on writing clean, maintainable code first, then optimize based on actual performance measurements and user requirements.

For specific performance issues or questions, refer to the [Contributing Guide](contributing.md) for how to report performance bugs or submit optimization patches.
