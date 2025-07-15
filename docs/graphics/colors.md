# Colors System

The Fern Colors system provides a comprehensive palette of predefined colors and utilities for color manipulation. It supports both solid colors and transparency with a simple, efficient API.

## Overview

The Colors system uses 32-bit ARGB color values where:
- **A** (Alpha): Transparency (0x00 = transparent, 0xFF = opaque)
- **R** (Red): Red component (0x00 to 0xFF)
- **G** (Green): Green component (0x00 to 0xFF)
- **B** (Blue): Blue component (0x00 to 0xFF)

Format: `0xAARRGGBB`

## Color Categories

### Basic Colors
```cpp
Colors::Black       // 0xFF000000
Colors::White       // 0xFFFFFFFF
Colors::Red         // 0xFFFF0000
Colors::Green       // 0xFF00FF00
Colors::Blue        // 0xFF0000FF
Colors::Yellow      // 0xFFFFFF00
Colors::Cyan        // 0xFF00FFFF
Colors::Magenta     // 0xFFFF00FF
```

### Grayscale Colors
```cpp
Colors::Gray        // 0xFF808080
Colors::DarkGray    // 0xFF404040
Colors::LightGray   // 0xFFC0C0C0
Colors::Charcoal    // 0xFF202020
```

### Red Variations
```cpp
Colors::DarkRed     // 0xFF8B0000
Colors::Crimson     // 0xFFDC143C
Colors::LightRed    // 0xFFFF6666
Colors::Coral       // 0xFFFF7F50
```

### Green Variations
```cpp
Colors::DarkGreen   // 0xFF006400
Colors::LightGreen  // 0xFF90EE90
Colors::Lime        // 0xFF32CD32
Colors::Forest      // 0xFF228B22
Colors::Olive       // 0xFF808000
```

### Blue Variations
```cpp
Colors::DarkBlue    // 0xFF00008B
Colors::LightBlue   // 0xFFADD8E6
Colors::SkyBlue     // 0xFF87CEEB
Colors::Navy        // 0xFF000080
Colors::Turquoise   // 0xFF40E0D0
```

### Yellow/Orange Variations
```cpp
Colors::Orange      // 0xFFFFA500
Colors::Gold        // 0xFFFFD700
Colors::Amber       // 0xFFFFBF00
```

### Purple/Pink Variations
```cpp
Colors::Purple      // 0xFF800080
Colors::Violet      // 0xFF8A2BE2
Colors::Pink        // 0xFFFFC0CB
Colors::HotPink     // 0xFFFF69B4
```

### Brown Tones
```cpp
Colors::Brown       // 0xFFA52A2A
Colors::Tan         // 0xFFD2B48C
Colors::SaddleBrown // 0xFF8B4513
```

### UI Semantic Colors
```cpp
Colors::Success     // 0xFF28A745 (Green for success states)
Colors::Warning     // 0xFFFFC107 (Yellow for warnings)
Colors::Danger      // 0xFFDC3545 (Red for errors/danger)
Colors::Info        // 0xFF17A2B8 (Blue for information)
Colors::Primary     // 0xFF007BFF (Primary brand color)
Colors::Secondary   // 0xFF6C757D (Secondary/muted color)
```

### Transparency
```cpp
Colors::Transparent     // 0x00000000 (Fully transparent)
Colors::SemiTransparent // 0x80000000 (50% transparent black)
```

## Basic Usage

### Using Predefined Colors
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Colors Example");
    
    // Using predefined colors
    auto redButton = Button("Red Button", 100, 100, 120, 40);
    redButton->setBackgroundColor(Colors::Red);
    redButton->setColor(Colors::White);
    
    auto blueText = Text("Blue Text", 100, 150, 200, 30);
    blueText->setColor(Colors::Blue);
    
    // Using UI semantic colors
    auto successButton = Button("Success", 100, 200, 120, 40);
    successButton->setBackgroundColor(Colors::Success);
    successButton->setColor(Colors::White);
    
    addWidget(redButton);
    addWidget(blueText);
    addWidget(successButton);
    
    // Main loop
    while (isRunning()) {
        clear();
        updateWidgets();
        present();
    }
    
    return 0;
}
```

### Creating Custom Colors
```cpp
// Custom color in ARGB format
uint32_t customBlue = 0xFF336699;      // Opaque custom blue
uint32_t customSemi = 0x80FF0000;      // Semi-transparent red
uint32_t customTransparent = 0x00FFFFFF; // Transparent white

// Using custom colors
auto customButton = Button("Custom", 100, 100, 120, 40);
customButton->setBackgroundColor(customBlue);
```

## Color Utilities

### Color Blending
```cpp
// Blend two colors with interpolation factor t (0.0 to 1.0)
uint32_t blendColors(uint32_t color1, uint32_t color2, float t);

// Example usage
uint32_t redToBlue = Colors::blendColors(Colors::Red, Colors::Blue, 0.5f);
// Result: Purple color (50% mix of red and blue)

// Smooth transition from red to blue
for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
    uint32_t blendedColor = Colors::blendColors(Colors::Red, Colors::Blue, t);
    // Use blendedColor for gradual color transitions
}
```

## Advanced Usage

### Creating Color Palettes
```cpp
// Define a consistent color palette for your application
namespace AppColors {
    constexpr uint32_t Background = Colors::LightGray;
    constexpr uint32_t Primary = Colors::Blue;
    constexpr uint32_t Secondary = Colors::DarkGray;
    constexpr uint32_t Accent = Colors::Orange;
    constexpr uint32_t Text = Colors::Charcoal;
    constexpr uint32_t TextMuted = Colors::Gray;
    constexpr uint32_t Border = Colors::LightGray;
    constexpr uint32_t Error = Colors::Danger;
}

// Use the palette
auto primaryButton = Button("Primary", 0, 0, 100, 40);
primaryButton->setBackgroundColor(AppColors::Primary);
primaryButton->setColor(Colors::White);
```

### Color Variations
```cpp
// Create color variations programmatically
uint32_t createLighterColor(uint32_t baseColor, float factor) {
    return Colors::blendColors(baseColor, Colors::White, factor);
}

uint32_t createDarkerColor(uint32_t baseColor, float factor) {
    return Colors::blendColors(baseColor, Colors::Black, factor);
}

// Usage
uint32_t baseColor = Colors::Blue;
uint32_t lightBlue = createLighterColor(baseColor, 0.3f);
uint32_t darkBlue = createDarkerColor(baseColor, 0.3f);
```

### Transparency Effects
```cpp
// Create semi-transparent overlays
uint32_t createTransparentColor(uint32_t baseColor, uint8_t alpha) {
    return (baseColor & 0x00FFFFFF) | (alpha << 24);
}

// Usage
uint32_t semiTransparentRed = createTransparentColor(Colors::Red, 0x80); // 50% opacity
uint32_t lightOverlay = createTransparentColor(Colors::White, 0x40);     // 25% opacity
```

## Color Schemes

### Light Theme
```cpp
namespace LightTheme {
    constexpr uint32_t Background = Colors::White;
    constexpr uint32_t Surface = Colors::LightGray;
    constexpr uint32_t Primary = Colors::Blue;
    constexpr uint32_t Secondary = Colors::Gray;
    constexpr uint32_t Text = Colors::Charcoal;
    constexpr uint32_t TextSecondary = Colors::Gray;
    constexpr uint32_t Border = Colors::LightGray;
}
```

### Dark Theme
```cpp
namespace DarkTheme {
    constexpr uint32_t Background = Colors::Charcoal;
    constexpr uint32_t Surface = Colors::DarkGray;
    constexpr uint32_t Primary = Colors::SkyBlue;
    constexpr uint32_t Secondary = Colors::Gray;
    constexpr uint32_t Text = Colors::LightGray;
    constexpr uint32_t TextSecondary = Colors::Gray;
    constexpr uint32_t Border = Colors::DarkGray;
}
```

### Using Themes
```cpp
// Switch between themes
bool isDarkMode = false;

auto getThemeColor = [&](uint32_t lightColor, uint32_t darkColor) {
    return isDarkMode ? darkColor : lightColor;
};

// Apply theme colors
auto button = Button("Themed Button", 100, 100, 120, 40);
button->setBackgroundColor(getThemeColor(LightTheme::Primary, DarkTheme::Primary));
button->setColor(getThemeColor(LightTheme::Text, DarkTheme::Text));
```

## Color Accessibility

### Contrast Guidelines
```cpp
// High contrast combinations for accessibility
namespace AccessibleColors {
    // Dark text on light backgrounds
    constexpr uint32_t TextOnLight = Colors::Charcoal;
    constexpr uint32_t BackgroundLight = Colors::White;
    
    // Light text on dark backgrounds
    constexpr uint32_t TextOnDark = Colors::White;
    constexpr uint32_t BackgroundDark = Colors::Charcoal;
    
    // Status colors with sufficient contrast
    constexpr uint32_t SuccessText = Colors::DarkGreen;
    constexpr uint32_t WarningText = Colors::SaddleBrown;
    constexpr uint32_t ErrorText = Colors::DarkRed;
}
```

### Color-Blind Friendly Palette
```cpp
namespace ColorBlindSafe {
    constexpr uint32_t Blue = Colors::Blue;
    constexpr uint32_t Orange = Colors::Orange;
    constexpr uint32_t Purple = Colors::Purple;
    constexpr uint32_t Yellow = Colors::Gold;
    constexpr uint32_t Pink = Colors::HotPink;
    constexpr uint32_t Brown = Colors::SaddleBrown;
}
```

## Best Practices

### 1. Consistency
```cpp
// Use consistent color naming and values
namespace AppTheme {
    constexpr uint32_t PRIMARY = Colors::Blue;
    constexpr uint32_t SECONDARY = Colors::Gray;
    constexpr uint32_t ACCENT = Colors::Orange;
    constexpr uint32_t SUCCESS = Colors::Success;
    constexpr uint32_t WARNING = Colors::Warning;
    constexpr uint32_t ERROR = Colors::Danger;
}
```

### 2. Semantic Usage
```cpp
// Use semantic color names for UI elements
auto createStatusButton = [](const std::string& text, uint32_t color) {
    auto button = Button(text, 0, 0, 100, 40);
    button->setBackgroundColor(color);
    button->setColor(Colors::White);
    return button;
};

auto successBtn = createStatusButton("Success", Colors::Success);
auto warningBtn = createStatusButton("Warning", Colors::Warning);
auto errorBtn = createStatusButton("Error", Colors::Danger);
```

### 3. Color Hierarchies
```cpp
// Create visual hierarchies with color intensity
auto primaryAction = Button("Primary", 0, 0, 100, 40);
primaryAction->setBackgroundColor(Colors::Primary);

auto secondaryAction = Button("Secondary", 0, 0, 100, 40);
secondaryAction->setBackgroundColor(Colors::Secondary);

auto tertiaryAction = Button("Tertiary", 0, 0, 100, 40);
tertiaryAction->setBackgroundColor(Colors::LightGray);
```

## Color Format Reference

### ARGB Breakdown
```cpp
// Color: 0xAARRGGBB
uint32_t color = 0xFF336699;

uint8_t alpha = (color >> 24) & 0xFF;  // 0xFF (255, fully opaque)
uint8_t red   = (color >> 16) & 0xFF;  // 0x33 (51)
uint8_t green = (color >> 8) & 0xFF;   // 0x66 (102)
uint8_t blue  = color & 0xFF;          // 0x99 (153)
```

### Creating Colors from Components
```cpp
// Create color from RGBA components
uint32_t createColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// Usage
uint32_t customColor = createColor(51, 102, 153);        // RGB
uint32_t transparentColor = createColor(51, 102, 153, 128); // RGBA with 50% opacity
```

## Performance Tips

1. **Use constexpr colors**: Predefined colors are computed at compile time
2. **Avoid frequent color blending**: Cache blended colors when possible
3. **Use appropriate alpha values**: Fully opaque (0xFF) or transparent (0x00) are fastest
4. **Consider color space**: ARGB format is optimized for rendering

## Related Components

- [`Containers`](../layout/containers.md) - For applying background colors
- [`Widgets`](../widgets/) - All widgets support color customization
- [`Primitives`](primitives.md) - For drawing colored shapes
- [`Fonts`](fonts.md) - For text color styling

## Example Applications

### Status Indicators
```cpp
auto createStatusIndicator = [](const std::string& status, uint32_t color) {
    auto indicator = Circle(0, 0, 10);
    indicator->setColor(color);
    auto label = Text(status, 20, 0, 100, 20);
    return Row({indicator, label});
};

auto online = createStatusIndicator("Online", Colors::Success);
auto offline = createStatusIndicator("Offline", Colors::Danger);
auto away = createStatusIndicator("Away", Colors::Warning);
```

### Progress Bars
```cpp
auto createProgressBar = [](float progress) {
    auto background = Container(Colors::LightGray, 0, 0, 200, 20);
    auto foreground = Container(Colors::Primary, 0, 0, 200 * progress, 20);
    // Overlay foreground on background
    return background; // Simplified example
};
```

The Colors system provides a solid foundation for creating visually appealing and accessible user interfaces in Fern.
