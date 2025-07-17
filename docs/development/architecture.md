# Fern UI Framework Architecture

## Overview

This document provides a comprehensive overview of the Fern UI Framework architecture, design principles, and internal workings. Understanding this architecture will help developers contribute effectively and make informed decisions when building applications.

## Table of Contents

1. [Core Architecture](#core-architecture)
2. [Component Design](#component-design)
3. [Rendering System](#rendering-system)
4. [Event System](#event-system)
5. [Layout System](#layout-system)
6. [Cross-Platform Abstraction](#cross-platform-abstraction)
7. [Memory Management](#memory-management)
8. [Performance Considerations](#performance-considerations)
9. [Design Patterns](#design-patterns)
10. [Future Architecture](#future-architecture)

## Core Architecture

### High-Level Overview

```
┌─────────────────────────────────────────────────────────┐
│                 Application Layer                       │
├─────────────────────────────────────────────────────────┤
│                   Widget System                         │
│  ┌───────────────┐ ┌───────────────┐ ┌───────────────┐  │
│  │    Widgets    │ │    Layouts    │ │   Containers  │  │
│  └───────────────┘ └───────────────┘ └───────────────┘  │
├─────────────────────────────────────────────────────────┤
│                    Core Systems                         │
│  ┌───────────────┐ ┌───────────────┐ ┌───────────────┐  │
│  │     Canvas    │ │     Input     │ │    Signals    │  │
│  └───────────────┘ └───────────────┘ └───────────────┘  │
├─────────────────────────────────────────────────────────┤
│                 Platform Abstraction                    │
│  ┌───────────────┐ ┌───────────────┐ ┌───────────────┐  │
│  │ Linux Renderer│ │  Web Renderer │ │ Future Platfos│  │
│  └───────────────┘ └───────────────┘ └───────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### Directory Structure

```
src/cpp/
├── include/fern/
│   ├── fern.hpp                 # Main framework header
│   ├── core/                    # Core framework components
│   │   ├── canvas.hpp           # Drawing abstraction
│   │   ├── input.hpp            # Input event structures
│   │   ├── signal.hpp           # Event system
│   │   ├── types.hpp            # Common types
│   │   └── widget_manager.hpp   # Widget lifecycle
│   ├── font/                    # Font rendering system
│   │   ├── font.hpp             # Font abstractions
│   │   ├── font_manager.hpp     # Font resource management
│   │   └── ttf_font_renderer.hpp # TTF rendering
│   ├── graphics/                # Graphics primitives
│   │   ├── colors.hpp           # Color utilities
│   │   └── primitives.hpp       # Drawing primitives
│   ├── platform/                # Platform abstraction
│   │   └── renderer.hpp         # Renderer interface
│   ├── text/                    # Text rendering system
│   │   └── font.hpp             # Text-specific font handling
│   └── ui/                      # UI components
│       ├── containers/          # Container widgets
│       │   └── container.hpp    # Base container
│       ├── layout/              # Layout management
│       │   └── layout.hpp       # Layout widgets
│       └── widgets/             # Individual widgets
│           ├── widget.hpp       # Base widget class
│           ├── button_widget.hpp
│           ├── text_widget.hpp
│           └── ...
└── src/                        # Implementation files
    ├── fern.cpp                # Main framework implementation
    ├── core/                   # Core implementations
    ├── font/                   # Font system implementations
    ├── graphics/               # Graphics implementations
    ├── platform/               # Platform-specific code
    ├── text/                   # Text rendering implementations
    └── ui/                     # UI implementations
```

## Component Design

### Widget System Architecture

#### Base Widget Class
```cpp
class Widget {
    // Core interface that all widgets implement
    virtual void render() = 0;
    virtual bool handleInput(const InputState& input) = 0;
    
    // Position and sizing
    virtual void setPosition(int x, int y);
    virtual void resize(int width, int height);
    
    // Virtual destructor for proper cleanup
    virtual ~Widget() = default;
};
```

#### Configuration Pattern
Every widget follows a consistent configuration pattern:

```cpp
// 1. Style Configuration Class
class WidgetStyle {
    // Fluent interface for styling
    WidgetStyle& color(uint32_t c) { /* ... */ return *this; }
    WidgetStyle& size(int s) { /* ... */ return *this; }
};

// 2. Configuration Class
class WidgetConfig {
    // Combines position, content, and style
    WidgetConfig(int x, int y, /* content params */);
    WidgetConfig& style(const WidgetStyle& s) { /* ... */ return *this; }
};

// 3. Widget Implementation
class ConcreteWidget : public Widget {
    ConcreteWidget(const WidgetConfig& config);
    void render() override;
    bool handleInput(const InputState& input) override;
};

// 4. Factory Function
std::shared_ptr<ConcreteWidget> CreateWidget(const WidgetConfig& config, bool addToManager = true);
```

### Widget Lifecycle

1. **Creation**: Widget constructed with configuration
2. **Registration**: Added to WidgetManager (optional)
3. **Layout**: Positioned by layout system (if applicable)
4. **Rendering**: render() called each frame
5. **Input**: handleInput() called for events
6. **Destruction**: Automatic cleanup via RAII

## Rendering System

### Canvas Abstraction

The Canvas class provides a unified drawing interface:

```cpp
class Canvas {
public:
    // Basic drawing operations
    static void fill(uint32_t color);
    static void drawRect(int x, int y, int w, int h, uint32_t color);
    static void drawCircle(int x, int y, int radius, uint32_t color);
    static void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    
    // Text rendering
    static void drawText(const std::string& text, int x, int y, uint32_t color);
    static void drawTTFText(const std::string& text, int x, int y, int size, uint32_t color);
    
    // Advanced operations
    static void drawCircleArc(int centerX, int centerY, int radius, 
                             float startAngle, float endAngle, uint32_t color);
};
```

### Platform Renderer Interface

```cpp
class PlatformRenderer {
public:
    virtual void initialize(int width, int height) = 0;
    virtual void present(uint32_t* pixelBuffer, int width, int height) = 0;
    virtual void setTitle(const std::string& title) = 0;
    virtual bool shouldClose() = 0;
    virtual void pollEvents() = 0;
    virtual void shutdown() = 0;
    
    // Event callbacks
    virtual void setMouseCallback(std::function<void(int, int)> callback) = 0;
    virtual void setClickCallback(std::function<void(bool)> callback) = 0;
    virtual void setResizeCallback(std::function<void(int, int)> callback) = 0;
    virtual void setKeyCallback(std::function<void(KeyCode, bool)> callback) = 0;
    virtual void setTextInputCallback(std::function<void(const std::string&)> callback) = 0;
};
```

### Rendering Pipeline

1. **Clear**: Canvas buffer cleared with background color
2. **Widget Traversal**: WidgetManager calls render() on all widgets
3. **Layered Rendering**: Widgets render in registration order
4. **Buffer Presentation**: Platform renderer displays the buffer
5. **Frame Completion**: Process repeats for next frame

## Event System

### Signal-Slot Pattern

Fern uses a type-safe signal-slot system for event handling:

```cpp
template<typename... Args>
class Signal {
public:
    void connect(std::function<void(Args...)> slot);
    void disconnect(std::function<void(Args...)> slot);
    void emit(Args... args);
    
private:
    std::vector<std::function<void(Args...)>> slots_;
};

// Usage example
class Button {
public:
    Signal<> onClick;  // Signal with no parameters
    Signal<float> onValueChanged;  // Signal with parameter
};
```

### Input Processing

Input flows through the system as follows:

1. **Platform Layer**: Raw input events captured
2. **Input State**: Events converted to InputState structure
3. **Widget Manager**: Distributes input to widgets
4. **Widget Handling**: Widgets process input and return handling status
5. **Event Propagation**: Unhandled events continue to other widgets

```cpp
struct InputState {
    // Mouse state
    int mouseX, mouseY;
    bool mouseLeftPressed;
    bool mouseLeftReleased;
    bool mouseLeftHeld;
    
    // Keyboard state
    bool keyPressed[256];
    bool keyReleased[256];
    std::string textInput;
};
```

## Layout System

### Layout Architecture

The layout system uses a composable widget approach:

```cpp
// Layout widgets are regular widgets that manage children
class LayoutWidget : public Widget {
public:
    virtual void add(std::shared_ptr<Widget> child);
    virtual void remove(std::shared_ptr<Widget> child);
    
protected:
    std::vector<std::shared_ptr<Widget>> children_;
    virtual void layoutChildren() = 0;  // Implemented by concrete layouts
};

// Concrete layout implementations
class ColumnWidget : public LayoutWidget {
    void layoutChildren() override;  // Arrange children vertically
};

class RowWidget : public LayoutWidget {
    void layoutChildren() override;  // Arrange children horizontally
};
```

### Layout Process

1. **Measurement**: Widgets calculate their preferred sizes
2. **Allocation**: Layout widgets assign space to children
3. **Positioning**: Children positioned within allocated space
4. **Propagation**: Process recurses through widget tree

### Responsive Design

- **Expanded Widgets**: Fill available space proportionally
- **Fixed Widgets**: Maintain constant size
- **SizedBox**: Provides spacing and fixed dimensions
- **Center Widgets**: Center content within available space

## Cross-Platform Abstraction

### Platform Factory Pattern

```cpp
class PlatformFactory {
public:
    static std::unique_ptr<PlatformRenderer> createRenderer();
    
private:
    // Returns appropriate renderer based on compile-time flags
#ifdef __EMSCRIPTEN__
    return std::make_unique<WebRenderer>();
#else
    return std::make_unique<LinuxRenderer>();
#endif
};
```

### Platform-Specific Implementation

#### Linux Platform
- **Window Management**: Uses native Linux windowing
- **Event Handling**: Direct system event processing
- **Rendering**: Direct framebuffer or OpenGL rendering

#### Web Platform  
- **Canvas Integration**: HTML5 canvas element
- **Event Handling**: JavaScript event bridge
- **Rendering**: WebAssembly pixel buffer transfer

### Compilation Targets

```cpp
// Platform-specific compilation
#ifdef __EMSCRIPTEN__
    // Web-specific code
    #include <emscripten.h>
    #include <emscripten/html5.h>
#else
    // Native platform code
    #include <platform_specific_headers.h>
#endif
```

## Memory Management

### RAII and Smart Pointers

Fern extensively uses RAII principles and smart pointers:

```cpp
// Widget ownership through shared_ptr
std::shared_ptr<Widget> widget = Button(config);

// Automatic cleanup when last reference is released
// No manual memory management required

// Layout widgets manage child lifecycles
auto layout = Column({
    widget1,  // shared_ptr
    widget2,  // shared_ptr
    widget3   // shared_ptr
});
// All children automatically cleaned up when layout is destroyed
```

### Resource Management

- **Fonts**: Managed by FontManager with automatic cleanup
- **Textures**: Reference counted and automatically freed
- **Event Connections**: Automatically disconnected on widget destruction
- **Platform Resources**: RAII wrappers ensure proper cleanup

### Memory Safety Features

- **No Raw Pointers**: All widgets use shared_ptr
- **Automatic Cleanup**: Destructors handle all resource cleanup
- **Exception Safety**: RAII ensures cleanup even with exceptions
- **No Memory Leaks**: Smart pointers prevent common leak scenarios

## Performance Considerations

### Rendering Optimization

- **Dirty Flagging**: Only re-render when changes occur
- **Culling**: Skip rendering for off-screen widgets
- **Batching**: Group similar drawing operations
- **Buffer Reuse**: Minimize memory allocations

### Event Processing Optimization

- **Early Termination**: Stop processing when widget handles input
- **Spatial Partitioning**: Optimize hit-testing for large widget counts
- **Event Filtering**: Process only relevant events

### Memory Optimization

- **Object Pooling**: Reuse temporary objects
- **Copy Minimization**: Move semantics and references
- **Cache Efficiency**: Structure data for cache-friendly access
- **Lazy Loading**: Load resources only when needed

## Design Patterns

### Patterns Used in Fern

1. **Factory Pattern**: Widget creation functions
2. **Observer Pattern**: Signal-slot system
3. **Strategy Pattern**: Platform renderers
4. **Composite Pattern**: Layout widgets
5. **Builder Pattern**: Configuration classes
6. **RAII Pattern**: Resource management
7. **Template Pattern**: Widget base classes

### Widget Development Patterns

```cpp
// Consistent pattern for all widgets
class NewWidget : public Widget {
private:
    // Configuration
    NewWidgetConfig config_;
    
    // Internal state
    bool isDirty_;
    
public:
    // Constructor with configuration
    NewWidget(const NewWidgetConfig& config);
    
    // Core widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Widget-specific interface
    void setSpecificProperty(Type value);
    Type getSpecificProperty() const;
    
    // Events
    Signal<Type> onSpecificEvent;
};
```

## Future Architecture

### Planned Enhancements

1. **Animation System**: Timeline-based animations
2. **Theme System**: Consistent styling across applications
3. **Accessibility**: Screen reader and keyboard navigation support
4. **3D Rendering**: Optional 3D widget support
5. **Plugin System**: Extensible architecture for custom widgets

### Scalability Considerations

- **Widget Virtualization**: Handle thousands of widgets efficiently
- **Multi-threading**: Background processing for heavy operations
- **GPU Acceleration**: Hardware-accelerated rendering
- **Streaming**: Dynamic loading of UI resources

### API Evolution

- **Backward Compatibility**: Maintain API stability
- **Deprecation Strategy**: Gradual migration of old APIs
- **Version Management**: Clear versioning for breaking changes
- **Extension Points**: Allow customization without framework changes

## Development Guidelines

### Architecture Principles

1. **Separation of Concerns**: Clear boundaries between systems
2. **Dependency Inversion**: Depend on abstractions, not concretions
3. **Open/Closed Principle**: Open for extension, closed for modification
4. **Single Responsibility**: Each class has one reason to change
5. **DRY (Don't Repeat Yourself)**: Avoid code duplication

### Code Organization

- **Header-only when possible**: Simplify distribution
- **Clear module boundaries**: Well-defined interfaces
- **Consistent naming**: Follow established conventions
- **Documentation**: Comprehensive API documentation
- **Testing**: Testable architecture design

---

This architecture document provides the foundation for understanding and contributing to the Fern UI Framework. The design emphasizes simplicity, performance, and cross-platform compatibility while maintaining flexibility for future enhancements.

## Related Documentation

- [Contributing Guide](contributing.md)
- [Testing Guide](testing.md)
- [Performance Guide](performance.md)
- [API Reference](../api-reference.md)
- [Platform Guides](../platform/)
