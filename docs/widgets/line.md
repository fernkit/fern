# Line Widget

The `LineWidget` provides a simple way to draw lines with customizable start and end points, thickness, and color. It's useful for creating dividers, borders, geometric shapes, and visual connections between elements.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Styling](#styling)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating a Line

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic line from point A to point B
auto line = Line(Point(50, 50), Point(200, 50), 2, Colors::White);

// Without adding to manager
auto line = Line(Point(50, 100), Point(200, 150), 3, Colors::Red, false);
```

### Using LineWidget Class

```cpp
// Create with constructor
auto line = std::make_shared<LineWidget>(
    Point(100, 100),   // start point
    Point(300, 200),   // end point
    4,                 // thickness
    Colors::Blue       // color
);

// Add to widget manager
addWidget(line);
```

## Configuration

### Basic Properties

```cpp
// Create line
auto line = Line(Point(50, 50), Point(200, 100), 2, Colors::Green);

// Modify properties
line->setStart(Point(75, 75));
line->setEnd(Point(225, 125));
line->setThickness(5);
line->setColor(Colors::Red);

// Get properties
Point start = line->getStart();
Point end = line->getEnd();
int thickness = line->getThickness();
uint32_t color = line->getColor();
```

### Line Types

```cpp
// Horizontal line
auto horizontalLine = Line(Point(50, 100), Point(300, 100), 2, Colors::White);

// Vertical line
auto verticalLine = Line(Point(100, 50), Point(100, 300), 2, Colors::White);

// Diagonal line
auto diagonalLine = Line(Point(50, 50), Point(200, 200), 2, Colors::White);
```

## Styling

### Thickness Variations

```cpp
// Thin line (1px)
auto thinLine = Line(Point(50, 50), Point(200, 50), 1, Colors::White);

// Medium line (3px)
auto mediumLine = Line(Point(50, 70), Point(200, 70), 3, Colors::White);

// Thick line (5px)
auto thickLine = Line(Point(50, 90), Point(200, 90), 5, Colors::White);

// Extra thick line (10px)
auto extraThickLine = Line(Point(50, 110), Point(200, 110), 10, Colors::White);
```

### Color Options

```cpp
// Using color constants
auto redLine = Line(Point(50, 50), Point(200, 50), 2, Colors::Red);
auto blueLine = Line(Point(50, 70), Point(200, 70), 2, Colors::Blue);
auto greenLine = Line(Point(50, 90), Point(200, 90), 2, Colors::Green);

// Using hex colors
auto customLine = Line(Point(50, 110), Point(200, 110), 2, 0xFF6B35);  // Orange
auto transparentLine = Line(Point(50, 130), Point(200, 130), 2, 0x80FF0000);  // Semi-transparent red
```

### Dynamic Styling

```cpp
auto line = Line(Point(100, 100), Point(300, 200), 2, Colors::White);

// Animate thickness
void animateThickness(int frame) {
    int thickness = 2 + (int)(3 * sin(frame * 0.1));  // Oscillate between 2-5
    line->setThickness(thickness);
}

// Animate color
void animateColor(int frame) {
    uint32_t red = (uint32_t)(128 + 127 * sin(frame * 0.05));
    uint32_t green = (uint32_t)(128 + 127 * sin(frame * 0.05 + 2.0));
    uint32_t blue = (uint32_t)(128 + 127 * sin(frame * 0.05 + 4.0));
    uint32_t color = (red << 16) | (green << 8) | blue;
    line->setColor(color);
}
```

## Examples

### Divider Lines

```cpp
void createDividers() {
    // Horizontal divider
    auto horizontalDivider = Line(
        Point(50, 200), 
        Point(Fern::getWidth() - 50, 200), 
        1, 
        Colors::Gray
    );
    
    // Vertical divider
    auto verticalDivider = Line(
        Point(400, 50), 
        Point(400, Fern::getHeight() - 50), 
        1, 
        Colors::Gray
    );
    
    // Section divider with padding
    int sectionY = 300;
    auto sectionDivider = Line(
        Point(100, sectionY), 
        Point(Fern::getWidth() - 100, sectionY), 
        2, 
        Colors::White
    );
}
```

### Border Creation

```cpp
void createBorder() {
    int borderThickness = 3;
    uint32_t borderColor = Colors::Blue;
    
    // Top border
    auto topBorder = Line(
        Point(100, 100), 
        Point(500, 100), 
        borderThickness, 
        borderColor
    );
    
    // Right border
    auto rightBorder = Line(
        Point(500, 100), 
        Point(500, 300), 
        borderThickness, 
        borderColor
    );
    
    // Bottom border
    auto bottomBorder = Line(
        Point(500, 300), 
        Point(100, 300), 
        borderThickness, 
        borderColor
    );
    
    // Left border
    auto leftBorder = Line(
        Point(100, 300), 
        Point(100, 100), 
        borderThickness, 
        borderColor
    );
}
```

### Geometric Shapes

```cpp
void createTriangle() {
    Point p1(200, 100);
    Point p2(300, 250);
    Point p3(100, 250);
    
    // Create triangle with lines
    auto side1 = Line(p1, p2, 3, Colors::Red);
    auto side2 = Line(p2, p3, 3, Colors::Red);
    auto side3 = Line(p3, p1, 3, Colors::Red);
}

void createGrid() {
    int gridSize = 50;
    int gridThickness = 1;
    uint32_t gridColor = Colors::Gray;
    
    // Vertical lines
    for (int x = 100; x <= 500; x += gridSize) {
        auto verticalLine = Line(
            Point(x, 100), 
            Point(x, 400), 
            gridThickness, 
            gridColor
        );
    }
    
    // Horizontal lines
    for (int y = 100; y <= 400; y += gridSize) {
        auto horizontalLine = Line(
            Point(100, y), 
            Point(500, y), 
            gridThickness, 
            gridColor
        );
    }
}
```

### Progress Indicator

```cpp
void createProgressBar() {
    int barWidth = 300;
    int barHeight = 20;
    Point barStart(100, 200);
    
    // Background line (full width)
    auto backgroundLine = Line(
        barStart, 
        Point(barStart.x + barWidth, barStart.y), 
        barHeight, 
        Colors::Gray
    );
    
    // Progress line (variable width based on progress)
    int progress = 65; // 65% progress
    int progressWidth = (barWidth * progress) / 100;
    
    auto progressLine = Line(
        barStart, 
        Point(barStart.x + progressWidth, barStart.y), 
        barHeight, 
        Colors::Green
    );
}
```

### Connection Lines

```cpp
void createConnections() {
    // Create some circles to connect
    auto circle1 = Circle(20, Point(150, 150), Colors::Blue);
    auto circle2 = Circle(20, Point(350, 200), Colors::Red);
    auto circle3 = Circle(20, Point(250, 300), Colors::Green);
    
    // Connect circles with lines
    auto connection1 = Line(Point(150, 150), Point(350, 200), 2, Colors::Yellow);
    auto connection2 = Line(Point(350, 200), Point(250, 300), 2, Colors::Yellow);
    auto connection3 = Line(Point(250, 300), Point(150, 150), 2, Colors::Yellow);
}
```

### Animated Lines

```cpp
void createAnimatedLines() {
    std::vector<std::shared_ptr<LineWidget>> animatedLines;
    
    // Create multiple lines for wave effect
    for (int i = 0; i < 10; ++i) {
        auto line = Line(
            Point(100 + i * 30, 200), 
            Point(100 + i * 30, 200), 
            3, 
            Colors::Cyan
        );
        animatedLines.push_back(line);
    }
    
    // Animation function
    int animationFrame = 0;
    auto animateWave = [&animatedLines, &animationFrame]() {
        for (int i = 0; i < animatedLines.size(); ++i) {
            double wave = sin((animationFrame + i * 10) * 0.1) * 50;
            Point start(100 + i * 30, 200);
            Point end(100 + i * 30, 200 + (int)wave);
            
            animatedLines[i]->setStart(start);
            animatedLines[i]->setEnd(end);
        }
        animationFrame++;
    };
    
    // Call animateWave() in your render loop
}
```

### Graph/Chart Lines

```cpp
void createLineChart() {
    std::vector<int> dataPoints = {50, 80, 120, 90, 150, 110, 180, 160};
    
    // Create lines connecting data points
    for (int i = 0; i < dataPoints.size() - 1; ++i) {
        Point start(100 + i * 50, 300 - dataPoints[i]);
        Point end(100 + (i + 1) * 50, 300 - dataPoints[i + 1]);
        
        auto chartLine = Line(start, end, 2, Colors::Green);
        
        // Add data point circles
        auto startPoint = Circle(4, start, Colors::Blue);
        auto endPoint = Circle(4, end, Colors::Blue);
    }
    
    // Add axis lines
    auto xAxis = Line(Point(80, 300), Point(500, 300), 2, Colors::White);
    auto yAxis = Line(Point(100, 320), Point(100, 100), 2, Colors::White);
}
```

### Crosshair/Cursor

```cpp
void createCrosshair() {
    Point center(200, 200);
    int size = 20;
    
    // Horizontal crosshair line
    auto horizontalCrosshair = Line(
        Point(center.x - size, center.y), 
        Point(center.x + size, center.y), 
        2, 
        Colors::Red
    );
    
    // Vertical crosshair line
    auto verticalCrosshair = Line(
        Point(center.x, center.y - size), 
        Point(center.x, center.y + size), 
        2, 
        Colors::Red
    );
}
```

### Arrows

```cpp
void createArrow() {
    Point start(100, 200);
    Point end(300, 200);
    
    // Main arrow line
    auto arrowLine = Line(start, end, 3, Colors::Yellow);
    
    // Arrowhead lines
    int arrowSize = 15;
    double angle = 0.5; // radians
    
    // Right arrowhead line
    Point arrowRight(
        end.x - arrowSize * cos(angle),
        end.y - arrowSize * sin(angle)
    );
    auto arrowLineRight = Line(end, arrowRight, 3, Colors::Yellow);
    
    // Left arrowhead line
    Point arrowLeft(
        end.x - arrowSize * cos(angle),
        end.y + arrowSize * sin(angle)
    );
    auto arrowLineLeft = Line(end, arrowLeft, 3, Colors::Yellow);
}
```

## Best Practices

### 1. Choose Appropriate Thickness

```cpp
// Thin lines for subtle elements
auto subtleLine = Line(Point(x1, y1), Point(x2, y2), 1, Colors::Gray);

// Medium lines for UI elements
auto uiLine = Line(Point(x1, y1), Point(x2, y2), 2, Colors::White);

// Thick lines for emphasis
auto emphasizedLine = Line(Point(x1, y1), Point(x2, y2), 4, Colors::Red);
```

### 2. Use Consistent Colors

```cpp
// Define color scheme
const uint32_t UI_BORDER_COLOR = Colors::Gray;
const uint32_t ACCENT_COLOR = Colors::Blue;
const uint32_t ERROR_COLOR = Colors::Red;

// Apply consistently
auto borderLine = Line(Point(x1, y1), Point(x2, y2), 1, UI_BORDER_COLOR);
auto accentLine = Line(Point(x1, y1), Point(x2, y2), 2, ACCENT_COLOR);
```

### 3. Responsive Design

```cpp
// Adapt to screen size
int screenWidth = Fern::getWidth();
int screenHeight = Fern::getHeight();

// Full-width divider
auto divider = Line(
    Point(20, screenHeight / 2), 
    Point(screenWidth - 20, screenHeight / 2), 
    1, 
    Colors::Gray
);
```

### 4. Performance Considerations

```cpp
// Reuse lines when possible
std::vector<std::shared_ptr<LineWidget>> linePool;

// Create line factory
auto createLine = [&linePool](Point start, Point end, int thickness, uint32_t color) {
    if (!linePool.empty()) {
        auto line = linePool.back();
        linePool.pop_back();
        line->setStart(start);
        line->setEnd(end);
        line->setThickness(thickness);
        line->setColor(color);
        return line;
    }
    return Line(start, end, thickness, color, false);
};
```

### 5. Visual Hierarchy

```cpp
// Use different thickness for hierarchy
auto primaryLine = Line(Point(x1, y1), Point(x2, y2), 3, Colors::White);    // Primary
auto secondaryLine = Line(Point(x1, y1), Point(x2, y2), 2, Colors::Gray);   // Secondary
auto tertiaryLine = Line(Point(x1, y1), Point(x2, y2), 1, Colors::DarkGray); // Tertiary
```

## Troubleshooting

### Common Issues

1. **Line Not Visible**
   ```cpp
   // Check if line is within screen bounds
   int screenWidth = Fern::getWidth();
   int screenHeight = Fern::getHeight();
   
   // Ensure both points are visible
   auto line = Line(Point(50, 50), Point(200, 100), 2, Colors::White);
   
   // Check color contrast
   auto visibleLine = Line(Point(50, 50), Point(200, 100), 2, Colors::White);  // On dark background
   ```

2. **Line Too Thin or Thick**
   ```cpp
   // Adjust thickness based on screen size
   int screenWidth = Fern::getWidth();
   int baseThickness = 2;
   int scaledThickness = std::max(1, (screenWidth * baseThickness) / 800);
   
   auto line = Line(Point(50, 50), Point(200, 100), scaledThickness, Colors::White);
   ```

3. **Lines Not Connecting Properly**
   ```cpp
   // Ensure exact point matching
   Point connectionPoint(150, 150);
   auto line1 = Line(Point(50, 50), connectionPoint, 2, Colors::Red);
   auto line2 = Line(connectionPoint, Point(250, 250), 2, Colors::Red);
   ```

4. **Performance Issues with Many Lines**
   ```cpp
   // Consider using a single drawing call for multiple lines
   // Or limit the number of active lines
   const int MAX_LINES = 1000;
   
   // Use efficient update patterns
   std::vector<std::shared_ptr<LineWidget>> activeLinesOnly;
   ```

### Debugging

```cpp
// Debug line properties
#ifdef DEBUG
    auto line = Line(Point(50, 50), Point(200, 100), 2, Colors::White);
    std::cout << "Line - Start: (" << line->getStart().x << ", " << line->getStart().y << ")"
              << ", End: (" << line->getEnd().x << ", " << line->getEnd().y << ")"
              << ", Thickness: " << line->getThickness()
              << ", Color: 0x" << std::hex << line->getColor() << std::endl;
#endif
```

## Related Documentation

- [Circle Widget](circle.md) - For drawing circles and points
- [Graphics Primitives](../graphics/primitives.md) - For custom drawing
- [Colors System](../graphics/colors.md) - For color management
- [Layout System](../layout/overview.md) - For positioning elements

---

*This documentation covers the Fern Line Widget. For more examples and advanced usage, see the examples directory.*
