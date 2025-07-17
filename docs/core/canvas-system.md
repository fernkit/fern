# Canvas System Guide

At the heart of every visual experience lies a fundamental concept: the canvas. In Fern, the canvas represents the digital equivalent of an artist's canvas - a pixel-based surface where all visual elements come to life. This guide explores not just how to use Fern's canvas system, but the fascinating principles of pixel-based graphics, color representation, and the mathematical foundations that transform abstract ideas into visible pixels.

## Understanding Digital Canvas

### The Pixel Grid Reality

Every digital display is composed of discrete picture elements (pixels) arranged in a grid. Unlike traditional art where you have continuous surfaces, digital graphics must map continuous mathematical concepts onto discrete pixel coordinates:

```cpp
// A canvas is fundamentally a 2D array of colored squares
//
//    X→ 0   1   2   3   4   5
// Y↓ ┌───┬───┬───┬───┬───┬───┐
// 0  │ R │ G │ B │ W │ K │ Y │
//    ├───┼───┼───┼───┼───┼───┤
// 1  │ C │ M │ R │ G │ B │ W │
//    ├───┼───┼───┼───┼───┼───┤
// 2  │ Y │ K │ C │ M │ R │ G │
//    └───┴───┴───┴───┴───┴───┘
//
// Each cell contains a single color value (32-bit RGBA)
```

This discretization presents unique challenges and opportunities:

1. **Coordinate Systems**: Perfect mathematical lines become approximated pixel patterns
2. **Color Quantization**: Infinite color possibilities become discrete color values
3. **Sampling**: Continuous shapes must be sampled at pixel locations
4. **Performance**: Direct pixel access enables optimized rendering algorithms

## Canvas Philosophy in Fern

Fern's canvas system embodies several core principles:

- **Direct Pixel Access**: Provides low-level control over every pixel for maximum flexibility
- **Memory Efficiency**: Uses flat memory layouts for optimal cache performance
- **Bounds Safety**: Automatic coordinate validation prevents buffer overruns
- **RGBA Color Model**: Industry-standard 32-bit color representation with alpha channel
- **Platform Abstraction**: Consistent interface across different rendering platforms

## Color Representation Deep Dive

### RGBA Color Format

Fern uses 32-bit RGBA color values stored in the format `0xAABBGGRR`:

```cpp
// Color component breakdown:
// 0xAABBGGRR
//   ││││└─ Red component   (0x00-0xFF)
//   │││└── Green component (0x00-0xFF) 
//   ││└─── Blue component  (0x00-0xFF)
//   │└──── Alpha component (0x00-0xFF)
//
// Examples:
uint32_t red     = 0xFF0000FF; // Opaque red
uint32_t green   = 0xFF00FF00; // Opaque green  
uint32_t blue    = 0xFFFF0000; // Opaque blue
uint32_t white   = 0xFFFFFFFF; // Opaque white
uint32_t black   = 0xFF000000; // Opaque black
uint32_t clear   = 0x00000000; // Transparent

// Semi-transparent colors
uint32_t halfRed = 0x800000FF; // 50% transparent red
uint32_t fadeBlue = 0x40FF0000; // 25% transparent blue
```

### Color Manipulation Utilities

```cpp
class ColorUtils {
public:
    // Extract individual color components
    static uint8_t getRed(uint32_t color) {
        return color & 0xFF;
    }
    
    static uint8_t getGreen(uint32_t color) {
        return (color >> 8) & 0xFF;
    }
    
    static uint8_t getBlue(uint32_t color) {
        return (color >> 16) & 0xFF;
    }
    
    static uint8_t getAlpha(uint32_t color) {
        return (color >> 24) & 0xFF;
    }
    
    // Create color from components
    static uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
    
    // Color mixing and manipulation
    static uint32_t blend(uint32_t color1, uint32_t color2, float factor) {
        uint8_t r1 = getRed(color1), g1 = getGreen(color1), b1 = getBlue(color1);
        uint8_t r2 = getRed(color2), g2 = getGreen(color2), b2 = getBlue(color2);
        
        uint8_t r = r1 + (r2 - r1) * factor;
        uint8_t g = g1 + (g2 - g1) * factor;
        uint8_t b = b1 + (b2 - b1) * factor;
        
        return rgba(r, g, b, 255);
    }
    
    // Convert HSV to RGB
    static uint32_t hsv(float h, float s, float v, uint8_t a = 255) {
        // HSV to RGB conversion algorithm
        int i = (int)(h * 6.0f);
        float f = h * 6.0f - i;
        float p = v * (1.0f - s);
        float q = v * (1.0f - f * s);
        float t = v * (1.0f - (1.0f - f) * s);
        
        float r, g, b;
        switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }
        
        return rgba((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), a);
    }
};
```

## Basic Canvas Operations

### Canvas Initialization and Setup

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern (this sets up the global canvas)
    setupFern();
    
    // Access the global canvas
    Canvas* canvas = globalCanvas;
    
    // Get canvas dimensions
    int width = canvas->getWidth();
    int height = canvas->getHeight();
    
    std::cout << "Canvas size: " << width << "x" << height << std::endl;
    
    while (shouldKeepRunning()) {
        // Clear canvas for new frame
        canvas->clear(0xFF000000); // Clear to black
        
        // Your drawing operations here...
        
        render(); // Present the canvas to screen
    }
    
    return 0;
}
```

### Pixel-Level Operations

```cpp
void demonstratePixelOperations() {
    Canvas* canvas = globalCanvas;
    
    // Set individual pixels
    canvas->setPixel(100, 100, 0xFFFF0000); // Red pixel
    canvas->setPixel(101, 100, 0xFF00FF00); // Green pixel
    canvas->setPixel(102, 100, 0xFF0000FF); // Blue pixel
    
    // Read pixel values
    uint32_t redPixel = canvas->getPixel(100, 100);
    std::cout << "Red pixel value: 0x" << std::hex << redPixel << std::endl;
    
    // Bounds checking is automatic
    canvas->setPixel(-10, -10, 0xFFFFFFFF); // Safely ignored
    canvas->setPixel(10000, 10000, 0xFFFFFFFF); // Safely ignored
    
    // Create a gradient
    for (int x = 0; x < 256; ++x) {
        for (int y = 0; y < 256; ++y) {
            uint32_t color = ColorUtils::rgba(x, y, 128, 255);
            canvas->setPixel(x, y, color);
        }
    }
}
```

## Advanced Canvas Techniques

### Direct Buffer Access for Performance

For high-performance operations, you can access the pixel buffer directly:

```cpp
class HighPerformanceDrawing {
public:
    static void fastClear(Canvas* canvas, uint32_t color) {
        uint32_t* buffer = canvas->getBuffer();
        int pixelCount = canvas->getWidth() * canvas->getHeight();
        
        // Use memset for single-byte values or loop for complex colors
        if ((color & 0xFF) == ((color >> 8) & 0xFF) && 
            ((color >> 8) & 0xFF) == ((color >> 16) & 0xFF) &&
            ((color >> 16) & 0xFF) == ((color >> 24) & 0xFF)) {
            // All components are the same - use memset
            memset(buffer, color & 0xFF, pixelCount * 4);
        } else {
            // Use optimized loop
            for (int i = 0; i < pixelCount; ++i) {
                buffer[i] = color;
            }
        }
    }
    
    static void fastHorizontalLine(Canvas* canvas, int x, int y, int length, uint32_t color) {
        if (y < 0 || y >= canvas->getHeight()) return;
        
        int startX = std::max(0, x);
        int endX = std::min(canvas->getWidth(), x + length);
        
        if (startX >= endX) return;
        
        uint32_t* buffer = canvas->getBuffer();
        uint32_t* lineStart = buffer + y * canvas->getWidth() + startX;
        
        for (int i = 0; i < endX - startX; ++i) {
            lineStart[i] = color;
        }
    }
    
    static void fastVerticalLine(Canvas* canvas, int x, int y, int length, uint32_t color) {
        if (x < 0 || x >= canvas->getWidth()) return;
        
        int startY = std::max(0, y);
        int endY = std::min(canvas->getHeight(), y + length);
        
        if (startY >= endY) return;
        
        uint32_t* buffer = canvas->getBuffer();
        int width = canvas->getWidth();
        
        for (int currentY = startY; currentY < endY; ++currentY) {
            buffer[currentY * width + x] = color;
        }
    }
};
```

### Geometric Shape Drawing

```cpp
class CanvasShapes {
public:
    static void drawRectangle(Canvas* canvas, int x, int y, int width, int height, 
                             uint32_t color, bool filled = true) {
        if (filled) {
            drawFilledRectangle(canvas, x, y, width, height, color);
        } else {
            drawRectangleOutline(canvas, x, y, width, height, color);
        }
    }
    
    static void drawFilledRectangle(Canvas* canvas, int x, int y, int width, int height, uint32_t color) {
        for (int currentY = y; currentY < y + height; ++currentY) {
            for (int currentX = x; currentX < x + width; ++currentX) {
                canvas->setPixel(currentX, currentY, color);
            }
        }
    }
    
    static void drawRectangleOutline(Canvas* canvas, int x, int y, int width, int height, uint32_t color) {
        // Top and bottom edges
        for (int currentX = x; currentX < x + width; ++currentX) {
            canvas->setPixel(currentX, y, color);                    // Top edge
            canvas->setPixel(currentX, y + height - 1, color);       // Bottom edge
        }
        
        // Left and right edges
        for (int currentY = y; currentY < y + height; ++currentY) {
            canvas->setPixel(x, currentY, color);                    // Left edge
            canvas->setPixel(x + width - 1, currentY, color);        // Right edge
        }
    }
    
    static void drawCircle(Canvas* canvas, int centerX, int centerY, int radius, 
                          uint32_t color, bool filled = true) {
        if (filled) {
            drawFilledCircle(canvas, centerX, centerY, radius, color);
        } else {
            drawCircleOutline(canvas, centerX, centerY, radius, color);
        }
    }
    
    static void drawFilledCircle(Canvas* canvas, int centerX, int centerY, int radius, uint32_t color) {
        int radiusSquared = radius * radius;
        
        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                if (x * x + y * y <= radiusSquared) {
                    canvas->setPixel(centerX + x, centerY + y, color);
                }
            }
        }
    }
    
    static void drawCircleOutline(Canvas* canvas, int centerX, int centerY, int radius, uint32_t color) {
        // Bresenham's circle algorithm for efficient circle outline
        int x = 0;
        int y = radius;
        int decision = 1 - radius;
        
        while (x <= y) {
            // Draw 8 octants of the circle
            canvas->setPixel(centerX + x, centerY + y, color);
            canvas->setPixel(centerX - x, centerY + y, color);
            canvas->setPixel(centerX + x, centerY - y, color);
            canvas->setPixel(centerX - x, centerY - y, color);
            canvas->setPixel(centerX + y, centerY + x, color);
            canvas->setPixel(centerX - y, centerY + x, color);
            canvas->setPixel(centerX + y, centerY - x, color);
            canvas->setPixel(centerX - y, centerY - x, color);
            
            x++;
            if (decision < 0) {
                decision += 2 * x + 1;
            } else {
                y--;
                decision += 2 * (x - y) + 1;
            }
        }
    }
    
    static void drawLine(Canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color) {
        // Bresenham's line algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        int x = x1, y = y1;
        
        while (true) {
            canvas->setPixel(x, y, color);
            
            if (x == x2 && y == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
};
```

### Advanced Visual Effects

```cpp
class CanvasEffects {
public:
    static void applyGradient(Canvas* canvas, int x, int y, int width, int height,
                             uint32_t startColor, uint32_t endColor, bool horizontal = true) {
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                float factor;
                if (horizontal) {
                    factor = (float)currentX / (width - 1);
                } else {
                    factor = (float)currentY / (height - 1);
                }
                
                uint32_t color = ColorUtils::blend(startColor, endColor, factor);
                canvas->setPixel(x + currentX, y + currentY, color);
            }
        }
    }
    
    static void applyRadialGradient(Canvas* canvas, int centerX, int centerY, int maxRadius,
                                   uint32_t centerColor, uint32_t edgeColor) {
        for (int y = centerY - maxRadius; y <= centerY + maxRadius; ++y) {
            for (int x = centerX - maxRadius; x <= centerX + maxRadius; ++x) {
                int dx = x - centerX;
                int dy = y - centerY;
                float distance = sqrt(dx * dx + dy * dy);
                
                if (distance <= maxRadius) {
                    float factor = distance / maxRadius;
                    uint32_t color = ColorUtils::blend(centerColor, edgeColor, factor);
                    canvas->setPixel(x, y, color);
                }
            }
        }
    }
    
    static void applyNoise(Canvas* canvas, int x, int y, int width, int height, float intensity) {
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                uint32_t currentColor = canvas->getPixel(x + currentX, y + currentY);
                
                // Generate noise values for each color component
                float noiseR = (rand() / (float)RAND_MAX - 0.5f) * intensity;
                float noiseG = (rand() / (float)RAND_MAX - 0.5f) * intensity;
                float noiseB = (rand() / (float)RAND_MAX - 0.5f) * intensity;
                
                uint8_t r = std::clamp((int)(ColorUtils::getRed(currentColor) + noiseR * 255), 0, 255);
                uint8_t g = std::clamp((int)(ColorUtils::getGreen(currentColor) + noiseG * 255), 0, 255);
                uint8_t b = std::clamp((int)(ColorUtils::getBlue(currentColor) + noiseB * 255), 0, 255);
                uint8_t a = ColorUtils::getAlpha(currentColor);
                
                canvas->setPixel(x + currentX, y + currentY, ColorUtils::rgba(r, g, b, a));
            }
        }
    }
    
    static void applyBlur(Canvas* canvas, int x, int y, int width, int height, int radius) {
        // Simple box blur implementation
        std::vector<uint32_t> tempBuffer(width * height);
        
        // Copy original pixels
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                tempBuffer[currentY * width + currentX] = 
                    canvas->getPixel(x + currentX, y + currentY);
            }
        }
        
        // Apply blur
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                int totalR = 0, totalG = 0, totalB = 0, totalA = 0;
                int count = 0;
                
                // Average pixels in blur radius
                for (int dy = -radius; dy <= radius; ++dy) {
                    for (int dx = -radius; dx <= radius; ++dx) {
                        int sampleX = currentX + dx;
                        int sampleY = currentY + dy;
                        
                        if (sampleX >= 0 && sampleX < width && 
                            sampleY >= 0 && sampleY < height) {
                            uint32_t color = tempBuffer[sampleY * width + sampleX];
                            totalR += ColorUtils::getRed(color);
                            totalG += ColorUtils::getGreen(color);
                            totalB += ColorUtils::getBlue(color);
                            totalA += ColorUtils::getAlpha(color);
                            count++;
                        }
                    }
                }
                
                if (count > 0) {
                    uint32_t blurredColor = ColorUtils::rgba(
                        totalR / count, totalG / count, 
                        totalB / count, totalA / count
                    );
                    canvas->setPixel(x + currentX, y + currentY, blurredColor);
                }
            }
        }
    }
};
```

## Canvas Patterns and Utilities

### Pattern Generation

```cpp
class CanvasPatterns {
public:
    static void checkerboard(Canvas* canvas, int x, int y, int width, int height,
                            int squareSize, uint32_t color1, uint32_t color2) {
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                int checkX = currentX / squareSize;
                int checkY = currentY / squareSize;
                bool useColor1 = (checkX + checkY) % 2 == 0;
                
                uint32_t color = useColor1 ? color1 : color2;
                canvas->setPixel(x + currentX, y + currentY, color);
            }
        }
    }
    
    static void stripes(Canvas* canvas, int x, int y, int width, int height,
                       int stripeWidth, uint32_t color1, uint32_t color2, bool horizontal = true) {
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                int position = horizontal ? currentY : currentX;
                bool useColor1 = (position / stripeWidth) % 2 == 0;
                
                uint32_t color = useColor1 ? color1 : color2;
                canvas->setPixel(x + currentX, y + currentY, color);
            }
        }
    }
    
    static void grid(Canvas* canvas, int x, int y, int width, int height,
                    int cellWidth, int cellHeight, uint32_t backgroundColor, uint32_t lineColor) {
        // Fill background
        CanvasShapes::drawFilledRectangle(canvas, x, y, width, height, backgroundColor);
        
        // Draw vertical lines
        for (int currentX = 0; currentX <= width; currentX += cellWidth) {
            CanvasShapes::drawLine(canvas, x + currentX, y, x + currentX, y + height - 1, lineColor);
        }
        
        // Draw horizontal lines
        for (int currentY = 0; currentY <= height; currentY += cellHeight) {
            CanvasShapes::drawLine(canvas, x, y + currentY, x + width - 1, y + currentY, lineColor);
        }
    }
    
    static void concentricCircles(Canvas* canvas, int centerX, int centerY,
                                 int maxRadius, int ringWidth, uint32_t color1, uint32_t color2) {
        for (int radius = ringWidth; radius <= maxRadius; radius += ringWidth) {
            bool useColor1 = (radius / ringWidth) % 2 == 1;
            uint32_t color = useColor1 ? color1 : color2;
            
            // Draw ring (filled circle minus smaller filled circle)
            CanvasShapes::drawFilledCircle(canvas, centerX, centerY, radius, color);
            if (radius > ringWidth) {
                CanvasShapes::drawFilledCircle(canvas, centerX, centerY, radius - ringWidth, 
                                              useColor1 ? color2 : color1);
            }
        }
    }
};
```

### Canvas Utilities and Helpers

```cpp
class CanvasUtils {
public:
    static void copyRegion(Canvas* source, Canvas* destination,
                          int srcX, int srcY, int dstX, int dstY,
                          int width, int height) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t pixel = source->getPixel(srcX + x, srcY + y);
                destination->setPixel(dstX + x, dstY + y, pixel);
            }
        }
    }
    
    static void floodFill(Canvas* canvas, int x, int y, uint32_t newColor) {
        uint32_t originalColor = canvas->getPixel(x, y);
        if (originalColor == newColor) return;
        
        std::stack<std::pair<int, int>> stack;
        stack.push({x, y});
        
        while (!stack.empty()) {
            auto [currentX, currentY] = stack.top();
            stack.pop();
            
            if (canvas->getPixel(currentX, currentY) != originalColor) continue;
            
            canvas->setPixel(currentX, currentY, newColor);
            
            // Add neighboring pixels
            stack.push({currentX + 1, currentY});
            stack.push({currentX - 1, currentY});
            stack.push({currentX, currentY + 1});
            stack.push({currentX, currentY - 1});
        }
    }
    
    static void drawText(Canvas* canvas, int x, int y, const std::string& text,
                        uint32_t color, int fontSize = 1) {
        // Simple bitmap text rendering (would need actual font data)
        // This is a placeholder showing the concept
        for (size_t i = 0; i < text.length(); ++i) {
            char c = text[i];
            int charX = x + i * (8 * fontSize); // 8 pixels wide per character
            
            // Draw simple character representation
            drawSimpleChar(canvas, charX, y, c, color, fontSize);
        }
    }
    
    static bool isPointInBounds(Canvas* canvas, int x, int y) {
        return x >= 0 && x < canvas->getWidth() && 
               y >= 0 && y < canvas->getHeight();
    }
    
    static void saveAsPPM(Canvas* canvas, const std::string& filename) {
        // Save canvas as PPM image format (simple RGB format)
        std::ofstream file(filename);
        file << "P3\n";
        file << canvas->getWidth() << " " << canvas->getHeight() << "\n";
        file << "255\n";
        
        for (int y = 0; y < canvas->getHeight(); ++y) {
            for (int x = 0; x < canvas->getWidth(); ++x) {
                uint32_t pixel = canvas->getPixel(x, y);
                uint8_t r = ColorUtils::getRed(pixel);
                uint8_t g = ColorUtils::getGreen(pixel);
                uint8_t b = ColorUtils::getBlue(pixel);
                file << (int)r << " " << (int)g << " " << (int)b << " ";
            }
            file << "\n";
        }
    }
    
private:
    static void drawSimpleChar(Canvas* canvas, int x, int y, char c, uint32_t color, int fontSize) {
        // Simple character rendering - would need actual font bitmap data
        // This is just a placeholder
        CanvasShapes::drawFilledRectangle(canvas, x, y, 6 * fontSize, 8 * fontSize, color);
    }
};
```

## Performance Optimization

### Memory Layout and Cache Optimization

```cpp
class CanvasOptimization {
public:
    static void efficientClear(Canvas* canvas, uint32_t color) {
        uint32_t* buffer = canvas->getBuffer();
        size_t pixelCount = canvas->getWidth() * canvas->getHeight();
        
        // Use CPU-optimized methods for clearing
        if (color == 0) {
            // Zero is special case - use memset
            memset(buffer, 0, pixelCount * sizeof(uint32_t));
        } else {
            // For other colors, use optimized loop
            // Modern compilers will vectorize this
            for (size_t i = 0; i < pixelCount; ++i) {
                buffer[i] = color;
            }
        }
    }
    
    static void efficientHorizontalGradient(Canvas* canvas, int x, int y, int width, int height,
                                           uint32_t startColor, uint32_t endColor) {
        // Pre-calculate color values for each column
        std::vector<uint32_t> colorLookup(width);
        for (int i = 0; i < width; ++i) {
            float factor = (float)i / (width - 1);
            colorLookup[i] = ColorUtils::blend(startColor, endColor, factor);
        }
        
        // Use lookup table for fast gradient rendering
        uint32_t* buffer = canvas->getBuffer();
        int canvasWidth = canvas->getWidth();
        
        for (int row = 0; row < height; ++row) {
            uint32_t* rowStart = buffer + (y + row) * canvasWidth + x;
            for (int col = 0; col < width; ++col) {
                rowStart[col] = colorLookup[col];
            }
        }
    }
    
    static void cacheFriendlyProcessing(Canvas* canvas, 
                                       std::function<uint32_t(uint32_t)> processor) {
        // Process pixels in cache-friendly order (row by row)
        uint32_t* buffer = canvas->getBuffer();
        size_t pixelCount = canvas->getWidth() * canvas->getHeight();
        
        for (size_t i = 0; i < pixelCount; ++i) {
            buffer[i] = processor(buffer[i]);
        }
    }
};
```

### Clipping and Bounds Optimization

```cpp
class ClippingUtils {
public:
    struct Rect {
        int x, y, width, height;
        
        bool contains(int px, int py) const {
            return px >= x && px < x + width && py >= y && py < y + height;
        }
        
        Rect intersect(const Rect& other) const {
            int left = std::max(x, other.x);
            int top = std::max(y, other.y);
            int right = std::min(x + width, other.x + other.width);
            int bottom = std::min(y + height, other.y + other.height);
            
            if (left >= right || top >= bottom) {
                return {0, 0, 0, 0}; // No intersection
            }
            
            return {left, top, right - left, bottom - top};
        }
    };
    
    static void drawClippedRectangle(Canvas* canvas, const Rect& rect, 
                                    const Rect& clipRect, uint32_t color) {
        Rect clipped = rect.intersect(clipRect);
        if (clipped.width <= 0 || clipped.height <= 0) return;
        
        CanvasShapes::drawFilledRectangle(canvas, clipped.x, clipped.y, 
                                         clipped.width, clipped.height, color);
    }
    
    static Rect getCanvasBounds(Canvas* canvas) {
        return {0, 0, canvas->getWidth(), canvas->getHeight()};
    }
};
```

## Integration with Fern Systems

### Canvas in the Rendering Pipeline

```cpp
class RenderingPipeline {
public:
    void renderFrame() {
        Canvas* canvas = globalCanvas;
        
        // 1. Clear canvas for new frame
        canvas->clear(backgroundColor_);
        
        // 2. Render background elements
        renderBackground(canvas);
        
        // 3. Render widgets and UI elements
        renderWidgets(canvas);
        
        // 4. Render overlays and effects
        renderOverlays(canvas);
        
        // 5. Present to screen (handled by platform layer)
        // render() call will present the canvas contents
    }
    
private:
    uint32_t backgroundColor_ = 0xFF202020; // Dark gray
    
    void renderBackground(Canvas* canvas) {
        // Draw background patterns, images, etc.
        CanvasPatterns::grid(canvas, 0, 0, canvas->getWidth(), canvas->getHeight(),
                           50, 50, backgroundColor_, 0xFF404040);
    }
    
    void renderWidgets(Canvas* canvas) {
        // Widgets render themselves to the canvas
        // This is handled automatically by the widget system
    }
    
    void renderOverlays(Canvas* canvas) {
        // Draw debug info, cursors, tooltips, etc.
        if (showDebugInfo_) {
            renderDebugOverlay(canvas);
        }
    }
    
    void renderDebugOverlay(Canvas* canvas) {
        // Draw FPS, memory usage, etc.
        CanvasShapes::drawFilledRectangle(canvas, 10, 10, 200, 100, 0x80000000);
        // Add text rendering for debug info
    }
    
    bool showDebugInfo_ = false;
};
```

### Custom Canvas-Based Widgets

```cpp
class CustomCanvasWidget : public Widget {
public:
    CustomCanvasWidget(int x, int y, int width, int height)
        : x_(x), y_(y), width_(width), height_(height) {}
    
    void render() override {
        Canvas* canvas = globalCanvas;
        
        // Custom rendering using direct canvas access
        renderBackground(canvas);
        renderContent(canvas);
        renderBorder(canvas);
    }
    
    bool handleInput(const InputState& input) override {
        if (contains(input.mouseX, input.mouseY)) {
            if (input.mouseClicked) {
                // Handle interaction
                onCanvasClick(input.mouseX - x_, input.mouseY - y_);
                return true;
            }
        }
        return false;
    }
    
    // Widget interface implementation
    int getWidth() const override { return width_; }
    int getHeight() const override { return height_; }
    void setPosition(int x, int y) override { x_ = x; y_ = y; }
    int getX() const override { return x_; }
    int getY() const override { return y_; }
    void resize(int width, int height) override { width_ = width; height_ = height; }
    
private:
    int x_, y_, width_, height_;
    
    void renderBackground(Canvas* canvas) {
        CanvasShapes::drawFilledRectangle(canvas, x_, y_, width_, height_, 0xFF303030);
    }
    
    void renderContent(Canvas* canvas) {
        // Custom drawing logic specific to this widget
        CanvasEffects::applyRadialGradient(canvas, x_ + width_/2, y_ + height_/2, 
                                          std::min(width_, height_)/2,
                                          0xFF6060FF, 0xFF303060);
    }
    
    void renderBorder(Canvas* canvas) {
        CanvasShapes::drawRectangleOutline(canvas, x_, y_, width_, height_, 0xFF808080);
    }
    
    void onCanvasClick(int localX, int localY) {
        std::cout << "Canvas clicked at local position: " << localX << ", " << localY << std::endl;
    }
    
    bool contains(int x, int y) const {
        return x >= x_ && x < x_ + width_ && y >= y_ && y < y_ + height_;
    }
};
```

## Canvas Debugging and Analysis

### Pixel Analysis Tools

```cpp
class CanvasDebugger {
public:
    static void analyzePixelDistribution(Canvas* canvas) {
        std::map<uint32_t, int> colorCounts;
        
        for (int y = 0; y < canvas->getHeight(); ++y) {
            for (int x = 0; x < canvas->getWidth(); ++x) {
                uint32_t color = canvas->getPixel(x, y);
                colorCounts[color]++;
            }
        }
        
        std::cout << "Color distribution:" << std::endl;
        for (const auto& [color, count] : colorCounts) {
            std::cout << "Color 0x" << std::hex << color << ": " 
                      << std::dec << count << " pixels" << std::endl;
        }
    }
    
    static void highlightChangedPixels(Canvas* canvas, Canvas* reference, uint32_t highlightColor) {
        for (int y = 0; y < canvas->getHeight(); ++y) {
            for (int x = 0; x < canvas->getWidth(); ++x) {
                uint32_t current = canvas->getPixel(x, y);
                uint32_t ref = reference->getPixel(x, y);
                
                if (current != ref) {
                    canvas->setPixel(x, y, highlightColor);
                }
            }
        }
    }
    
    static void drawPixelGrid(Canvas* canvas, int gridSize, uint32_t gridColor) {
        for (int x = 0; x < canvas->getWidth(); x += gridSize) {
            for (int y = 0; y < canvas->getHeight(); ++y) {
                canvas->setPixel(x, y, gridColor);
            }
        }
        
        for (int y = 0; y < canvas->getHeight(); y += gridSize) {
            for (int x = 0; x < canvas->getWidth(); ++x) {
                canvas->setPixel(x, y, gridColor);
            }
        }
    }
    
    static void visualizeColorChannels(Canvas* canvas, int x, int y, int width, int height) {
        for (int currentY = 0; currentY < height; ++currentY) {
            for (int currentX = 0; currentX < width; ++currentX) {
                uint32_t pixel = canvas->getPixel(x + currentX, y + currentY);
                
                uint8_t r = ColorUtils::getRed(pixel);
                uint8_t g = ColorUtils::getGreen(pixel);
                uint8_t b = ColorUtils::getBlue(pixel);
                
                // Show red channel in top-left quadrant
                if (currentX < width/2 && currentY < height/2) {
                    canvas->setPixel(x + currentX, y + currentY, ColorUtils::rgba(r, 0, 0, 255));
                }
                // Green channel in top-right
                else if (currentX >= width/2 && currentY < height/2) {
                    canvas->setPixel(x + currentX, y + currentY, ColorUtils::rgba(0, g, 0, 255));
                }
                // Blue channel in bottom-left
                else if (currentX < width/2 && currentY >= height/2) {
                    canvas->setPixel(x + currentX, y + currentY, ColorUtils::rgba(0, 0, b, 255));
                }
                // Alpha channel in bottom-right (as grayscale)
                else {
                    uint8_t a = ColorUtils::getAlpha(pixel);
                    canvas->setPixel(x + currentX, y + currentY, ColorUtils::rgba(a, a, a, 255));
                }
            }
        }
    }
};
```

## Common Canvas Patterns

### Animation and Motion

```cpp
class CanvasAnimations {
public:
    static void animatedWavePattern(Canvas* canvas, float time) {
        int width = canvas->getWidth();
        int height = canvas->getHeight();
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Create wave pattern based on position and time
                float waveValue = sin((x * 0.02f) + (time * 2.0f)) * 
                                 cos((y * 0.03f) + (time * 1.5f));
                
                // Map wave value to color
                uint8_t intensity = (uint8_t)((waveValue + 1.0f) * 127.5f);
                uint32_t color = ColorUtils::hsv(time * 0.1f + waveValue * 0.2f, 0.8f, intensity / 255.0f);
                
                canvas->setPixel(x, y, color);
            }
        }
    }
    
    static void particleSystem(Canvas* canvas, std::vector<Particle>& particles, float deltaTime) {
        // Clear previous particle positions
        canvas->clear(0xFF000000);
        
        // Update and render particles
        for (auto& particle : particles) {
            // Update particle position
            particle.x += particle.velocityX * deltaTime;
            particle.y += particle.velocityY * deltaTime;
            particle.life -= deltaTime;
            
            // Apply gravity
            particle.velocityY += 100.0f * deltaTime;
            
            // Render particle if still alive
            if (particle.life > 0) {
                uint8_t alpha = (uint8_t)(particle.life * 255);
                uint32_t color = (alpha << 24) | (particle.color & 0x00FFFFFF);
                
                CanvasShapes::drawFilledCircle(canvas, (int)particle.x, (int)particle.y, 
                                              particle.size, color);
            }
        }
        
        // Remove dead particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                                      [](const Particle& p) { return p.life <= 0; }),
                       particles.end());
    }
    
private:
    struct Particle {
        float x, y;
        float velocityX, velocityY;
        float life;
        uint32_t color;
        int size;
    };
};
```

### Data Visualization

```cpp
class DataVisualization {
public:
    static void drawBarChart(Canvas* canvas, int x, int y, int width, int height,
                            const std::vector<float>& values, uint32_t barColor) {
        if (values.empty()) return;
        
        float maxValue = *std::max_element(values.begin(), values.end());
        if (maxValue <= 0) return;
        
        int barWidth = width / values.size();
        
        for (size_t i = 0; i < values.size(); ++i) {
            int barHeight = (int)((values[i] / maxValue) * height);
            int barX = x + i * barWidth;
            int barY = y + height - barHeight;
            
            CanvasShapes::drawFilledRectangle(canvas, barX, barY, barWidth - 1, barHeight, barColor);
        }
    }
    
    static void drawLineGraph(Canvas* canvas, int x, int y, int width, int height,
                             const std::vector<float>& values, uint32_t lineColor) {
        if (values.size() < 2) return;
        
        float minValue = *std::min_element(values.begin(), values.end());
        float maxValue = *std::max_element(values.begin(), values.end());
        float range = maxValue - minValue;
        if (range <= 0) return;
        
        // Convert values to screen coordinates
        std::vector<int> screenY(values.size());
        for (size_t i = 0; i < values.size(); ++i) {
            float normalized = (values[i] - minValue) / range;
            screenY[i] = y + height - (int)(normalized * height);
        }
        
        // Draw lines between points
        for (size_t i = 0; i < values.size() - 1; ++i) {
            int x1 = x + (i * width) / (values.size() - 1);
            int x2 = x + ((i + 1) * width) / (values.size() - 1);
            
            CanvasShapes::drawLine(canvas, x1, screenY[i], x2, screenY[i + 1], lineColor);
        }
    }
    
    static void drawPieChart(Canvas* canvas, int centerX, int centerY, int radius,
                            const std::vector<float>& values, const std::vector<uint32_t>& colors) {
        float total = 0;
        for (float value : values) total += value;
        if (total <= 0) return;
        
        float currentAngle = 0;
        
        for (size_t i = 0; i < values.size() && i < colors.size(); ++i) {
            float sliceAngle = (values[i] / total) * 2 * M_PI;
            
            // Draw pie slice
            drawPieSlice(canvas, centerX, centerY, radius, currentAngle, 
                        currentAngle + sliceAngle, colors[i]);
            
            currentAngle += sliceAngle;
        }
    }
    
private:
    static void drawPieSlice(Canvas* canvas, int centerX, int centerY, int radius,
                            float startAngle, float endAngle, uint32_t color) {
        // Simple pie slice drawing - could be optimized
        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                float distance = sqrt(x * x + y * y);
                if (distance <= radius) {
                    float angle = atan2(y, x);
                    if (angle < 0) angle += 2 * M_PI;
                    
                    if (angle >= startAngle && angle <= endAngle) {
                        canvas->setPixel(centerX + x, centerY + y, color);
                    }
                }
            }
        }
    }
};
```

## Troubleshooting Common Issues

### Canvas Performance Problems

```cpp
// Inefficient: Setting pixels one by one in non-optimal order
void inefficientDraw(Canvas* canvas) {
    for (int x = 0; x < canvas->getWidth(); ++x) {    // Column-major access
        for (int y = 0; y < canvas->getHeight(); ++y) { // Poor cache locality
            canvas->setPixel(x, y, 0xFFFF0000);
        }
    }
}

// Efficient: Row-major access with minimal bounds checking
void efficientDraw(Canvas* canvas) {
    uint32_t* buffer = canvas->getBuffer();
    int width = canvas->getWidth();
    int height = canvas->getHeight();
    
    for (int y = 0; y < height; ++y) {              // Row-major access
        uint32_t* row = buffer + y * width;          // Direct buffer access
        for (int x = 0; x < width; ++x) {           // Good cache locality
            row[x] = 0xFFFF0000;
        }
    }
}
```

### Color and Alpha Issues

```cpp
// Common alpha blending mistakes and solutions
class AlphaBlending {
public:
    // Incorrect: Simple replacement ignores existing alpha
    static void incorrectBlend(Canvas* canvas, int x, int y, uint32_t newColor) {
        canvas->setPixel(x, y, newColor); // Overwrites existing pixel
    }
    
    // Correct: Proper alpha blending
    static void correctBlend(Canvas* canvas, int x, int y, uint32_t newColor) {
        uint32_t existing = canvas->getPixel(x, y);
        uint32_t blended = blendColors(existing, newColor);
        canvas->setPixel(x, y, blended);
    }
    
private:
    static uint32_t blendColors(uint32_t background, uint32_t foreground) {
        uint8_t fgA = ColorUtils::getAlpha(foreground);
        uint8_t fgR = ColorUtils::getRed(foreground);
        uint8_t fgG = ColorUtils::getGreen(foreground);
        uint8_t fgB = ColorUtils::getBlue(foreground);
        
        uint8_t bgA = ColorUtils::getAlpha(background);
        uint8_t bgR = ColorUtils::getRed(background);
        uint8_t bgG = ColorUtils::getGreen(background);
        uint8_t bgB = ColorUtils::getBlue(background);
        
        float alpha = fgA / 255.0f;
        float invAlpha = 1.0f - alpha;
        
        uint8_t resultR = (uint8_t)(fgR * alpha + bgR * invAlpha);
        uint8_t resultG = (uint8_t)(fgG * alpha + bgG * invAlpha);
        uint8_t resultB = (uint8_t)(fgB * alpha + bgB * invAlpha);
        uint8_t resultA = std::max(fgA, bgA);
        
        return ColorUtils::rgba(resultR, resultG, resultB, resultA);
    }
};
```

### Memory and Bounds Issues

```cpp
// Safe canvas operations with proper bounds checking
class SafeCanvasOps {
public:
    static void safeDrawRectangle(Canvas* canvas, int x, int y, int width, int height, uint32_t color) {
        // Clip rectangle to canvas bounds
        int clipX = std::max(0, x);
        int clipY = std::max(0, y);
        int clipWidth = std::min(width, canvas->getWidth() - clipX);
        int clipHeight = std::min(height, canvas->getHeight() - clipY);
        
        if (clipWidth <= 0 || clipHeight <= 0) return;
        
        for (int currentY = clipY; currentY < clipY + clipHeight; ++currentY) {
            for (int currentX = clipX; currentX < clipX + clipWidth; ++currentX) {
                canvas->setPixel(currentX, currentY, color);
            }
        }
    }
    
    static void validateCanvasState(Canvas* canvas) {
        if (!canvas) {
            throw std::runtime_error("Canvas is null");
        }
        
        if (canvas->getWidth() <= 0 || canvas->getHeight() <= 0) {
            throw std::runtime_error("Canvas has invalid dimensions");
        }
        
        if (!canvas->getBuffer()) {
            throw std::runtime_error("Canvas buffer is null");
        }
    }
};
```

## Summary

The canvas system in Fern provides the fundamental infrastructure for all visual output in your applications. By understanding pixel-level graphics, color theory, and efficient rendering techniques, you can create everything from simple UI elements to complex visual effects and data visualizations.

Key takeaways:

- **Pixel-Level Control**: Direct access to every pixel enables maximum flexibility for custom graphics
- **RGBA Color Model**: Standard 32-bit color representation with transparency support
- **Performance Focus**: Optimized memory layouts and access patterns for efficient rendering
- **Bounds Safety**: Automatic coordinate validation prevents crashes and undefined behavior
- **Platform Abstraction**: Consistent interface across different rendering backends

The canvas system exemplifies Fern's philosophy: provide powerful, low-level capabilities wrapped in a safe, intuitive interface. Whether you're drawing simple shapes or implementing complex visual effects, the canvas gives you the foundation to bring your ideas to life while handling the complexity of pixel-perfect rendering.

Master the canvas, and you'll have the tools to create visually compelling applications that leverage the full power of pixel-based graphics while maintaining the safety and ease-of-use that makes Fern productive and enjoyable to work with.
