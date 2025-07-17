# Button Widget

The Button widget is one of the most fundamental interactive elements in Fern. It provides a clickable area that can trigger actions in your application.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Styling](#styling)
- [Event Handling](#event-handling)
- [Examples](#examples)
- [API Reference](#api-reference)

## Basic Usage

### Simple Button
```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    auto button = Button(ButtonConfig(100, 100, 200, 50, "Click Me"));
    
    button->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    addWidget(button);
}
```

### Modern Configuration
```cpp
void setupUI() {
    // Create button with modern configuration
    ButtonStyle style;
    style.normalColor(Colors::Blue)
        .hoverColor(Colors::LightBlue)
        .pressColor(Colors::DarkBlue)
        .textColor(Colors::White)
        .textScale(2);
    
    auto button = Button(ButtonConfig(100, 100, 200, 50, "Styled Button").style(style));
    
    button->onClick.connect([]() {
        std::cout << "Styled button clicked!" << std::endl;
    });
    
    addWidget(button);
}
```

### Button Presets
```cpp
void setupUI() {
    // Use predefined button styles for consistency
    auto primaryButton = Button(ButtonPresets::Primary(50, 50, 150, 40, "Primary"));
    auto secondaryButton = Button(ButtonPresets::Secondary(50, 100, 150, 40, "Secondary"));
    auto successButton = Button(ButtonPresets::Success(50, 150, 150, 40, "Success"));
    auto dangerButton = Button(ButtonPresets::Danger(50, 200, 150, 40, "Danger"));
    auto warningButton = Button(ButtonPresets::Warning(50, 250, 150, 40, "Warning"));
    
    addWidget(primaryButton);
    addWidget(secondaryButton);
    addWidget(successButton);
    addWidget(dangerButton);
    addWidget(warningButton);
}
```

### Auto-Sizing Button
```cpp
void setupUI() {
    ButtonStyle autoStyle;
    autoStyle.normalColor(Colors::Purple)
           .hoverColor(Colors::Magenta)
           .textColor(Colors::White)
           .textScale(2);
    
    auto button = Button(ButtonConfig(100, 100, 200, 40, "Auto Size").style(autoStyle));
    
    button->onClick.connect([button]() {
        static std::vector<std::string> texts = {
            "Short", "Medium text", "Very long button text here", "Auto Size"
        };
        static int index = 0;
        index = (index + 1) % texts.size();
        
        // Update text and auto-size
        ButtonConfig newConfig(button->getX(), button->getY(), 200, 40, texts[index]);
        newConfig.style(autoStyle);
        button->setConfig(newConfig);
        button->autoSizeToContent(20);  // 20px padding
    });
    
    addWidget(button);
}
```

### Border Radius Example
```cpp
void setupUI() {
    ButtonStyle roundedStyle;
    roundedStyle.normalColor(Colors::Orange)
              .hoverColor(Colors::Yellow)
              .pressColor(Colors::Red)
              .textColor(Colors::Black)
              .textScale(2)
              .borderRadius(15);  // Rounded corners
    
    auto roundButton = Button(ButtonConfig(100, 100, 120, 60, "Round").style(roundedStyle));
    addWidget(roundButton);
}
```

## Configuration

### ButtonConfig Constructor
```cpp
ButtonConfig(int x, int y, int width, int height, const std::string& label)
```

Parameters:
- `x, y`: Position of the button
- `width, height`: Dimensions of the button
- `label`: Text displayed on the button

### Fluent Configuration
```cpp
ButtonConfig config(100, 100, 200, 50, "Button");
config.style(buttonStyle)
      .label("New Text")
      .position(150, 150)
      .size(250, 60);
```

## Styling

### ButtonStyle Properties

#### Colors
```cpp
ButtonStyle style;
style.normalColor(Colors::Blue)        // Default state color
     .hoverColor(Colors::LightBlue)    // When mouse hovers
     .pressColor(Colors::DarkBlue)     // When button is pressed
     .textColor(Colors::White);        // Text color
```

#### Text Properties
```cpp
style.textScale(2)                     // Text size multiplier
     .textColor(Colors::White);        // Text color
```

#### Border Properties
```cpp
style.borderRadius(8)                  // Rounded corners
     .border(2, Colors::Gray);         // Border width and color
```

### Predefined Styles

#### Primary Button
```cpp
ButtonStyle primaryStyle;
primaryStyle.normalColor(Colors::Blue)
           .hoverColor(Colors::LightBlue)
           .pressColor(Colors::DarkBlue)
           .textColor(Colors::White)
           .textScale(2)
           .borderRadius(4);
```

#### Secondary Button
```cpp
ButtonStyle secondaryStyle;
secondaryStyle.normalColor(Colors::Gray)
             .hoverColor(Colors::LightGray)
             .pressColor(Colors::DarkGray)
             .textColor(Colors::Black)
             .textScale(2)
             .borderRadius(4);
```

#### Danger Button
```cpp
ButtonStyle dangerStyle;
dangerStyle.normalColor(Colors::Red)
          .hoverColor(Colors::LightRed)
          .pressColor(Colors::DarkRed)
          .textColor(Colors::White)
          .textScale(2)
          .borderRadius(4);
```

## Event Handling

### onClick Event
```cpp
button->onClick.connect([]() {
    std::cout << "Button clicked!" << std::endl;
});
```

### With Captured Variables
```cpp
int counter = 0;
auto counterText = Text(Point(0, 0), "Count: 0", 2, Colors::White);

button->onClick.connect([&counter, counterText]() {
    counter++;
    counterText->setText("Count: " + std::to_string(counter));
});
```

### Lambda with Parameters
```cpp
auto createButton = [](const std::string& name, int id) {
    auto button = Button(ButtonConfig(0, 0, 100, 40, name));
    button->onClick.connect([id]() {
        std::cout << "Button " << id << " clicked!" << std::endl;
    });
    return button;
};
```

## Examples

### Example 1: Simple Click Counter
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static int clickCount = 0;
static std::shared_ptr<TextWidget> counterDisplay;

void setupUI() {
    // Counter display
    counterDisplay = Text(Point(0, 0), "Clicks: 0", 3, Colors::White);
    
    // Click button
    ButtonStyle clickStyle;
    clickStyle.normalColor(Colors::Blue)
             .hoverColor(Colors::LightBlue)
             .pressColor(Colors::DarkBlue)
             .textColor(Colors::White)
             .textScale(2);
    
    auto clickButton = Button(ButtonConfig(0, 0, 150, 50, "Click Me!").style(clickStyle));
    clickButton->onClick.connect([]() {
        clickCount++;
        counterDisplay->setText("Clicks: " + std::to_string(clickCount));
    });
    
    // Reset button
    ButtonStyle resetStyle;
    resetStyle.normalColor(Colors::Red)
             .hoverColor(Colors::LightRed)
             .pressColor(Colors::DarkRed)
             .textColor(Colors::White)
             .textScale(2);
    
    auto resetButton = Button(ButtonConfig(0, 0, 100, 40, "Reset").style(resetStyle));
    resetButton->onClick.connect([]() {
        clickCount = 0;
        counterDisplay->setText("Clicks: 0");
    });
    
    // Layout
    std::vector<std::shared_ptr<Widget>> children = {
        counterDisplay,
        SizedBox(0, 30),
        clickButton,
        SizedBox(0, 20),
        resetButton
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Example 2: Color Selector
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> statusText;

void setupUI() {
    statusText = Text(Point(0, 0), "Select a color:", 2, Colors::White);
    
    // Color buttons
    auto createColorButton = [](uint32_t color, const std::string& name) {
        ButtonStyle style;
        style.normalColor(color)
             .hoverColor(Colors::White)
             .pressColor(Colors::Gray)
             .textColor(Colors::Black)
             .textScale(1);
        
        auto button = Button(ButtonConfig(0, 0, 80, 80, "").style(style));
        button->onClick.connect([name]() {
            statusText->setText(name + " selected!");
        });
        
        return button;
    };
    
    std::vector<std::shared_ptr<Widget>> colorRow = {
        createColorButton(Colors::Red, "Red"),
        SizedBox(15, 0),
        createColorButton(Colors::Green, "Green"),
        SizedBox(15, 0),
        createColorButton(Colors::Blue, "Blue"),
        SizedBox(15, 0),
        createColorButton(Colors::Yellow, "Yellow")
    };
    
    std::vector<std::shared_ptr<Widget>> mainColumn = {
        Text(Point(0, 0), "Color Picker", 3, Colors::White),
        SizedBox(0, 30),
        statusText,
        SizedBox(0, 40),
        Row(colorRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainColumn));
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

### Example 3: Toggle Button
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> modeText;
static std::shared_ptr<ButtonWidget> toggleButton;
static bool isOn = false;

void setupUI() {
    modeText = Text(Point(0, 0), "Mode: OFF", 3, Colors::Red);
    
    // Create toggle button
    ButtonStyle offStyle;
    offStyle.normalColor(Colors::Gray)
           .hoverColor(Colors::LightGray)
           .pressColor(Colors::DarkGray)
           .textColor(Colors::White)
           .textScale(2);
    
    toggleButton = Button(ButtonConfig(0, 0, 120, 50, "Turn ON").style(offStyle));
    
    toggleButton->onClick.connect([]() {
        isOn = !isOn;
        
        if (isOn) {
            modeText->setText("Mode: ON");
            modeText->setColor(Colors::Green);
            
            ButtonStyle onStyle;
            onStyle.normalColor(Colors::Green)
                  .hoverColor(Colors::LightGreen)
                  .pressColor(Colors::DarkGreen)
                  .textColor(Colors::White)
                  .textScale(2);
            
            toggleButton->setConfig(ButtonConfig(0, 0, 120, 50, "Turn OFF").style(onStyle));
        } else {
            modeText->setText("Mode: OFF");
            modeText->setColor(Colors::Red);
            
            ButtonStyle offStyle;
            offStyle.normalColor(Colors::Gray)
                   .hoverColor(Colors::LightGray)
                   .pressColor(Colors::DarkGray)
                   .textColor(Colors::White)
                   .textScale(2);
            
            toggleButton->setConfig(ButtonConfig(0, 0, 120, 50, "Turn ON").style(offStyle));
        }
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Toggle Switch", 3, Colors::White),
        SizedBox(0, 30),
        modeText,
        SizedBox(0, 40),
        toggleButton
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

## API Reference

### ButtonConfig Class

#### Constructor
```cpp
ButtonConfig(int x, int y, int width, int height, const std::string& label)
```

#### Methods
```cpp
ButtonConfig& style(const ButtonStyle& s)     // Set button style
ButtonConfig& label(const std::string& text)  // Set button text
ButtonConfig& position(int x, int y)          // Set position
ButtonConfig& size(int width, int height)     // Set size
```

#### Getters
```cpp
int getX() const
int getY() const
int getWidth() const
int getHeight() const
const std::string& getLabel() const
const ButtonStyle& getStyle() const
```

### ButtonStyle Class

#### Color Methods
```cpp
ButtonStyle& normalColor(uint32_t color)    // Default state color
ButtonStyle& hoverColor(uint32_t color)     // Hover state color
ButtonStyle& pressColor(uint32_t color)     // Pressed state color
ButtonStyle& textColor(uint32_t color)      // Text color
```

#### Text Methods
```cpp
ButtonStyle& textScale(int scale)           // Text size multiplier
```

#### Border Methods
```cpp
ButtonStyle& borderRadius(int radius)       // Corner radius
ButtonStyle& border(int width, uint32_t color) // Border properties
```

#### Getters
```cpp
uint32_t getNormalColor() const
uint32_t getHoverColor() const
uint32_t getPressColor() const
uint32_t getTextColor() const
int getTextScale() const
int getBorderRadius() const
int getBorderWidth() const
uint32_t getBorderColor() const
```

### ButtonWidget Class

#### Events
```cpp
Signal<> onClick    // Fired when button is clicked
```

#### Methods
```cpp
void setConfig(const ButtonConfig& config)  // Update button configuration
void setText(const std::string& text)       // Change button text
void setStyle(const ButtonStyle& style)     // Update button style
```

### Auto-Sizing Methods
```cpp
void autoSizeToContent(int padding = 16)           // Auto-size button to fit text
static int calculateTextWidth(const std::string& text, int textScale)  // Calculate text width
static int calculateTextHeight(int textScale)      // Calculate text height
```

### Button Presets

#### Primary Button
```cpp
ButtonConfig ButtonPresets::Primary(int x, int y, int width, int height, const std::string& label)
```
Bootstrap-style primary button (blue theme).

#### Secondary Button  
```cpp
ButtonConfig ButtonPresets::Secondary(int x, int y, int width, int height, const std::string& label)
```
Bootstrap-style secondary button (gray theme).

#### Success Button
```cpp
ButtonConfig ButtonPresets::Success(int x, int y, int width, int height, const std::string& label)
```
Bootstrap-style success button (green theme).

#### Danger Button
```cpp
ButtonConfig ButtonPresets::Danger(int x, int y, int width, int height, const std::string& label)
```
Bootstrap-style danger button (red theme).

#### Warning Button
```cpp
ButtonConfig ButtonPresets::Warning(int x, int y, int width, int height, const std::string& label)
```
Bootstrap-style warning button (yellow/orange theme).

### Border Radius Support
The Button widget now supports rounded corners through the `borderRadius()` method:
```cpp
ButtonStyle style;
style.borderRadius(10);  // 10 pixel corner radius
```

## Common Issues and Solutions

### Toggle Button Position Fix
**Problem**: Button position resets to (0,0) after state change.

**Solution**: Preserve position when updating button config:
```cpp
toggleButton->onClick.connect([toggleButton]() {
    // Preserve current position
    int currentX = toggleButton->getX();
    int currentY = toggleButton->getY();
    
    ButtonConfig newConfig(currentX, currentY, 120, 50, "New Text");
    newConfig.style(newStyle);
    toggleButton->setConfig(newConfig);
});
```

### Auto-Sizing Implementation
**Problem**: Button doesn't automatically resize based on content.

**Solution**: Use the `autoSizeToContent()` method:
```cpp
button->setConfig(newConfig);
button->autoSizeToContent(20);  // 20px padding around text
```

### Responsive Button Width
**Problem**: Button doesn't adapt to window resize.

**Solution**: Update button width in resize callback:
```cpp
void onWindowResize(int newWidth, int newHeight) {
    if (responsiveButton) {
        int newButtonWidth = newWidth * 0.3;  // 30% of screen width
        ButtonConfig newConfig(0, 0, newButtonWidth, 50, "Responsive");
        responsiveButton->setConfig(newConfig);
    }
}
```

### Border Radius Not Visible
**Problem**: Setting border radius doesn't change button appearance.

**Solution**: Ensure you're using a recent version with rounded rectangle support. The border radius is now properly implemented using `Draw::roundedRect()`.

## Best Practices

### Consistent Button Sizing
Use button presets for consistent styling across your application:
```cpp
// Good: Consistent styling
auto saveButton = Button(ButtonPresets::Primary(x, y, 120, 40, "Save"));
auto cancelButton = Button(ButtonPresets::Secondary(x, y + 50, 120, 40, "Cancel"));

// Avoid: Inconsistent manual styling
auto saveButton = Button(ButtonConfig(x, y, 120, 40, "Save")
    .style(ButtonStyle().normalColor(0xFF123456))); // Random color
```

### Responsive Design
Make buttons adapt to screen size:
```cpp
int screenWidth = Fern::getWidth();
int buttonWidth = std::min(200, screenWidth * 0.4);  // Max 200px or 40% of screen
auto button = Button(ButtonConfig(x, y, buttonWidth, 40, "Responsive"));
```

### Auto-Sizing for Dynamic Content
Use auto-sizing for buttons with changing text:
```cpp
button->onClick.connect([button]() {
    // Update text
    ButtonConfig newConfig = button->getConfig();
    newConfig.label("New dynamic text");
    button->setConfig(newConfig);
    
    // Auto-size to fit
    button->autoSizeToContent(15);  // 15px padding
});
```
