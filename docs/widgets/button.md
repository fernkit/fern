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

## Best Practices

### 1. Consistent Styling
Create reusable button styles:
```cpp
namespace ButtonStyles {
    ButtonStyle Primary() {
        ButtonStyle style;
        style.normalColor(Colors::Blue)
             .hoverColor(Colors::LightBlue)
             .pressColor(Colors::DarkBlue)
             .textColor(Colors::White)
             .textScale(2);
        return style;
    }
}
```

### 2. Responsive Sizing
Make buttons adapt to screen size:
```cpp
int screenWidth = Fern::getWidth();
int buttonWidth = std::min(300, screenWidth - 40); // Max 300px, min 20px margin
```

### 3. Clear Labels
Use descriptive, action-oriented labels:
```cpp
// Good
Button("Save Document")
Button("Delete Item")
Button("Send Message")

// Avoid
Button("OK")
Button("Do It")
Button("Click")
```

### 4. Visual Feedback
Always provide hover and press states:
```cpp
style.normalColor(Colors::Blue)
     .hoverColor(Colors::LightBlue)   // Lighter on hover
     .pressColor(Colors::DarkBlue);   // Darker when pressed
```

## Common Patterns

### Confirmation Dialog
```cpp
void createConfirmDialog(const std::string& message, std::function<void()> onConfirm) {
    auto confirmButton = Button(ButtonConfig(0, 0, 100, 40, "Confirm"));
    confirmButton->onClick.connect([onConfirm]() {
        onConfirm();
        // Close dialog logic here
    });
    
    auto cancelButton = Button(ButtonConfig(0, 0, 100, 40, "Cancel"));
    cancelButton->onClick.connect([]() {
        // Close dialog logic here
    });
    
    // Layout buttons horizontally
    std::vector<std::shared_ptr<Widget>> buttons = {
        confirmButton,
        SizedBox(20, 0),
        cancelButton
    };
    
    // Add to dialog layout
}
```

### Loading States
```cpp
class LoadingButton {
    std::shared_ptr<ButtonWidget> button;
    bool isLoading = false;
    
public:
    void setLoading(bool loading) {
        isLoading = loading;
        if (loading) {
            button->setText("Loading...");
            button->setEnabled(false);
        } else {
            button->setText("Submit");
            button->setEnabled(true);
        }
    }
};
```

## Troubleshooting

### Button Not Responding
- Check if button is properly added to widget manager
- Verify click handler is connected
- Ensure button is not obscured by other widgets

### Styling Issues
- Verify color values are valid
- Check text scale is appropriate for button size
- Ensure border radius doesn't exceed button dimensions

### Layout Problems
- Use proper spacing between buttons
- Consider screen size when setting button dimensions
- Test on different screen resolutions

---

**Related Documentation:**
- [Text Widget](text.md)
- [Layout System](../layout/overview.md)
- [Colors System](../graphics/colors.md)
- [Event Handling](../input/events.md)
