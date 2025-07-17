# Button Widget - Interactive Elements Made Simple

Buttons are the workhorses of user interfaces. Every time you click "Save", "Submit", "OK", or any other action in an application, you're interacting with a button widget. Understanding how buttons work, both conceptually and technically, will give you insight into interactive UI design and help you build better applications.

## What Makes a Button?

At its core, a button is a visual element that:
- **Looks clickable**: Uses visual cues like borders, colors, and text to signal it's interactive
- **Responds to interaction**: Changes appearance when you hover over it or click it
- **Triggers actions**: Executes code when activated, connecting user intent to application logic
- **Provides feedback**: Gives immediate visual response so users know their click registered

In Fern, buttons combine several lower-level concepts:
- **Rectangle drawing** for the button background
- **Text rendering** for the button label  
- **Input handling** to detect mouse interactions
- **State management** to track hover/pressed/normal states
- **Signal emission** to notify your code when clicked

## The Anatomy of User Interaction

Before diving into code, let's understand what happens when you interact with a button:

1. **Visual Recognition**: Your eye sees a rectangular area with text that looks clickable
2. **Hover Feedback**: As you move your mouse over it, the button changes color to confirm it's interactive
3. **Click Action**: You press the mouse button, and the button visually "presses down" with a different color
4. **Signal Emission**: The button widget emits a signal that your application code receives
5. **Action Execution**: Your connected function runs, performing the intended action
6. **Visual Reset**: The button returns to its normal or hover state

This entire cycle happens in milliseconds, but understanding each step helps you design better interactions.

## Basic Button Usage

The simplest button requires just a position, size, and text label:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Create a basic button at position (100, 100) with size 200x50 pixels
    auto button = Button(ButtonConfig(100, 100, 200, 50, "Click Me"));
    
    // Connect an action to the button's click event
    button->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    // Add to the widget manager so it renders and receives input
    addWidget(button);
}

void draw() {
    Draw::fill(Colors::Black);  // Clear background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

This creates a fully functional button with:
- Default styling (colors, borders, text)
- Automatic hover effects
- Click detection and feedback
- Signal emission when clicked

## Understanding ButtonConfig

The `ButtonConfig` class is your primary tool for defining button properties. It uses a builder pattern that makes configuration both flexible and readable:

```cpp
// Basic configuration: x, y, width, height, text
ButtonConfig(100, 100, 200, 50, "My Button")

// Extended configuration with styling
ButtonConfig(100, 100, 200, 50, "Styled Button")
    .style(myButtonStyle)
    .tooltip("This button does something important")
```

The builder pattern allows you to chain method calls, setting only the properties you need while getting sensible defaults for everything else.

## Button Styling and Visual States

Buttons have three primary visual states, each serving an important purpose in user experience:

### Understanding Visual States

1. **Normal State**: The button's default appearance when not being interacted with
2. **Hover State**: How the button looks when the mouse cursor is over it (but not clicked)
3. **Pressed State**: The button's appearance when being actively clicked

These states provide crucial feedback to users about the current interaction state.

### Creating Custom Styles

```cpp
void setupUI() {
    // Create a custom button style
    ButtonStyle style;
    style.normalColor(Colors::Blue)         // Default state: blue background
         .hoverColor(Colors::LightBlue)     // Hover state: lighter blue
         .pressColor(Colors::DarkBlue)      // Pressed state: darker blue
         .textColor(Colors::White)          // Text is always white
         .textScale(2)                      // Text size multiplier
         .borderRadius(8);                  // Rounded corners
    
    // Apply the style to a button
    auto styledButton = Button(ButtonConfig(200, 200, 180, 60, "Styled Button").style(style));
    
    styledButton->onClick.connect([]() {
        std::cout << "Styled button clicked!" << std::endl;
    });
    
    addWidget(styledButton);
}
```

### Button Style Properties

The `ButtonStyle` class provides comprehensive control over button appearance:

```cpp
ButtonStyle style;

// Background colors for different states
style.normalColor(Colors::Green)        // Default background
     .hoverColor(Colors::LightGreen)    // Hover background  
     .pressColor(Colors::DarkGreen);    // Pressed background

// Text properties
style.textColor(Colors::White)          // Text color
     .textScale(1.5);                   // Text size (1.0 = normal, 2.0 = double)

// Border and shape
style.borderRadius(10)                  // Rounded corners (0 = square)
     .borderWidth(2)                    // Border thickness
     .borderColor(Colors::Black);       // Border color

// Advanced properties
style.shadow(true)                      // Drop shadow effect
     .shadowColor(Colors::Gray)         // Shadow color
     .shadowOffset(2, 2);               // Shadow position offset
```

## Event Handling and Signals

Buttons communicate with your application through Fern's signal-slot system. This is a powerful pattern borrowed from frameworks like Qt that provides clean separation between UI and logic.

### The Signal-Slot Pattern

A **signal** is an event that can occur (like a button click). A **slot** is a function that responds to that event. You **connect** slots to signals to define what happens when events occur.

```cpp
// Signal: button->onClick
// Slot: the lambda function []() { ... }
// Connection: .connect() links them together

button->onClick.connect([]() {
    std::cout << "This is a slot function!" << std::endl;
});
```

### Multiple Handlers

You can connect multiple functions to the same button:

```cpp
auto saveButton = Button(ButtonConfig(0, 0, 100, 40, "Save"));

// Connect multiple handlers
saveButton->onClick.connect([]() {
    std::cout << "Saving data..." << std::endl;
});

saveButton->onClick.connect([]() {
    std::cout << "Updating UI..." << std::endl;
});

saveButton->onClick.connect([]() {
    std::cout << "Logging action..." << std::endl;
});

// When clicked, all three functions will run in order
```

### Capturing Variables

Lambda functions can capture variables from their surrounding scope:

```cpp
void setupUI() {
    static int clickCount = 0;  // Static so it persists
    static std::shared_ptr<TextWidget> counterDisplay;
    
    counterDisplay = Text(Point(100, 50), "Clicks: 0", 2, Colors::White);
    
    auto countButton = Button(ButtonConfig(100, 100, 150, 50, "Count"));
    
    // Capture variables in the lambda
    countButton->onClick.connect([&]() {
        clickCount++;
        counterDisplay->setText("Clicks: " + std::to_string(clickCount));
    });
    
    addWidget(counterDisplay);
    addWidget(countButton);
}
```

## Button Layouts and Positioning

While you can position buttons with exact coordinates, using layout widgets creates more flexible, responsive interfaces.

### Manual Positioning

```cpp
// Fixed positions - works but not responsive
auto button1 = Button(ButtonConfig(100, 100, 120, 40, "Button 1"));
auto button2 = Button(ButtonConfig(230, 100, 120, 40, "Button 2"));
auto button3 = Button(ButtonConfig(360, 100, 120, 40, "Button 3"));
```

### Layout-Based Positioning

```cpp
// Responsive positioning using layouts
void setupUI() {
    std::vector<std::shared_ptr<Widget>> buttons = {
        Button(ButtonConfig(0, 0, 120, 40, "Save")),
        SizedBox(10, 0),  // 10 pixels horizontal spacing
        Button(ButtonConfig(0, 0, 120, 40, "Cancel")),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Help"))
    };
    
    // Create horizontal button row
    auto buttonRow = Row(buttons);
    buttonRow.setMainAxisAlignment(MainAxisAlignment::Center);
    
    // Center the button row on screen
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(buttonRow);
    
    addWidget(centerWidget);
}
```

### Common Button Layout Patterns

**Action Bar (right-aligned buttons):**
```cpp
std::vector<std::shared_ptr<Widget>> actionButtons = {
    Button(ButtonConfig(0, 0, 80, 35, "Cancel")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, 80, 35, "OK"))
};

auto actionRow = Row(actionButtons);
actionRow.setMainAxisAlignment(MainAxisAlignment::End);  // Right-align
```

**Button Grid:**
```cpp
// First row of buttons
auto row1 = Row({
    Button(ButtonConfig(0, 0, 100, 40, "1")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, 100, 40, "2")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, 100, 40, "3"))
});

// Second row of buttons
auto row2 = Row({
    Button(ButtonConfig(0, 0, 100, 40, "4")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, 100, 40, "5")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, 100, 40, "6"))
});

// Combine rows vertically
auto buttonGrid = Column({
    row1,
    SizedBox(0, 10),  // Vertical spacing between rows
    row2
});
```

## Button Presets and Common Styles

Fern provides several preset button styles for common UI patterns:

```cpp
void setupUI() {
    std::vector<std::shared_ptr<Widget>> presetButtons = {
        Button(ButtonPresets::Primary(0, 0, 120, 40, "Primary")),
        SizedBox(10, 0),
        Button(ButtonPresets::Secondary(0, 0, 120, 40, "Secondary")),
        SizedBox(10, 0),
        Button(ButtonPresets::Success(0, 0, 120, 40, "Success")),
        SizedBox(10, 0),
        Button(ButtonPresets::Danger(0, 0, 120, 40, "Danger")),
        SizedBox(10, 0),
        Button(ButtonPresets::Warning(0, 0, 120, 40, "Warning"))
    };
    
    auto buttonRow = Row(presetButtons);
    buttonRow.setMainAxisAlignment(MainAxisAlignment::Center);
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(buttonRow);
    addWidget(centerWidget);
}
```

These presets follow common UI design conventions:
- **Primary**: Main action button (usually blue)
- **Secondary**: Alternative action (usually gray)
- **Success**: Positive actions like "Save" or "Submit" (usually green)
- **Danger**: Destructive actions like "Delete" (usually red)
- **Warning**: Cautionary actions (usually orange/yellow)

## Advanced Button Features

### Auto-Sizing Buttons

Buttons can automatically resize to fit their text content:

```cpp
auto autoButton = Button(ButtonConfig(0, 0, 100, 40, "Short"));

autoButton->onClick.connect([autoButton]() {
    static int state = 0;
    std::vector<std::string> texts = {
        "Short",
        "Medium length text",
        "This is a very long button text that demonstrates auto-sizing"
    };
    
    state = (state + 1) % texts.size();
    
    // Update button text and auto-resize
    ButtonConfig newConfig(autoButton->getX(), autoButton->getY(), 100, 40, texts[state]);
    autoButton->setConfig(newConfig);
    autoButton->autoSizeToContent(20);  // 20 pixels padding
});
```

### Toggle Buttons

Create buttons that maintain pressed/unpressed state:

```cpp
void setupUI() {
    static bool isToggled = false;
    static std::shared_ptr<ButtonWidget> toggleButton;
    
    ButtonStyle normalStyle;
    normalStyle.normalColor(Colors::Gray).textColor(Colors::White);
    
    ButtonStyle toggledStyle;
    toggledStyle.normalColor(Colors::Blue).textColor(Colors::White);
    
    toggleButton = Button(ButtonConfig(0, 0, 120, 40, "Toggle: OFF").style(normalStyle));
    
    toggleButton->onClick.connect([&]() {
        isToggled = !isToggled;
        
        if (isToggled) {
            toggleButton->setText("Toggle: ON");
            toggleButton->setStyle(toggledStyle);
        } else {
            toggleButton->setText("Toggle: OFF");
            toggleButton->setStyle(normalStyle);
        }
    });
    
    addWidget(toggleButton);
}
```

### Disabled Buttons

Buttons can be disabled to prevent interaction:

```cpp
ButtonStyle disabledStyle;
disabledStyle.normalColor(Colors::DarkGray)
             .textColor(Colors::Gray)
             .hoverColor(Colors::DarkGray)   // Same as normal
             .pressColor(Colors::DarkGray);  // Same as normal

auto disabledButton = Button(ButtonConfig(0, 0, 120, 40, "Disabled").style(disabledStyle));

// Don't connect any onClick handlers for disabled buttons
```

## State Management with Buttons

Buttons often need to update other parts of your interface when clicked. Here are common patterns:

### Counter Example

```cpp
static int counter = 0;
static std::shared_ptr<TextWidget> counterDisplay;

void setupUI() {
    counterDisplay = Text(Point(0, 0), "Count: 0", 3, Colors::White);
    
    auto incrementButton = Button(ButtonConfig(0, 0, 80, 40, "+")); 
    incrementButton->onClick.connect([]() {
        counter++;
        counterDisplay->setText("Count: " + std::to_string(counter));
    });
    
    auto decrementButton = Button(ButtonConfig(0, 0, 80, 40, "-"));
    decrementButton->onClick.connect([]() {
        counter--;
        counterDisplay->setText("Count: " + std::to_string(counter));
    });
    
    auto resetButton = Button(ButtonConfig(0, 0, 80, 40, "Reset"));
    resetButton->onClick.connect([]() {
        counter = 0;
        counterDisplay->setText("Count: 0");
    });
    
    std::vector<std::shared_ptr<Widget>> elements = {
        counterDisplay,
        SizedBox(0, 20),
        Row({
            decrementButton,
            SizedBox(10, 0),
            incrementButton, 
            SizedBox(10, 0),
            resetButton
        })
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(Column(elements));
    addWidget(centerWidget);
}
```

### Form Validation

```cpp
static std::shared_ptr<TextInput> emailInput;
static std::shared_ptr<TextInput> passwordInput;
static std::shared_ptr<ButtonWidget> submitButton;

void setupUI() {
    emailInput = TextInput(Point(0, 0), Size(250, 35));
    passwordInput = TextInput(Point(0, 0), Size(250, 35));
    
    // Initially disabled submit button
    ButtonStyle disabledStyle;
    disabledStyle.normalColor(Colors::DarkGray).textColor(Colors::Gray);
    
    submitButton = Button(ButtonConfig(0, 0, 100, 40, "Submit").style(disabledStyle));
    
    // Enable submit button when both fields have content
    auto validateForm = []() {
        bool isValid = !emailInput->getText().empty() && !passwordInput->getText().empty();
        
        if (isValid) {
            ButtonStyle enabledStyle;
            enabledStyle.normalColor(Colors::Green).textColor(Colors::White);
            submitButton->setStyle(enabledStyle);
            
            // Connect submit handler only when enabled
            submitButton->onClick.connect([]() {
                std::cout << "Form submitted!" << std::endl;
            });
        }
    };
    
    // Validate when text changes (this would need onTextChanged signals)
    // For now, validate on a separate button click
    auto validateButton = Button(ButtonConfig(0, 0, 100, 40, "Validate"));
    validateButton->onClick.connect(validateForm);
    
    std::vector<std::shared_ptr<Widget>> formElements = {
        Text(Point(0, 0), "Email:", 2, Colors::White),
        SizedBox(0, 5),
        emailInput,
        SizedBox(0, 15),
        Text(Point(0, 0), "Password:", 2, Colors::White),
        SizedBox(0, 5),
        passwordInput,
        SizedBox(0, 20),
        Row({validateButton, SizedBox(10, 0), submitButton})
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(Column(formElements));
    addWidget(centerWidget);
}
```

## Performance and Best Practices

### Button Creation Efficiency

Create buttons once in `setupUI()`, not every frame:

```cpp
// Good - create once
void setupUI() {
    auto button = Button(ButtonConfig(0, 0, 100, 40, "Click"));
    addWidget(button);
}

// Bad - creates new button every frame
void draw() {
    auto button = Button(ButtonConfig(0, 0, 100, 40, "Click"));  // Don't do this!
    addWidget(button);
}
```

### Memory Management

Use shared_ptr for buttons you need to reference later:

```cpp
static std::shared_ptr<ButtonWidget> importantButton;

void setupUI() {
    importantButton = Button(ButtonConfig(0, 0, 100, 40, "Important"));
    
    importantButton->onClick.connect([]() {
        // Can safely reference importantButton here
        importantButton->setText("Clicked!");
    });
    
    addWidget(importantButton);
}
```

### Event Handler Performance

Keep event handlers lightweight:

```cpp
// Good - quick operations
button->onClick.connect([]() {
    counter++;
    updateDisplay();
});

// Be careful with heavy operations
button->onClick.connect([]() {
    // Heavy file I/O or network operations might block the UI
    // Consider using background threads for heavy work
    processLargeFile();  // This could cause lag
});
```

## Troubleshooting Common Issues

### Button Not Responding

**Problem**: Button appears but doesn't respond to clicks
**Solutions**:
- Ensure you called `addWidget(button)`
- Check that another widget isn't covering the button
- Verify the button's position is within the window bounds

### Button Not Appearing

**Problem**: Button doesn't show up on screen
**Solutions**:
- Check button position and size (e.g., not at negative coordinates)
- Ensure `addWidget()` was called after button creation
- Verify the button isn't positioned outside the window

### Click Handler Not Working

**Problem**: Button responds visually but click handler doesn't run
**Solutions**:
- Ensure you connected the handler with `.connect()`
- Check that the lambda capture is correct (use `[&]` or `[=]` as needed)
- Verify the handler code doesn't throw exceptions

### Layout Issues

**Problem**: Button appears in wrong position when using layouts
**Solutions**:
- Use `Point(0, 0)` for buttons inside layout widgets
- Check your layout hierarchy and alignment settings
- Add proper `SizedBox` spacing between elements

## Complete Working Examples

### Simple Click Counter

```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static int clicks = 0;
static std::shared_ptr<TextWidget> display;

void setupUI() {
    display = Text(Point(0, 0), "Clicks: 0", 3, Colors::White);
    
    auto clickButton = Button(ButtonConfig(0, 0, 150, 50, "Click Me!"));
    clickButton->onClick.connect([]() {
        clicks++;
        display->setText("Clicks: " + std::to_string(clicks));
    });
    
    std::vector<std::shared_ptr<Widget>> elements = {
        display,
        SizedBox(0, 30),
        clickButton
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(Column(elements));
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

### Multi-Button Interface

```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> statusText;

void setupUI() {
    statusText = Text(Point(0, 0), "Ready", 2, Colors::White);
    
    // Create styled buttons
    ButtonStyle successStyle;
    successStyle.normalColor(Colors::Green).hoverColor(Colors::LightGreen).textColor(Colors::White);
    
    ButtonStyle warningStyle;
    warningStyle.normalColor(Colors::Orange).hoverColor(Colors::Gold).textColor(Colors::White);
    
    ButtonStyle dangerStyle;  
    dangerStyle.normalColor(Colors::Red).hoverColor(Colors::LightRed).textColor(Colors::White);
    
    auto saveButton = Button(ButtonConfig(0, 0, 100, 40, "Save").style(successStyle));
    auto warningButton = Button(ButtonConfig(0, 0, 100, 40, "Warning").style(warningStyle));
    auto deleteButton = Button(ButtonConfig(0, 0, 100, 40, "Delete").style(dangerStyle));
    
    // Connect handlers
    saveButton->onClick.connect([]() {
        statusText->setText("Data saved successfully!");
    });
    
    warningButton->onClick.connect([]() {
        statusText->setText("Warning: Check your inputs!");
    });
    
    deleteButton->onClick.connect([]() {
        statusText->setText("Item deleted!");
    });
    
    std::vector<std::shared_ptr<Widget>> elements = {
        Text(Point(0, 0), "Button Demo", 4, Colors::White),
        SizedBox(0, 30),
        statusText,
        SizedBox(0, 30),
        Row({
            saveButton,
            SizedBox(15, 0),
            warningButton,
            SizedBox(15, 0),
            deleteButton
        })
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(Column(elements));
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

Understanding buttons deeply - from their visual states to their event handling to their role in larger interfaces - gives you a solid foundation for building any kind of interactive application. Buttons might seem simple, but they embody many fundamental concepts of UI design and user interaction that apply throughout interface development.

