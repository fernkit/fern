# Styling Guide

This guide covers styling principles, theming, and best practices for creating visually consistent and appealing user interfaces with the Fern UI Framework.

## Overview

Fern provides a comprehensive styling system that includes:
- Color management and theming
- Typography and font styling
- Layout and spacing systems
- Component styling patterns
- Animation and visual effects
- Responsive design principles

## Core Styling Components

### Colors
- [Colors System](colors.md) - Color constants and utilities
- Theme-based color schemes
- Accessibility considerations

### Typography
- [Font System](fonts.md) - Font loading and rendering
- Font hierarchies and sizes
- Text styling patterns

### Layout
- [Layout System](../layout/overview.md) - Spacing and positioning
- Container styling
- Responsive layouts

### Graphics
- [Primitives Drawing](primitives.md) - Basic shape rendering
- Custom graphics and icons
- Visual effects

## Theming System

### Creating a Theme
```cpp
#include "fern/fern.hpp"
using namespace Fern;

namespace Theme {
    // Color Palette
    struct Colors {
        static constexpr uint32_t Primary = 0xFF2196F3;      // Blue
        static constexpr uint32_t Secondary = 0xFF9C27B0;    // Purple
        static constexpr uint32_t Success = 0xFF4CAF50;      // Green
        static constexpr uint32_t Warning = 0xFFFF9800;      // Orange
        static constexpr uint32_t Error = 0xFFF44336;        // Red
        static constexpr uint32_t Info = 0xFF00BCD4;         // Cyan
        
        // Neutral Colors
        static constexpr uint32_t Background = 0xFFFAFAFA;   // Light gray
        static constexpr uint32_t Surface = 0xFFFFFFFF;      // White
        static constexpr uint32_t Text = 0xFF212121;         // Dark gray
        static constexpr uint32_t TextSecondary = 0xFF757575; // Medium gray
        static constexpr uint32_t Border = 0xFFE0E0E0;       // Light border
        static constexpr uint32_t Divider = 0xFFBDBDBD;      // Divider line
    };
    
    // Typography
    struct Typography {
        static constexpr int HeaderSize = 24;
        static constexpr int SubheaderSize = 18;
        static constexpr int BodySize = 16;
        static constexpr int CaptionSize = 12;
        static constexpr int ButtonSize = 14;
        
        static const std::string HeaderFont;
        static const std::string BodyFont;
        static const std::string MonoFont;
    };
    
    // Spacing
    struct Spacing {
        static constexpr int XSmall = 4;
        static constexpr int Small = 8;
        static constexpr int Medium = 16;
        static constexpr int Large = 24;
        static constexpr int XLarge = 32;
        static constexpr int XXLarge = 48;
    };
    
    // Dimensions
    struct Dimensions {
        static constexpr int ButtonHeight = 40;
        static constexpr int InputHeight = 36;
        static constexpr int BorderRadius = 4;
        static constexpr int BorderWidth = 1;
        static constexpr int ShadowOffset = 2;
    };
}
```

### Applying Themes
```cpp
// Themed button creation
std::shared_ptr<Button> createPrimaryButton(const std::string& text, int x, int y, int width = 120) {
    auto button = Button(text, x, y, width, Theme::Dimensions::ButtonHeight);
    button->setBackgroundColor(Theme::Colors::Primary);
    button->setColor(Theme::Colors::Surface);
    button->setFontSize(Theme::Typography::ButtonSize);
    return button;
}

// Themed text creation
std::shared_ptr<Text> createHeaderText(const std::string& text, int x, int y, int width = 300) {
    auto header = Text(text, x, y, width, Theme::Typography::HeaderSize + 8);
    header->setFontSize(Theme::Typography::HeaderSize);
    header->setColor(Theme::Colors::Text);
    return header;
}

// Themed container creation
std::shared_ptr<ContainerWidget> createCard(int x, int y, int width, int height) {
    auto card = Container(Theme::Colors::Surface, x, y, width, height);
    // Add shadow effect (simplified)
    Draw::rect(x + Theme::Dimensions::ShadowOffset, 
              y + Theme::Dimensions::ShadowOffset, 
              width, height, 0x20000000); // Semi-transparent shadow
    return card;
}
```

## Component Styling Patterns

### Button Styles
```cpp
namespace ButtonStyles {
    // Primary button
    auto createPrimary(const std::string& text, int x, int y, int width = 120) {
        auto button = Button(text, x, y, width, Theme::Dimensions::ButtonHeight);
        button->setBackgroundColor(Theme::Colors::Primary);
        button->setColor(Theme::Colors::Surface);
        button->setBorderRadius(Theme::Dimensions::BorderRadius);
        return button;
    }
    
    // Secondary button
    auto createSecondary(const std::string& text, int x, int y, int width = 120) {
        auto button = Button(text, x, y, width, Theme::Dimensions::ButtonHeight);
        button->setBackgroundColor(Theme::Colors::Surface);
        button->setColor(Theme::Colors::Primary);
        button->setBorderColor(Theme::Colors::Primary);
        button->setBorderWidth(Theme::Dimensions::BorderWidth);
        return button;
    }
    
    // Ghost button
    auto createGhost(const std::string& text, int x, int y, int width = 120) {
        auto button = Button(text, x, y, width, Theme::Dimensions::ButtonHeight);
        button->setBackgroundColor(0x00000000); // Transparent
        button->setColor(Theme::Colors::Primary);
        return button;
    }
    
    // Danger button
    auto createDanger(const std::string& text, int x, int y, int width = 120) {
        auto button = Button(text, x, y, width, Theme::Dimensions::ButtonHeight);
        button->setBackgroundColor(Theme::Colors::Error);
        button->setColor(Theme::Colors::Surface);
        return button;
    }
}
```

### Input Styles
```cpp
namespace InputStyles {
    // Standard input
    auto createStandard(const std::string& placeholder, int x, int y, int width = 200) {
        auto input = TextInput(placeholder, x, y, width, Theme::Dimensions::InputHeight);
        input->setBackgroundColor(Theme::Colors::Surface);
        input->setColor(Theme::Colors::Text);
        input->setBorderColor(Theme::Colors::Border);
        input->setBorderWidth(Theme::Dimensions::BorderWidth);
        return input;
    }
    
    // Focused input
    auto createFocused(const std::string& placeholder, int x, int y, int width = 200) {
        auto input = createStandard(placeholder, x, y, width);
        input->setBorderColor(Theme::Colors::Primary);
        input->setBorderWidth(2);
        return input;
    }
    
    // Error input
    auto createError(const std::string& placeholder, int x, int y, int width = 200) {
        auto input = createStandard(placeholder, x, y, width);
        input->setBorderColor(Theme::Colors::Error);
        input->setBorderWidth(2);
        return input;
    }
}
```

### Card Styles
```cpp
namespace CardStyles {
    // Basic card
    auto createBasic(int x, int y, int width, int height) {
        auto card = Container(Theme::Colors::Surface, x, y, width, height);
        // Add border
        Draw::roundedRectBorder(x, y, width, height, 
                               Theme::Dimensions::BorderRadius, 
                               Theme::Dimensions::BorderWidth, 
                               Theme::Colors::Border);
        return card;
    }
    
    // Elevated card
    auto createElevated(int x, int y, int width, int height) {
        // Shadow
        Draw::roundedRect(x + 2, y + 2, width, height, 
                         Theme::Dimensions::BorderRadius, 
                         0x20000000); // Semi-transparent shadow
        
        // Card
        auto card = Container(Theme::Colors::Surface, x, y, width, height);
        return card;
    }
    
    // Outlined card
    auto createOutlined(int x, int y, int width, int height) {
        auto card = Container(0x00000000, x, y, width, height); // Transparent background
        Draw::roundedRectBorder(x, y, width, height,
                               Theme::Dimensions::BorderRadius,
                               Theme::Dimensions::BorderWidth,
                               Theme::Colors::Border);
        return card;
    }
}
```

## Layout and Spacing

### Consistent Spacing
```cpp
// Use theme spacing consistently
auto createForm() {
    auto title = createHeaderText("User Registration", 0, 0);
    auto nameInput = InputStyles::createStandard("Full Name", 0, 0, 300);
    auto emailInput = InputStyles::createStandard("Email", 0, 0, 300);
    auto submitButton = ButtonStyles::createPrimary("Submit", 0, 0, 100);
    
    return Column({
        title,
        SizedBox(0, Theme::Spacing::Large),
        nameInput,
        SizedBox(0, Theme::Spacing::Medium),
        emailInput,
        SizedBox(0, Theme::Spacing::Large),
        submitButton
    });
}
```

### Grid Systems
```cpp
// Create consistent grid layouts
class GridSystem {
private:
    int containerWidth_;
    int columns_;
    int gutterSize_;
    
public:
    GridSystem(int width, int columns = 12, int gutter = Theme::Spacing::Medium) 
        : containerWidth_(width), columns_(columns), gutterSize_(gutter) {}
    
    int getColumnWidth(int span) {
        int totalGutterWidth = (columns_ - 1) * gutterSize_;
        int availableWidth = containerWidth_ - totalGutterWidth;
        return (availableWidth * span) / columns_;
    }
    
    int getColumnOffset(int column) {
        return column * (getColumnWidth(1) + gutterSize_);
    }
};
```

## Typography Hierarchy

### Text Styles
```cpp
namespace TextStyles {
    // Display text (largest)
    auto createDisplay(const std::string& text, int x, int y, int width = 400) {
        auto textWidget = Text(text, x, y, width, 40);
        textWidget->setFontSize(32);
        textWidget->setColor(Theme::Colors::Text);
        return textWidget;
    }
    
    // Heading 1
    auto createH1(const std::string& text, int x, int y, int width = 300) {
        auto textWidget = Text(text, x, y, width, 32);
        textWidget->setFontSize(Theme::Typography::HeaderSize);
        textWidget->setColor(Theme::Colors::Text);
        return textWidget;
    }
    
    // Heading 2
    auto createH2(const std::string& text, int x, int y, int width = 300) {
        auto textWidget = Text(text, x, y, width, 24);
        textWidget->setFontSize(Theme::Typography::SubheaderSize);
        textWidget->setColor(Theme::Colors::Text);
        return textWidget;
    }
    
    // Body text
    auto createBody(const std::string& text, int x, int y, int width = 300) {
        auto textWidget = Text(text, x, y, width, 20);
        textWidget->setFontSize(Theme::Typography::BodySize);
        textWidget->setColor(Theme::Colors::Text);
        return textWidget;
    }
    
    // Caption text
    auto createCaption(const std::string& text, int x, int y, int width = 200) {
        auto textWidget = Text(text, x, y, width, 16);
        textWidget->setFontSize(Theme::Typography::CaptionSize);
        textWidget->setColor(Theme::Colors::TextSecondary);
        return textWidget;
    }
}
```

## Color Usage Guidelines

### Semantic Colors
```cpp
namespace SemanticColors {
    // Status indicators
    auto createSuccessIndicator(const std::string& message, int x, int y) {
        auto indicator = Circle(x, y, 6);
        indicator->setColor(Theme::Colors::Success);
        
        auto text = Text(message, x + 20, y - 8, 200, 16);
        text->setColor(Theme::Colors::Success);
        
        return Row({indicator, text});
    }
    
    auto createWarningIndicator(const std::string& message, int x, int y) {
        auto indicator = Circle(x, y, 6);
        indicator->setColor(Theme::Colors::Warning);
        
        auto text = Text(message, x + 20, y - 8, 200, 16);
        text->setColor(Theme::Colors::Warning);
        
        return Row({indicator, text});
    }
    
    auto createErrorIndicator(const std::string& message, int x, int y) {
        auto indicator = Circle(x, y, 6);
        indicator->setColor(Theme::Colors::Error);
        
        auto text = Text(message, x + 20, y - 8, 200, 16);
        text->setColor(Theme::Colors::Error);
        
        return Row({indicator, text});
    }
}
```

### Color Accessibility
```cpp
namespace AccessibleColors {
    // High contrast color combinations
    struct HighContrast {
        static constexpr uint32_t TextOnLight = 0xFF000000;
        static constexpr uint32_t TextOnDark = 0xFFFFFFFF;
        static constexpr uint32_t BackgroundLight = 0xFFFFFFFF;
        static constexpr uint32_t BackgroundDark = 0xFF000000;
    };
    
    // Color blind friendly palette
    struct ColorBlindSafe {
        static constexpr uint32_t Blue = 0xFF0173B2;
        static constexpr uint32_t Orange = 0xFFDE8F05;
        static constexpr uint32_t Green = 0xFF029E73;
        static constexpr uint32_t Red = 0xFFD55E00;
        static constexpr uint32_t Purple = 0xFFCC78BC;
        static constexpr uint32_t Brown = 0xFF8B4513;
    };
}
```

## Dark Theme Support

### Dark Theme Colors
```cpp
namespace DarkTheme {
    struct Colors {
        static constexpr uint32_t Primary = 0xFF90CAF9;      // Light blue
        static constexpr uint32_t Secondary = 0xFFCE93D8;    // Light purple
        static constexpr uint32_t Background = 0xFF121212;   // Dark gray
        static constexpr uint32_t Surface = 0xFF1E1E1E;      // Slightly lighter
        static constexpr uint32_t Text = 0xFFFFFFFF;         // White
        static constexpr uint32_t TextSecondary = 0xFFB3B3B3; // Light gray
        static constexpr uint32_t Border = 0xFF333333;       // Dark border
    };
}
```

### Theme Switching
```cpp
class ThemeManager {
private:
    bool isDarkMode_;
    
public:
    ThemeManager() : isDarkMode_(false) {}
    
    void setDarkMode(bool enable) {
        isDarkMode_ = enable;
        // Update all widgets with new theme
        updateAllWidgets();
    }
    
    uint32_t getBackgroundColor() {
        return isDarkMode_ ? DarkTheme::Colors::Background : Theme::Colors::Background;
    }
    
    uint32_t getTextColor() {
        return isDarkMode_ ? DarkTheme::Colors::Text : Theme::Colors::Text;
    }
    
    uint32_t getPrimaryColor() {
        return isDarkMode_ ? DarkTheme::Colors::Primary : Theme::Colors::Primary;
    }
    
private:
    void updateAllWidgets() {
        // Update widget colors based on current theme
        // This would iterate through all widgets and update their colors
    }
};
```

## Animation and Visual Effects

### Smooth Transitions
```cpp
class AnimationHelper {
public:
    // Fade in/out animation
    static void fadeIn(std::shared_ptr<Widget> widget, float duration = 0.3f) {
        // Gradually increase opacity over time
        float startTime = getCurrentTime();
        
        auto updateOpacity = [=](float t) {
            float progress = (getCurrentTime() - startTime) / duration;
            progress = std::min(1.0f, progress);
            
            uint32_t alpha = (uint32_t)(255 * progress);
            widget->setOpacity(alpha);
            
            return progress < 1.0f;
        };
        
        // Register animation callback
        registerAnimation(updateOpacity);
    }
    
    // Scale animation
    static void scaleIn(std::shared_ptr<Widget> widget, float duration = 0.2f) {
        float startTime = getCurrentTime();
        int originalWidth = widget->getWidth();
        int originalHeight = widget->getHeight();
        
        auto updateScale = [=](float t) {
            float progress = (getCurrentTime() - startTime) / duration;
            progress = std::min(1.0f, progress);
            
            // Ease-out animation
            float scale = 1.0f - (1.0f - progress) * (1.0f - progress);
            
            widget->resize(originalWidth * scale, originalHeight * scale);
            
            return progress < 1.0f;
        };
        
        registerAnimation(updateScale);
    }
};
```

### Visual Feedback
```cpp
namespace VisualFeedback {
    // Hover effect
    void applyHoverEffect(std::shared_ptr<Button> button) {
        button->onHover([=]() {
            uint32_t hoverColor = Colors::blendColors(
                Theme::Colors::Primary, 
                Theme::Colors::Surface, 
                0.1f
            );
            button->setBackgroundColor(hoverColor);
        });
        
        button->onLeave([=]() {
            button->setBackgroundColor(Theme::Colors::Primary);
        });
    }
    
    // Click ripple effect
    void applyRippleEffect(std::shared_ptr<Button> button) {
        button->onClick([=]() {
            int centerX = button->getX() + button->getWidth() / 2;
            int centerY = button->getY() + button->getHeight() / 2;
            
            // Create expanding circle animation
            AnimationHelper::createRipple(centerX, centerY, 
                                        Theme::Colors::Surface, 
                                        0.3f);
        });
    }
}
```

## Responsive Design

### Breakpoints
```cpp
namespace Breakpoints {
    static constexpr int Mobile = 600;
    static constexpr int Tablet = 960;
    static constexpr int Desktop = 1280;
    static constexpr int LargeDesktop = 1920;
}

class ResponsiveHelper {
public:
    static bool isMobile() {
        return getWidth() <= Breakpoints::Mobile;
    }
    
    static bool isTablet() {
        return getWidth() > Breakpoints::Mobile && getWidth() <= Breakpoints::Tablet;
    }
    
    static bool isDesktop() {
        return getWidth() > Breakpoints::Tablet;
    }
    
    static int getResponsiveSpacing() {
        if (isMobile()) return Theme::Spacing::Small;
        if (isTablet()) return Theme::Spacing::Medium;
        return Theme::Spacing::Large;
    }
    
    static int getResponsiveFontSize(int baseSize) {
        if (isMobile()) return baseSize * 0.9f;
        if (isTablet()) return baseSize;
        return baseSize * 1.1f;
    }
};
```

## Best Practices

### 1. Consistency
```cpp
// Use consistent spacing and sizing
const int STANDARD_BUTTON_HEIGHT = 40;
const int STANDARD_INPUT_HEIGHT = 36;
const int STANDARD_PADDING = 16;

// Use consistent color schemes
const uint32_t PRIMARY_COLOR = Theme::Colors::Primary;
const uint32_t SECONDARY_COLOR = Theme::Colors::Secondary;
```

### 2. Accessibility
```cpp
// Ensure sufficient color contrast
bool hasGoodContrast(uint32_t foreground, uint32_t background) {
    // Simplified contrast check
    float ratio = calculateContrastRatio(foreground, background);
    return ratio >= 4.5f; // WCAG AA standard
}

// Provide alternative text for visual elements
auto createAccessibleButton(const std::string& text, const std::string& altText) {
    auto button = ButtonStyles::createPrimary(text, 0, 0);
    button->setAccessibilityLabel(altText);
    return button;
}
```

### 3. Performance
```cpp
// Cache styled components
class StyledComponentCache {
private:
    std::unordered_map<std::string, std::shared_ptr<Widget>> cache_;
    
public:
    std::shared_ptr<Widget> getOrCreate(const std::string& key, 
                                       std::function<std::shared_ptr<Widget>()> creator) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        
        auto widget = creator();
        cache_[key] = widget;
        return widget;
    }
};
```

## Style Guide Checklist

### Colors
- [ ] Define a consistent color palette
- [ ] Use semantic colors appropriately
- [ ] Ensure accessibility compliance
- [ ] Support dark mode if needed

### Typography
- [ ] Establish clear font hierarchy
- [ ] Use consistent font sizes
- [ ] Ensure readability at all sizes
- [ ] Load fonts efficiently

### Layout
- [ ] Use consistent spacing
- [ ] Follow grid system principles
- [ ] Ensure responsive behavior
- [ ] Maintain visual balance

### Components
- [ ] Apply consistent styling patterns
- [ ] Use appropriate visual feedback
- [ ] Maintain state consistency
- [ ] Optimize for performance

### Animation
- [ ] Use subtle, meaningful animations
- [ ] Maintain consistent timing
- [ ] Provide reduced motion options
- [ ] Optimize animation performance

## Related Documentation

- [Colors System](colors.md) - Color management and utilities
- [Font System](fonts.md) - Typography and text rendering
- [Primitives Drawing](primitives.md) - Basic graphics operations
- [Layout Overview](../layout/overview.md) - Layout system fundamentals
- [Widget Documentation](../widgets/) - Individual widget styling

## Tools and Resources

### Design Systems
- Material Design principles
- Human Interface Guidelines
- Accessibility guidelines (WCAG)

### Color Tools
- Contrast ratio calculators
- Color palette generators
- Accessibility checkers

### Typography Tools
- Font pairing guides
- Font loading optimization
- Typography scale calculators

The Styling Guide provides a comprehensive framework for creating consistent, accessible, and visually appealing user interfaces in Fern applications.
