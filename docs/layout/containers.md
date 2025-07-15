# Container System

The Container system provides styled wrapper widgets that can hold other widgets. Containers offer background colors, gradients, and layout capabilities for creating visually appealing user interfaces.

## Overview

Containers are fundamental building blocks that wrap other widgets and provide styling such as:
- Solid background colors
- Linear gradients
- Centered positioning
- Custom dimensions and positioning

## Container Types

### 1. Basic Container
The standard container with a solid background color.

```cpp
std::shared_ptr<ContainerWidget> Container(
    uint32_t color, int x, int y, int width, int height,
    std::shared_ptr<Widget> child = nullptr, bool addToManager = false);
```

### 2. Centered Container
A container that automatically centers its child within its bounds.

```cpp
std::shared_ptr<CenteredContainerWidget> CenteredContainer(
    uint32_t color, int width, int height,
    bool addToManager = false,
    std::shared_ptr<Widget> child = nullptr);
```

### 3. Gradient Container
A container with a linear gradient background.

```cpp
std::shared_ptr<GradientContainerWidget> GradientContainer(
    int x, int y, int width, int height,
    const LinearGradient& gradient,
    bool addToManager = false,
    std::shared_ptr<Widget> child = nullptr);
```

## Basic Usage

### Simple Container
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Container Example");
    
    // Create a text widget
    auto text = Text("Hello Container!", 0, 0, 200, 30);
    
    // Wrap it in a blue container
    auto container = Container(
        Colors::Blue,     // Background color
        100, 100,         // Position (x, y)
        250, 80,          // Size (width, height)
        text,             // Child widget
        true              // Add to manager
    );
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Centered Container
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Centered Container Example");
    
    auto button = Button("Click Me!", 0, 0, 120, 40);
    
    // Create a centered container
    auto centeredContainer = CenteredContainer(
        Colors::LightGray,  // Background color
        200, 100,           // Container size
        true,               // Add to manager
        button              // Child widget (will be centered)
    );
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Gradient Container
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Gradient Container Example");
    
    // Create a gradient from blue to purple
    LinearGradient gradient({
        {Colors::Blue, 0.0f},
        {Colors::Purple, 1.0f}
    }, LinearGradient::HORIZONTAL);
    
    auto text = Text("Gradient Background", 0, 0, 200, 30);
    text->setColor(Colors::White);
    
    auto gradientContainer = GradientContainer(
        100, 100,         // Position
        300, 150,         // Size
        gradient,         // Gradient definition
        true,             // Add to manager
        text              // Child widget
    );
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

## Advanced Usage

### Nested Containers
```cpp
// Create layered containers for complex styling
auto innerText = Text("Nested Content", 0, 0, 150, 25);

auto innerContainer = Container(
    Colors::White,
    0, 0, 200, 60,
    innerText
);

auto outerContainer = Container(
    Colors::DarkGray,
    150, 150, 220, 80,
    innerContainer,
    true
);
```

### Container with Multiple Children (using Layout)
```cpp
// Use containers with layout widgets
auto title = Text("Card Title", 0, 0, 200, 25);
auto content = Text("Card content here", 0, 0, 200, 20);

auto cardLayout = Column({title, content});

auto cardContainer = Container(
    Colors::White,
    100, 100, 250, 100,
    cardLayout,
    true
);
```

### Multi-Color Gradients
```cpp
// Create a rainbow gradient
LinearGradient rainbow({
    {Colors::Red, 0.0f},
    {Colors::Orange, 0.2f},
    {Colors::Yellow, 0.4f},
    {Colors::Green, 0.6f},
    {Colors::Blue, 0.8f},
    {Colors::Purple, 1.0f}
}, LinearGradient::HORIZONTAL);

auto rainbowContainer = GradientContainer(
    50, 50, 400, 100,
    rainbow,
    true
);
```

### Vertical Gradients
```cpp
// Create a vertical gradient
LinearGradient verticalGradient({
    {Colors::LightBlue, 0.0f},
    {Colors::DarkBlue, 1.0f}
}, LinearGradient::VERTICAL);

auto verticalContainer = GradientContainer(
    200, 200, 200, 300,
    verticalGradient,
    true
);
```

## API Reference

### ContainerWidget Methods
```cpp
void render() override;                           // Renders background and child
bool handleInput(const InputState& input) override; // Delegates to child
void setChild(std::shared_ptr<Widget> child);   // Sets/replaces child widget
```

### LinearGradient Class
```cpp
// Constructor
LinearGradient(const std::vector<GradientStop>& stops, Direction direction = HORIZONTAL);

// Methods
uint32_t colorAt(float position) const;  // Get color at position (0.0-1.0)
bool isVertical() const;                 // Check if gradient is vertical

// Gradient stop structure
struct GradientStop {
    uint32_t color;      // Color at this stop
    float position;      // Position from 0.0 to 1.0
};
```

## Common Patterns

### 1. Card-like Components
```cpp
auto createCard(const std::string& title, const std::string& content) {
    auto titleText = Text(title, 0, 0, 200, 25);
    titleText->setFontSize(18);
    
    auto contentText = Text(content, 0, 0, 200, 40);
    
    auto cardLayout = Column({
        titleText,
        SizedBox(0, 10),
        contentText
    });
    
    return Container(
        Colors::White,
        0, 0, 220, 90,
        Padding(cardLayout, 15)
    );
}
```

### 2. Styled Buttons
```cpp
auto createStyledButton(const std::string& text, uint32_t color) {
    auto button = Button(text, 0, 0, 100, 35);
    
    return Container(
        color,
        0, 0, 120, 50,
        Center(button)
    );
}
```

### 3. Section Headers
```cpp
auto createSectionHeader(const std::string& title) {
    auto headerText = Text(title, 0, 0, 300, 30);
    headerText->setColor(Colors::White);
    headerText->setFontSize(20);
    
    LinearGradient headerGradient({
        {Colors::DarkBlue, 0.0f},
        {Colors::Blue, 1.0f}
    });
    
    return GradientContainer(
        0, 0, 400, 40,
        headerGradient,
        false,
        Padding(headerText, 15, 5, 15, 5)
    );
}
```

## Best Practices

### 1. Consistent Styling
```cpp
// Define standard colors and sizes
const uint32_t CARD_BACKGROUND = Colors::White;
const uint32_t ACCENT_COLOR = Colors::Blue;
const int STANDARD_PADDING = 16;
const int CARD_RADIUS = 8;  // For future rounded corners

// Use consistent container styles
auto standardCard = Container(
    CARD_BACKGROUND,
    0, 0, 300, 200,
    Padding(content, STANDARD_PADDING)
);
```

### 2. Color Hierarchy
```cpp
// Use color to indicate hierarchy
auto primaryContainer = Container(Colors::Blue, 0, 0, 200, 100, content);
auto secondaryContainer = Container(Colors::Gray, 0, 0, 200, 100, content);
auto accentContainer = Container(Colors::Orange, 0, 0, 200, 100, content);
```

### 3. Responsive Design
```cpp
// Adjust container size based on content
auto createResponsiveContainer(std::shared_ptr<Widget> child) {
    int containerWidth = std::max(200, child->getWidth() + 40);
    int containerHeight = std::max(100, child->getHeight() + 20);
    
    return Container(
        Colors::LightGray,
        0, 0, containerWidth, containerHeight,
        Center(child)
    );
}
```

## Visual Examples

### Basic Container
```
┌─────────────────────────────────┐
│ (Blue Background)               │
│                                 │
│    Hello Container!             │
│                                 │
└─────────────────────────────────┘
```

### Centered Container
```
┌─────────────────────────────────┐
│ (Light Gray Background)         │
│                                 │
│        ┌─────────────┐          │
│        │ Click Me!   │          │
│        └─────────────┘          │
│                                 │
└─────────────────────────────────┘
```

### Gradient Container
```
┌─────────────────────────────────┐
│ Blue → → → → → → → → → → Purple │
│                                 │
│    Gradient Background          │
│                                 │
└─────────────────────────────────┘
```

## Performance Considerations

1. **Gradient rendering**: Gradients are more expensive than solid colors
2. **Nested containers**: Deep nesting can impact performance
3. **Child updates**: Containers only re-render when their child changes
4. **Memory usage**: Each container stores its styling information

## Tips

1. **Use containers for visual grouping**: Group related widgets together
2. **Combine with padding**: Use `Padding` widgets inside containers for better spacing
3. **Consider accessibility**: Ensure sufficient color contrast
4. **Test with different content**: Verify containers work with various child widgets
5. **Use gradients sparingly**: Too many gradients can look overwhelming

## Related Components

- [`Padding`](padding.md) - For adding space around container content
- [`Center`](center.md) - For centering widgets (also available as CenteredContainer)
- [`Column`](column.md) - For vertical layouts inside containers
- [`Row`](row.md) - For horizontal layouts inside containers
- [`Colors`](../graphics/colors.md) - For color constants and utilities

## Color Utilities

Containers work with the Fern color system:

```cpp
// Using predefined colors
Container(Colors::Red, 0, 0, 100, 100, child);

// Using custom colors (ARGB format)
Container(0xFF336699, 0, 0, 100, 100, child);  // Custom blue

// Using RGB helper (if available)
Container(RGB(51, 102, 153), 0, 0, 100, 100, child);
```
