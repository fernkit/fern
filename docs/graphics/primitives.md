# Graphics Primitives Guide

Every complex interface begins with simple shapes - rectangles that become buttons, circles that become indicators, lines that create borders and separators. In this guide, you'll learn not just how to draw shapes in Fern, but how to think about visual composition, understand the mathematics behind computer graphics, and create interfaces that feel both precise and beautiful.

## Understanding Computer Graphics Fundamentals

### The Canvas as a Pixel Grid

Your screen is a grid of millions of tiny colored squares called pixels. When you draw shapes, you're actually calculating which pixels to turn on and what colors to make them. This process involves fascinating mathematics:

1. **Coordinate Systems**: Computers use (x, y) coordinates where (0, 0) is typically the top-left corner
2. **Rasterization**: Converting mathematical shapes into pixel patterns
3. **Alpha Blending**: Combining colors when shapes overlap or have transparency
4. **Anti-aliasing**: Smoothing edges by using intermediate colors

```cpp
// A rectangle isn't just a shape - it's a mathematical description
Draw::rect(100, 50, 200, 100, Colors::Blue);
// Translates to: "Color all pixels where 100 ≤ x < 300 and 50 ≤ y < 150 with blue"
```

### Visual Design Through Primitive Shapes

Every sophisticated interface element builds from these basic primitives:

- **Rectangles**: The foundation of buttons, panels, cards, and layouts
- **Circles**: Perfect for indicators, avatars, badges, and organic elements  
- **Lines**: Essential for borders, separators, connections, and emphasis
- **Rounded Rectangles**: Modern, friendly buttons and containers

Understanding these building blocks helps you think like a designer - seeing complex interfaces as compositions of simple, well-placed shapes.

## Graphics Philosophy in Fern

Fern's drawing system follows these design principles:

- **Immediate Mode**: Draw exactly what you want, when you want it
- **Mathematical Precision**: Every pixel placed with intentional purpose  
- **Performance Conscious**: Efficient algorithms for real-time rendering
- **Composable Design**: Build complex graphics from simple primitives
- **Educational Transparency**: Understanding how graphics actually work

This approach ensures your graphics feel both technically excellent and visually precise while maintaining the educational clarity that makes Fern special.

## Essential Drawing Primitives

### Rectangles - The Foundation of Interface Design

Rectangles are the workhorses of interface design - they create structure, hierarchy, and visual containers:

```cpp
using namespace Fern;

// Basic filled rectangle - solid and dependable
Draw::rect(100, 100, 200, 80, Colors::Primary);

// Create visual hierarchy with layered rectangles
Draw::rect(50, 50, 300, 200, Colors::LightGray);      // Background container
Draw::rect(70, 70, 260, 160, Colors::White);          // Content area
Draw::rect(80, 80, 240, 30, Colors::Primary);         // Header bar
```

**Use rectangles for**:
- Button backgrounds and click areas
- Panel and card containers  
- Progress bars and sliders
- Layout sections and dividers
- Background fills and overlays

### Circles - Organic Elements and Indicators  

Circles feel natural, friendly, and draw attention - perfect for status indicators and focal points:

```cpp
// Status indicators with color psychology
Draw::circle(50, 50, 8, Colors::Success);    // Online indicator - green
Draw::circle(80, 50, 8, Colors::Warning);    // Away indicator - yellow  
Draw::circle(110, 50, 8, Colors::Danger);    // Offline indicator - red

// Profile avatar placeholder
Draw::circle(200, 100, 25, Colors::LightGray);

// Loading spinner dots (would animate in practice)
for (int i = 0; i < 8; ++i) {
    float angle = i * 45.0f * M_PI / 180.0f;  // Convert to radians
    int x = 300 + 15 * cos(angle);
    int y = 100 + 15 * sin(angle);
    uint32_t alpha = 0x20 + (i * 0x20);  // Fade effect
    Draw::circle(x, y, 3, Colors::Primary | (alpha << 24));
}
```

**Use circles for**:
- Status indicators and badges
- Avatar and profile pictures
- Loading spinners and progress dots
- Navigation dots and pagination
- Decorative elements and icons

### Lines - Connection and Structure

Lines create relationships, define boundaries, and guide the eye through your interface:

```cpp
// Horizontal separator - classic and clean
Draw::line(50, 200, 350, 200, 1, Colors::LightGray);

// Vertical divider between sections
Draw::line(200, 50, 200, 300, 1, Colors::LightGray);

// Emphasis line under headings
Draw::line(50, 80, 150, 80, 3, Colors::Primary);

// Connection lines between elements
Draw::line(100, 150, 250, 250, 2, Colors::DarkGray);

// Border frame around content
Draw::line(50, 50, 350, 50, 2, Colors::Charcoal);     // Top
Draw::line(350, 50, 350, 300, 2, Colors::Charcoal);   // Right  
Draw::line(350, 300, 50, 300, 2, Colors::Charcoal);   // Bottom
Draw::line(50, 300, 50, 50, 2, Colors::Charcoal);     // Left
```

**Use lines for**:
- Section dividers and separators
- Borders and frames around content
- Underlines for emphasis and links
- Connectors between related elements
- Grid systems and alignment guides

### Rounded Rectangles - Modern and Friendly

Rounded corners feel approachable, modern, and high-quality - they're the hallmark of contemporary interface design:

```cpp
// Modern button with rounded corners
Draw::roundedRect(100, 100, 120, 40, 8, Colors::Primary);

// Card-style content containers
Draw::roundedRect(50, 200, 300, 150, 12, Colors::White);

// Notification toast with soft edges
Draw::roundedRect(250, 50, 200, 60, 6, Colors::Success);

// Tag or badge style elements
Draw::roundedRect(100, 300, 80, 25, 12, Colors::Info);
```

**Use rounded rectangles for**:
- Modern buttons and interactive elements
- Card layouts and content panels
- Notification toasts and alerts
- Tags, badges, and labels
- Modal dialogs and overlays

## Your First Graphics Composition

Let's create a complete interface element that demonstrates how primitives work together:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

// Create a user profile card using primitive shapes
auto createProfileCard = [](Canvas* canvas, int x, int y) {
    // Card background with soft shadow effect
    Draw::roundedRect(x + 3, y + 3, 250, 120, 12, 0x20000000);  // Shadow
    Draw::roundedRect(x, y, 250, 120, 12, Colors::White);        // Card background
    
    // Avatar circle  
    Draw::circle(x + 40, y + 35, 20, Colors::LightGray);         // Avatar background
    Draw::circle(x + 40, y + 35, 18, Colors::Primary);           // Avatar color
    
    // Status indicator
    Draw::circle(x + 55, y + 45, 6, Colors::Success);            // Online status
    
    // Name underline
    Draw::line(x + 70, y + 45, x + 200, y + 45, 2, Colors::Primary);
    
    // Action button
    Draw::roundedRect(x + 70, y + 60, 80, 30, 6, Colors::Primary);
    
    // Separator line
    Draw::line(x + 20, y + 100, x + 230, y + 100, 1, Colors::LightGray);
};

int main() {
    setupFern();
    auto canvas = getCanvas();
    
    // Background
    Draw::fill(Colors::LightGray);
    
    // Create multiple profile cards
    createProfileCard(canvas, 50, 50);
    createProfileCard(canvas, 350, 50);
    createProfileCard(canvas, 50, 200);
    createProfileCard(canvas, 350, 200);
    
    runFern();
    return 0;
}
```

## Advanced Composition Techniques

### Layering and Depth

Create visual depth by carefully layering shapes with appropriate colors and transparency:

```cpp
// Create a modal dialog with depth
auto createModalDialog = [](Canvas* canvas, int centerX, int centerY) {
    // Background overlay - darkens the entire screen
    Draw::fill(0x60000000);  // 37% transparent black
    
    // Dialog shadow for depth
    Draw::roundedRect(centerX - 147, centerY - 97, 300, 200, 15, 0x40000000);  // Shadow
    
    // Dialog background
    Draw::roundedRect(centerX - 150, centerY - 100, 300, 200, 12, Colors::White);
    
    // Header section with subtle gradient effect (simulated with rectangles)
    Draw::roundedRect(centerX - 150, centerY - 100, 300, 50, 12, Colors::Primary);
    Draw::rect(centerX - 150, centerY - 65, 300, 15, Colors::Primary);  // Remove bottom corners
    
    // Header separator
    Draw::line(centerX - 140, centerY - 50, centerX + 140, centerY - 50, 1, 0x20000000);
    
    // Action buttons with proper spacing
    Draw::roundedRect(centerX - 40, centerY + 60, 80, 30, 6, Colors::Primary);    // OK button
    Draw::roundedRect(centerX - 140, centerY + 60, 80, 30, 6, Colors::Secondary); // Cancel button
};
```

### Pattern Creation with Primitives

Use repeated primitives to create patterns, textures, and visual rhythms:

```cpp
// Create a dotted pattern for loading states
auto createDottedPattern = [](Canvas* canvas, int x, int y, int width, int height) {
    const int dotSize = 3;
    const int spacing = 8;
    
    for (int dy = 0; dy < height; dy += spacing) {
        for (int dx = 0; dx < width; dx += spacing) {
            // Checkerboard pattern using modulo
            if ((dx / spacing + dy / spacing) % 2 == 0) {
                Draw::circle(x + dx, y + dy, dotSize, Colors::LightGray);
            }
        }
    }
};

// Create a graph grid background
auto createGridBackground = [](Canvas* canvas, int x, int y, int width, int height) {
    const int gridSize = 20;
    
    // Vertical grid lines
    for (int gx = x; gx <= x + width; gx += gridSize) {
        Draw::line(gx, y, gx, y + height, 1, 0x20000000);
    }
    
    // Horizontal grid lines  
    for (int gy = y; gy <= y + height; gy += gridSize) {
        Draw::line(x, gy, x + width, gy, 1, 0x20000000);
    }
    
    // Emphasize major grid lines
    Draw::line(x + width/2, y, x + width/2, y + height, 2, 0x40000000);  // Center vertical
    Draw::line(x, y + height/2, x + width, y + height/2, 2, 0x40000000); // Center horizontal
};
```

### Data Visualization with Primitives

Transform data into visual stories using primitive shapes:

```cpp
// Create a simple bar chart
auto createBarChart = [](Canvas* canvas, const std::vector<float>& data, int x, int y, int width, int height) {
    // Find data range
    float maxValue = *std::max_element(data.begin(), data.end());
    float minValue = *std::min_element(data.begin(), data.end());
    float range = maxValue - minValue;
    
    // Chart background
    Draw::rect(x, y, width, height, Colors::White);
    Draw::line(x, y + height, x + width, y + height, 2, Colors::Charcoal);  // X-axis
    Draw::line(x, y, x, y + height, 2, Colors::Charcoal);                   // Y-axis
    
    // Draw bars
    int barWidth = width / data.size() - 4;
    for (size_t i = 0; i < data.size(); ++i) {
        float normalizedValue = (data[i] - minValue) / range;
        int barHeight = static_cast<int>(normalizedValue * height);
        int barX = x + 2 + i * (barWidth + 4);
        int barY = y + height - barHeight;
        
        // Color based on value
        uint32_t barColor = normalizedValue > 0.7f ? Colors::Success : 
                           normalizedValue > 0.4f ? Colors::Warning : Colors::Danger;
        
        Draw::roundedRect(barX, barY, barWidth, barHeight, 2, barColor);
    }
};

// Usage with sample data
std::vector<float> salesData = {23.5f, 45.2f, 67.8f, 34.1f, 89.3f, 56.7f};
createBarChart(canvas, salesData, 100, 100, 300, 200);
```

## Performance and Optimization

### Efficient Drawing Strategies

Understand the performance characteristics of different drawing operations:

```cpp
// Performance considerations for different primitives
namespace DrawingPerformance {
    // Fastest: Simple filled rectangles
    void drawFastBackground(Canvas* canvas) {
        Draw::rect(0, 0, 800, 600, Colors::LightGray);  // Single rect fill
    }
    
    // Moderate: Circles and rounded rectangles (more complex math)
    void drawModerateComplexity(Canvas* canvas) {
        Draw::roundedRect(100, 100, 200, 100, 12, Colors::Primary);
        Draw::circle(200, 150, 25, Colors::White);
    }
    
    // Slower: Multiple overlapping shapes with transparency
    void drawComplexComposition(Canvas* canvas) {
        for (int i = 0; i < 50; ++i) {
            Draw::circle(100 + i * 2, 100 + i, 20, 0x40FF0000);  // Lots of alpha blending
        }
    }
    
    // Optimization: Batch similar operations
    void drawOptimizedPattern(Canvas* canvas) {
        // Group similar drawing calls together
        std::vector<std::pair<int, int>> circlePositions;
        for (int i = 0; i < 10; ++i) {
            circlePositions.push_back({100 + i * 20, 100});
        }
        
        // Draw all circles with same color at once
        for (const auto& pos : circlePositions) {
            Draw::circle(pos.first, pos.second, 8, Colors::Primary);
        }
    }
}
```

### Memory and Rendering Considerations

```cpp
// Smart drawing for large datasets
class EfficientRenderer {
private:
    Canvas* canvas;
    
public:
    EfficientRenderer(Canvas* c) : canvas(c) {}
    
    // Only draw what's visible (viewport culling)
    void drawVisibleItems(const std::vector<GraphicsItem>& items, int viewX, int viewY, int viewWidth, int viewHeight) {
        for (const auto& item : items) {
            // Skip items outside the visible area
            if (item.x + item.width < viewX || item.x > viewX + viewWidth ||
                item.y + item.height < viewY || item.y > viewY + viewHeight) {
                continue;
            }
            
            // Draw only visible items
            Draw::roundedRect(item.x, item.y, item.width, item.height, item.radius, item.color);
        }
    }
    
    // Level-of-detail rendering based on zoom
    void drawWithLevelOfDetail(const GraphicsItem& item, float zoomLevel) {
        if (zoomLevel < 0.5f) {
            // Very zoomed out - just draw a simple rectangle
            Draw::rect(item.x, item.y, item.width, item.height, item.color);
        } else if (zoomLevel < 1.0f) {
            // Medium zoom - rounded rectangle
            Draw::roundedRect(item.x, item.y, item.width, item.height, 4, item.color);
        } else {
            // Full detail - all decorative elements
            Draw::roundedRect(item.x, item.y, item.width, item.height, item.radius, item.color);
            Draw::circle(item.x + 10, item.y + 10, 5, Colors::White);  // Detail indicator
        }
    }
};
```

## Real-World Application Examples

### Dashboard Widget Creation

```cpp
// Create a complete dashboard widget using only primitives
auto createMetricWidget = [](Canvas* canvas, const std::string& title, 
                            float value, float maxValue, int x, int y) {
    const int width = 200, height = 120;
    
    // Widget background with subtle border
    Draw::roundedRect(x, y, width, height, 8, Colors::White);
    Draw::roundedRectBorder(x, y, width, height, 8, 1, Colors::LightGray);
    
    // Header area
    Draw::rect(x, y, width, 30, Colors::LightGray);
    Draw::roundedRect(x, y, width, 30, 8, Colors::LightGray);  // Rounded top
    Draw::rect(x, y + 15, width, 15, Colors::LightGray);       // Square bottom of header
    
    // Progress visualization
    float percentage = value / maxValue;
    int progressWidth = static_cast<int>((width - 20) * percentage);
    
    // Progress background
    Draw::roundedRect(x + 10, y + 70, width - 20, 12, 6, Colors::LightGray);
    
    // Progress fill with color based on value
    uint32_t progressColor = percentage > 0.8f ? Colors::Danger :
                            percentage > 0.6f ? Colors::Warning : Colors::Success;
    if (progressWidth > 0) {
        Draw::roundedRect(x + 10, y + 70, progressWidth, 12, 6, progressColor);
    }
    
    // Status indicator
    Draw::circle(x + width - 20, y + 15, 6, progressColor);
};

// Create a complete dashboard
auto createDashboard = [](Canvas* canvas) {
    // Background
    Draw::fill(0xFFF5F5F5);  // Light gray background
    
    // Dashboard widgets
    createMetricWidget(canvas, "CPU Usage", 67.0f, 100.0f, 50, 50);
    createMetricWidget(canvas, "Memory", 45.0f, 100.0f, 270, 50);
    createMetricWidget(canvas, "Disk Space", 89.0f, 100.0f, 490, 50);
    createMetricWidget(canvas, "Network", 23.0f, 100.0f, 50, 190);
    
    // Dashboard title
    Draw::line(50, 30, 650, 30, 3, Colors::Primary);
};
```

Graphics primitives are the building blocks of every beautiful interface. By understanding how rectangles, circles, lines, and rounded rectangles work together, you can create sophisticated visual compositions that feel both precise and elegant. The key is thinking of your interface as a carefully orchestrated composition of simple, well-placed shapes.

Remember: great graphics feel natural and unforced. When users focus on your content rather than noticing your drawing techniques, you've achieved graphical harmony.
