# Event System

The Fern Event System provides a high-level abstraction for handling user interactions and system events. It builds on top of the low-level input handling system to provide a more convenient and structured way to respond to events.

## Overview

The event system includes:
- **Event Types**: Different categories of events (mouse, keyboard, system)
- **Event Handlers**: Functions that respond to specific events
- **Event Propagation**: How events flow through the widget hierarchy
- **Event Filtering**: Selective event processing
- **Custom Events**: User-defined event types

## Event Types

### Mouse Events
```cpp
enum class MouseEvent {
    Click,
    DoubleClick,
    Press,
    Release,
    Move,
    Enter,
    Leave,
    Wheel
};
```

### Keyboard Events
```cpp
enum class KeyboardEvent {
    KeyPress,
    KeyRelease,
    KeyHeld,
    TextInput
};
```

### System Events
```cpp
enum class SystemEvent {
    WindowResize,
    WindowClose,
    WindowMinimize,
    WindowMaximize,
    WindowFocus,
    WindowBlur
};
```

### Widget Events
```cpp
enum class WidgetEvent {
    Focus,
    Blur,
    Show,
    Hide,
    Move,
    Resize,
    ValueChanged
};
```

## Event Handling

### Basic Event Handling
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Event System Example");
    
    // Create a button with event handlers
    auto button = Button("Click Me!", 100, 100, 120, 40);
    
    // Handle click events
    button->onClick([](){ 
        printf("Button clicked!\\n"); 
    });
    
    // Handle hover events
    button->onHover([](bool isHovering){ 
        if (isHovering) {
            printf("Mouse entered button\\n");
        } else {
            printf("Mouse left button\\n");
        }
    });
    
    addWidget(button);
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Event Handler Registration
```cpp
class EventHandler {
public:
    // Register event handlers
    void registerMouseHandler(std::function<void(MouseEvent, int, int)> handler) {
        mouseHandler_ = handler;
    }
    
    void registerKeyboardHandler(std::function<void(KeyboardEvent, KeyCode)> handler) {
        keyboardHandler_ = handler;
    }
    
    void registerSystemHandler(std::function<void(SystemEvent)> handler) {
        systemHandler_ = handler;
    }
    
    // Process events
    void processMouseEvent(MouseEvent event, int x, int y) {
        if (mouseHandler_) {
            mouseHandler_(event, x, y);
        }
    }
    
    void processKeyboardEvent(KeyboardEvent event, KeyCode key) {
        if (keyboardHandler_) {
            keyboardHandler_(event, key);
        }
    }
    
    void processSystemEvent(SystemEvent event) {
        if (systemHandler_) {
            systemHandler_(event);
        }
    }
    
private:
    std::function<void(MouseEvent, int, int)> mouseHandler_;
    std::function<void(KeyboardEvent, KeyCode)> keyboardHandler_;
    std::function<void(SystemEvent)> systemHandler_;
};
```

## Event Propagation

### Widget Hierarchy Events
```cpp
class EventPropagator {
public:
    // Event propagation through widget hierarchy
    bool propagateMouseEvent(std::shared_ptr<Widget> widget, MouseEvent event, int x, int y) {
        // Check if event is within widget bounds
        if (x >= widget->getX() && x <= widget->getX() + widget->getWidth() &&
            y >= widget->getY() && y <= widget->getY() + widget->getHeight()) {
            
            // Try children first (top-down)
            for (auto& child : widget->getChildren()) {
                if (propagateMouseEvent(child, event, x, y)) {
                    return true; // Event consumed by child
                }
            }
            
            // If not consumed by children, try the widget itself
            return widget->handleMouseEvent(event, x, y);
        }
        
        return false;
    }
    
    bool propagateKeyboardEvent(std::shared_ptr<Widget> focusedWidget, KeyboardEvent event, KeyCode key) {
        if (focusedWidget) {
            return focusedWidget->handleKeyboardEvent(event, key);
        }
        return false;
    }
};
```

### Event Bubbling
```cpp
class EventBubbler {
public:
    // Bubble event up the widget hierarchy
    bool bubbleEvent(std::shared_ptr<Widget> widget, const Event& event) {
        std::shared_ptr<Widget> current = widget;
        
        while (current) {
            if (current->handleEvent(event)) {
                return true; // Event consumed
            }
            
            current = current->getParent();
        }
        
        return false; // Event not consumed
    }
    
    // Stop event propagation
    void stopPropagation(Event& event) {
        event.stopPropagation = true;
    }
    
    // Prevent default action
    void preventDefault(Event& event) {
        event.preventDefault = true;
    }
};
```

## Custom Events

### Defining Custom Events
```cpp
// Custom event structure
struct CustomEvent {
    enum Type {
        UserLogin,
        UserLogout,
        DataLoaded,
        NetworkError,
        AnimationComplete
    };
    
    Type type;
    std::any data;
    uint32_t timestamp;
    bool stopPropagation = false;
    bool preventDefault = false;
};

// Custom event dispatcher
class EventDispatcher {
private:
    std::unordered_map<CustomEvent::Type, std::vector<std::function<void(const CustomEvent&)>>> handlers_;
    
public:
    void addEventListener(CustomEvent::Type type, std::function<void(const CustomEvent&)> handler) {
        handlers_[type].push_back(handler);
    }
    
    void dispatchEvent(const CustomEvent& event) {
        auto it = handlers_.find(event.type);
        if (it != handlers_.end()) {
            for (const auto& handler : it->second) {
                handler(event);
                if (event.stopPropagation) break;
            }
        }
    }
    
    void removeEventListener(CustomEvent::Type type) {
        handlers_.erase(type);
    }
};
```

### Using Custom Events
```cpp
// Create event dispatcher
EventDispatcher dispatcher;

// Register event handlers
dispatcher.addEventListener(CustomEvent::UserLogin, [](const CustomEvent& event) {
    std::string username = std::any_cast<std::string>(event.data);
    printf("User logged in: %s\\n", username.c_str());
});

dispatcher.addEventListener(CustomEvent::DataLoaded, [](const CustomEvent& event) {
    int itemCount = std::any_cast<int>(event.data);
    printf("Data loaded: %d items\\n", itemCount);
});

// Dispatch events
void userLogin(const std::string& username) {
    CustomEvent event;
    event.type = CustomEvent::UserLogin;
    event.data = username;
    event.timestamp = getCurrentTime();
    
    dispatcher.dispatchEvent(event);
}

void dataLoaded(int itemCount) {
    CustomEvent event;
    event.type = CustomEvent::DataLoaded;
    event.data = itemCount;
    event.timestamp = getCurrentTime();
    
    dispatcher.dispatchEvent(event);
}
```

## Event Filtering

### Event Filters
```cpp
class EventFilter {
public:
    virtual bool filter(const Event& event) = 0;
    virtual ~EventFilter() = default;
};

// Mouse event filter
class MouseEventFilter : public EventFilter {
private:
    int minX_, minY_, maxX_, maxY_;
    
public:
    MouseEventFilter(int minX, int minY, int maxX, int maxY) 
        : minX_(minX), minY_(minY), maxX_(maxX), maxY_(maxY) {}
    
    bool filter(const Event& event) override {
        if (event.type == Event::Mouse) {
            const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(event);
            return (mouseEvent.x >= minX_ && mouseEvent.x <= maxX_ &&
                    mouseEvent.y >= minY_ && mouseEvent.y <= maxY_);
        }
        return true;
    }
};

// Keyboard event filter
class KeyboardEventFilter : public EventFilter {
private:
    std::set<KeyCode> allowedKeys_;
    
public:
    KeyboardEventFilter(const std::set<KeyCode>& allowedKeys) 
        : allowedKeys_(allowedKeys) {}
    
    bool filter(const Event& event) override {
        if (event.type == Event::Keyboard) {
            const KeyboardEvent& keyEvent = static_cast<const KeyboardEvent&>(event);
            return allowedKeys_.find(keyEvent.key) != allowedKeys_.end();
        }
        return true;
    }
};
```

### Using Event Filters
```cpp
// Create event manager with filters
class FilteredEventManager {
private:
    std::vector<std::unique_ptr<EventFilter>> filters_;
    
public:
    void addFilter(std::unique_ptr<EventFilter> filter) {
        filters_.push_back(std::move(filter));
    }
    
    bool processEvent(const Event& event) {
        // Apply all filters
        for (const auto& filter : filters_) {
            if (!filter->filter(event)) {
                return false; // Event filtered out
            }
        }
        
        // Process event if it passes all filters
        return handleEvent(event);
    }
    
private:
    bool handleEvent(const Event& event) {
        // Handle the event
        return true;
    }
};

// Usage
FilteredEventManager eventManager;

// Add filters
eventManager.addFilter(std::make_unique<MouseEventFilter>(0, 0, 800, 600));
eventManager.addFilter(std::make_unique<KeyboardEventFilter>(
    std::set<KeyCode>{KeyCode::A, KeyCode::B, KeyCode::C}
));
```

## Advanced Event Handling

### Event Queuing
```cpp
class EventQueue {
private:
    std::queue<Event> eventQueue_;
    std::mutex queueMutex_;
    
public:
    void enqueue(const Event& event) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        eventQueue_.push(event);
    }
    
    bool dequeue(Event& event) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        if (eventQueue_.empty()) return false;
        
        event = eventQueue_.front();
        eventQueue_.pop();
        return true;
    }
    
    void processEvents() {
        Event event;
        while (dequeue(event)) {
            handleEvent(event);
        }
    }
    
private:
    void handleEvent(const Event& event) {
        // Process the event
    }
};
```

### Event Timing
```cpp
class TimedEventHandler {
private:
    struct TimedEvent {
        Event event;
        uint32_t executeTime;
    };
    
    std::priority_queue<TimedEvent, std::vector<TimedEvent>, 
                       std::function<bool(const TimedEvent&, const TimedEvent&)>> timedEvents_;
    
public:
    TimedEventHandler() : timedEvents_([](const TimedEvent& a, const TimedEvent& b) {
        return a.executeTime > b.executeTime;
    }) {}
    
    void scheduleEvent(const Event& event, uint32_t delay) {
        TimedEvent timedEvent;
        timedEvent.event = event;
        timedEvent.executeTime = getCurrentTime() + delay;
        
        timedEvents_.push(timedEvent);
    }
    
    void processTimedEvents() {
        uint32_t currentTime = getCurrentTime();
        
        while (!timedEvents_.empty() && timedEvents_.top().executeTime <= currentTime) {
            const TimedEvent& timedEvent = timedEvents_.top();
            handleEvent(timedEvent.event);
            timedEvents_.pop();
        }
    }
};
```

## Event System Integration

### Widget Event Integration
```cpp
class EventAwareWidget : public Widget {
protected:
    EventDispatcher eventDispatcher_;
    
public:
    // Add event listener
    void addEventListener(const std::string& eventType, std::function<void(const Event&)> handler) {
        eventDispatcher_.addEventListener(eventType, handler);
    }
    
    // Remove event listener
    void removeEventListener(const std::string& eventType) {
        eventDispatcher_.removeEventListener(eventType);
    }
    
    // Dispatch event
    void dispatchEvent(const std::string& eventType, const Event& event) {
        eventDispatcher_.dispatchEvent(eventType, event);
    }
    
    // Handle input
    bool handleInput(const InputState& input) override {
        // Convert input to events
        if (input.mouseClicked) {
            MouseEvent mouseEvent;
            mouseEvent.type = MouseEvent::Click;
            mouseEvent.x = input.mouseX;
            mouseEvent.y = input.mouseY;
            
            dispatchEvent("mouseClick", mouseEvent);
        }
        
        if (input.keyPressed) {
            KeyboardEvent keyEvent;
            keyEvent.type = KeyboardEvent::KeyPress;
            keyEvent.key = input.lastKeyPressed;
            
            dispatchEvent("keyPress", keyEvent);
        }
        
        return Widget::handleInput(input);
    }
};
```

## Performance Considerations

### Event Pooling
```cpp
class EventPool {
private:
    std::vector<std::unique_ptr<Event>> pool_;
    std::mutex poolMutex_;
    
public:
    std::unique_ptr<Event> acquireEvent() {
        std::lock_guard<std::mutex> lock(poolMutex_);
        
        if (!pool_.empty()) {
            auto event = std::move(pool_.back());
            pool_.pop_back();
            return event;
        }
        
        return std::make_unique<Event>();
    }
    
    void releaseEvent(std::unique_ptr<Event> event) {
        std::lock_guard<std::mutex> lock(poolMutex_);
        
        // Reset event state
        event->reset();
        pool_.push_back(std::move(event));
    }
};
```

### Event Batching
```cpp
class EventBatcher {
private:
    std::vector<Event> batch_;
    uint32_t lastBatchTime_;
    uint32_t batchInterval_;
    
public:
    EventBatcher(uint32_t interval = 16) : batchInterval_(interval), lastBatchTime_(0) {}
    
    void addEvent(const Event& event) {
        batch_.push_back(event);
        
        uint32_t currentTime = getCurrentTime();
        if (currentTime - lastBatchTime_ >= batchInterval_) {
            processBatch();
            lastBatchTime_ = currentTime;
        }
    }
    
private:
    void processBatch() {
        // Process all events in the batch
        for (const auto& event : batch_) {
            handleEvent(event);
        }
        
        batch_.clear();
    }
};
```

## Best Practices

### 1. Event Handler Organization
```cpp
class OrganizedEventHandler {
private:
    std::unordered_map<std::string, std::vector<std::function<void(const Event&)>>> eventHandlers_;
    
public:
    void on(const std::string& eventType, std::function<void(const Event&)> handler) {
        eventHandlers_[eventType].push_back(handler);
    }
    
    void off(const std::string& eventType) {
        eventHandlers_.erase(eventType);
    }
    
    void emit(const std::string& eventType, const Event& event) {
        auto it = eventHandlers_.find(eventType);
        if (it != eventHandlers_.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }
};
```

### 2. Event Debugging
```cpp
class EventDebugger {
private:
    bool debugEnabled_;
    std::ofstream logFile_;
    
public:
    EventDebugger(bool enabled = false) : debugEnabled_(enabled) {
        if (debugEnabled_) {
            logFile_.open("event_log.txt");
        }
    }
    
    void logEvent(const Event& event) {
        if (debugEnabled_) {
            logFile_ << "[" << getCurrentTime() << "] Event: " 
                     << event.toString() << std::endl;
        }
    }
};
```

### 3. Memory Management
```cpp
// Use RAII for event handlers
class ScopedEventHandler {
private:
    EventDispatcher* dispatcher_;
    std::string eventType_;
    
public:
    ScopedEventHandler(EventDispatcher* dispatcher, const std::string& eventType, 
                       std::function<void(const Event&)> handler)
        : dispatcher_(dispatcher), eventType_(eventType) {
        dispatcher_->addEventListener(eventType, handler);
    }
    
    ~ScopedEventHandler() {
        dispatcher_->removeEventListener(eventType_);
    }
};
```

## Related Components

- [Input Handling](input-handling.md) - Low-level input processing
- [Signal/Slot System](signals.md) - Widget communication system
- [Widget System](../widgets/) - Widget-level event handling
- [Button Widget](../widgets/button.md) - Example of event-driven widget

## Tips

1. **Use appropriate event types**: Choose the right granularity for your events
2. **Avoid event loops**: Be careful with event handlers that trigger other events
3. **Clean up handlers**: Remove event listeners when widgets are destroyed
4. **Consider performance**: Use event batching for high-frequency events
5. **Debug events**: Use logging to track event flow during development
6. **Handle edge cases**: Consider what happens when events fire rapidly or out of order

The Event System provides a powerful foundation for creating responsive and interactive user interfaces in Fern applications.
