# Spacing Layout

The Spacing layout widget creates fixed-size empty space in your layout. It's useful for adding precise spacing between widgets or creating visual separators.

## Overview

The `SpacingWidget` (accessible via the `SizedBox` function) creates an invisible widget with specified dimensions. It's commonly used to add consistent spacing between elements in Column and Row layouts.

## API Reference

### Constructor
```cpp
SpacingWidget(int width, int height);
```

### Factory Function
```cpp
std::shared_ptr<SpacingWidget> SizedBox(int width, int height, bool addToManager = false);
```

### Methods
```cpp
void render() override;                         // Does nothing (invisible widget)
bool handleInput(const InputState& input) override; // Returns false (no interaction)
```

## Parameters

- **width**: Width of the spacing in pixels
- **height**: Height of the spacing in pixels  
- **addToManager**: Whether to automatically add to the widget manager

## Basic Usage

### Fixed Spacing Between Widgets
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Spacing Example");
    
    auto text1 = Text("First Widget", 0, 0, 200, 30);
    auto text2 = Text("Second Widget", 0, 0, 200, 30);
    
    // Add 20px vertical spacing between widgets
    auto column = Column({
        text1,
        SizedBox(0, 20),  // 20px vertical space
        text2
    }, true);
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Horizontal Spacing
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Horizontal Spacing Example");
    
    auto button1 = Button("Button 1", 0, 0, 100, 40);
    auto button2 = Button("Button 2", 0, 0, 100, 40);
    
    // Add 30px horizontal spacing between buttons
    auto row = Row({
        button1,
        SizedBox(30, 0),  // 30px horizontal space
        button2
    }, true);
    
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

### 1. Menu Item Spacing
```cpp
auto menuItems = Column({
    Button("New File", 0, 0, 150, 35),
    SizedBox(0, 5),
    Button("Open File", 0, 0, 150, 35),
    SizedBox(0, 5),
    Button("Save File", 0, 0, 150, 35),
    SizedBox(0, 10),  // Larger space for separator
    Button("Exit", 0, 0, 150, 35)
}, true);
```

### 2. Form Field Spacing
```cpp
auto loginForm = Column({
    Text("Username:", 0, 0, 100, 25),
    SizedBox(0, 5),
    TextInput("Enter username", 0, 0, 200, 30),
    SizedBox(0, 15),
    Text("Password:", 0, 0, 100, 25),
    SizedBox(0, 5),
    TextInput("Enter password", 0, 0, 200, 30),
    SizedBox(0, 20),
    Button("Login", 0, 0, 100, 35)
}, true);
```

### 3. Button Group Spacing
```cpp
auto buttonGroup = Row({
    Button("Save", 0, 0, 80, 30),
    SizedBox(10, 0),
    Button("Cancel", 0, 0, 80, 30),
    SizedBox(10, 0),
    Button("Help", 0, 0, 80, 30)
}, true);
```

### 4. Creating Flexible Layouts
```cpp
// Push content to the right with expandable space
auto toolbar = Row({
    Text("App Title", 0, 0, 100, 30),
    SizedBox(200, 0),  // Fixed space
    Button("Settings", 0, 0, 80, 30)
}, true);
```

## Layout Behavior

### Size Behavior
- SpacingWidget has fixed dimensions specified in constructor
- Does not resize based on content or parent constraints
- Takes up space in layout calculations but renders nothing

### Positioning
- Positioned like any other widget in the layout
- Contributes to the overall size of parent layouts
- Can be positioned independently when not in a layout

## Best Practices

### 1. Consistent Spacing Values
```cpp
// Define standard spacing constants
const int SMALL_SPACING = 5;
const int MEDIUM_SPACING = 10;
const int LARGE_SPACING = 20;

auto form = Column({
    widget1,
    SizedBox(0, MEDIUM_SPACING),
    widget2,
    SizedBox(0, LARGE_SPACING),
    widget3
});
```

### 2. Responsive Spacing
```cpp
// Adjust spacing based on screen size
int screenWidth = getWidth();
int spacing = screenWidth > 800 ? 20 : 10;

auto layout = Row({
    leftWidget,
    SizedBox(spacing, 0),
    rightWidget
});
```

### 3. Semantic Spacing
```cpp
// Use meaningful spacing for different contexts
const int BUTTON_SPACING = 8;
const int SECTION_SPACING = 24;
const int PARAGRAPH_SPACING = 16;

auto ui = Column({
    titleText,
    SizedBox(0, SECTION_SPACING),
    contentText,
    SizedBox(0, PARAGRAPH_SPACING),
    buttonRow
});
```

### 4. Combining with Other Layouts
```cpp
// Use spacing within nested layouts
auto cardContent = Column({
    Text("Card Title", 0, 0, 200, 25),
    SizedBox(0, 10),
    Text("Card description", 0, 0, 200, 20),
    SizedBox(0, 15),
    Row({
        Button("OK", 0, 0, 60, 30),
        SizedBox(10, 0),
        Button("Cancel", 0, 0, 60, 30)
    })
});
```

## Visual Examples

### Vertical Spacing
```
┌─────────────────┐
│   Widget 1      │
└─────────────────┘
        
        ↑ 20px spacing
        ↓
        
┌─────────────────┐
│   Widget 2      │
└─────────────────┘
```

### Horizontal Spacing
```
┌─────────┐    ┌─────────┐
│Widget 1 │    │Widget 2 │
└─────────┘    └─────────┘
           ↑30px↓
```

### Mixed Spacing
```
┌─────────────────┐
│   Title         │
└─────────────────┘
        
        ↑ 15px
        ↓
        
┌─────────────────┐
│   Content       │
└─────────────────┘
        
        ↑ 25px
        ↓
        
┌─────────┐    ┌─────────┐
│Button 1 │    │Button 2 │
└─────────┘    └─────────┘
           ↑10px↓
```

## Advanced Usage

### Dynamic Spacing
```cpp
// Adjust spacing based on content
int contentHeight = getContentHeight();
int spacing = contentHeight > 500 ? 30 : 15;

auto layout = Column({
    header,
    SizedBox(0, spacing),
    content,
    SizedBox(0, spacing),
    footer
});
```

### Conditional Spacing
```cpp
// Add spacing only when needed
std::vector<std::shared_ptr<Widget>> items;
items.push_back(firstWidget);

if (needsSpacing) {
    items.push_back(SizedBox(0, 10));
}

items.push_back(secondWidget);
auto layout = Column(items);
```

## Tips

1. **Use consistent spacing values**: Create a spacing scale (4px, 8px, 16px, 24px, 32px)
2. **Consider visual hierarchy**: Use larger spacing to separate different sections
3. **Test on different screen sizes**: Ensure spacing looks good on various displays
4. **Don't overuse**: Too much spacing can make layouts feel disconnected
5. **Combine with padding**: Use SizedBox for spacing between widgets, Padding for space around widgets

## Related Widgets

- [`Padding`](padding.md) - For adding space around widgets
- [`Column`](column.md) - For vertical layouts that benefit from spacing
- [`Row`](row.md) - For horizontal layouts with spacing between elements
- [`Container`](../widgets/container.md) - For more complex spacing and styling
- [`Expanded`](expanded.md) - For flexible space that expands to fill available area

## Performance Notes

- SpacingWidget is extremely lightweight
- No rendering or input handling overhead
- Minimal memory footprint
- Safe to use many instances in complex layouts
