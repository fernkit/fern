# Understanding the Widget System

Widgets are the building blocks of every Fern application. If you've ever wondered how interactive elements like buttons know when you click them, how text appears properly formatted on screen, or how layouts automatically arrange themselves, this guide will demystify the entire widget system from the ground up.

## What Are Widgets?

A widget is essentially a smart piece of your user interface that knows how to:
- **Draw itself**: Each widget contains the logic to render its appearance using Fern's drawing primitives
- **Handle input**: Widgets can respond to mouse clicks, key presses, and other user interactions
- **Manage state**: Widgets maintain their own internal state and can update their appearance when that state changes
- **Participate in layouts**: Widgets can be arranged automatically by layout containers

Think of widgets as self-contained UI components. A button widget knows it should look like a rectangle with text, change color when you hover over it, and emit a signal when clicked. A text widget knows how to render characters, handle different font sizes, and wrap text across multiple lines.

## The Widget Hierarchy

Every Fern application has a widget hierarchy - a tree structure where widgets can contain other widgets. Understanding this hierarchy is crucial for building complex interfaces.

### Parent-Child Relationships

```
Application Window
└── CenterWidget (positions content in center)
    └── Column (arranges children vertically)
        ├── Text ("Welcome to My App")
        ├── SizedBox (spacing)
        ├── Row (arranges children horizontally)
        │   ├── Button ("Login")
        │   └── Button ("Register")
        └── Text ("© 2025 My Company")
```

In this hierarchy:
- The **CenterWidget** is the parent of the **Column**
- The **Column** is the parent of multiple **Text** widgets, a **SizedBox**, and a **Row**
- The **Row** is the parent of two **Button** widgets

### Why Hierarchy Matters

The widget hierarchy determines several important things:

1. **Rendering Order**: Widgets are drawn in hierarchy order, with children drawn on top of their parents
2. **Input Processing**: Input events (like mouse clicks) travel through the hierarchy to find the widget that should handle them
3. **Layout Calculation**: Parent widgets calculate positions and sizes for their children
4. **Lifecycle Management**: When a parent widget is removed, all its children are automatically removed too

## Widget Lifecycle

Every widget goes through a predictable lifecycle as your application runs. Understanding this lifecycle helps you write better applications and debug problems.

### Creation and Setup

```cpp
void setupUI() {
    // 1. Widget is created
    auto button = Button(ButtonConfig(0, 0, 200, 50, "Click Me"));
    
    // 2. Widget properties can be configured
    button->onClick.connect([]() {
        std::cout << "Clicked!" << std::endl;
    });
    
    // 3. Widget is added to the manager
    addWidget(button);
    
    // 4. Widget is now part of the active UI
}
```

Once a widget is added with `addWidget()`, it becomes part of the active interface and will:
- Receive regular update calls
- Be rendered every frame
- Process input events when appropriate
- Remain active until explicitly removed

### The Render Loop

Every frame (typically 60 times per second), each widget in your application goes through this process:

1. **Update**: The widget can update its internal state, handle animations, or process logic
2. **Input Processing**: If there are input events (mouse moves, clicks, key presses), relevant widgets process them
3. **Rendering**: Each widget draws itself using Fern's drawing primitives

This happens automatically - you don't need to manually call these functions.

### Widget Removal

Widgets can be removed from the interface in several ways:

```cpp
// Remove a specific widget
WidgetManager::getInstance().removeWidget(myButton);

// Clear all widgets (useful for scene transitions)
WidgetManager::getInstance().clear();

// Widgets are also automatically removed when they go out of scope
{
    auto temporaryWidget = Button(ButtonConfig(0, 0, 100, 50, "Temp"));
    addWidget(temporaryWidget);
} // temporaryWidget is automatically removed here
```

## Widget Manager

The Widget Manager is Fern's central system for coordinating all widgets. It's a singleton (only one instance exists) that handles the complex orchestration of widget rendering, input distribution, and lifecycle management.

### What the Widget Manager Does

1. **Z-Order Management**: Maintains the visual layering of widgets. Widgets added later appear on top of earlier widgets.

2. **Input Distribution**: When you click the mouse, the Widget Manager figures out which widget should receive the click event. It tests widgets from top to bottom (reverse order) until one handles the event.

3. **Automatic Rendering**: Every frame, the Widget Manager calls the render method of each widget in the correct order.

4. **Memory Management**: Keeps track of all active widgets and ensures they're properly cleaned up.

### Adding and Removing Widgets

```cpp
// Add a widget (it goes to the top of the z-order)
addWidget(myWidget);

// This is equivalent to:
WidgetManager::getInstance().addWidget(myWidget);

// Remove a specific widget
WidgetManager::getInstance().removeWidget(myWidget);

// Remove all widgets
WidgetManager::getInstance().clear();
```

The `addWidget()` function is just a convenience function that calls the Widget Manager internally.

### Z-Order and Layering

Widgets are rendered in the order they were added:

```cpp
auto background = Button(ButtonConfig(0, 0, 300, 200, "Background"));
auto foreground = Button(ButtonConfig(50, 50, 200, 100, "Foreground"));

addWidget(background);  // Added first, rendered first (bottom layer)
addWidget(foreground);  // Added second, rendered second (top layer)
```

The `foreground` button will appear on top of the `background` button where they overlap.

## Input Handling

Input handling in Fern follows a clear, predictable pattern that's important to understand for building responsive applications.

### The Input Flow

1. **Event Capture**: Fern's platform layer captures raw input events (mouse movement, clicks, key presses)
2. **Event Processing**: The Input system converts these into standardized InputState objects
3. **Widget Distribution**: The Widget Manager distributes input to widgets, starting from the topmost widget
4. **Event Handling**: Each widget decides whether to handle the event and whether to stop its propagation

### Input Propagation

When you click the mouse, here's what happens:

```cpp
// Widgets in z-order (top to bottom):
// 1. TopButton (added last)
// 2. MiddleButton  
// 3. BottomButton (added first)

// Input processing order:
// 1. TopButton.handleInput() - if it returns true, processing stops
// 2. MiddleButton.handleInput() - only called if TopButton returned false
// 3. BottomButton.handleInput() - only called if others returned false
```

A widget returns `true` from `handleInput()` to indicate it handled the event and no other widgets should process it.

### Input State

Every frame, widgets receive an `InputState` object containing all current input information:

```cpp
struct InputState {
    // Mouse information
    int mouseX, mouseY;        // Current mouse position
    bool mouseDown;            // Is mouse button currently held down?
    bool mouseClicked;         // Was mouse clicked this frame?
    
    // Keyboard information  
    KeyCode lastKeyPressed;    // Most recent key press
    bool keyPressed;           // Was any key pressed this frame?
    std::string textInput;     // Text input for text fields
    
    // Helper methods
    bool isKeyDown(KeyCode key);
    bool isKeyJustPressed(KeyCode key);
    bool isKeyJustReleased(KeyCode key);
};
```

This comprehensive input state allows widgets to respond to any kind of user interaction.

## Widget Types Overview

Fern provides several categories of widgets, each designed for specific purposes:

### Display Widgets
- **Text**: Renders text with various fonts and sizes
- **Circle**: Draws filled or outlined circles
- **Line**: Draws lines between points

### Interactive Widgets  
- **Button**: Clickable buttons with hover effects
- **TextInput**: Text entry fields
- **Slider**: Draggable value selectors
- **RadioButton**: Single-choice selection
- **Dropdown**: Multi-option selection menus

### Layout Widgets
- **Column**: Arranges children vertically
- **Row**: Arranges children horizontally  
- **CenterWidget**: Centers content within available space
- **Container**: Groups widgets with padding and decoration
- **SizedBox**: Creates spacing or fixed-size areas

### Indicator Widgets
- **ProgressBar**: Shows completion progress
- **CircularIndicator**: Animated loading indicators

Each widget type is designed to excel at its specific purpose while following consistent patterns for configuration, styling, and event handling.

## Creating Custom Widgets

While Fern provides many built-in widgets, you can create your own by inheriting from the base Widget class:

```cpp
class MyCustomWidget : public Widget {
public:
    MyCustomWidget(Point position, Size size) 
        : position_(position), size_(size) {}
    
    void render() override {
        // Draw your custom widget here
        Draw::rect(position_.x, position_.y, size_.width, size_.height, Colors::Blue);
        
        // Add custom graphics, text, etc.
    }
    
    bool handleInput(const InputState& input) override {
        // Check if input is within this widget's bounds
        bool mouseInside = (input.mouseX >= position_.x && 
                           input.mouseX < position_.x + size_.width &&
                           input.mouseY >= position_.y && 
                           input.mouseY < position_.y + size_.height);
        
        if (mouseInside && input.mouseClicked) {
            // Handle the click
            std::cout << "Custom widget clicked!" << std::endl;
            return true;  // Event handled, stop propagation
        }
        
        return false;  // Event not handled
    }
    
private:
    Point position_;
    Size size_;
};
```

This demonstrates the two essential methods every widget must implement:
- `render()`: Draw the widget's appearance
- `handleInput()`: Process user input events

## Widget Communication

Widgets often need to communicate with each other or with your application logic. Fern provides several patterns for this:

### Signals and Slots

The primary communication mechanism is the signal-slot system:

```cpp
auto button = Button(ButtonConfig(0, 0, 100, 50, "Click"));
auto textWidget = Text(Point(0, 60), "Not clicked", 2, Colors::White);

button->onClick.connect([textWidget]() {
    textWidget->setText("Button was clicked!");
});
```

### Static Variables

For application-wide state, use static variables:

```cpp
static int clickCount = 0;
static std::shared_ptr<TextWidget> counterDisplay;

void setupUI() {
    counterDisplay = Text(Point(0, 0), "Clicks: 0", 2, Colors::White);
    
    auto button = Button(ButtonConfig(0, 30, 100, 50, "Click"));
    button->onClick.connect([]() {
        clickCount++;
        counterDisplay->setText("Clicks: " + std::to_string(clickCount));
    });
    
    addWidget(counterDisplay);
    addWidget(button);
}
```

### Parent-Child Communication

Layout widgets can coordinate behavior among their children, and children can access parent widget properties when needed.

## Performance Considerations

While Fern handles most performance optimizations automatically, understanding a few key concepts helps you build efficient applications:

### Widget Count

Every widget has a small performance cost for input processing and rendering. For most applications, hundreds of widgets perform perfectly well, but be mindful when creating thousands of widgets.

### Render Efficiency

Widgets only render what's visible. If a widget is completely covered by others, it still processes input and updates, but its rendering can be optimized.

### Memory Management

Use `std::shared_ptr` for widgets to ensure proper memory management, and call `WidgetManager::getInstance().clear()` when transitioning between different parts of your application.

## Best Practices

### Widget Organization
- Group related widgets using layout containers
- Use descriptive variable names for widgets you'll reference later
- Keep widget creation code organized in logical functions

### State Management
- Use static variables for persistent state
- Consider the widget hierarchy when deciding where to store state
- Update widget properties through their public methods, not by recreating widgets

### Input Handling
- Return `true` from `handleInput()` when you've handled an event
- Test input bounds carefully to ensure widgets respond in their visual area
- Use the InputState helper methods for clean, readable input code

### Performance
- Avoid creating widgets every frame in your draw loop
- Use `WidgetManager::getInstance().clear()` for scene transitions
- Prefer built-in widgets over custom ones when possible

The widget system is the heart of every Fern application. Understanding these concepts deeply will help you build sophisticated, responsive user interfaces that feel natural and performant.
