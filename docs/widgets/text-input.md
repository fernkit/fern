# Text Input Widget Guide

Text input fields are the bridge between users and your application - they're how people communicate their thoughts, preferences, and data to your program. In this guide, you'll learn how to create responsive, accessible text input widgets in Fern while understanding the complex systems behind keyboard input, text rendering, and user interaction.

## Understanding Text Input Systems

Creating a text input widget involves orchestrating several complex systems:

1. **Keyboard Input Handling**: Capturing keypresses, handling special keys, and managing text composition
2. **Text Rendering**: Drawing characters as the user types, handling different fonts and sizes
3. **Cursor Management**: Positioning and animating the text cursor, handling selection
4. **Focus States**: Managing which input field is active and responding to focus changes
5. **Text Validation**: Constraining input length, filtering characters, providing feedback

Fern handles these complexities while exposing a clean, modern API that feels natural to use.

## Text Input Philosophy in Fern

Fern's text input system follows these principles:

- **Visual Feedback**: Clear indication of active state, cursor position, and input constraints
- **Responsive Interaction**: Immediate visual response to user actions
- **Flexible Styling**: Support for both bitmap and TTF fonts with comprehensive theming
- **Signal-Based Communication**: Type-safe event handling for text changes and user actions
- **Accessibility First**: Proper focus management and keyboard navigation

This approach ensures text inputs feel natural and responsive while maintaining the educational transparency that makes Fern special.

## Your First Text Input

Let's start with the simplest possible text input:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Create a basic text input
    auto textInput = TextInput(TextInputConfig(100, 100, 200, 30));
    
    // Handle text changes
    textInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "User typed: " << text << std::endl;
    });
    
    // Start the game loop
    while (shouldKeepRunning()) {
        processInput();
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `TextInputConfig(100, 100, 200, 30)`: Position (100,100) with size 200x30 pixels
- `onTextChanged`: Signal emitted whenever the user types or deletes text
- The input automatically handles cursor movement, character insertion, and visual feedback

Click the text field and start typing - you'll see immediate visual feedback and console output.

## Understanding Focus and Interaction

Text inputs use **focus** to determine which field is active for keyboard input:

```cpp
// Create multiple text inputs
auto nameInput = TextInput(TextInputConfig(100, 50, 200, 30).placeholder("Name"));
auto emailInput = TextInput(TextInputConfig(100, 100, 200, 30).placeholder("Email"));

// Handle focus changes
nameInput->onFocusChanged.connect([](bool focused) {
    if (focused) {
        std::cout << "Name field is now active" << std::endl;
    }
});

emailInput->onFocusChanged.connect([](bool focused) {
    if (focused) {
        std::cout << "Email field is now active" << std::endl;
    }
});
```

**Focus Behavior:**
- **Click to focus**: Clicking a text input gives it focus
- **Tab navigation**: Tab key moves between focusable widgets
- **Visual indication**: Focused inputs show a different border color
- **Exclusive focus**: Only one input can be focused at a time

## Styling Your Text Inputs

Fern provides comprehensive styling options through the `TextInputStyle` system:

```cpp
// Create a modern-looking text input
TextInputStyle modernStyle;
modernStyle.backgroundColor(Colors::White)
           .borderColor(Colors::LightGray)
           .focusBorderColor(Colors::Blue)
           .textColor(Colors::Black)
           .padding(10)
           .borderWidth(2);

auto styledInput = TextInput(
    TextInputConfig(100, 100, 250, 40)
        .placeholder("Modern input...")
        .style(modernStyle)
);
```

### Font Styling

Text inputs support both bitmap and TTF fonts:

```cpp
// Bitmap font input (retro style)
TextInputStyle retroStyle;
retroStyle.useBitmapFont()
          .fontSize(3)
          .backgroundColor(Colors::Black)
          .textColor(Colors::Green)
          .borderColor(Colors::DarkGreen);

auto retroInput = TextInput(
    TextInputConfig(100, 150, 300, 35)
        .placeholder("RETRO INPUT...")
        .style(retroStyle)
);

// TTF font input (modern style)
TextInputStyle modernStyle;
modernStyle.useTTFFont("arial")
           .fontSize(18)
           .backgroundColor(Colors::White)
           .textColor(Colors::DarkBlue)
           .padding(8);

auto modernInput = TextInput(
    TextInputConfig(100, 200, 300, 40)
        .placeholder("Modern input...")
        .style(modernStyle)
);
```

### Color States

Different visual states provide user feedback:

```cpp
TextInputStyle stateStyle;
stateStyle.backgroundColor(Colors::White)
          .borderColor(Colors::Gray)           // Normal state
          .focusBorderColor(Colors::Blue)      // When focused
          .textColor(Colors::Black)            // Text color
          .cursorColor(Colors::Red);           // Cursor color

auto stateInput = TextInput(
    TextInputConfig(100, 250, 200, 30)
        .style(stateStyle)
);
```

## Event Handling and Signals

Text inputs communicate through Fern's signal system:

### Text Change Events

```cpp
auto textInput = TextInput(TextInputConfig(100, 100, 200, 30));

// Triggered on every character typed or deleted
textInput->onTextChanged.connect([](const std::string& text) {
    std::cout << "Current text: '" << text << "'" << std::endl;
    
    // Real-time validation
    if (text.length() > 10) {
        std::cout << "Warning: Text is getting long!" << std::endl;
    }
});
```

### Enter Key Handling

```cpp
// Triggered when user presses Enter
textInput->onEnterPressed.connect([](const std::string& text) {
    std::cout << "User submitted: '" << text << "'" << std::endl;
    
    // Process the input
    if (!text.empty()) {
        processUserInput(text);
    }
});
```

### Focus Management

```cpp
textInput->onFocusChanged.connect([](bool focused) {
    if (focused) {
        std::cout << "Input gained focus - user can now type" << std::endl;
    } else {
        std::cout << "Input lost focus - user finished editing" << std::endl;
    }
});
```

## Advanced Text Input Features

### Input Validation and Constraints

```cpp
// Create input with maximum length
auto constrainedInput = TextInput(
    TextInputConfig(100, 100, 200, 30)
        .maxLength(20)
        .placeholder("Max 20 characters")
);

// Real-time validation
constrainedInput->onTextChanged.connect([](const std::string& text) {
    // Custom validation logic
    bool isValid = true;
    
    // Check for numbers only
    for (char c : text) {
        if (!std::isdigit(c)) {
            isValid = false;
            break;
        }
    }
    
    if (!isValid) {
        std::cout << "Only numbers allowed!" << std::endl;
    }
});
```

### Dynamic Placeholder Updates

```cpp
auto dynamicInput = TextInput(
    TextInputConfig(100, 100, 250, 30)
        .placeholder("Type to change placeholder...")
);

dynamicInput->onTextChanged.connect([dynamicInput](const std::string& text) {
    if (text.length() < 5) {
        dynamicInput->setPlaceholder("Keep typing...");
    } else if (text.length() < 10) {
        dynamicInput->setPlaceholder("Almost there!");
    } else {
        dynamicInput->setPlaceholder("Perfect length!");
    }
});
```

### Programmatic Text Control

```cpp
// Control text input programmatically
auto controlledInput = TextInput(TextInputConfig(100, 100, 200, 30));

// Set text from code
controlledInput->setText("Hello, World!");

// Clear input
controlledInput->clear();

// Get current text
std::string currentText = controlledInput->getText();

// Force focus (useful for forms)
controlledInput->setFocus(true);
```

## Text Input Presets

Fern provides convenient presets for common use cases:

```cpp
// Standard input field
auto defaultInput = TextInput(TextInputPresets::Default(100, 100));

// Modern styled input
auto modernInput = TextInput(TextInputPresets::Modern(100, 150));

// TTF font input
auto ttfInput = TextInput(TextInputPresets::WithTTF(100, 200, "arial"));
```

## Common Text Input Patterns

### Login Form

```cpp
class LoginForm {
public:
    LoginForm(int x, int y) {
        // Username input
        username_ = TextInput(
            TextInputConfig(x, y, 200, 30)
                .placeholder("Username")
                .style(createFormStyle())
        );
        
        // Password input (you'd implement password masking)
        password_ = TextInput(
            TextInputConfig(x, y + 40, 200, 30)
                .placeholder("Password")
                .style(createFormStyle())
        );
        
        // Handle Enter on either field
        username_->onEnterPressed.connect([this](const std::string&) {
            password_->setFocus(true);
        });
        
        password_->onEnterPressed.connect([this](const std::string&) {
            submitForm();
        });
    }
    
    void submitForm() {
        std::string user = username_->getText();
        std::string pass = password_->getText();
        
        if (!user.empty() && !pass.empty()) {
            std::cout << "Logging in user: " << user << std::endl;
            // Process login...
        }
    }
    
private:
    std::shared_ptr<TextInputWidget> username_;
    std::shared_ptr<TextInputWidget> password_;
    
    TextInputStyle createFormStyle() {
        TextInputStyle style;
        return style.backgroundColor(Colors::White)
                   .borderColor(Colors::Gray)
                   .focusBorderColor(Colors::Blue)
                   .padding(8);
    }
};
```

### Search Box

```cpp
class SearchBox {
public:
    SearchBox(int x, int y) {
        searchInput_ = TextInput(
            TextInputConfig(x, y, 300, 35)
                .placeholder("🔍 Search...")
                .style(createSearchStyle())
        );
        
        // Real-time search
        searchInput_->onTextChanged.connect([this](const std::string& query) {
            if (query.length() >= 3) {
                performSearch(query);
            }
        });
        
        // Search on Enter
        searchInput_->onEnterPressed.connect([this](const std::string& query) {
            if (!query.empty()) {
                performSearch(query);
                showResults();
            }
        });
    }
    
private:
    std::shared_ptr<TextInputWidget> searchInput_;
    
    TextInputStyle createSearchStyle() {
        TextInputStyle style;
        return style.backgroundColor(Colors::LightGray)
                   .borderColor(Colors::Gray)
                   .focusBorderColor(Colors::Blue)
                   .padding(10)
                   .useTTFFont()
                   .fontSize(16);
    }
    
    void performSearch(const std::string& query) {
        std::cout << "Searching for: " << query << std::endl;
        // Implement search logic...
    }
    
    void showResults() {
        std::cout << "Showing search results..." << std::endl;
    }
};
```

### Settings Panel

```cpp
struct Setting {
    std::string label;
    std::string value;
    std::function<void(const std::string&)> onChange;
};

class SettingsPanel {
public:
    SettingsPanel(int x, int y, const std::vector<Setting>& settings) {
        int currentY = y;
        
        for (const auto& setting : settings) {
            // Create label (you'd use Text widget)
            
            // Create input
            auto input = TextInput(
                TextInputConfig(x + 150, currentY, 200, 30)
                    .placeholder(setting.value)
                    .style(createSettingStyle())
            );
            
            input->setText(setting.value);
            input->onTextChanged.connect(setting.onChange);
            
            inputs_.push_back(input);
            currentY += 40;
        }
    }
    
private:
    std::vector<std::shared_ptr<TextInputWidget>> inputs_;
    
    TextInputStyle createSettingStyle() {
        TextInputStyle style;
        return style.backgroundColor(Colors::White)
                   .borderColor(Colors::LightGray)
                   .focusBorderColor(Colors::Blue)
                   .padding(6);
    }
};
```

## Text Input in Layouts

Text inputs work seamlessly with Fern's layout system:

```cpp
// Vertical form layout
auto form = Column({
    Text(TextPresets::Title(0, 0, "Contact Form")),
    TextInput(TextInputPresets::Default(0, 0).placeholder("Name")),
    TextInput(TextInputPresets::Default(0, 0).placeholder("Email")),
    TextInput(TextInputPresets::Default(0, 0).placeholder("Message")),
    Button(ButtonConfig(0, 0, 100, 35, "Submit"))
});
```

## Performance Considerations

### Text Rendering Efficiency

Text inputs re-render on every keystroke, so consider:

```cpp
// Efficient: Use bitmap fonts for fast rendering
TextInputStyle fastStyle;
fastStyle.useBitmapFont().fontSize(2);

// Slower: TTF fonts require more processing
TextInputStyle niceStyle;
niceStyle.useTTFFont("arial").fontSize(18);

// Choose based on your needs:
// - Bitmap for fast, retro interfaces
// - TTF for professional, readable interfaces
```

### Memory Management

```cpp
// Text inputs are automatically managed
auto input = TextInput(TextInputConfig(100, 100, 200, 30));
// Automatically cleaned up when references go away

// For temporary inputs:
{
    auto tempInput = TextInput(TextInputConfig(50, 50, 150, 25));
    // Automatically cleaned up when scope ends
}
```

## Advanced Text Input Techniques

### Custom Validation

```cpp
class ValidatedInput {
public:
    ValidatedInput(TextInputConfig config, std::function<bool(const std::string&)> validator)
        : validator_(validator) {
        
        input_ = TextInput(config);
        
        input_->onTextChanged.connect([this](const std::string& text) {
            bool isValid = validator_(text);
            updateValidationState(isValid);
        });
    }
    
    void updateValidationState(bool isValid) {
        TextInputStyle style = input_->getConfig().getStyle();
        
        if (isValid) {
            style.borderColor(Colors::Green);
        } else {
            style.borderColor(Colors::Red);
        }
        
        // Update style (you'd need to implement this)
        // input_->setStyle(style);
    }
    
private:
    std::shared_ptr<TextInputWidget> input_;
    std::function<bool(const std::string&)> validator_;
};

// Usage
ValidatedInput emailInput(
    TextInputConfig(100, 100, 200, 30).placeholder("Email"),
    [](const std::string& text) {
        return text.find('@') != std::string::npos; // Simple email check
    }
);
```

### Input History

```cpp
class HistoryInput {
public:
    HistoryInput(TextInputConfig config) {
        input_ = TextInput(config);
        
        input_->onEnterPressed.connect([this](const std::string& text) {
            if (!text.empty()) {
                history_.push_back(text);
                historyIndex_ = history_.size();
                input_->clear();
            }
        });
        
        // Handle up/down arrows for history navigation
        // (You'd need to add key handling for this)
    }
    
    void navigateHistory(int direction) {
        if (history_.empty()) return;
        
        historyIndex_ += direction;
        historyIndex_ = std::clamp(historyIndex_, 0, (int)history_.size());
        
        if (historyIndex_ < history_.size()) {
            input_->setText(history_[historyIndex_]);
        } else {
            input_->clear();
        }
    }
    
private:
    std::shared_ptr<TextInputWidget> input_;
    std::vector<std::string> history_;
    int historyIndex_ = 0;
};
```

## Troubleshooting Common Issues

### Input Not Responding

**Check these common issues:**

1. **Input not focused**:
```cpp
// Ensure input has focus to receive keyboard input
input->setFocus(true);

// Or check if input is focused
if (!input->isFocused()) {
    std::cout << "Input needs focus to receive keyboard input" << std::endl;
}
```

2. **Input outside clickable area**:
```cpp
// Ensure input is positioned within screen bounds
auto input = TextInput(TextInputConfig(10, 10, 200, 30)); // Safe position
```

3. **No event handlers connected**:
```cpp
// Make sure you've connected to the signals
input->onTextChanged.connect([](const std::string& text) {
    // This will actually run when text changes
});
```

### Visual Issues

**Text not appearing:**
```cpp
// Check text color contrasts with background
TextInputStyle style;
style.backgroundColor(Colors::White)
     .textColor(Colors::Black);        // Ensure contrast
```

**Cursor not visible:**
```cpp
// Ensure cursor color is different from background
style.cursorColor(Colors::Black)
     .backgroundColor(Colors::White);
```

### Performance Issues

**Slow input response:**
```cpp
// Use bitmap fonts for faster rendering
TextInputStyle fastStyle;
fastStyle.useBitmapFont().fontSize(2);

// Avoid very large input fields
auto efficientInput = TextInput(TextInputConfig(100, 100, 200, 30)); // Reasonable size
```

## Summary

Text input widgets in Fern provide a sophisticated foundation for user interaction while maintaining the transparency and educational value that makes Fern special. Whether you're building simple forms or complex data entry interfaces, understanding focus management, event handling, and styling options will help you create responsive, user-friendly input systems.

Key takeaways:
- **Focus management**: Only focused inputs receive keyboard input
- **Signal-based events**: Clean, type-safe communication through onTextChanged, onEnterPressed, and onFocusChanged
- **Flexible styling**: Support for both bitmap and TTF fonts with comprehensive theming
- **Built-in features**: Cursor management, text selection, and visual feedback
- **Layout integration**: Works seamlessly with Fern's layout system
- **Performance awareness**: Choose appropriate fonts and sizes for your use case

Text inputs showcase Fern's commitment to both simplicity and power - they're easy to use for basic cases but provide the depth needed for sophisticated applications. Master text inputs, and you'll have the foundation for creating engaging, interactive user interfaces that feel natural and responsive.
 