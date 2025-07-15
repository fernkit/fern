# Font System

The Fern Font System provides comprehensive text rendering capabilities supporting both bitmap fonts and TrueType fonts (TTF). It offers a unified API for loading, managing, and rendering text with different font types and sizes.

## Overview

The font system supports two main font types:
- **Bitmap Fonts**: Built-in pixel-based fonts, fast and lightweight
- **TTF Fonts**: TrueType fonts for high-quality, scalable text rendering

## Font Types

### Bitmap Fonts
- Built-in pixel-based fonts
- Fast rendering performance
- Fixed character set
- Best for pixel-perfect UI elements

### TTF Fonts
- TrueType font support
- Scalable to any size
- High-quality rendering
- Support for custom fonts

## API Reference

### Font Class

#### TTF Font Management
```cpp
static bool loadTTFFont(const std::string& name, const std::string& fontPath);
static void setDefaultTTFFont(const std::string& name);
static bool hasTTFFont();
```

#### Unified Text Rendering
```cpp
static void renderText(Canvas* canvas, const std::string& text, int x, int y, 
                      int size, uint32_t color, FontType type = FontType::Bitmap);
```

#### Specific Font Rendering
```cpp
static void renderBitmap(Canvas* canvas, const std::string& text, int x, int y, 
                        int size, uint32_t color);
static void renderTTF(Canvas* canvas, const std::string& text, int x, int y, 
                     int size, uint32_t color, const std::string& fontName = "");
```

#### Text Metrics
```cpp
static int getTextWidth(const std::string& text, int size, FontType type = FontType::Bitmap);
static int getTextHeight(int size, FontType type = FontType::Bitmap);
```

### TTF Helper Namespace

```cpp
namespace TTF {
    bool load(const std::string& name, const std::string& path);
    void setDefault(const std::string& name);
    void render(Canvas* canvas, const std::string& text, int x, int y, 
               int size, uint32_t color, const std::string& fontName = "");
    int textWidth(const std::string& text, int size, const std::string& fontName = "");
    int textHeight(int size, const std::string& fontName = "");
}
```

## Basic Usage

### Using Bitmap Fonts
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Bitmap Font Example");
    
    // Get canvas for rendering
    Canvas* canvas = getCanvas();
    
    while (isRunning()) {
        clear();
        
        // Render bitmap text
        Font::renderBitmap(canvas, "Hello, Bitmap Font!", 100, 100, 16, Colors::Black);
        
        // Or use unified API
        Font::renderText(canvas, "Unified API", 100, 150, 20, Colors::Blue, FontType::Bitmap);
        
        present();
    }
    
    return 0;
}
```

### Using TTF Fonts
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "TTF Font Example");
    
    // Load a TTF font
    if (Font::loadTTFFont("arial", "assets/fonts/arial.ttf")) {
        Font::setDefaultTTFFont("arial");
    }
    
    Canvas* canvas = getCanvas();
    
    while (isRunning()) {
        clear();
        
        if (Font::hasTTFFont()) {
            // Render TTF text
            Font::renderTTF(canvas, "Hello, TTF Font!", 100, 100, 24, Colors::Black);
            
            // Or use unified API
            Font::renderText(canvas, "Scalable Text", 100, 150, 32, Colors::Blue, FontType::TTF);
        } else {
            // Fallback to bitmap font
            Font::renderBitmap(canvas, "TTF not available", 100, 100, 16, Colors::Red);
        }
        
        present();
    }
    
    return 0;
}
```

### Using TTF Helper Functions
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "TTF Helper Example");
    
    // Load fonts using helper functions
    TTF::load("roboto", "assets/fonts/roboto.ttf");
    TTF::load("opensans", "assets/fonts/opensans.ttf");
    TTF::setDefault("roboto");
    
    Canvas* canvas = getCanvas();
    
    while (isRunning()) {
        clear();
        
        // Use helper functions for rendering
        TTF::render(canvas, "Default Font", 100, 100, 20, Colors::Black);
        TTF::render(canvas, "Specific Font", 100, 150, 18, Colors::Blue, "opensans");
        
        present();
    }
    
    return 0;
}
```

## Advanced Usage

### Text Measurement and Layout
```cpp
#include "fern/fern.hpp"
using namespace Fern;

void drawCenteredText(Canvas* canvas, const std::string& text, int x, int y, 
                     int size, uint32_t color) {
    int textWidth = Font::getTextWidth(text, size, FontType::TTF);
    int textHeight = Font::getTextHeight(size, FontType::TTF);
    
    int centeredX = x - textWidth / 2;
    int centeredY = y - textHeight / 2;
    
    Font::renderTTF(canvas, text, centeredX, centeredY, size, color);
}

void drawTextBox(Canvas* canvas, const std::string& text, int x, int y, 
                int width, int height, int fontSize, uint32_t color) {
    // Calculate text dimensions
    int textWidth = Font::getTextWidth(text, fontSize, FontType::TTF);
    int textHeight = Font::getTextHeight(fontSize, FontType::TTF);
    
    // Center text in box
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - textHeight) / 2;
    
    // Draw background box
    Draw::rect(x, y, width, height, Colors::LightGray);
    
    // Draw text
    Font::renderTTF(canvas, text, textX, textY, fontSize, color);
}
```

### Multi-Line Text Rendering
```cpp
#include "fern/fern.hpp"
using namespace Fern;

void drawMultiLineText(Canvas* canvas, const std::string& text, int x, int y, 
                      int fontSize, uint32_t color, int lineSpacing = 5) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    
    // Split text into lines
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    
    int lineHeight = Font::getTextHeight(fontSize, FontType::TTF) + lineSpacing;
    int currentY = y;
    
    for (const auto& line : lines) {
        Font::renderTTF(canvas, line, x, currentY, fontSize, color);
        currentY += lineHeight;
    }
}
```

### Font Fallback System
```cpp
#include "fern/fern.hpp"
using namespace Fern;

class FontManager {
private:
    std::vector<std::string> fontStack_;
    
public:
    void addFont(const std::string& name, const std::string& path) {
        if (Font::loadTTFFont(name, path)) {
            fontStack_.push_back(name);
        }
    }
    
    void renderText(Canvas* canvas, const std::string& text, int x, int y, 
                   int size, uint32_t color) {
        for (const auto& fontName : fontStack_) {
            try {
                Font::renderTTF(canvas, text, x, y, size, color, fontName);
                return;
            } catch (...) {
                continue; // Try next font
            }
        }
        
        // Final fallback to bitmap font
        Font::renderBitmap(canvas, text, x, y, size, color);
    }
};
```

## Widget Integration

### Text Widget with Font Support
```cpp
class CustomTextWidget : public Widget {
private:
    std::string text_;
    int fontSize_;
    uint32_t color_;
    FontType fontType_;
    std::string fontName_;
    
public:
    CustomTextWidget(const std::string& text, int x, int y, int fontSize, 
                    uint32_t color, FontType type = FontType::Bitmap)
        : text_(text), fontSize_(fontSize), color_(color), fontType_(type) {
        x_ = x;
        y_ = y;
        
        // Calculate widget dimensions based on text
        width_ = Font::getTextWidth(text_, fontSize_, fontType_);
        height_ = Font::getTextHeight(fontSize_, fontType_);
    }
    
    void render() override {
        Canvas* canvas = getCanvas();
        
        if (fontType_ == FontType::TTF) {
            Font::renderTTF(canvas, text_, x_, y_, fontSize_, color_, fontName_);
        } else {
            Font::renderBitmap(canvas, text_, x_, y_, fontSize_, color_);
        }
    }
    
    void setText(const std::string& text) {
        text_ = text;
        // Recalculate dimensions
        width_ = Font::getTextWidth(text_, fontSize_, fontType_);
        height_ = Font::getTextHeight(fontSize_, fontType_);
    }
    
    void setFont(const std::string& fontName) {
        fontName_ = fontName;
        fontType_ = FontType::TTF;
        // Recalculate dimensions
        width_ = Font::getTextWidth(text_, fontSize_, fontType_);
        height_ = Font::getTextHeight(fontSize_, fontType_);
    }
};
```

## Font Loading and Management

### Loading Multiple Fonts
```cpp
#include "fern/fern.hpp"
using namespace Fern;

class FontLibrary {
private:
    std::unordered_map<std::string, std::string> fonts_;
    
public:
    void init() {
        // Load system fonts
        loadFont("system", "/system/fonts/default.ttf");
        loadFont("mono", "/system/fonts/mono.ttf");
        
        // Load custom fonts
        loadFont("title", "assets/fonts/title.ttf");
        loadFont("body", "assets/fonts/body.ttf");
        loadFont("icon", "assets/fonts/icons.ttf");
    }
    
    bool loadFont(const std::string& name, const std::string& path) {
        if (Font::loadTTFFont(name, path)) {
            fonts_[name] = path;
            return true;
        }
        return false;
    }
    
    std::vector<std::string> getAvailableFonts() {
        std::vector<std::string> fontNames;
        for (const auto& font : fonts_) {
            fontNames.push_back(font.first);
        }
        return fontNames;
    }
};
```

### Font Preloading
```cpp
void preloadFonts() {
    // Load commonly used fonts at application startup
    struct FontInfo {
        std::string name;
        std::string path;
        bool isDefault;
    };
    
    std::vector<FontInfo> fontsToLoad = {
        {"default", "assets/fonts/default.ttf", true},
        {"bold", "assets/fonts/bold.ttf", false},
        {"italic", "assets/fonts/italic.ttf", false},
        {"mono", "assets/fonts/mono.ttf", false}
    };
    
    for (const auto& font : fontsToLoad) {
        if (Font::loadTTFFont(font.name, font.path)) {
            printf("Loaded font: %s\n", font.name.c_str());
            if (font.isDefault) {
                Font::setDefaultTTFFont(font.name);
            }
        } else {
            printf("Failed to load font: %s\n", font.name.c_str());
        }
    }
}
```

## Performance Considerations

### Font Caching
```cpp
class FontCache {
private:
    struct CachedGlyph {
        std::vector<uint8_t> bitmap;
        int width, height;
        int bearingX, bearingY;
        int advance;
    };
    
    std::unordered_map<std::string, CachedGlyph> cache_;
    
public:
    std::string getCacheKey(char c, int fontSize, const std::string& fontName) {
        return fontName + "_" + std::to_string(fontSize) + "_" + std::to_string(c);
    }
    
    CachedGlyph* getGlyph(char c, int fontSize, const std::string& fontName) {
        std::string key = getCacheKey(c, fontSize, fontName);
        auto it = cache_.find(key);
        return (it != cache_.end()) ? &it->second : nullptr;
    }
    
    void cacheGlyph(char c, int fontSize, const std::string& fontName, 
                   const CachedGlyph& glyph) {
        std::string key = getCacheKey(c, fontSize, fontName);
        cache_[key] = glyph;
    }
};
```

### Optimized Text Rendering
```cpp
void renderOptimizedText(Canvas* canvas, const std::string& text, int x, int y, 
                        int fontSize, uint32_t color) {
    // Pre-calculate text dimensions
    int textWidth = Font::getTextWidth(text, fontSize, FontType::TTF);
    int textHeight = Font::getTextHeight(fontSize, FontType::TTF);
    
    // Skip rendering if text is outside visible area
    if (x + textWidth < 0 || x > canvas->getWidth() || 
        y + textHeight < 0 || y > canvas->getHeight()) {
        return;
    }
    
    // Render the text
    Font::renderTTF(canvas, text, x, y, fontSize, color);
}
```

## Best Practices

### 1. Font Selection
```cpp
// Use appropriate fonts for different contexts
namespace AppFonts {
    const std::string TITLE = "title";
    const std::string BODY = "body";
    const std::string MONO = "mono";
    const std::string ICON = "icon";
}

void renderUI() {
    Canvas* canvas = getCanvas();
    
    // Use title font for headings
    Font::renderTTF(canvas, "Application Title", 100, 50, 24, Colors::Black, AppFonts::TITLE);
    
    // Use body font for regular text
    Font::renderTTF(canvas, "Regular content text", 100, 100, 16, Colors::DarkGray, AppFonts::BODY);
    
    // Use mono font for code
    Font::renderTTF(canvas, "console.log('Hello')", 100, 150, 14, Colors::Blue, AppFonts::MONO);
}
```

### 2. Responsive Font Sizes
```cpp
int getResponsiveFontSize(int baseFontSize) {
    int screenWidth = getWidth();
    
    if (screenWidth < 600) {
        return baseFontSize * 0.8f;  // Smaller on mobile
    } else if (screenWidth > 1200) {
        return baseFontSize * 1.2f;  // Larger on desktop
    }
    
    return baseFontSize;
}
```

### 3. Text Accessibility
```cpp
void renderAccessibleText(Canvas* canvas, const std::string& text, int x, int y, 
                         int fontSize, uint32_t textColor, uint32_t backgroundColor) {
    // Ensure minimum font size for readability
    int minFontSize = 12;
    fontSize = std::max(fontSize, minFontSize);
    
    // Check color contrast (simplified example)
    if (textColor == backgroundColor) {
        textColor = (backgroundColor == Colors::White) ? Colors::Black : Colors::White;
    }
    
    // Render with accessible settings
    Font::renderTTF(canvas, text, x, y, fontSize, textColor);
}
```

## Error Handling

### Font Loading Errors
```cpp
bool loadFontSafely(const std::string& name, const std::string& path) {
    try {
        if (Font::loadTTFFont(name, path)) {
            printf("Successfully loaded font: %s\n", name.c_str());
            return true;
        } else {
            printf("Failed to load font: %s from %s\n", name.c_str(), path.c_str());
            return false;
        }
    } catch (const std::exception& e) {
        printf("Exception loading font %s: %s\n", name.c_str(), e.what());
        return false;
    }
}
```

### Fallback Rendering
```cpp
void renderTextWithFallback(Canvas* canvas, const std::string& text, int x, int y, 
                           int fontSize, uint32_t color) {
    // Try TTF first
    if (Font::hasTTFFont()) {
        try {
            Font::renderTTF(canvas, text, x, y, fontSize, color);
            return;
        } catch (...) {
            // Fall through to bitmap font
        }
    }
    
    // Fallback to bitmap font
    Font::renderBitmap(canvas, text, x, y, fontSize, color);
}
```

## Related Components

- [`Text Widget`](../widgets/text.md) - High-level text widget using the font system
- [`Colors`](colors.md) - Color constants for text rendering
- [`Canvas`](../core/canvas.md) - Low-level canvas operations
- [`Primitives`](primitives.md) - Basic drawing functions

## Tips

1. **Preload fonts**: Load fonts at application startup to avoid delays
2. **Use appropriate font sizes**: Ensure text is readable on target devices
3. **Cache measurements**: Store text width/height calculations for performance
4. **Test on different platforms**: Font rendering may vary across systems
5. **Provide fallbacks**: Always have a backup font system
6. **Consider file sizes**: TTF fonts can be large, optimize for your target platform

The Font System provides a robust foundation for text rendering in Fern applications, supporting both simple bitmap fonts and advanced TTF typography.
