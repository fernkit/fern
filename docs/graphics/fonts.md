# Typography and Font System Guide

Typography is the voice of your interface - it determines not just what your application says, but how it sounds in users' minds. Great typography creates hierarchy, establishes personality, ensures accessibility, and guides users through information with clarity and elegance. In this guide, you'll learn not just how to render text in Fern, but how to think about typography as a design system that speaks to users before they even read the words.

## Understanding Digital Typography

### The Evolution of Computer Text

Text rendering on computers has traveled through several revolutionary stages:

1. **Bitmap Fonts (1970s-1990s)**: Every character stored as a pixel grid - fast, precise, but fixed size
2. **Vector Fonts (1980s-present)**: Characters defined by mathematical curves - scalable and smooth
3. **Subpixel Rendering (2000s)**: Using monitor color structure for sharper text
4. **Variable Fonts (2010s)**: Single font files with multiple weights and styles

Each approach solves different problems, and Fern embraces both bitmap and vector (TTF) fonts to give you the perfect tool for every situation.

### Typography Psychology and Communication

Typography communicates meaning through multiple channels:

- **Legibility**: How easily individual characters can be distinguished
- **Readability**: How easily words and sentences can be understood
- **Personality**: The emotional tone and character the typeface conveys
- **Hierarchy**: How size, weight, and spacing guide attention
- **Cultural Context**: The associations and expectations different fonts carry

```cpp
// Typography creates emotional context
Font::renderText(canvas, "Welcome!", x, y, 24, Colors::Primary, FontType::TTF);    // Friendly, professional
Font::renderText(canvas, "Error!", x, y, 16, Colors::Danger, FontType::Bitmap);   // Urgent, system-level
Font::renderText(canvas, "Loading...", x, y, 12, Colors::Gray, FontType::Bitmap); // Patient, informational
```

## Font Philosophy in Fern

Fern's typography system follows these design principles:

- **Dual System**: Bitmap fonts for UI precision, TTF fonts for content beauty
- **Performance Conscious**: Efficient rendering for real-time applications
- **Accessibility First**: Clear, readable text at all sizes and contexts
- **Creative Freedom**: Support for custom fonts and advanced typography
- **Educational Transparency**: Understanding how text rendering actually works

This approach ensures your text feels both technically excellent and beautifully readable while maintaining the educational clarity that makes Fern special.

## The Two Font Systems

### Bitmap Fonts - Pixel Perfect Precision

Bitmap fonts are hand-crafted pixel art where every character is designed at specific sizes for perfect clarity:

```cpp
using namespace Fern;

// Bitmap fonts excel at UI elements and system text
Font::renderBitmap(canvas, "Health: 100/100", 10, 10, 12, Colors::White);
Font::renderBitmap(canvas, "Press SPACE to continue", 10, 30, 8, Colors::LightGray);
Font::renderBitmap(canvas, "GAME OVER", 100, 200, 16, Colors::Red);
```

**Use bitmap fonts for**:
- UI labels, buttons, and status text
- Retro/pixel art aesthetic
- Performance-critical situations
- Fixed-size interface elements
- Guaranteed pixel-perfect alignment

**Bitmap font characteristics**:
- Lightning fast rendering
- Consistent appearance across platforms
- Perfect for pixel-aligned interfaces
- Limited to designed sizes
- Ideal for system/UI text

### TTF Fonts - Scalable Typography Beauty

TrueType fonts use mathematical curves to create smooth, scalable text at any size:

```cpp
// Load a custom font for your application
Font::loadTTFFont("heading", "/path/to/fonts/Roboto-Bold.ttf");
Font::loadTTFFont("body", "/path/to/fonts/OpenSans-Regular.ttf");
Font::setDefaultTTFFont("body");

// TTF fonts excel at content and artistic typography
Font::renderTTF(canvas, "Welcome to Fern", 50, 50, 32, Colors::Charcoal, "heading");
Font::renderTTF(canvas, "This paragraph demonstrates beautiful, scalable typography...", 
                50, 100, 14, Colors::DarkGray, "body");
```

**Use TTF fonts for**:
- Article text, documentation, content
- Headlines and marketing copy
- Creative and branded typography
- Multi-language support
- Professional document interfaces

**TTF font characteristics**:
- Smooth at any size
- Rich character sets and language support
- Professional typography features
- Slightly slower rendering
- Perfect for content and branding

## Typography Hierarchy and Scale

Create visual hierarchy through size, weight, and spacing:

```cpp
// Establish a typographic scale (based on musical harmony)
namespace TypographyScale {
    constexpr int Headline1 = 32;    // 2.0x base
    constexpr int Headline2 = 24;    // 1.5x base
    constexpr int Headline3 = 20;    // 1.25x base
    constexpr int Body = 16;         // 1.0x base (comfortable reading size)
    constexpr int Caption = 12;      // 0.75x base
    constexpr int Small = 10;        // 0.625x base
}

// Apply hierarchy to create scannable content
auto createArticle = [](Canvas* canvas) {
    int x = 50, y = 50;
    
    // Main headline - largest, boldest
    Font::renderTTF(canvas, "The Art of Interface Design", 
                    x, y, TypographyScale::Headline1, Colors::Charcoal, "heading");
    y += 50;
    
    // Subheading - medium emphasis
    Font::renderTTF(canvas, "Creating intuitive user experiences", 
                    x, y, TypographyScale::Headline3, Colors::DarkGray, "body");
    y += 35;
    
    // Body text - comfortable reading
    Font::renderTTF(canvas, "Great interface design is invisible. Users shouldn't think about...", 
                    x, y, TypographyScale::Body, Colors::DarkGray, "body");
    y += 25;
    
    // Caption - supporting information
    Font::renderTTF(canvas, "Published March 2024 • 5 min read", 
                    x, y, TypographyScale::Caption, Colors::Gray, "body");
};
```

## Your First Typography System

Let's create a complete typography system that demonstrates both font types working together:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    setupFern();
    
    // Load custom fonts for content
    Font::loadTTFFont("display", "assets/fonts/Roboto-Bold.ttf");
    Font::loadTTFFont("text", "assets/fonts/OpenSans-Regular.ttf");
    Font::setDefaultTTFFont("text");
    
    auto canvas = getCanvas();
    
    // Application title - TTF for elegance
    Font::renderTTF(canvas, "Typography Demo", 50, 30, 28, Colors::Primary, "display");
    
    // Content paragraph - TTF for readability
    std::string content = "This paragraph demonstrates how TTF fonts create beautiful, "
                         "readable text for content. Notice the smooth curves and "
                         "professional appearance.";
    Font::renderTTF(canvas, content, 50, 80, 14, Colors::Charcoal, "text");
    
    // UI Elements - Bitmap for precision
    Font::renderBitmap(canvas, "File", 50, 150, 12, Colors::White);        // Menu item
    Font::renderBitmap(canvas, "Edit", 80, 150, 12, Colors::White);        // Menu item
    Font::renderBitmap(canvas, "View", 110, 150, 12, Colors::White);       // Menu item
    
    // Status bar - Bitmap for system feel
    Font::renderBitmap(canvas, "Ready", 50, 400, 10, Colors::LightGray);
    Font::renderBitmap(canvas, "Line 1, Col 1", 200, 400, 10, Colors::Gray);
    
    // Button labels - Bitmap for UI consistency
    Font::renderBitmap(canvas, "OK", 350, 200, 12, Colors::White);
    Font::renderBitmap(canvas, "Cancel", 400, 200, 12, Colors::White);
    
    runFern();
    return 0;
}
```

## Advanced Typography Techniques

### Text Measurement and Layout

Proper text layout requires understanding text metrics:

```cpp
// Measure text for precise layout
auto createCenteredText = [](Canvas* canvas, const std::string& text, 
                            int containerWidth, int y, int size, uint32_t color) {
    int textWidth = Font::getTextWidth(text, size, FontType::TTF);
    int x = (containerWidth - textWidth) / 2;  // Center horizontally
    
    Font::renderTTF(canvas, text, x, y, size, color);
    return x + textWidth;  // Return end position for chaining
};

// Create perfectly aligned text
createCenteredText(canvas, "Centered Headline", 800, 100, 24, Colors::Primary);
createCenteredText(canvas, "Perfectly aligned subtitle", 800, 130, 16, Colors::DarkGray);

// Multi-line text with proper line spacing
auto renderParagraph = [](Canvas* canvas, const std::string& text, 
                         int x, int y, int width, int fontSize, uint32_t color) {
    int lineHeight = Font::getTextHeight(fontSize, FontType::TTF) * 1.4f;  // 1.4x for readability
    int currentY = y;
    
    // Simple word wrapping (simplified example)
    std::vector<std::string> words = splitWords(text);
    std::string currentLine;
    
    for (const auto& word : words) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        int lineWidth = Font::getTextWidth(testLine, fontSize, FontType::TTF);
        
        if (lineWidth > width && !currentLine.empty()) {
            // Render current line and start new one
            Font::renderTTF(canvas, currentLine, x, currentY, fontSize, color);
            currentY += lineHeight;
            currentLine = word;
        } else {
            currentLine = testLine;
        }
    }
    
    // Render final line
    if (!currentLine.empty()) {
        Font::renderTTF(canvas, currentLine, x, currentY, fontSize, color);
    }
};
```

### Font Loading and Management

Organize your typography system with proper font management:

```cpp
// Typography system for your application
class TypographySystem {
private:
    std::map<std::string, std::string> fontPaths;
    std::string defaultFont;
    
public:
    // Load your application's font family
    void initialize() {
        // Load font variants
        loadFont("display-light", "assets/fonts/Roboto-Light.ttf");
        loadFont("display-regular", "assets/fonts/Roboto-Regular.ttf");
        loadFont("display-bold", "assets/fonts/Roboto-Bold.ttf");
        
        loadFont("body-regular", "assets/fonts/OpenSans-Regular.ttf");
        loadFont("body-italic", "assets/fonts/OpenSans-Italic.ttf");
        loadFont("body-bold", "assets/fonts/OpenSans-Bold.ttf");
        
        loadFont("mono", "assets/fonts/FiraCode-Regular.ttf");
        
        setDefault("body-regular");
    }
    
    void loadFont(const std::string& name, const std::string& path) {
        if (Font::loadTTFFont(name, path)) {
            fontPaths[name] = path;
            std::cout << "Loaded font: " << name << std::endl;
        } else {
            std::cerr << "Failed to load font: " << name << " from " << path << std::endl;
        }
    }
    
    void setDefault(const std::string& name) {
        defaultFont = name;
        Font::setDefaultTTFFont(name);
    }
    
    // Semantic rendering functions
    void renderHeadline(Canvas* canvas, const std::string& text, int x, int y, uint32_t color = Colors::Charcoal) {
        Font::renderTTF(canvas, text, x, y, 24, color, "display-bold");
    }
    
    void renderSubheading(Canvas* canvas, const std::string& text, int x, int y, uint32_t color = Colors::DarkGray) {
        Font::renderTTF(canvas, text, x, y, 18, color, "display-regular");
    }
    
    void renderBody(Canvas* canvas, const std::string& text, int x, int y, uint32_t color = Colors::DarkGray) {
        Font::renderTTF(canvas, text, x, y, 14, color, "body-regular");
    }
    
    void renderCode(Canvas* canvas, const std::string& text, int x, int y, uint32_t color = Colors::Charcoal) {
        Font::renderTTF(canvas, text, x, y, 12, color, "mono");
    }
    
    void renderUI(Canvas* canvas, const std::string& text, int x, int y, uint32_t color = Colors::White) {
        Font::renderBitmap(canvas, text, x, y, 12, color);  // Use bitmap for UI
    }
};

// Global typography instance
TypographySystem typography;
```

### Responsive Typography

Create typography that adapts to different screen sizes and contexts:

```cpp
// Responsive typography based on screen size
class ResponsiveTypography {
private:
    int screenWidth;
    int screenHeight;
    
public:
    ResponsiveTypography(int width, int height) : screenWidth(width), screenHeight(height) {}
    
    // Calculate responsive font sizes
    int getHeadlineSize() const {
        if (screenWidth < 600) return 20;      // Mobile
        if (screenWidth < 1200) return 24;     // Tablet
        return 32;                             // Desktop
    }
    
    int getBodySize() const {
        if (screenWidth < 600) return 12;      // Mobile
        if (screenWidth < 1200) return 14;     // Tablet  
        return 16;                             // Desktop
    }
    
    int getMaxLineWidth() const {
        // Optimal reading width: 45-75 characters
        int charWidth = getBodySize() * 0.6f;  // Approximate character width
        int optimalChars = 65;                 // Sweet spot for readability
        return std::min(screenWidth - 100, optimalChars * charWidth);
    }
    
    void renderResponsiveHeadline(Canvas* canvas, const std::string& text, int x, int y) {
        Font::renderTTF(canvas, text, x, y, getHeadlineSize(), Colors::Primary, "display-bold");
    }
    
    void renderResponsiveBody(Canvas* canvas, const std::string& text, int x, int y) {
        Font::renderTTF(canvas, text, x, y, getBodySize(), Colors::DarkGray, "body-regular");
    }
};
```

## Typography Accessibility and Inclusive Design

### Font Size and Readability

Ensure your typography works for users with different visual abilities:

```cpp
// Accessibility-focused typography settings
namespace AccessibleTypography {
    // Minimum sizes for readability
    constexpr int MinimumUIText = 12;      // Smallest for UI elements
    constexpr int MinimumBody = 14;        // Comfortable body text
    constexpr int LargeText = 18;          // For accessibility preference
    constexpr int ExtraLarge = 24;         // High accessibility needs
    
    // High-contrast color combinations
    struct TextStyle {
        uint32_t color;
        uint32_t background;
        int size;
        std::string font;
    };
    
    const TextStyle HighContrastDark = {Colors::White, Colors::Black, 16, "body-bold"};
    const TextStyle HighContrastLight = {Colors::Black, Colors::White, 16, "body-bold"};
    const TextStyle StandardReadable = {Colors::Charcoal, Colors::LightGray, 14, "body-regular"};
}

// User preference system
class AccessibilityPreferences {
private:
    float fontSizeMultiplier = 1.0f;
    bool highContrast = false;
    bool reducedMotion = false;
    
public:
    void setFontSizeMultiplier(float multiplier) {
        fontSizeMultiplier = std::clamp(multiplier, 0.8f, 2.0f);
    }
    
    void setHighContrast(bool enabled) {
        highContrast = enabled;
    }
    
    int adjustFontSize(int baseSize) const {
        return static_cast<int>(baseSize * fontSizeMultiplier);
    }
    
    uint32_t getTextColor() const {
        return highContrast ? Colors::White : Colors::Charcoal;
    }
    
    uint32_t getBackgroundColor() const {
        return highContrast ? Colors::Black : Colors::White;
    }
};
```

### Internationalization and Character Support

Design typography systems that work across languages:

```cpp
// Multi-language typography considerations
namespace InternationalTypography {
    // Different languages have different reading patterns
    enum class ReadingDirection {
        LeftToRight,    // English, Spanish, French, etc.
        RightToLeft,    // Arabic, Hebrew
        TopToBottom     // Traditional Chinese, Japanese
    };
    
    // Font recommendations by script
    std::map<std::string, std::string> scriptFonts = {
        {"latin", "body-regular"},          // English, European languages
        {"cyrillic", "body-regular"},       // Russian, Bulgarian, etc.
        {"arabic", "noto-arabic"},          // Arabic scripts
        {"cjk", "noto-cjk"},               // Chinese, Japanese, Korean
        {"devanagari", "noto-devanagari"}   // Hindi, Sanskrit
    };
    
    // Language-specific line heights and spacing
    float getLineHeightMultiplier(const std::string& language) {
        if (language == "thai" || language == "myanmar") return 1.6f;  // Scripts with ascenders/descenders
        if (language == "arabic" || language == "urdu") return 1.5f;   // Connected scripts
        return 1.4f;  // Default for Latin scripts
    }
}
```

## Real-World Typography Examples

### Document Interface

```cpp
// Create a document editing interface with proper typography
auto createDocumentInterface = [](Canvas* canvas) {
    // Document title area
    Font::renderTTF(canvas, "Research Paper Draft", 50, 30, 20, Colors::Charcoal, "display-bold");
    Font::renderTTF(canvas, "Last edited: March 15, 2024", 50, 55, 12, Colors::Gray, "body-regular");
    
    // Document content with proper typography
    int contentX = 50, contentY = 100;
    int lineHeight = 20;
    
    Font::renderTTF(canvas, "Abstract", contentX, contentY, 16, Colors::Primary, "display-bold");
    contentY += 30;
    
    std::string abstractText = "This paper examines the relationship between typography and user experience "
                              "in digital interfaces. Through careful analysis of font choices, sizing, "
                              "and spacing, we demonstrate how typography affects both readability and "
                              "emotional response in interactive applications.";
    
    renderParagraph(canvas, abstractText, contentX, contentY, 500, 14, Colors::DarkGray);
    contentY += 80;
    
    Font::renderTTF(canvas, "1. Introduction", contentX, contentY, 16, Colors::Primary, "display-bold");
    contentY += 25;
    
    // Code example with monospace font
    Font::renderTTF(canvas, "Font::renderTTF(canvas, text, x, y, size, color);", 
                    contentX + 20, contentY, 12, Colors::DarkBlue, "mono");
};
```

### Game UI Typography

```cpp
// Typography for game interfaces - mixing bitmap and TTF
auto createGameUI = [](Canvas* canvas) {
    // Player stats - bitmap for crisp UI
    Font::renderBitmap(canvas, "Health:", 10, 10, 12, Colors::White);
    Font::renderBitmap(canvas, "100/100", 70, 10, 12, Colors::LightGreen);
    
    Font::renderBitmap(canvas, "Mana:", 10, 25, 12, Colors::White);
    Font::renderBitmap(canvas, "50/75", 55, 25, 12, Colors::SkyBlue);
    
    Font::renderBitmap(canvas, "XP:", 10, 40, 12, Colors::White);
    Font::renderBitmap(canvas, "1,250", 40, 40, 12, Colors::Gold);
    
    // Dialog text - TTF for story immersion
    Font::renderTTF(canvas, "The ancient wizard speaks:", 200, 350, 14, Colors::LightGray, "body-italic");
    Font::renderTTF(canvas, "\"Young adventurer, your journey has only just begun...\"", 
                    200, 375, 16, Colors::White, "body-regular");
    
    // Action prompts - bitmap for immediacy
    Font::renderBitmap(canvas, "Press SPACE to continue", 200, 420, 10, Colors::Yellow);
    Font::renderBitmap(canvas, "Press TAB for inventory", 200, 435, 10, Colors::LightGray);
};
```

Typography is the invisible foundation that makes interfaces feel professional, accessible, and beautiful. By understanding both the technical aspects of font rendering and the design principles of good typography, you can create Fern applications that communicate clearly and feel delightfully readable.

Remember: typography should be felt, not noticed. When users can focus on your content without struggling to read it, you've achieved typographic success.
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
