# Primitives Drawing

The Fern Primitives Drawing system provides low-level drawing functions for rendering basic shapes and forms directly to the canvas. These functions are the foundation for all visual elements in the Fern UI framework.

## Overview

The `Draw` namespace contains functions for drawing geometric primitives including rectangles, circles, lines, and filled shapes. All drawing operations work directly with the global canvas buffer and support colors with alpha transparency.

## API Reference

### Basic Shapes

#### Fill Screen
```cpp
void fill(uint32_t color);
```
Fills the entire canvas with a solid color.

#### Rectangle
```cpp
void rect(int x, int y, int width, int height, uint32_t color);
```
Draws a filled rectangle at the specified position and size.

#### Circle
```cpp
void circle(int cx, int cy, int radius, uint32_t color);
```
Draws a filled circle with center at (cx, cy) and the specified radius.

#### Line
```cpp
void line(int x1, int y1, int x2, int y2, int thickness, uint32_t color);
```
Draws a line from (x1, y1) to (x2, y2) with the specified thickness.

### Advanced Shapes

#### Rounded Rectangle
```cpp
void roundedRect(int x, int y, int width, int height, int radius, uint32_t color);
```
Draws a filled rectangle with rounded corners.

#### Rounded Rectangle Border
```cpp
void roundedRectBorder(int x, int y, int width, int height, int radius, int borderWidth, uint32_t color);
```
Draws a border around a rounded rectangle with the specified border width.

## Basic Usage

### Drawing Simple Shapes
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Primitives Example");
    
    while (isRunning()) {
        // Clear screen with white background
        Draw::fill(Colors::White);
        
        // Draw a red rectangle
        Draw::rect(100, 100, 200, 150, Colors::Red);
        
        // Draw a blue circle
        Draw::circle(400, 200, 50, Colors::Blue);
        
        // Draw a green line
        Draw::line(50, 50, 300, 300, 3, Colors::Green);
        
        present();
    }
    
    return 0;
}
```

### Rounded Shapes
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Rounded Shapes Example");
    
    while (isRunning()) {
        Draw::fill(Colors::LightGray);
        
        // Draw rounded rectangle
        Draw::roundedRect(100, 100, 200, 100, 20, Colors::Blue);
        
        // Draw rounded rectangle with border
        Draw::roundedRectBorder(350, 100, 200, 100, 15, 5, Colors::Red);
        
        present();
    }
    
    return 0;
}
```

## Advanced Usage

### Creating Custom Shapes
```cpp
// Draw a simple house using primitives
void drawHouse(int x, int y, int size, uint32_t color) {
    // House base
    Draw::rect(x, y + size/2, size, size/2, color);
    
    // Roof (triangle using multiple lines)
    int roofHeight = size/3;
    for (int i = 0; i < roofHeight; i++) {
        int lineWidth = (size * i) / roofHeight;
        Draw::line(x + size/2 - lineWidth/2, y + i, 
                  x + size/2 + lineWidth/2, y + i, 1, Colors::Brown);
    }
    
    // Door
    Draw::rect(x + size/3, y + size/2 + size/4, size/6, size/4, Colors::Brown);
    
    // Window
    Draw::circle(x + 2*size/3, y + size/2 + size/6, size/12, Colors::LightBlue);
}
```

### Pattern Drawing
```cpp
// Draw a grid pattern
void drawGrid(int spacing, uint32_t color) {
    int width = getWidth();
    int height = getHeight();
    
    // Vertical lines
    for (int x = 0; x < width; x += spacing) {
        Draw::line(x, 0, x, height, 1, color);
    }
    
    // Horizontal lines
    for (int y = 0; y < height; y += spacing) {
        Draw::line(0, y, width, y, 1, color);
    }
}
```

### Gradient Effects
```cpp
// Create a gradient effect using multiple rectangles
void drawGradient(int x, int y, int width, int height, 
                 uint32_t startColor, uint32_t endColor) {
    for (int i = 0; i < height; i++) {
        float t = (float)i / height;
        uint32_t color = Colors::blendColors(startColor, endColor, t);
        Draw::rect(x, y + i, width, 1, color);
    }
}
```

## Common Patterns

### 1. UI Elements
```cpp
// Custom button with border
void drawButton(int x, int y, int width, int height, 
               uint32_t bgColor, uint32_t borderColor, int borderWidth) {
    // Background
    Draw::roundedRect(x, y, width, height, 5, bgColor);
    
    // Border
    Draw::roundedRectBorder(x, y, width, height, 5, borderWidth, borderColor);
}

// Progress bar
void drawProgressBar(int x, int y, int width, int height, 
                    float progress, uint32_t bgColor, uint32_t fgColor) {
    // Background
    Draw::roundedRect(x, y, width, height, height/2, bgColor);
    
    // Progress
    int progressWidth = width * progress;
    if (progressWidth > 0) {
        Draw::roundedRect(x, y, progressWidth, height, height/2, fgColor);
    }
}
```

### 2. Decorative Elements
```cpp
// Draw a star shape
void drawStar(int cx, int cy, int radius, uint32_t color) {
    const int points = 5;
    const float angle = 2.0f * M_PI / points;
    
    for (int i = 0; i < points; i++) {
        float a1 = i * angle;
        float a2 = (i + 2) * angle;
        
        int x1 = cx + cos(a1) * radius;
        int y1 = cy + sin(a1) * radius;
        int x2 = cx + cos(a2) * radius;
        int y2 = cy + sin(a2) * radius;
        
        Draw::line(x1, y1, x2, y2, 2, color);
    }
}

// Draw a simple chart
void drawBarChart(int x, int y, int width, int height, 
                 const std::vector<float>& values, uint32_t color) {
    int barWidth = width / values.size();
    float maxValue = *std::max_element(values.begin(), values.end());
    
    for (size_t i = 0; i < values.size(); i++) {
        int barHeight = (values[i] / maxValue) * height;
        Draw::rect(x + i * barWidth, y + height - barHeight, 
                  barWidth - 2, barHeight, color);
    }
}
```

### 3. Game Graphics
```cpp
// Draw a simple maze cell
void drawMazeCell(int x, int y, int size, bool walls[4], uint32_t wallColor) {
    // walls array: [top, right, bottom, left]
    if (walls[0]) Draw::rect(x, y, size, 2, wallColor);                    // Top
    if (walls[1]) Draw::rect(x + size - 2, y, 2, size, wallColor);        // Right
    if (walls[2]) Draw::rect(x, y + size - 2, size, 2, wallColor);        // Bottom
    if (walls[3]) Draw::rect(x, y, 2, size, wallColor);                   // Left
}

// Draw a game character
void drawCharacter(int x, int y, int size, uint32_t color) {
    // Body
    Draw::circle(x, y, size/2, color);
    
    // Eyes
    Draw::circle(x - size/4, y - size/6, size/8, Colors::Black);
    Draw::circle(x + size/4, y - size/6, size/8, Colors::Black);
    
    // Smile
    Draw::line(x - size/3, y + size/6, x + size/3, y + size/6, 2, Colors::Black);
}
```

## Best Practices

### 1. Performance Optimization
```cpp
// Cache frequently used values
const int SCREEN_WIDTH = getWidth();
const int SCREEN_HEIGHT = getHeight();
const uint32_t BACKGROUND_COLOR = Colors::White;

// Batch similar operations
void drawMultipleRects(const std::vector<Rect>& rects, uint32_t color) {
    for (const auto& rect : rects) {
        Draw::rect(rect.x, rect.y, rect.width, rect.height, color);
    }
}
```

### 2. Color Management
```cpp
// Use consistent color schemes
namespace GameColors {
    constexpr uint32_t PLAYER = Colors::Blue;
    constexpr uint32_t ENEMY = Colors::Red;
    constexpr uint32_t COLLECTIBLE = Colors::Gold;
    constexpr uint32_t BACKGROUND = Colors::DarkGray;
}
```

### 3. Coordinate System
```cpp
// Use relative positioning for scalability
void drawScalableUI(float scale) {
    int baseSize = 100 * scale;
    int baseX = 50 * scale;
    int baseY = 50 * scale;
    
    Draw::rect(baseX, baseY, baseSize, baseSize, Colors::Blue);
}
```

### 4. Bounds Checking
```cpp
// Ensure drawing stays within canvas bounds
void safeDraw(int x, int y, int width, int height, uint32_t color) {
    int canvasWidth = getWidth();
    int canvasHeight = getHeight();
    
    // Clamp to canvas bounds
    x = std::max(0, std::min(x, canvasWidth - width));
    y = std::max(0, std::min(y, canvasHeight - height));
    width = std::min(width, canvasWidth - x);
    height = std::min(height, canvasHeight - y);
    
    if (width > 0 && height > 0) {
        Draw::rect(x, y, width, height, color);
    }
}
```

## Drawing Algorithms

### Circle Drawing
The circle function uses a simple distance-based algorithm:
```cpp
// For each pixel in the bounding square
for (int x = -radius; x <= radius; x++) {
    for (int y = -radius; y <= radius; y++) {
        if (x*x + y*y <= radius*radius) {
            // Pixel is inside circle
            setPixel(cx + x, cy + y, color);
        }
    }
}
```

### Line Drawing
Lines are drawn using Bresenham's algorithm with thickness:
```cpp
// Bresenham's line algorithm with circles for thickness
while (true) {
    circle(x1, y1, thickness, color);  // Draw thick point
    
    if (x1 == x2 && y1 == y2) break;
    
    // Bresenham step calculations
    int e2 = 2 * err;
    if (e2 > -dy) { err -= dy; x1 += sx; }
    if (e2 < dx) { err += dx; y1 += sy; }
}
```

## Performance Considerations

### 1. Drawing Order
- Fill background first, then draw objects front-to-back
- Minimize overdraw by culling off-screen objects

### 2. Batch Operations
```cpp
// Instead of individual draws
Draw::rect(x1, y1, w, h, color);
Draw::rect(x2, y2, w, h, color);
Draw::rect(x3, y3, w, h, color);

// Use batch functions when possible
std::vector<Rect> rects = {{x1,y1,w,h}, {x2,y2,w,h}, {x3,y3,w,h}};
drawMultipleRects(rects, color);
```

### 3. Color Blending
- Use pre-blended colors when possible
- Avoid complex alpha operations in inner loops

## Integration with Widgets

Primitives are used internally by widgets for rendering:

```cpp
// Widget uses primitives for custom rendering
class CustomWidget : public Widget {
    void render() override {
        // Draw widget background
        Draw::roundedRect(x_, y_, width_, height_, 5, backgroundColor_);
        
        // Draw border
        Draw::roundedRectBorder(x_, y_, width_, height_, 5, 2, borderColor_);
        
        // Draw custom content
        Draw::circle(x_ + width_/2, y_ + height_/2, 10, Colors::Red);
    }
};
```

## Related Components

- [`Colors`](colors.md) - Color constants and utilities
- [`Canvas`](../core/canvas.md) - Low-level canvas operations
- [`Widgets`](../widgets/) - High-level components that use primitives
- [`Styling`](styling.md) - Styling system built on primitives

## Tips

1. **Use appropriate thickness**: Line thickness of 1-3 pixels works well for most UI elements
2. **Consider pixel alignment**: Integer coordinates prevent blurry lines
3. **Optimize for common cases**: Cache frequently used shapes
4. **Test on different resolutions**: Ensure shapes scale appropriately
5. **Use rounded rectangles**: They provide a more modern, polished look

The Primitives Drawing system provides the foundation for all visual elements in Fern, offering both simplicity and flexibility for creating custom graphics.
