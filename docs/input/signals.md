# Signal/Slot System in Fern

The Signal/Slot system in Fern provides a powerful pattern for event-driven programming and decoupled communication between components. This system enables widgets to emit signals when events occur and allows other components to react to those events by connecting slots (callback functions).

## Overview

The Signal/Slot system consists of:
- **Signals**: Objects that emit events with optional data
- **Slots**: Callback functions that respond to signal emissions
- **Connections**: Links between signals and slots

## Core Concepts

### Signal Class

The `Signal` template class is the foundation of the event system:

```cpp
#include <fern/core/signal.hpp>

// Basic signal without parameters
Signal<> buttonClicked;

// Signal with parameters
Signal<int, int> mouseMove;
Signal<std::string> textChanged;
```

### Connection Management

Signals return connection IDs when slots are connected, allowing for later disconnection:

```cpp
auto connectionId = signal.connect([](int x, int y) {
    // Handle event
});

// Later disconnect
signal.disconnect(connectionId);
```

## Basic Usage

### Creating and Using Signals

```cpp
#include <fern/core/signal.hpp>
#include <iostream>

class Button {
public:
    Signal<> clicked;
    
    void handleClick() {
        // Emit the signal
        clicked.emit();
    }
};

// Usage
Button button;
button.clicked.connect([]() {
    std::cout << "Button was clicked!" << std::endl;
});
```

### Signals with Parameters

```cpp
class TextInput {
public:
    Signal<const std::string&> textChanged;
    
    void updateText(const std::string& newText) {
        text = newText;
        textChanged.emit(text);
    }
    
private:
    std::string text;
};

// Usage
TextInput input;
input.textChanged.connect([](const std::string& text) {
    std::cout << "Text changed to: " << text << std::endl;
});
```

## Advanced Features

### Multiple Slot Connections

Multiple slots can be connected to the same signal:

```cpp
Signal<int> valueChanged;

// Connect multiple handlers
valueChanged.connect([](int value) {
    std::cout << "Handler 1: " << value << std::endl;
});

valueChanged.connect([](int value) {
    std::cout << "Handler 2: " << value << std::endl;
});

// Both handlers will be called
valueChanged.emit(42);
```

### Connection Management

```cpp
class EventManager {
public:
    void setupConnections() {
        // Store connection IDs for later cleanup
        connections_.push_back(
            button.clicked.connect([this]() { handleButtonClick(); })
        );
        
        connections_.push_back(
            input.textChanged.connect([this](const std::string& text) {
                handleTextChanged(text);
            })
        );
    }
    
    void cleanup() {
        // Disconnect all connections
        for (auto id : connections_) {
            // Note: You'd need to keep track of which signal each ID belongs to
            // This is a simplified example
        }
        connections_.clear();
    }
    
private:
    std::vector<Signal<>::ConnectionID> connections_;
    
    void handleButtonClick() { /* ... */ }
    void handleTextChanged(const std::string& text) { /* ... */ }
};
```

## Common Use Cases

### Widget Communication

```cpp
class Application {
public:
    void setupUI() {
        // Connect widget signals to application logic
        saveButton.clicked.connect([this]() { saveDocument(); });
        
        textEditor.textChanged.connect([this](const std::string& text) {
            documentModified = true;
            updateTitle();
        });
        
        fileMenu.fileSelected.connect([this](const std::string& filename) {
            loadDocument(filename);
        });
    }
    
private:
    ButtonWidget saveButton;
    TextInputWidget textEditor;
    MenuWidget fileMenu;
    bool documentModified = false;
    
    void saveDocument() { /* ... */ }
    void updateTitle() { /* ... */ }
    void loadDocument(const std::string& filename) { /* ... */ }
};
```

### Custom Widget Events

```cpp
class CustomSlider {
public:
    Signal<float> valueChanged;
    Signal<> dragStarted;
    Signal<> dragEnded;
    
    void handleMouseMove(int x, int y) {
        if (dragging) {
            float newValue = calculateValue(x, y);
            if (newValue != value) {
                value = newValue;
                valueChanged.emit(value);
            }
        }
    }
    
    void handleMouseDown(int x, int y) {
        dragging = true;
        dragStarted.emit();
    }
    
    void handleMouseUp(int x, int y) {
        dragging = false;
        dragEnded.emit();
    }
    
private:
    float value = 0.0f;
    bool dragging = false;
    
    float calculateValue(int x, int y) {
        // Calculate value based on mouse position
        return 0.0f; // Placeholder
    }
};
```

## Best Practices

### 1. Signal Naming

Use descriptive names that clearly indicate what event occurred:

```cpp
// Good
Signal<> buttonPressed;
Signal<> buttonReleased;
Signal<const std::string&> textChanged;
Signal<int, int> mouseMoved;

// Avoid
Signal<> signal1;
Signal<> event;
```

### 2. Parameter Design

Keep signal parameters minimal and meaningful:

```cpp
// Good - specific information
Signal<float> volumeChanged;
Signal<int, int> positionChanged;

// Better - structured data for complex events
struct SelectionChangedEvent {
    int startPos;
    int endPos;
    std::string selectedText;
};
Signal<const SelectionChangedEvent&> selectionChanged;
```

### 3. Connection Lifetime Management

Always manage connection lifetimes properly:

```cpp
class Component {
public:
    ~Component() {
        // Disconnect all connections in destructor
        for (auto& [signal, connectionId] : connections_) {
            signal->disconnect(connectionId);
        }
    }
    
    template<typename... Args>
    void connect(Signal<Args...>& signal, std::function<void(Args...)> slot) {
        auto id = signal.connect(slot);
        connections_.push_back({&signal, id});
    }
    
private:
    std::vector<std::pair<void*, Signal<>::ConnectionID>> connections_;
};
```

### 4. Avoid Circular Dependencies

Be careful when connecting signals to slots that might emit other signals:

```cpp
// Potential issue - could create infinite loops
textA.textChanged.connect([&](const std::string& text) {
    textB.setText(text); // This might emit textB.textChanged
});

textB.textChanged.connect([&](const std::string& text) {
    textA.setText(text); // This might emit textA.textChanged
});
```

## Thread Safety

⚠️ **Warning**: The current Signal implementation is not thread-safe. If you need to use signals across threads, implement appropriate synchronization:

```cpp
// For thread-safe usage, you would need to add mutex protection
class ThreadSafeSignal {
    // Implementation would include std::mutex for thread safety
};
```

## Integration with Widget System

Most Fern widgets use the Signal/Slot system internally:

```cpp
// Button widget example
ButtonWidget button;
button.clicked.connect([&]() {
    // Handle button click
});

// Text input widget
TextInputWidget input;
input.textChanged.connect([&](const std::string& text) {
    // Handle text change
});
```

## Performance Considerations

- Signal emission is O(n) where n is the number of connected slots
- Connection and disconnection are O(1) for connection, O(n) for disconnection
- Prefer stack-allocated slots over heap-allocated ones when possible
- Consider using object pools for frequently created/destroyed connections

## See Also

- [Event System](events.md) - For system-level event handling
- [Input Handling](input-handling.md) - For keyboard and mouse input
- [Widget Documentation](../widgets/) - For widget-specific events
