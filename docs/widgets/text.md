# Text Widget

The Text widget is used to display text content in your Fern application. It supports various styling options, fonts, and text effects.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Styling](#styling)
- [Font System](#font-system)
- [Examples](#examples)
- [API Reference](#api-reference)

## Basic Usage

### Simple Text
```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    auto text = Text(Point(100, 100), "Hello, World!", 2, Colors::White);
    addWidget(text);
}
```

### Modern Configuration
```cpp
void setupUI() {
    TextStyle style;
    style.fontSize(3)
         .color(Colors::Blue)
         .useBitmapFont();
    
    auto text = Text(TextConfig(100, 100, "Styled Text").style(style));
    addWidget(text);
}
```

## Configuration

### TextConfig Constructor
```cpp
TextConfig(int x, int y, const std::string& text)
```

Parameters:
- `x, y`: Position of the text
- `text`: The text content to display

### Legacy Constructor
```cpp
Text(Point position, const std::string& text, int size, uint32_t color)
```

Parameters:
- `position`: Point(x, y) position
- `text`: Text content
- `size`: Font size multiplier
- `color`: Text color

## Styling

### TextStyle Properties

#### Basic Properties
```cpp
TextStyle style;
style.color(Colors::White)           // Text color
     .fontSize(3)                    // Size multiplier
     .backgroundColor(Colors::Black)  // Background color
     .padding(10);                   // Padding around text
```

#### Alignment
```cpp
style.alignment(0);  // 0=left, 1=center, 2=right
```

#### Shadow Effects
```cpp
style.shadow(true, Colors::Black, 2);  // Enable shadow with color and offset
```

#### Font Selection
```cpp
style.useBitmapFont();              // Use bitmap font (default)
style.useTTFFont("Arial");          // Use TTF font
```

### Text Colors

#### Basic Colors
```cpp
auto redText = Text(Point(0, 0), "Red Text", 2, Colors::Red);
auto greenText = Text(Point(0, 0), "Green Text", 2, Colors::Green);
auto blueText = Text(Point(0, 0), "Blue Text", 2, Colors::Blue);
```

#### Custom Colors
```cpp
uint32_t customColor = 0xFF6A5ACD;  // Slate blue
auto customText = Text(Point(0, 0), "Custom Color", 2, customColor);
```

### Text Sizes

#### Size Comparison
```cpp
auto smallText = Text(Point(0, 0), "Small (1)", 1, Colors::White);
auto normalText = Text(Point(0, 0), "Normal (2)", 2, Colors::White);
auto largeText = Text(Point(0, 0), "Large (3)", 3, Colors::White);
auto extraLargeText = Text(Point(0, 0), "Extra Large (4)", 4, Colors::White);
```

## Font System

### Bitmap Fonts
Default font system with pixel-perfect rendering:
```cpp
TextStyle bitmapStyle;
bitmapStyle.useBitmapFont()
          .fontSize(2)
          .color(Colors::White);
```

### TTF Fonts
True Type Font support (when available):
```cpp
TextStyle ttfStyle;
ttfStyle.useTTFFont("Arial")
        .fontSize(16)  // TTF fonts use point sizes
        .color(Colors::Black);
```

### Font Sizing
- **Bitmap fonts**: Size is a multiplier (1x, 2x, 3x, etc.)
- **TTF fonts**: Size is in points (12pt, 16pt, 24pt, etc.)

## Examples

### Example 1: Text Showcase
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Title
    auto title = Text(Point(0, 0), "Text Styles", 4, Colors::White);
    
    // Different text examples
    auto smallText = Text(Point(0, 0), "Small Text (size 1)", 1, Colors::LightGray);
    auto normalText = Text(Point(0, 0), "Normal Text (size 2)", 2, Colors::White);
    auto largeText = Text(Point(0, 0), "Large Text (size 3)", 3, Colors::Yellow);
    auto extraLargeText = Text(Point(0, 0), "Extra Large (size 4)", 4, Colors::Orange);
    
    // Colored text examples
    auto redText = Text(Point(0, 0), "Red Text", 2, Colors::Red);
    auto greenText = Text(Point(0, 0), "Green Text", 2, Colors::Green);
    auto blueText = Text(Point(0, 0), "Blue Text", 2, Colors::Blue);
    auto cyanText = Text(Point(0, 0), "Cyan Text", 2, Colors::Cyan);
    auto magentaText = Text(Point(0, 0), "Magenta Text", 2, Colors::Magenta);
    
    // Layout sections
    std::vector<std::shared_ptr<Widget>> sizeSection = {
        Text(Point(0, 0), "Size Examples:", 2, Colors::LightBlue),
        SizedBox(0, 20),
        smallText,
        SizedBox(0, 15),
        normalText,
        SizedBox(0, 15),
        largeText,
        SizedBox(0, 15),
        extraLargeText
    };
    
    std::vector<std::shared_ptr<Widget>> colorSection = {
        Text(Point(0, 0), "Color Examples:", 2, Colors::LightBlue),
        SizedBox(0, 20),
        redText,
        SizedBox(0, 15),
        greenText,
        SizedBox(0, 15),
        blueText,
        SizedBox(0, 15),
        cyanText,
        SizedBox(0, 15),
        magentaText
    };
    
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        title,
        SizedBox(0, 30),
        Column(sizeSection),
        SizedBox(0, 40),
        Column(colorSection)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Example 2: Dynamic Text Updates
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> dynamicText;
static int updateCount = 0;

void setupUI() {
    dynamicText = Text(Point(0, 0), "Updates: 0", 3, Colors::White);
    
    ButtonStyle updateStyle;
    updateStyle.normalColor(Colors::Green)
              .hoverColor(Colors::LightGreen)
              .pressColor(Colors::DarkGreen)
              .textColor(Colors::White)
              .textScale(2);
    
    auto updateButton = Button(ButtonConfig(0, 0, 150, 50, "Update Text").style(updateStyle));
    updateButton->onClick.connect([]() {
        updateCount++;
        dynamicText->setText("Updates: " + std::to_string(updateCount));
        
        // Change color based on count
        if (updateCount % 5 == 0) {
            dynamicText->setColor(Colors::Red);
        } else if (updateCount % 3 == 0) {
            dynamicText->setColor(Colors::Blue);
        } else {
            dynamicText->setColor(Colors::White);
        }
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Dynamic Text Example", 3, Colors::Yellow),
        SizedBox(0, 30),
        dynamicText,
        SizedBox(0, 40),
        updateButton
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Example 3: Text with Background
```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Text with background using modern configuration
    TextStyle backgroundStyle;
    backgroundStyle.fontSize(2)
                  .color(Colors::White)
                  .backgroundColor(Colors::DarkBlue)
                  .padding(15);
    
    auto backgroundText = Text(TextConfig(0, 0, "Text with Background").style(backgroundStyle));
    
    // Shadow text
    TextStyle shadowStyle;
    shadowStyle.fontSize(3)
              .color(Colors::Yellow)
              .shadow(true, Colors::Black, 3);
    
    auto shadowText = Text(TextConfig(0, 0, "Shadow Text").style(shadowStyle));
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Text Effects", 4, Colors::White),
        SizedBox(0, 40),
        backgroundText,
        SizedBox(0, 40),
        shadowText
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

## API Reference

### TextConfig Class

#### Constructor
```cpp
TextConfig(int x, int y, const std::string& text)
```

#### Methods
```cpp
TextConfig& style(const TextStyle& s)           // Set text style
TextConfig& position(int x, int y)              // Set position
TextConfig& text(const std::string& content)    // Set text content
```

#### Getters
```cpp
int getX() const
int getY() const
const std::string& getText() const
const TextStyle& getStyle() const
```

### TextStyle Class

#### Color Methods
```cpp
TextStyle& color(uint32_t color)                // Text color
TextStyle& backgroundColor(uint32_t color)      // Background color
```

#### Font Methods
```cpp
TextStyle& fontSize(int size)                   // Font size
TextStyle& useBitmapFont()                      // Use bitmap font
TextStyle& useTTFFont(const std::string& name)  // Use TTF font
```

#### Layout Methods
```cpp
TextStyle& padding(int p)                       // Padding around text
TextStyle& alignment(int a)                     // 0=left, 1=center, 2=right
```

#### Effects Methods
```cpp
TextStyle& shadow(bool enabled, uint32_t color, int offset)  // Shadow effect
```

#### Getters
```cpp
uint32_t getColor() const
uint32_t getBackgroundColor() const
int getFontSize() const
FontType getFontType() const
int getPadding() const
int getAlignment() const
bool hasShadow() const
uint32_t getShadowColor() const
int getShadowOffset() const
```

### TextWidget Class

#### Methods
```cpp
void setText(const std::string& text)           // Update text content
void setSize(int size)                         // Update font size
void setColor(uint32_t color)                  // Update text color
void setConfig(const TextConfig& config)       // Update configuration
```

#### Getters
```cpp
const std::string& getText() const             // Get current text
int getSize() const                            // Get font size
uint32_t getColor() const                      // Get text color
```

### Factory Functions

#### Modern Factory
```cpp
std::shared_ptr<TextWidget> Text(const TextConfig& config, bool addToManager = true)
```

#### Legacy Factory
```cpp
std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                int size, uint32_t color, bool addToManager = true)
```

### Preset Configurations

#### Title Text
```cpp
TextConfig title = TextPresets::Title(x, y, "Title Text");
```

#### Subtitle Text
```cpp
TextConfig subtitle = TextPresets::Subtitle(x, y, "Subtitle Text");
```

#### Body Text
```cpp
TextConfig body = TextPresets::Body(x, y, "Body Text");
```

## Best Practices

### 1. Consistent Typography
Establish a type hierarchy:
```cpp
namespace Typography {
    TextStyle H1() { return TextStyle().fontSize(4).color(Colors::White); }
    TextStyle H2() { return TextStyle().fontSize(3).color(Colors::LightGray); }
    TextStyle Body() { return TextStyle().fontSize(2).color(Colors::White); }
    TextStyle Caption() { return TextStyle().fontSize(1).color(Colors::Gray); }
}
```

### 2. Readable Colors
Ensure sufficient contrast:
```cpp
// Good contrast combinations
Text(Point(0, 0), "Light on dark", 2, Colors::White);  // On dark background
Text(Point(0, 0), "Dark on light", 2, Colors::Black);  // On light background

// Avoid low contrast
// Text(Point(0, 0), "Gray on gray", 2, Colors::Gray);  // Hard to read
```

### 3. Responsive Text Size
Scale text based on screen size:
```cpp
int screenWidth = Fern::getWidth();
int textSize = (screenWidth < 600) ? 2 : 3;  // Smaller text on small screens
```

### 4. Proper Spacing
Use consistent spacing between text elements:
```cpp
std::vector<std::shared_ptr<Widget>> textElements = {
    Text(Point(0, 0), "Title", 4, Colors::White),
    SizedBox(0, 20),  // Consistent spacing
    Text(Point(0, 0), "Subtitle", 2, Colors::Gray),
    SizedBox(0, 15),  // Smaller spacing for related content
    Text(Point(0, 0), "Body text", 2, Colors::White)
};
```

## Common Patterns

### Loading Text
```cpp
class LoadingText {
    std::shared_ptr<TextWidget> text;
    std::vector<std::string> frames = {"Loading", "Loading.", "Loading..", "Loading..."};
    int currentFrame = 0;
    
public:
    void update() {
        text->setText(frames[currentFrame]);
        currentFrame = (currentFrame + 1) % frames.size();
    }
};
```

### Highlighted Text
```cpp
void createHighlightedText(const std::string& content, const std::string& highlight) {
    // Find highlight position
    size_t pos = content.find(highlight);
    if (pos != std::string::npos) {
        // Create segments: before, highlight, after
        auto beforeText = Text(Point(0, 0), content.substr(0, pos), 2, Colors::White);
        auto highlightText = Text(Point(0, 0), highlight, 2, Colors::Yellow);
        auto afterText = Text(Point(0, 0), content.substr(pos + highlight.length()), 2, Colors::White);
        
        // Layout in row
        std::vector<std::shared_ptr<Widget>> segments = {
            beforeText, highlightText, afterText
        };
        
        return Row(segments);
    }
}
```

### Multiline Text
```cpp
void createMultilineText(const std::vector<std::string>& lines) {
    std::vector<std::shared_ptr<Widget>> textLines;
    
    for (const auto& line : lines) {
        textLines.push_back(Text(Point(0, 0), line, 2, Colors::White));
        textLines.push_back(SizedBox(0, 5));  // Line spacing
    }
    
    return Column(textLines);
}
```

## Troubleshooting

### Text Not Visible
- Check text color against background
- Verify text size is appropriate
- Ensure text is within screen bounds

### Font Issues
- Bitmap fonts work on all platforms
- TTF fonts require font files to be available
- Check font name spelling for TTF fonts

### Layout Problems
- Text width calculation is automatic
- Use proper spacing between text elements
- Consider text wrapping for long content

### Performance Issues
- Avoid frequent text updates
- Cache text widgets when possible
- Use appropriate text sizes

---

**Related Documentation:**
- [Button Widget](button.md)
- [Layout System](../layout/overview.md)
- [Colors System](../graphics/colors.md)
- [Font System](../graphics/fonts.md)
