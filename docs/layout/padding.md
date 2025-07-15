# Padding Layout

The Padding layout widget adds space around its child widget, creating visual breathing room and improving layout aesthetics.

## Overview

The `PaddingWidget` wraps a single child widget and adds padding on all sides. It's commonly used to create margins around widgets or to provide consistent spacing in layouts.

## API Reference

### Constructor
```cpp
PaddingWidget(int x, int y, int width, int height, 
              int left, int top, int right, int bottom);
```

### Factory Functions
```cpp
// Apply equal padding to all sides
std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, int all, bool addToManager = false);

// Apply different padding to each side
std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, 
                                     int left, int top, int right, int bottom, bool addToManager = false);
```

### Methods
```cpp
void add(std::shared_ptr<Widget> child);  // Add child widget (replaces any existing child)
```

## Parameters

- **child**: The widget to wrap with padding
- **all**: Padding value applied to all sides (left, top, right, bottom)
- **left**: Left padding in pixels
- **top**: Top padding in pixels
- **right**: Right padding in pixels
- **bottom**: Bottom padding in pixels
- **addToManager**: Whether to automatically add to the widget manager

## Basic Usage

### Equal Padding
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Padding Example");
    
    // Create a text widget
    auto text = Text("Hello, World!", 50, 50, 200, 30);
    
    // Add 20px padding on all sides
    auto paddedText = Padding(text, 20, true);
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Different Padding Per Side
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Custom Padding Example");
    
    // Create a button widget
    auto button = Button("Click Me!", 100, 100, 120, 40);
    
    // Add custom padding: 10px left, 20px top, 10px right, 5px bottom
    auto paddedButton = Padding(button, 10, 20, 10, 5, true);
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

## Common Use Cases

### 1. Card-like Layouts
```cpp
// Create a "card" with padding around content
auto content = Column({
    Text("Card Title", 0, 0, 200, 25),
    Text("Card content goes here...", 0, 0, 200, 20)
}, true);

auto card = Padding(content, 15, true);
```

### 2. Form Layouts
```cpp
// Add spacing around form elements
auto nameInput = TextInput("Enter your name", 0, 0, 200, 30);
auto emailInput = TextInput("Enter your email", 0, 0, 200, 30);

auto form = Column({
    Padding(nameInput, 10),
    Padding(emailInput, 10)
}, true);
```

### 3. Button Spacing
```cpp
// Create consistently spaced buttons
auto button1 = Button("Save", 0, 0, 80, 30);
auto button2 = Button("Cancel", 0, 0, 80, 30);

auto buttonRow = Row({
    Padding(button1, 5),
    Padding(button2, 5)
}, true);
```

## Layout Behavior

### Size Calculation
- The PaddingWidget reserves space for the padding values
- Child widget gets the remaining space: `(width - left - right) × (height - top - bottom)`
- If the remaining space is negative, the child widget will be clipped

### Positioning
- Child widget is positioned at `(x + left, y + top)`
- Padding effectively creates an inner content area

## Best Practices

### 1. Consistent Spacing
```cpp
// Use consistent padding values throughout your app
const int STANDARD_PADDING = 16;
const int SMALL_PADDING = 8;
const int LARGE_PADDING = 24;

auto paddedWidget = Padding(widget, STANDARD_PADDING, true);
```

### 2. Responsive Padding
```cpp
// Adjust padding based on screen size
int screenWidth = getWidth();
int padding = screenWidth > 800 ? 20 : 10;

auto responsivePadding = Padding(content, padding, true);
```

### 3. Nested Padding
```cpp
// Combine padding with other layouts
auto innerContent = Text("Content", 0, 0, 100, 20);
auto paddedContent = Padding(innerContent, 10);
auto centeredContent = Center(paddedContent, true);
```

## Visual Examples

### Simple Padding
```
┌─────────────────────────────────┐
│        (padding: 20px)          │
│   ┌─────────────────────────┐   │
│   │                         │   │
│   │    Child Widget         │   │
│   │                         │   │
│   └─────────────────────────┘   │
│                                 │
└─────────────────────────────────┘
```

### Asymmetric Padding
```
┌─────────────────────────────────┐
│         (top: 20px)             │
│ (left:  ┌─────────────────┐  (right: │
│  10px)  │                 │   15px)  │
│         │  Child Widget   │          │
│         │                 │          │
│         └─────────────────┘          │
│           (bottom: 5px)              │
└─────────────────────────────────────┘
```

## Tips

1. **Use meaningful padding values**: Standard values like 8, 16, 24, 32 create visual harmony
2. **Consider content hierarchy**: Use more padding for important content
3. **Test on different screen sizes**: Ensure padding scales appropriately
4. **Combine with other layouts**: Padding works well with Column, Row, and Center layouts
5. **Don't over-pad**: Too much padding can waste screen space, especially on mobile

## Related Widgets

- [`SpacingWidget`](spacing.md) - For adding fixed-size spacing between widgets
- [`Column`](column.md) - For vertical layouts that can benefit from padded elements
- [`Row`](row.md) - For horizontal layouts with padded elements
- [`Center`](center.md) - For centering padded content
- [`Container`](../widgets/container.md) - For more complex styling and padding options

## Performance Notes

- Padding widgets are lightweight and have minimal performance impact
- Child widgets are positioned once when the layout is arranged
- Redrawing only occurs when the child widget changes
