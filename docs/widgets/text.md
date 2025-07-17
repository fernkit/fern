# Text Widget Guide

Text is the foundation of almost every user interface - from simple labels to complex documents. In this guide, you'll learn not just how to display text in Fern, but understand the fundamental concepts behind text rendering, typography, and how pixels become readable information.

## Understanding Text in Digital Interfaces

Before diving into Fern's text system, let's understand what's actually happening when text appears on your screen. Every letter you see is composed of tiny colored pixels arranged in specific patterns. A computer needs to:

1. **Know what to draw**: The text content itself
2. **Know where to draw it**: Position coordinates  
3. **Know how to draw it**: Font, size, color, alignment
4. **Calculate the pixel patterns**: Transform character data into visual pixels
5. **Handle different rendering methods**: Bitmap fonts vs scalable fonts

Fern exposes this entire pipeline, letting you see and control each step.

## Text Philosophy in Fern

Fern takes a **dual-approach** to text rendering:

- **Bitmap Fonts**: Small, pixelated, retro-style text perfect for game interfaces and compact displays
- **TTF Fonts**: Smooth, scalable fonts suitable for documents and modern interfaces

This isn't just about aesthetics - these represent two fundamentally different approaches to storing and rendering text. Bitmap fonts store each character as a pre-drawn pixel pattern, while TTF fonts store mathematical descriptions that can be scaled to any size.

## Your First Text Widget

Let's start with the simplest possible text:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Simple text at position 100, 50
    auto text = Text(Point(100, 50), "Hello, Fern!", 2, Colors::White);
    
    // Start the game loop
    while (shouldKeepRunning()) {
        processInput();
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `Point(100, 50)`: Position where text starts (X=100 pixels from left, Y=50 pixels from top)
- `"Hello, Fern!"`: The actual text content
- `2`: Font size (for bitmap fonts, this is a multiplier - 1x, 2x, 3x, etc.)
- `Colors::White`: Text color using Fern's color system

When this runs, Fern calculates which pixels need to be white to form the letters "Hello, Fern!" and draws them at the specified position.

## Modern Configuration System

While the simple constructor is great for quick text, Fern provides a powerful configuration system for complex text styling:

```cpp
// Create a text style
TextStyle style;
style.color(Colors::CornflowerBlue)
     .fontSize(3)
     .useBitmapFont()
     .backgroundColor(Colors::DarkGray)
     .padding(5)
     .alignment(1)  // Center alignment
     .shadow(true, Colors::Black, 2);

// Create text configuration
auto config = TextConfig(200, 100, "Styled Text")
    .style(style);

// Create the widget
auto styledText = Text(config);
```

**Understanding the Configuration Flow:**
1. **TextStyle**: Defines how the text should look (color, font, effects)
2. **TextConfig**: Combines position, content, and style
3. **Text()**: Factory function that creates the actual widget

This separation allows you to:
- Reuse styles across multiple text widgets
- Configure complex styling step by step
- Mix and match different aspects easily

## Font Systems Deep Dive

### Bitmap Fonts: The Pixel Art Approach

Bitmap fonts store each character as a fixed-size pixel pattern. Think of them like tiny stamps:

```cpp
// Bitmap font text - perfect for retro games
TextStyle retro;
retro.useBitmapFont()
     .fontSize(4)  // 4x scale of base pixel pattern
     .color(Colors::LimeGreen);

auto pixelText = Text(TextConfig(50, 50, "PIXEL PERFECT").style(retro));
```

**Bitmap Font Characteristics:**
- Sizes 1-5 work well (these are scale multipliers)
- Sharp, pixelated appearance
- Very fast rendering
- Perfect for game UIs, terminals, retro aesthetics
- Limited scaling options

### TTF Fonts: The Complex Reality

TTF (TrueType Font) fonts store mathematical descriptions of characters, but using them in Fern requires understanding the full loading process and its limitations. **You cannot simply specify a font name** - you must first load actual font files from your system.

#### The TTF Loading Process

Before you can use any TTF font, you must explicitly load it from a font file:

```cpp
// STEP 1: Load TTF fonts explicitly using actual font files
std::cout << "Loading TTF fonts..." << std::endl;

// Try to load fonts from common system locations
std::vector<std::pair<std::string, std::string>> fontOptions = {
    {"liberation", "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"},
    {"dejavu", "/usr/share/fonts/TTF/DejaVuSans.ttf"},
    {"ubuntu", "/usr/share/fonts/truetype/ubuntu/Ubuntu-Regular.ttf"},
    {"arial", "/System/Library/Fonts/Arial.ttf"},           // macOS
    {"arial", "C:\\Windows\\Fonts\\arial.ttf"},            // Windows
    {"custom", "assets/fonts/roboto.ttf"}                  // Your custom fonts
};

std::string loadedFontName = "";
for (const auto& fontOption : fontOptions) {
    if (Font::loadTTFFont(fontOption.first, fontOption.second)) {
        loadedFontName = fontOption.first;
        std::cout << "Successfully loaded: " << fontOption.first << std::endl;
        break;
    }
}

// STEP 2: Set default TTF font (optional but recommended)
if (!loadedFontName.empty()) {
    Font::setDefaultTTFFont(loadedFontName);
}
```

#### Using Loaded TTF Fonts

Only after loading can you use the font in your text widgets:

```cpp
// STEP 3: Now you can use the loaded font
if (!loadedFontName.empty()) {
    TextStyle modern;
    modern.useTTFFont(loadedFontName)  // Use the loaded font name
          .fontSize(24)                // Point size (16+ recommended)
          .color(Colors::Navy);

    auto smoothText = Text(TextConfig(50, 100, "Smooth and Scalable").style(modern));
} else {
    // Fallback to bitmap fonts if TTF loading fails
    TextStyle fallback;
    fallback.useBitmapFont()
            .fontSize(3)
            .color(Colors::Navy);
    
    auto fallbackText = Text(TextConfig(50, 100, "Bitmap Fallback").style(fallback));
}
```

#### The Reality of TTF Rendering

**TTF fonts in Fern are NOT perfect** - they come with significant challenges:

**Technical Challenges:**
- **Manual pixel-by-pixel rendering**: Fern doesn't use OS font rendering, so it implements TTF rasterization from scratch
- **No GPU acceleration**: Everything is computed on CPU, making it slower than system fonts
- **Limited hinting**: Advanced font hinting features may not be fully implemented
- **Memory intensive**: Each glyph must be rasterized and cached in memory

**Visual Artifacts You'll See:**
- **Aliasing at small sizes**: Text may appear jagged or pixelated
- **Inconsistent spacing**: Character spacing might not be perfectly uniform
- **Subpixel rendering limitations**: No ClearType/subpixel smoothing
- **Bold/italic limitations**: Style variations may not render correctly
- **Unicode limitations**: Extended character sets might not work properly

**Performance Impact:**
- **First-time rendering cost**: Initial glyph rasterization is expensive
- **Memory usage**: Each font size creates new glyph caches
- **Slower than bitmap**: Significantly more CPU-intensive than bitmap fonts

#### Platform-Specific Font Installation

**Linux (Ubuntu/Debian):**
```bash
# Install common fonts
sudo apt install fonts-liberation fonts-dejavu-core ttf-ubuntu-font-family

# Check available fonts
fc-list | grep -i liberation
```

**Linux (Arch):**
```bash
# Install font packages
sudo pacman -S ttf-liberation ttf-dejavu ttf-ubuntu-font-family

# Or install to user directory
mkdir -p ~/.local/share/fonts
cp your-font.ttf ~/.local/share/fonts/
fc-cache -fv
```

**Manual Font Installation:**
```cpp
// Copy font files to your project
// project/
//   assets/
//     fonts/
//       roboto.ttf
//       opensans.ttf

// Load from project directory
Font::loadTTFFont("roboto", "assets/fonts/roboto.ttf");
Font::loadTTFFont("opensans", "assets/fonts/opensans.ttf");
```

#### Recommended TTF Usage Strategy

```cpp
class FontManager {
public:
    static bool initializeFonts() {
        // Try to load at least one TTF font
        std::vector<std::pair<std::string, std::string>> fonts = {
            {"main", "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"},
            {"main", "assets/fonts/roboto.ttf"},
            {"fallback", "/usr/share/fonts/TTF/DejaVuSans.ttf"}
        };
        
        for (const auto& font : fonts) {
            if (Font::loadTTFFont(font.first, font.second)) {
                Font::setDefaultTTFFont(font.first);
                ttfAvailable_ = true;
                std::cout << "TTF font loaded successfully" << std::endl;
                return true;
            }
        }
        
        std::cout << "No TTF fonts available - using bitmap fallback" << std::endl;
        ttfAvailable_ = false;
        return false;
    }
    
    static TextStyle createStyle(int size, uint32_t color) {
        TextStyle style;
        if (ttfAvailable_ && size >= 16) {
            // Use TTF for larger text
            style.useTTFFont().fontSize(size).color(color);
        } else {
            // Use bitmap for small text or when TTF unavailable
            int bitmapSize = std::max(1, size / 8); // Rough conversion
            style.useBitmapFont().fontSize(bitmapSize).color(color);
        }
        return style;
    }

private:
    static bool ttfAvailable_;
};
```

**TTF Font Reality Check:**
- **Loading is required**: You must load actual font files, not just specify names
- **System-dependent**: Font availability varies by operating system
- **Performance cost**: Significantly slower than bitmap fonts
- **Visual imperfections**: Expect artifacts, especially at small sizes
- **Fallback essential**: Always have bitmap font fallbacks
- **Development complexity**: TTF rendering is inherently complex and imperfect

**When to Use TTF Fonts:**
- Large text (18pt+) where quality matters more than performance
- Professional documents or reading interfaces
- When you can guarantee font file availability
- Applications where visual polish is critical despite performance cost

**When to Avoid TTF Fonts:**
- Small UI text (under 16pt)
- Performance-critical applications
- Retro/pixel art aesthetics
- When font files are not available
- Real-time applications with frequent text updates

## Text Styling and Effects

### Colors and Backgrounds

Text isn't just about the letters - it's about how they interact with their environment:

```cpp
TextStyle highlighted;
highlighted.color(Colors::White)                    // White text
           .backgroundColor(Colors::Red)             // Red background
           .padding(8);                             // Space around text

auto important = Text(TextConfig(100, 200, "IMPORTANT!").style(highlighted));
```

### Text Alignment

Alignment determines how text positions itself relative to its coordinates:

```cpp
// Left aligned (default)
auto leftText = Text(TextConfig(200, 50, "Left").style(
    TextStyle().alignment(0)
));

// Center aligned 
auto centerText = Text(TextConfig(200, 80, "Center").style(
    TextStyle().alignment(1)
));

// Right aligned
auto rightText = Text(TextConfig(200, 110, "Right").style(
    TextStyle().alignment(2)
));
```

**Alignment Visualization:**
```
      x=200
        |
Left    |      (text starts at x)
  Center|      (text centers on x)  
     |Right   (text ends at x)
```

### Shadow Effects

Shadows add depth and improve readability:

```cpp
TextStyle withShadow;
withShadow.color(Colors::White)
          .shadow(true, Colors::Black, 3)  // enabled, color, offset
          .fontSize(4);

auto dramatic = Text(TextConfig(100, 300, "DRAMATIC!").style(withShadow));
```

The shadow is drawn first at an offset position, then the main text is drawn on top.

## Text Presets: Common Patterns

Instead of configuring every text widget from scratch, Fern provides presets for common use cases:

```cpp
// Use predefined styles for consistency
auto title = Text(TextPresets::Title(100, 50, "Main Title"));
auto subtitle = Text(TextPresets::Subtitle(100, 80, "Subtitle Text"));  
auto body = Text(TextPresets::Body(100, 110, "Regular body text content."));
auto caption = Text(TextPresets::Caption(100, 140, "Small caption text"));
auto error = Text(TextPresets::Error(100, 170, "Error message"));
auto success = Text(TextPresets::Success(100, 200, "Success!"));
```

These presets embody typography best practices and ensure visual consistency across your application.

## Dynamic Text: Updating Content

Text widgets can be updated after creation:

```cpp
// Create initial text
auto counter = Text(Point(100, 50), "Count: 0", 2, Colors::White);

// Later, update the content
int count = 0;
while (shouldKeepRunning()) {
    // Update text content
    counter->setText("Count: " + std::to_string(count));
    
    // You can also update styling
    if (count > 10) {
        counter->setColor(Colors::Red);
    }
    
    count++;
    processInput();
    render();
}
```

## Text in Layouts

Text widgets work seamlessly with Fern's layout system:

```cpp
// Text in a column layout
auto column = Column({
    Text(TextPresets::Title(0, 0, "Welcome")),
    Text(TextPresets::Body(0, 0, "This is body text that will be positioned by the column.")),
    Text(TextPresets::Caption(0, 0, "Caption text at the bottom."))
});
```

When used in layouts, the position coordinates in TextConfig become relative to the layout container.

## Performance Considerations

### When to Use Bitmap vs TTF

**Choose Bitmap Fonts when:**
- Building retro/pixel art interfaces
- Displaying lots of text that changes frequently
- Working with small text sizes
- Performance is critical

**Choose TTF Fonts when:**
- Building modern, professional interfaces
- Text readability is paramount
- You need larger text sizes
- Accessibility is important

### Text Caching

For text that doesn't change often, consider caching:

```cpp
// Instead of creating new text widgets repeatedly
TextStyle style = TextStyle().color(Colors::White).fontSize(3);

// Cache common text widgets
auto cachedTitle = Text(TextConfig(100, 50, "Fixed Title").style(style));
// Reuse rather than recreate
```

## Advanced Text Techniques

### Multi-line Text Simulation

While Fern doesn't have built-in multi-line support, you can create it manually:

```cpp
std::vector<std::string> lines = {
    "This is line one",
    "This is line two", 
    "This is line three"
};

int startY = 100;
int lineHeight = 25;
for (size_t i = 0; i < lines.size(); i++) {
    auto line = Text(Point(50, startY + i * lineHeight), lines[i], 2, Colors::White);
}
```

### Animated Text Effects

```cpp
// Pulsating text color
auto pulseText = Text(Point(100, 100), "PULSE", 3, Colors::White);

// In your game loop:
double time = getCurrentTime();
uint8_t alpha = (sin(time * 3) + 1) * 127; // 0-255 alpha
uint32_t pulseColor = (alpha << 24) | 0x00FFFFFF; // White with varying alpha
pulseText->setColor(pulseColor);
```

## Common Text Patterns

### Labels and Values

```cpp
// Status display pattern
auto healthLabel = Text(Point(20, 20), "Health:", 2, Colors::White);
auto healthValue = Text(Point(100, 20), "100", 2, Colors::Green);

// Update value dynamically
healthValue->setText(std::to_string(currentHealth));
healthValue->setColor(currentHealth > 50 ? Colors::Green : Colors::Red);
```

### Button Labels

```cpp
// Text centered on button
TextStyle buttonStyle;
buttonStyle.color(Colors::White)
           .fontSize(2)
           .alignment(1); // Center aligned

auto buttonText = Text(TextConfig(buttonCenterX, buttonCenterY, "Click Me").style(buttonStyle));
```

### Error Messages

```cpp
// Attention-grabbing error text
auto errorText = Text(TextPresets::Error(0, 0, "Connection Failed!"));
// Make it blink
if (shouldShowError && (int(getCurrentTime()) % 2 == 0)) {
    errorText->setColor(Colors::Red);
} else {
    errorText->setColor(Colors::Transparent);
}
```

## Troubleshooting Common Issues

### Text Not Appearing

**Check these common issues:**

1. **Color matches background**: If your text is black on a black background, you won't see it
```cpp
// Make sure text color contrasts with background
auto text = Text(Point(100, 50), "Visible Text", 2, Colors::White);
```

2. **Position outside screen**: Text positioned beyond screen boundaries won't be visible
```cpp
// Ensure position is within screen bounds
auto text = Text(Point(10, 10), "Visible Text", 2, Colors::White); // Safe position
```

3. **Size too small**: Very small text might not be readable
```cpp
// Use readable sizes
auto text = Text(Point(100, 50), "Readable", 2, Colors::White); // Size 2 is good minimum
```

### TTF Font Issues

**Font not loading properly:**
```cpp
// Always check if font loading succeeded
std::string fontName = "liberation";
std::string fontPath = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";

if (Font::loadTTFFont(fontName, fontPath)) {
    std::cout << "Font loaded successfully" << std::endl;
    Font::setDefaultTTFFont(fontName);
    
    // Now safe to use
    TextStyle style;
    style.useTTFFont(fontName).fontSize(20);
} else {
    std::cout << "Font loading failed - check file path" << std::endl;
    
    // Always have a bitmap fallback
    TextStyle style;
    style.useBitmapFont().fontSize(3);
}
```

**Common TTF Problems:**

1. **Font file not found**:
   - Check the exact file path exists
   - Verify file permissions are readable
   - Install required font packages on your system

2. **Poor quality at small sizes**:
   - TTF fonts below 16pt often look bad in Fern
   - Use bitmap fonts for small text instead
   - Consider the visual artifacts are normal

3. **Performance issues**:
   - TTF rendering is CPU-intensive
   - Limit TTF usage to essential large text
   - Cache TTF text widgets when possible

4. **Missing font styles**:
   - Bold/italic variants require separate font files
   - Load each style variant separately:
   ```cpp
   Font::loadTTFFont("liberation-bold", "/path/to/LiberationSans-Bold.ttf");
   Font::loadTTFFont("liberation-italic", "/path/to/LiberationSans-Italic.ttf");
   ```

**Installing Missing Fonts:**
```bash
# Ubuntu/Debian
sudo apt install fonts-liberation fonts-dejavu-core

# Arch Linux  
sudo pacman -S ttf-liberation ttf-dejavu

# Manual installation
mkdir -p ~/.local/share/fonts
cp your-font.ttf ~/.local/share/fonts/
fc-cache -fv  # Refresh font cache
```

### Performance Issues

**Too many text widgets:**
```cpp
// Instead of many individual widgets, consider:
// 1. Combining related text into single widgets
// 2. Using presets for consistency
// 3. Caching static text
```

## Summary

Text in Fern represents a perfect balance between control and convenience. You can start simple with basic positioning and colors, then gradually add sophisticated styling, effects, and dynamic behavior as your interface grows in complexity.

Key takeaways:
- **Start simple**: Use basic constructors for quick text
- **Graduate to configuration**: Use TextStyle and TextConfig for complex styling  
- **Choose the right font**: Bitmap for performance/retro, TTF for quality/modern
- **Use presets**: Leverage TextPresets for consistency
- **Consider performance**: Cache static text, update dynamic text efficiently
- **Think about readability**: Contrast, size, and alignment matter

Text is often the primary way users understand your interface - invest time in making it clear, readable, and beautiful. With Fern's text system, you have both the power to create stunning typography and the understanding of what's happening at the pixel level.

