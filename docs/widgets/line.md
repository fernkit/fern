# Line Widget Guide

Lines are the fundamental building blocks of digital graphics - the atomic elements from which all complex shapes, borders, connections, and visual structures emerge. In this guide, you'll learn not just how to draw lines in Fern, but understand the fascinating mathematical and algorithmic challenges behind rendering perfect lines on pixel-based displays.

## Understanding Lines in Digital Graphics

Drawing a line on a pixel grid involves solving one of computer graphics' most fundamental problems: how do you represent a continuous mathematical concept (an infinitely thin line) using discrete squares (pixels)? This deceptively simple challenge involves:

1. **Determining which pixels to fill**: Using algorithms like Bresenham's line algorithm to choose the optimal pixel path
2. **Handling thickness**: Creating lines thicker than one pixel while maintaining smooth, consistent appearance
3. **Managing slopes and angles**: Ensuring lines look identical regardless of their orientation or steepness
4. **Dealing with endpoints**: Precisely starting and ending lines at exact coordinates
5. **Anti-aliasing considerations**: Deciding whether to smooth edges or maintain crisp pixel boundaries

Fern implements these solutions using proven computer graphics algorithms, while providing you with an intuitive, easy-to-use interface. When you create a line, you're leveraging decades of research into efficient pixel plotting.

## Line Philosophy in Fern

Lines in Fern follow the principle of **explicit simplicity**:

- **Point-to-Point Definition**: Every line is defined by exactly two coordinates - no curves, no complex paths
- **Uniform Thickness**: Lines maintain consistent width along their entire length
- **Solid Rendering**: Lines are uniformly colored - no gradients, patterns, or transparency effects
- **Non-Interactive by Design**: Lines are pure visual elements that don't respond to mouse events
- **Efficient Implementation**: Optimized for performance with direct pixel manipulation

This approach makes lines perfect for UI frameworks, technical diagrams, borders, dividers, and geometric constructions while keeping the API minimal and predictable.

## Your First Line

Let's start with the most basic line possible:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Draw a horizontal line across the screen
    auto line = Line(Point(50, 100), Point(350, 100), 2, Colors::White);
    
    // Start the game loop
    while (shouldKeepRunning()) {
        processInput();
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `Point(50, 100)`: Starting coordinate (50 pixels from left edge, 100 pixels from top)
- `Point(350, 100)`: Ending coordinate (350 pixels from left edge, same Y position for horizontal line)
- `2`: Line thickness in pixels (the line will be 2 pixels wide)
- `Colors::White`: Line color using Fern's predefined color constants

This creates a crisp, horizontal white line that's 300 pixels long and 2 pixels thick. Behind the scenes, Fern calculates which pixels to fill to create this line using optimized algorithms.

## Understanding Line Mathematics

### Coordinate System and Positioning

Lines in Fern use the standard computer graphics coordinate system:

```cpp
// Screen coordinates: (0,0) at top-left, X increases right, Y increases down
//
//    0   50  100 150 200 250 300 350
//    |    |   |   |   |   |   |   |
// 0--+----+---+---+---+---+---+---+
//    |    |   |   |   |   |   |   |
// 50-+----+---+---+---+---+---+---+
//    |    |   |   |   |   |   |   |
//100-+----●━━━━━━━━━━━━━━━━━━━━━━━●   ← Our line from (50,100) to (350,100)
//    |    |   |   |   |   |   |   |
//150-+----+---+---+---+---+---+---+

// Horizontal line (same Y coordinates)
auto horizontal = Line(Point(100, 150), Point(300, 150), 2, Colors::Red);

// Vertical line (same X coordinates)  
auto vertical = Line(Point(200, 50), Point(200, 250), 2, Colors::Blue);

// Diagonal line (different X and Y)
auto diagonal = Line(Point(100, 100), Point(300, 200), 2, Colors::Green);
```

### Line Algorithms: The Magic Behind the Pixels

When you create a line from (50, 100) to (350, 100), Fern must determine exactly which pixels to color. For this horizontal line, it's straightforward - color all pixels from X=50 to X=350 at Y=100. But for diagonal lines, the challenge becomes much more complex.

**The Bresenham Algorithm (Simplified Explanation):**
```cpp
// For a diagonal line from (0,0) to (8,5), which pixels should be filled?
// The mathematical line passes through fractional coordinates, but pixels are integers.
//
// Y=5 ┌─┬─┬─┬─┬─┬─┬─┬─┬─┐
//   4 ├─┼─┼─┼─┼█┼─┼─┼█┼─┤ ← Algorithm chooses these pixels
//   3 ├─┼─┼─┼█┼─┼─┼█┼─┼─┤   to best approximate the line
//   2 ├─┼─┼█┼─┼─┼█┼─┼─┼─┤
//   1 ├─┼█┼─┼─┼█┼─┼─┼─┼─┤
//   0 ├█┼─┼─┼─┼─┼─┼─┼─┼─┤
//     └─┴─┴─┴─┴─┴─┴─┴─┴─┘
//      0 1 2 3 4 5 6 7 8

// Fern handles all this complexity automatically!
auto complexLine = Line(Point(0, 0), Point(80, 50), 1, Colors::Yellow);
```

## Line Thickness: Beyond Single Pixels

While mathematical lines have no thickness, visual lines must have width to be visible. Fern implements thickness by essentially drawing multiple parallel lines:

```cpp
// Thickness comparison
auto thin = Line(Point(50, 50), Point(300, 50), 1, Colors::White);     // 1 pixel
auto medium = Line(Point(50, 70), Point(300, 70), 3, Colors::White);   // 3 pixels  
auto thick = Line(Point(50, 100), Point(300, 100), 8, Colors::White);  // 8 pixels

// For diagonal lines, thickness becomes more complex
auto thinDiagonal = Line(Point(50, 150), Point(200, 200), 1, Colors::Blue);
auto thickDiagonal = Line(Point(50, 170), Point(200, 220), 6, Colors::Blue);
```

**How Thickness Works:**
- **Horizontal/Vertical lines**: Thickness extends perpendicular to the line direction
- **Diagonal lines**: Thickness is distributed around the mathematical line path
- **Odd thickness** (1, 3, 5): Line is centered on the mathematical path
- **Even thickness** (2, 4, 6): Line may be slightly offset due to pixel grid alignment

## Dynamic Lines: Real-Time Updates

Lines can be modified after creation, enabling animations and responsive interfaces:

```cpp
// Create a line that can be modified
auto dynamicLine = Line(Point(100, 100), Point(200, 100), 2, Colors::Green);

// Animation example: rotating line
double time = getCurrentTime();
double angle = time * 2.0; // 2 radians per second
int centerX = 150, centerY = 150;
int radius = 50;

// Calculate endpoint based on angle
int endX = centerX + cos(angle) * radius;
int endY = centerY + sin(angle) * radius;

// Update the line
dynamicLine->setStart(Point(centerX, centerY));
dynamicLine->setEnd(Point(endX, endY));

// You can also update other properties
dynamicLine->setThickness(3);
dynamicLine->setColor(Colors::Red);
```

## Common Line Patterns and Use Cases

### UI Borders and Frames

```cpp
// Create a complete rectangular border using four lines
class RectangleBorder {
public:
    RectangleBorder(int x, int y, int width, int height, int thickness, uint32_t color) {
        // Top border
        top_ = Line(Point(x, y), Point(x + width, y), thickness, color);
        
        // Right border
        right_ = Line(Point(x + width, y), Point(x + width, y + height), thickness, color);
        
        // Bottom border  
        bottom_ = Line(Point(x + width, y + height), Point(x, y + height), thickness, color);
        
        // Left border
        left_ = Line(Point(x, y + height), Point(x, y), thickness, color);
    }
    
    void setColor(uint32_t color) {
        top_->setColor(color);
        right_->setColor(color);
        bottom_->setColor(color);
        left_->setColor(color);
    }
    
private:
    std::shared_ptr<LineWidget> top_, right_, bottom_, left_;
};

// Usage
RectangleBorder windowBorder(100, 100, 300, 200, 2, Colors::Gray);
```

### Progress Indicators

```cpp
class LinearProgressBar {
public:
    LinearProgressBar(Point start, int maxLength, int thickness, uint32_t bgColor, uint32_t fillColor) 
        : start_(start), maxLength_(maxLength) {
        
        // Background line (full length, dimmed)
        background_ = Line(start, Point(start.x + maxLength, start.y), thickness, bgColor);
        
        // Progress line (starts at zero length)
        progress_ = Line(start, start, thickness, fillColor);
    }
    
    void setProgress(float percent) {
        percent = std::clamp(percent, 0.0f, 1.0f);
        int progressLength = (int)(maxLength_ * percent);
        progress_->setEnd(Point(start_.x + progressLength, start_.y));
    }
    
private:
    std::shared_ptr<LineWidget> background_;
    std::shared_ptr<LineWidget> progress_;
    Point start_;
    int maxLength_;
};

// Usage
LinearProgressBar healthBar(Point(100, 50), 200, 8, Colors::DarkGray, Colors::Green);
healthBar.setProgress(0.75f); // 75% complete
```

### Grid Systems and Technical Diagrams

```cpp
// Create a grid for technical drawings or game boards
class Grid {
public:
    Grid(int startX, int startY, int width, int height, int cellSize, uint32_t color) {
        int thickness = 1;
        
        // Vertical lines
        for (int x = startX; x <= startX + width; x += cellSize) {
            auto vLine = Line(Point(x, startY), Point(x, startY + height), thickness, color);
            lines_.push_back(vLine);
        }
        
        // Horizontal lines
        for (int y = startY; y <= startY + height; y += cellSize) {
            auto hLine = Line(Point(startX, y), Point(startX + width, y), thickness, color);
            lines_.push_back(hLine);
        }
    }
    
    void setColor(uint32_t color) {
        for (auto& line : lines_) {
            line->setColor(color);
        }
    }
    
private:
    std::vector<std::shared_ptr<LineWidget>> lines_;
};

// Usage
Grid blueprint(50, 50, 400, 300, 25, Colors::LightGray);
```

## Advanced Line Techniques

### Simulating Dashed Lines

```cpp
// Create dashed line effect using multiple short segments
class DashedLine {
public:
    DashedLine(Point start, Point end, int dashLength, int gapLength, int thickness, uint32_t color) {
        double totalLength = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
        double dx = (double)(end.x - start.x) / totalLength;
        double dy = (double)(end.y - start.y) / totalLength;
        
        double currentPos = 0;
        bool drawing = true;
        
        while (currentPos < totalLength) {
            int segmentLength = drawing ? dashLength : gapLength;
            
            if (drawing && currentPos + segmentLength <= totalLength) {
                Point segStart(start.x + dx * currentPos, start.y + dy * currentPos);
                Point segEnd(start.x + dx * (currentPos + segmentLength), 
                           start.y + dy * (currentPos + segmentLength));
                
                auto dash = Line(segStart, segEnd, thickness, color);
                segments_.push_back(dash);
            }
            
            currentPos += segmentLength;
            drawing = !drawing;
        }
    }
    
private:
    std::vector<std::shared_ptr<LineWidget>> segments_;
};

// Usage
DashedLine dashedBorder(Point(100, 100), Point(300, 100), 10, 5, 2, Colors::Yellow);
```

### Arrow Creation

```cpp
class Arrow {
public:
    Arrow(Point start, Point end, int thickness, uint32_t color, int arrowSize = 15) {
        // Main line
        mainLine_ = Line(start, end, thickness, color);
        
        // Calculate arrow angle
        double angle = atan2(end.y - start.y, end.x - start.x);
        double arrowAngle = 0.5; // radians
        
        // Arrowhead lines
        Point arrowLeft(
            end.x - arrowSize * cos(angle - arrowAngle),
            end.y - arrowSize * sin(angle - arrowAngle)
        );
        
        Point arrowRight(
            end.x - arrowSize * cos(angle + arrowAngle),
            end.y - arrowSize * sin(angle + arrowAngle)
        );
        
        leftArrow_ = Line(end, arrowLeft, thickness, color);
        rightArrow_ = Line(end, arrowRight, thickness, color);
    }
    
private:
    std::shared_ptr<LineWidget> mainLine_;
    std::shared_ptr<LineWidget> leftArrow_;
    std::shared_ptr<LineWidget> rightArrow_;
};

// Usage
Arrow pointer(Point(100, 100), Point(200, 150), 3, Colors::Red);
```

## Performance and Optimization

### Line Rendering Efficiency

Lines are among the most efficient widgets to render, but quantity still matters:

```cpp
// Efficient: Single thick line
auto efficientBorder = Line(Point(0, 0), Point(800, 0), 5, Colors::White);

// Less efficient: Multiple thin lines for same visual effect
for (int i = 0; i < 5; i++) {
    auto thinLine = Line(Point(0, i), Point(800, i), 1, Colors::White);
}

// Best practice: Use appropriate thickness rather than multiple lines
```

### Memory Management

```cpp
// Lines use automatic memory management
{
    auto temporaryLine = Line(Point(50, 50), Point(200, 200), 2, Colors::Blue);
    // Automatically cleaned up when scope ends
}

// For dynamic line collections
class LineManager {
public:
    void addLine(Point start, Point end, int thickness, uint32_t color) {
        lines_.push_back(Line(start, end, thickness, color));
    }
    
    void clearLines() {
        lines_.clear(); // Automatic cleanup
    }
    
private:
    std::vector<std::shared_ptr<LineWidget>> lines_;
};
```

## Troubleshooting Common Issues

### Lines Not Appearing

**Check these common problems:**

1. **Coordinates outside screen bounds**:
```cpp
// Bad: Line completely off-screen
auto offScreen = Line(Point(-100, -100), Point(-50, -50), 2, Colors::White);

// Good: Ensure at least part of line is on screen
auto onScreen = Line(Point(10, 10), Point(200, 100), 2, Colors::White);
```

2. **Color matches background**:
```cpp
// Invisible: White line on white background
auto invisible = Line(Point(50, 50), Point(200, 50), 2, Colors::White); // On white background

// Visible: Use contrasting colors
auto visible = Line(Point(50, 50), Point(200, 50), 2, Colors::Black);
```

3. **Zero-length lines**:
```cpp
// Won't show: Same start and end points
auto zeroLength = Line(Point(100, 100), Point(100, 100), 2, Colors::Red);

// Will show: Different endpoints
auto hasLength = Line(Point(100, 100), Point(150, 100), 2, Colors::Red);
```

### Thickness and Quality Issues

**Understanding thickness limitations:**

```cpp
// Very thick lines may appear pixelated or uneven
auto veryThick = Line(Point(50, 50), Point(200, 100), 20, Colors::Blue);

// For very thick "lines", consider using rectangles instead:
// Rectangle widgets might render better for thickness > 10 pixels

// Diagonal thick lines can show stepping artifacts
auto thickDiagonal = Line(Point(50, 50), Point(200, 200), 8, Colors::Green);
// This is normal - it's an inherent limitation of pixel-based rendering
```

### Performance with Many Lines

```cpp
// Potential performance issue: Thousands of lines
std::vector<std::shared_ptr<LineWidget>> manyLines;
for (int i = 0; i < 10000; i++) {
    manyLines.push_back(Line(Point(i%800, 50), Point(i%800, 100), 1, Colors::White));
}

// Better: Consider using custom drawing for large quantities
// Or use techniques like line culling (only draw visible lines)
```

## Summary

Lines in Fern provide the foundation for structured, geometric interfaces. Understanding their mathematical basis, performance characteristics, and visual properties enables you to create clean, professional interfaces that communicate clearly through visual structure.

Key takeaways:
- **Mathematical foundation**: Lines use proven algorithms like Bresenham's to map continuous math to discrete pixels
- **Simple API**: Point-to-point definition with configurable thickness and color
- **Dynamic capabilities**: All properties can be updated in real-time for animations
- **Performance efficient**: Generally fast to render, but quantity impacts performance
- **Visual building blocks**: Essential for borders, dividers, grids, progress bars, and geometric shapes
- **Non-interactive by design**: Pure visual elements that don't handle user input

Lines showcase Fern's philosophy perfectly: simple concepts with deep technical implementation, exposed through clean APIs that let you focus on creating rather than fighting with complexity. Master lines, and you'll have precise control over the visual structure and flow of your applications.

## Understanding Line Coordinates

Lines are defined by two points in screen space. Understanding coordinate relationships helps with positioning:

```cpp
// Horizontal line (same Y coordinates)
auto horizontal = Line(Point(100, 150), Point(300, 150), 2, Colors::Red);

// Vertical line (same X coordinates)  
auto vertical = Line(Point(200, 50), Point(200, 250), 2, Colors::Blue);

// Diagonal line (different X and Y)
auto diagonal = Line(Point(100, 100), Point(300, 200), 2, Colors::Green);
```

**Visual Understanding:**
```
Screen coordinates (0,0) at top-left

   100   200   300
    |     |     |
50--+-----●-----+    ← vertical line starts
    |     |     |
100-●-----------+    ← horizontal line starts  
    |     |     |
150-●-----------●    ← horizontal line ends
    |    / \    |
200-+---/   \---●    ← diagonal line ends
    |  /     \  |
250-+-/-------\-+    ← vertical line ends
```

## Line Thickness and Appearance

Line thickness affects both visual impact and performance:

```cpp
// Thin line - minimal visual impact, fast rendering
auto thin = Line(Point(50, 50), Point(250, 50), 1, Colors::Gray);

// Medium line - good balance of visibility and performance
auto medium = Line(Point(50, 100), Point(250, 100), 3, Colors::Blue);

// Thick line - high visual impact, slower rendering
auto thick = Line(Point(50, 150), Point(250, 150), 8, Colors::Red);
```

**Thickness Guidelines:**
- **1-2 pixels**: Subtle dividers, grid lines, technical diagrams
- **3-5 pixels**: UI borders, emphasis lines, navigation elements
- **6+ pixels**: Decorative elements, progress bars, bold graphics

## Dynamic Lines: Animation and Updates

Lines can be modified after creation, enabling animations and interactive graphics:

```cpp
// Create a line that grows over time
auto growingLine = Line(Point(100, 100), Point(100, 100), 2, Colors::Green);

// In your game loop:
double time = getCurrentTime();
int endX = 100 + (sin(time) + 1) * 100; // X position oscillates
growingLine->setEnd(Point(endX, 100));
```

### Rotating Lines

```cpp
// Line that rotates around a center point
auto rotatingLine = Line(Point(200, 200), Point(250, 200), 3, Colors::Yellow);

// In your game loop:
double time = getCurrentTime();
double angle = time * 2; // 2 radians per second
int centerX = 200, centerY = 200;
int radius = 50;

int endX = centerX + cos(angle) * radius;
int endY = centerY + sin(angle) * radius;
rotatingLine->setEnd(Point(endX, endY));
```

### Color Transitions

```cpp
// Line that changes color based on some condition
auto statusLine = Line(Point(50, 50), Point(200, 50), 4, Colors::Green);

void updateSystemStatus(float load) {
    if (load < 0.5f) {
        statusLine->setColor(Colors::Green);
    } else if (load < 0.8f) {
        statusLine->setColor(Colors::Yellow);
    } else {
        statusLine->setColor(Colors::Red);
    }
}
```

## Common Line Patterns

### Borders and Frames

```cpp
// Create a rectangular border using four lines
void createBorder(int x, int y, int width, int height, int thickness, uint32_t color) {
    // Top border
    auto top = Line(Point(x, y), Point(x + width, y), thickness, color);
    
    // Bottom border
    auto bottom = Line(Point(x, y + height), Point(x + width, y + height), thickness, color);
    
    // Left border
    auto left = Line(Point(x, y), Point(x, y + height), thickness, color);
    
    // Right border
    auto right = Line(Point(x + width, y), Point(x + width, y + height), thickness, color);
}

// Usage
createBorder(100, 100, 200, 150, 2, Colors::White);
```

### Dividers and Separators

```cpp
// Horizontal divider between sections
auto divider = Line(Point(50, 200), Point(350, 200), 1, Colors::Gray);

// Vertical separator between columns
auto separator = Line(Point(200, 50), Point(200, 300), 1, Colors::LightGray);
```

### Grid Systems

```cpp
// Create a grid using lines
void createGrid(int startX, int startY, int width, int height, int cellSize, uint32_t color) {
    int thickness = 1;
    
    // Vertical lines
    for (int x = startX; x <= startX + width; x += cellSize) {
        auto vLine = Line(Point(x, startY), Point(x, startY + height), thickness, color);
    }
    
    // Horizontal lines
    for (int y = startY; y <= startY + height; y += cellSize) {
        auto hLine = Line(Point(startX, y), Point(startX + width, y), thickness, color);
    }
}

// Usage - 10x10 grid with 20-pixel cells
createGrid(100, 100, 200, 200, 20, Colors::DarkGray);
```

## Connection Lines

Lines excel at showing relationships between UI elements:

```cpp
// Connect two widgets with a line
void connectWidgets(Point widget1Center, Point widget2Center, uint32_t color) {
    auto connection = Line(widget1Center, widget2Center, 2, color);
}

// Curved connection (simulated with multiple short lines)
void createCurvedConnection(Point start, Point end, uint32_t color) {
    int segments = 10;
    std::vector<Point> points;
    
    // Calculate curve points (simple quadratic curve)
    Point control = Point((start.x + end.x) / 2, start.y - 50); // Control point above
    
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*start.x + 2*(1-t)*t*control.x + t*t*end.x;
        float y = (1-t)*(1-t)*start.y + 2*(1-t)*t*control.y + t*t*end.y;
        points.push_back(Point((int)x, (int)y));
    }
    
    // Create lines between consecutive points
    for (size_t i = 0; i < points.size() - 1; i++) {
        auto segment = Line(points[i], points[i+1], 2, color);
    }
}
```

## Lines in Layouts

While lines are positioned absolutely, they can work with layout systems:

```cpp
// Lines as decorative elements in containers
auto container = Column({
    Text(TextPresets::Title(0, 0, "Section 1")),
    Line(Point(0, 0), Point(200, 0), 1, Colors::Gray), // Horizontal divider
    Text(TextPresets::Body(0, 0, "Content for section 1")),
    
    Text(TextPresets::Title(0, 0, "Section 2")),
    Line(Point(0, 0), Point(200, 0), 1, Colors::Gray), // Another divider
    Text(TextPresets::Body(0, 0, "Content for section 2"))
});
```

**Note**: When using lines in layouts, consider the line endpoints relative to the container's coordinate system.

## Advanced Line Techniques

### Line Patterns (Dashed Lines)

```cpp
// Simulate dashed lines with multiple short lines
void createDashedLine(Point start, Point end, int dashLength, int gapLength, 
                     int thickness, uint32_t color) {
    
    int totalLength = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
    float dx = (float)(end.x - start.x) / totalLength;
    float dy = (float)(end.y - start.y) / totalLength;
    
    int currentPos = 0;
    bool drawing = true;
    
    while (currentPos < totalLength) {
        int segmentLength = drawing ? dashLength : gapLength;
        
        if (drawing) {
            Point segStart = Point(start.x + dx * currentPos, start.y + dy * currentPos);
            Point segEnd = Point(start.x + dx * (currentPos + segmentLength), 
                               start.y + dy * (currentPos + segmentLength));
            auto dash = Line(segStart, segEnd, thickness, color);
        }
        
        currentPos += segmentLength;
        drawing = !drawing;
    }
}
```

### Interactive Line Tools

```cpp
// Line drawing tool that follows mouse
class LineTool {
public:
    LineTool(uint32_t color, int thickness) 
        : isDrawing_(false), color_(color), thickness_(thickness) {}
    
    void startLine(Point start) {
        if (!isDrawing_) {
            startPoint_ = start;
            currentLine_ = Line(start, start, thickness_, color_);
            isDrawing_ = true;
        }
    }
    
    void updateLine(Point current) {
        if (isDrawing_) {
            currentLine_->setEnd(current);
        }
    }
    
    void finishLine() {
        isDrawing_ = false;
        // Line is already added to the scene
    }
    
private:
    bool isDrawing_;
    Point startPoint_;
    std::shared_ptr<LineWidget> currentLine_;
    uint32_t color_;
    int thickness_;
};
```

## Performance Considerations

### Line Rendering Efficiency

Lines are generally efficient to render, but considerations include:

```cpp
// Efficient: Few thick lines
auto efficientBorder = Line(Point(0, 0), Point(800, 0), 5, Colors::White);

// Less efficient: Many thin lines for the same effect
for (int i = 0; i < 5; i++) {
    auto thinLine = Line(Point(0, i), Point(800, i), 1, Colors::White);
}
```

### Memory Management

```cpp
// Lines are automatically managed
auto line = Line(Point(50, 50), Point(200, 200), 3, Colors::Blue);
// Automatic cleanup when reference goes out of scope

// For temporary drawing effects:
{
    auto tempLine = Line(Point(100, 100), Point(200, 200), 2, Colors::Red);
    // Automatically cleaned up
}
```

## Mathematical Line Concepts

Understanding line mathematics helps with advanced positioning:

### Length and Distance

```cpp
// Calculate line length
double lineLength(Point start, Point end) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    return sqrt(dx*dx + dy*dy);
}

// Create line of specific length
Point createEndpoint(Point start, double angle, double length) {
    int endX = start.x + cos(angle) * length;
    int endY = start.y + sin(angle) * length;
    return Point(endX, endY);
}
```

### Slope and Angle

```cpp
// Calculate line angle
double lineAngle(Point start, Point end) {
    return atan2(end.y - start.y, end.x - start.x);
}

// Create perpendicular line
Point perpendicularEnd(Point start, Point end, double length) {
    double angle = lineAngle(start, end) + M_PI/2; // 90 degrees
    return createEndpoint(start, angle, length);
}
```

## Troubleshooting Common Issues

### Lines Not Appearing

**Check these common issues:**

1. **Points outside screen bounds**:
```cpp
// Bad: Line completely off-screen
auto offScreen = Line(Point(-100, -100), Point(-50, -50), 2, Colors::White);

// Good: Ensure at least part of line is visible
auto visible = Line(Point(0, 100), Point(200, 100), 2, Colors::White);
```

2. **Color matches background**:
```cpp
// Invisible line on same-color background
auto invisible = Line(Point(50, 50), Point(200, 50), 2, Colors::Black); // On black background

// Use contrasting colors
auto visible = Line(Point(50, 50), Point(200, 50), 2, Colors::White);
```

3. **Zero-length lines**:
```cpp
// Line with same start and end points won't be visible
auto zeroLength = Line(Point(100, 100), Point(100, 100), 2, Colors::Red);

// Ensure different endpoints
auto hasLength = Line(Point(100, 100), Point(200, 100), 2, Colors::Red);
```

### Thickness Issues

```cpp
// Very thick lines might look pixelated
auto veryThick = Line(Point(50, 50), Point(200, 50), 20, Colors::Blue);

// Consider using rectangles for very thick "lines"
// Rectangle widgets might render better for thickness > 10
```

### Performance Problems

```cpp
// Too many lines can impact performance
std::vector<std::shared_ptr<LineWidget>> manyLines;
for (int i = 0; i < 10000; i++) {
    manyLines.push_back(Line(Point(i, 50), Point(i, 100), 1, Colors::White));
}

// Consider: Single custom widget with batch rendering
```

## Summary

Lines in Fern provide a fundamental building block for creating structured, connected interfaces. Whether you're building borders, dividers, progress indicators, or geometric art, understanding line positioning, thickness, and performance characteristics enables you to create clean, professional-looking interfaces.

Key takeaways:
- **Point-to-point definition**: Lines connect two specific coordinates
- **Thickness control**: Configurable width for different visual impacts
- **Dynamic updates**: Start, end, thickness, and color can be changed during runtime
- **Non-interactive**: Lines are visual elements that don't respond to user input
- **Performance efficient**: Generally fast to render, but quantity matters
- **Mathematical foundation**: Understanding angles, length, and positioning helps with complex layouts

Lines exemplify Fern's approach: simple concepts that combine to create sophisticated interfaces. Master lines, and you'll have precise control over the visual structure and flow of your applications.

