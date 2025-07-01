# Fern Graphics C++ Library Documentation

## Introduction

Fern Graphics is a modern, lightweight graphics and UI library for creating interactive applications, games, and visualizations. Built with C++14, it provides a flexible API for drawing primitives, handling user input, and creating responsive user interfaces.

## Key Features

- **Immediate-mode drawing API** for graphics primitives and text
- **Widget-based UI system** with built-in components like buttons, text, and shapes
- **Layout engine** inspired by Flutter for responsive interface design
- **Event handling system** with signals and slots for interactive applications
- **Cross-platform compatibility** targeting web (via WebAssembly) and native platforms
- **Simple API** designed for both beginners and experienced developers

## Getting Started

### Installation

```bash
git clone https://github.com/RishiAhuja/fern.git
cd fern
mkdir build && cd build
cmake ..
make
```

### Basic Structure of a Fern Application

```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void draw() {
    // Clear the screen
    Draw::fill(Colors::DarkGray);
    
    // Draw some shapes
    Draw::circle(400, 300, 100, Colors::Blue);
    Draw::rect(100, 100, 200, 150, Colors::Red);
    
    // Draw text
    Draw::text("HELLO, FERN GRAPHICS!", 300, 50, 2, Colors::White);
}

int main() {
    // Initialize the library
    Fern::initialize();
    
    // Set the main drawing function
    Fern::setDrawCallback(draw);
    
    // Start the render loop
    Fern::startRenderLoop();
    
    return 0;
}
```

## Core Concepts

### Coordinate System

Fern uses a standard 2D coordinate system where:
- (0,0) is at the top-left corner
- X increases to the right
- Y increases downward

### Colors

Colors are represented as 32-bit RGBA values (0xAARRGGBB). The `Colors` namespace provides predefined constants:

```cpp
Draw::circle(100, 100, 50, Colors::Red);     // Predefined color
Draw::rect(200, 200, 100, 50, 0xFF00FF00);   // Custom color (ARGB format)
```

### Points and Geometry

```cpp
Point p1(100, 100);               // Create a point
Point p2 = p1 + Point(50, 20);    // Vector addition

// Use points with drawing functions
Draw::line(p1.x, p1.y, p2.x, p2.y, 2, Colors::White);
```

## Drawing API

Fern provides a comprehensive set of drawing primitives through the `Draw` namespace:

```cpp
// Basic shapes
Draw::rect(x, y, width, height, color);
Draw::circle(x, y, radius, color);
Draw::line(x1, y1, x2, y2, thickness, color);
Draw::pixel(x, y, color);
Draw::triangle(x1, y1, x2, y2, x3, y3, color);

// Text rendering
Draw::text(text, x, y, scale, color);

// Screen clearing
Draw::fill(color);
```

## Widget System

Fern includes a widget-based UI system for creating interactive interfaces:

### Base Widget Class

All UI elements inherit from the Widget base class, which provides:
- Positioning and sizing information
- Input handling capabilities
- Rendering functionality

### Widget Management

> **Important Change**: By default, widgets are NOT automatically added to the WidgetManager unless explicitly requested. This design choice optimizes performance and better reflects how widgets are typically used in layout hierarchies.

```cpp
// Root widget: add to widget manager with true
auto rootContainer = Container(Colors::Black, 0, 0, width, height, nullptr, true);

// Child widgets: use default (false) as they're managed by their parent
auto childText = Text(Point(0, 0), "CHILD TEXT", 2, Colors::White);
rootContainer->setChild(childText);
```

When to use `addToManager = true`:
- For root/standalone widgets that aren't children of other widgets
- When you need direct access to a widget through the WidgetManager

When to use `addToManager = false` (default):
- For any widget that is a child of another widget
- When building complex layouts (most use cases)

### Available Widgets

#### Container Widget

```cpp
// Create a container with color, position, size, child widget, and widget management flag
auto container = Container(
    Colors::DarkBlue,        // Container color
    100, 100,                // Position (x, y)
    300, 200,                // Size (width, height)
    nullptr,                 // Child widget (optional)
    false                    // Add to widget manager (defaults to false)
);

// Add a child later
container->setChild(Text(Point(0, 0), "HELLO WORLD", 2, Colors::White));
```

The Container widget can hold a single child widget and provides:
- Background color or gradient
- Automatic sizing of child widgets to match container dimensions
- Layout management for child layout widgets

#### Text Widget

```cpp
// Create text at position (100,100) with font scale 2
auto myText = Text(Point(100, 100), "HELLO WORLD", 2, Colors::White);

// Update text content later
myText->setText("UPDATED TEXT");
```

##### TTF Font Support (Experimental)

Fern Graphics C++ now includes experimental support for TrueType Font (TTF) rendering, allowing you to use custom fonts in your applications.

**⚠️ Note**: TTF font support is currently experimental and may have limitations. For production applications, consider using the default bitmap font system.

```cpp
// Load a TTF font
bool fontLoaded = TTF::load("myFont", "path/to/font.ttf");
if (fontLoaded) {
    TTF::setDefault("myFont");
}

// Create text with TTF font
auto ttfText = Text(Point(100, 100), "Custom Font Text", 48, Colors::Blue, true, FontType::TTF);

// Or specify font explicitly
auto ttfText2 = Text(Point(100, 200), "Another Text", 32, Colors::Red, true, FontType::TTF);
```

**TTF Font Loading**:
- Use `TTF::load(name, path)` to load a font file
- Set a default TTF font with `TTF::setDefault(name)`
- Supports .ttf font files with simple glyph outlines
- Font files should be accessible at runtime (use `--embed-file` with Emscripten)

**Text Rendering with TTF**:
- Add `FontType::TTF` parameter to Text constructor
- Font size is specified in points (typical values: 12, 16, 24, 48, etc.)
- All standard colors and positioning work as expected

**Current Limitations**:
- Composite glyphs are not yet supported
- Complex scripts and advanced typography features are not implemented
- Performance is not optimized for large amounts of text
- Font hinting and subpixel rendering are not available

**WebAssembly Font Embedding**:
When building for WebAssembly, embed font files using:
```bash
fern --cpp --embed-file fonts yourapp.cpp
```

#### Button Widget

```cpp
// Define button configuration
ButtonConfig btnConfig = {
    .x = 100,
    .y = 200,
    .width = 200,
    .height = 50,
    .normalColor = Colors::Blue,
    .hoverColor = Colors::LightBlue,
    .pressColor = Colors::DarkBlue,
    .label = "CLICK ME",
    .textScale = 2,
    .textColor = Colors::White
};

// Create button from config
auto myButton = Button(btnConfig);

// Connect click handler
myButton->onClick.connect([]() {
    std::cout << "Button clicked!" << std::endl;
});
```

#### Circle Widget

```cpp
// Create circle with radius 30 at (200,200)
auto myCircle = Circle(30, Point(200, 200), Colors::Red);

// Modify properties
myCircle->setRadius(40);
myCircle->setColor(Colors::Green);
```

#### Line Widget

```cpp
// Create line from (100,100) to (300,200)
auto myLine = Line(Point(100, 100), Point(300, 200), 3, Colors::Yellow);

// Update end points
myLine->setStart(Point(150, 150));
myLine->setEnd(Point(350, 250));
```

## Layout System

Fern provides a powerful layout system inspired by Flutter for creating responsive UIs:

### Column Layout

Arranges widgets vertically:

```cpp
// Default is not to add to widget manager (addToManager = false)
auto column = Column({
    Text(Point(0, 0), "TITLE", 3, Colors::White),
    Circle(50, Point(0, 0), Colors::Blue),
    Text(Point(0, 0), "Bottom Text", 2, Colors::Green)
});

// To add the column to widget manager directly:
auto managedColumn = Column({
    Text(Point(0, 0), "TITLE", 3, Colors::White),
    Circle(50, Point(0, 0), Colors::Blue),
    Text(Point(0, 0), "Bottom Text", 2, Colors::Green)
}, true);  // explicitly set addToManager = true
```

### Row Layout

Arranges widgets horizontally:

```cpp
auto row = Row({
    Text(Point(0, 0), "LEFT", 2, Colors::White),
    Circle(30, Point(0, 0), Colors::Blue),
    Text(Point(0, 0), "RIGHT", 2, Colors::Green)
}, false, MainAxisAlignment::SpaceBetween);  // false is default, shown for clarity
```

### Expanded Layout

Makes a child widget expand to fill available space:

```cpp
auto layout = Row({
    // Fixed width container
    Container(Colors::Red, 0, 0, 100, 0, false),
    
    // This will expand to fill remaining space
    Expanded(
        Container(Colors::Blue, 0, 0, 0, 0, false),
        1  // Flex factor
    ),
    
    // This will take twice as much space as the previous expanded widget
    Expanded(
        Container(Colors::Green, 0, 0, 0, 0, false),
        2  // Flex factor (2x)
    )
}, false);
```

### Padding Layout

Adds padding around a child widget:

```cpp
auto paddedText = Padding(
    Text(Point(0, 0), "PADDED TEXT", 2, Colors::White, false),
    15,  // 15 pixels of padding on all sides
    false
);
```
### Responsive Layout Example

```cpp
// Create a responsive layout with expanded sections
void setupResponsiveUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    addWidget(
        Container(
            Colors::Black,
            0, 0, width, height,
            false,
            Column({
                // Fixed height header
                Container(
                    Colors::DarkBlue,
                    0, 0, 0, 80,
                    false,
                    Center(Text(Point(0, 0), "HEADER", 2, Colors::White, false), false)
                ),
                
                // Content area takes all remaining vertical space
                Expanded(
                    Row({
                        // Sidebar takes 1 part of horizontal space
                        Expanded(
                            Container(Colors::DarkGray, 0, 0, 0, 0, false),
                            1
                        ),
                        
                        // Main content takes 3 parts of horizontal space
                        Expanded(
                            Container(Colors::Black, 0, 0, 0, 0, false),
                            3
                        )
                    }, false),
                    1
                ),
                
                // Fixed height footer
                Container(
                    Colors::DarkGray,
                    0, 0, 0, 60,
                    false,
                    Center(Text(Point(0, 0), "FOOTER", 2, Colors::White, false), false)
                )
            }, false)
        )
    );
}

```



### Creating Responsive UIs

```cpp
void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    auto mainLayout = Column({
        Text(Point(0, 0), "RESPONSIVE LAYOUT", 3, Colors::White, false),
        Circle(std::min(width, height)/10, Point(0, 0), Colors::Blue, false),
        Button({
            .width = width/2,
            .height = 50,
            .normalColor = Colors::Green,
            .label = "RESIZE ME"
        }, false)
    });
}

void draw() {
    static int lastWidth = 0;
    static int lastHeight = 0;
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Rebuild UI when size changes
    if (width != lastWidth || height != lastHeight) {
        WidgetManager::getInstance().clear();
        setupUI();
        
        lastWidth = width;
        lastHeight = height;
    }
}
```

## Input Handling

### Getting Input State

```cpp
void update() {
    const auto& input = Input::getState();
    
    // Check mouse position
    int mouseX = input.mouseX;
    int mouseY = input.mouseY;
    
    // Check mouse buttons
    bool leftMouseDown = input.leftMouseDown;
    
}
```

Key Inputs are not yet implemented.

### Working with Signals

Fern uses a signal-slot system for event handling:

```cpp
// Define a signal
Signal<void(int, int)> onPositionChanged;

// Connect a handler
onPositionChanged.connect([](int x, int y) {
    std::cout << "Position changed to: " << x << ", " << y << std::endl;
});

// Emit the signal
onPositionChanged.emit(100, 200);
```

## Advanced Examples

### Interactive UI Dashboard

```cpp
std::shared_ptr<TextWidget> statusText;
std::shared_ptr<CircleWidget> indicator;
int counter = 0;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    auto mainLayout = Column({
        Text(Point(0, 0), "INTERACTIVE DASHBOARD", 3, Colors::White, false),
        
        Row({
            Button({
                .width = 150,
                .height = 50,
                .normalColor = Colors::Red,
                .label = "DECREASE",
                .onClick = []() {
                    counter--;
                    statusText->setText("Counter: " + std::to_string(counter));
                    indicator->setRadius(20 + counter);
                }
            }, false),
            
            SizedBox(20, 0, false),  // Spacing
            
            Button({
                .width = 150,
                .height = 50,
                .normalColor = Colors::Green,
                .label = "INCREASE",
                .onClick = []() {
                    counter++;
                    statusText->setText("Counter: " + std::to_string(counter));
                    indicator->setRadius(20 + counter);
                }
            }, false)
        }, false),
        
        indicator = Circle(20, Point(0, 0), Colors::Blue, false),
        
        statusText = Text(Point(0, 0), "COUNTER: 0", 2, Colors::Yellow, false)
    });
}
```

## Troubleshooting

### Common Issues and Solutions

#### Widgets Not Appearing

**Problem**: You've created widgets but they don't show up on screen.

**Solutions**:
- Remember that widgets are NOT added to the WidgetManager by default (addToManager = false)
- For root widgets, make sure to set addToManager = true
- Check if positions are within the visible canvas area
- Verify that the draw callback is properly registered
- Make sure the widget's color has appropriate alpha value (not transparent)

#### Layout Issues

**Problem**: Widgets in layouts aren't positioning correctly.

**Solutions**:
- Remember that child widgets in layouts should use the default addToManager = false
- Make sure the parent/root layout is added to the widget manager (addToManager = true)
- Check widget size calculations (getWidth/getHeight)
- Verify that the layout dimensions are properly set


#### Issues with Expanded Widgets

**Problem**: Expanded widgets not taking up the expected space or causing layout issues.

**Solutions**:
- Ensure the parent widget has a defined size or is itself in an Expanded widget
- Check that flex factors are properly set (default is 1)
- Verify that child widgets of Expanded have flexible dimensions (usually 0)
- When nesting Expanded widgets, ensure each level has proper constraints

#### Performance Issues

**Problem**: Application runs slowly, especially with many widgets.

**Solutions**:
- Minimize widget recreation each frame
- Use static or cached values where possible
- Consider using simpler drawing primitives for background elements
- Profile your application to identify bottlenecks

## Performance Optimization

### Render Optimization

```cpp
// Instead of:
void draw() {
    // Draw complex background every frame
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Complex calculation
            Draw::pixel(x, y, calculateColor(x, y));
        }
    }
}

// Better approach:
uint32_t* backgroundBuffer = nullptr;

void initializeBackground(int width, int height) {
    // Create once
    backgroundBuffer = new uint32_t[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            backgroundBuffer[y * width + x] = calculateColor(x, y);
        }
    }
}

void draw() {
    // Blit pre-calculated background
    Draw::image(0, 0, width, height, backgroundBuffer);
}
```

### Widget Optimization

```cpp
// Avoid recreating widgets unnecessarily

// Bad:
void draw() {
    Text(Point(100, 100), "Hello", 2, Colors::White); // Creates new widget every frame
}

// Good:
std::shared_ptr<TextWidget> helloText;

void setupUI() {
    helloText = Text(Point(100, 100), "Hello", 2, Colors::White);
}

void draw() {
    // Just use existing widget
}
```

## Advanced Topics

### Custom Widgets

Create your own widgets by inheriting from the Widget base class:

```cpp
class GaugeWidget : public Widget {
public:
    GaugeWidget(int x, int y, int width, int height, float minValue, float maxValue)
        : minValue_(minValue), maxValue_(maxValue), currentValue_(minValue) {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
    }
    
    void setValue(float value) {
        currentValue_ = std::max(minValue_, std::min(maxValue_, value));
    }
    
    void render() override {
        // Background
        Draw::rect(x_, y_, width_, height_, Colors::DarkGray);
        
        // Fill based on current value
        float fillRatio = (currentValue_ - minValue_) / (maxValue_ - minValue_);
        int fillWidth = static_cast<int>(width_ * fillRatio);
        Draw::rect(x_, y_, fillWidth, height_, fillColor_);
        
        // Border
        Draw::rect(x_, y_, width_, height_, Colors::White, false);
        
        // Value text
        std::string valueText = std::to_string(static_cast<int>(currentValue_));
        int textX = x_ + width_ / 2;
        int textY = y_ + height_ / 2;
        Draw::text(valueText.c_str(), textX, textY, 1, Colors::White);
    }
    
    bool handleInput(const InputState& input) override {
        return false;  // No input handling for this widget
    }
    
    void setFillColor(uint32_t color) { fillColor_ = color; }
    
private:
    float minValue_;
    float maxValue_;
    float currentValue_;
    uint32_t fillColor_ = Colors::Blue;
};

// Usage:
auto gauge = std::make_shared<GaugeWidget>(100, 100, 200, 30, 0, 100);
gauge->setValue(75);
gauge->setFillColor(Colors::Green);
addWidget(gauge);
```

## Contributing to Fern Graphics

We welcome contributions to the Fern Graphics library! Here's how to get involved:

1. **Fork** the repository on GitHub
2. **Clone** your fork to your local machine
3. **Create a branch** for your feature or bugfix
4. **Make your changes**, following coding standards and adding tests
5. **Push your branch** to your fork on GitHub
6. **Submit a pull request** from your branch to the main repository

### Coding Standards

- Use modern C++ features (C++14)
- Follow consistent indentation (4 spaces)
- Use clear, descriptive names for functions and variables
- Document public APIs with comments
- Add tests for new functionality

## Future Development

The Fern Graphics team is actively working on these upcoming features:

- Additional layout options (Grid, Stack. already implemented Row, Column, and Expanded)
- Animation system for transitions and effects
- Theme support for consistent styling
- Advanced input handling (multi-touch, gestures)
- Enhanced text rendering capabilities
- More widget types (sliders, dropdowns, etc.)

---

