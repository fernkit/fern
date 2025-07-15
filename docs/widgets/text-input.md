# Text Input Widget

The `TextInputWidget` provides an interactive text input field that allows users to type and edit text. It supports placeholders, text validation, focus management, and customizable styling.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Styling](#styling)
- [Events and Signals](#events-and-signals)
- [Focus Management](#focus-management)
- [Advanced Features](#advanced-features)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating a Text Input

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic text input
auto textInput = TextInput(TextInputConfig(100, 100, 200, 30));

// With placeholder
auto textInput = TextInput(
    TextInputConfig(100, 100, 200, 30)
        .placeholder("Enter your name...")
);

// Using presets
auto textInput = TextInput(TextInputPresets::Default(100, 100));
```

### Simple Example

```cpp
void setupUI() {
    // Create a text input
    auto textInput = TextInput(
        TextInputConfig(100, 100, 300, 40)
            .placeholder("Type something...")
    );
    
    // Handle text changes
    textInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "Text: " << text << std::endl;
    });
    
    // Handle enter key
    textInput->onEnterPressed.connect([](const std::string& text) {
        std::cout << "Submitted: " << text << std::endl;
    });
    
    addWidget(textInput);
}
```

## Configuration

### TextInputConfig

The `TextInputConfig` class provides a fluent interface for configuring text input properties:

```cpp
TextInputConfig config(x, y, width, height);

// Method chaining
config.placeholder("Enter text...")
      .maxLength(100)
      .style(customStyle);

// Individual setters
config.setPosition(200, 150);
config.setSize(400, 50);
```

### Configuration Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `x, y` | `int` | Required | Position coordinates |
| `width, height` | `int` | Required | Widget dimensions |
| `placeholder` | `std::string` | `""` | Placeholder text when empty |
| `maxLength` | `size_t` | `256` | Maximum text length |
| `style` | `TextInputStyle` | Default | Visual styling |

### Preset Configurations

```cpp
// Standard text input
auto input1 = TextInput(TextInputPresets::Default(100, 100));

// Modern styled input
auto input2 = TextInput(TextInputPresets::Modern(100, 150));

// TTF font input
auto input3 = TextInput(TextInputPresets::WithTTF(100, 200, "Arial"));
```

## Styling

### TextInputStyle

The `TextInputStyle` class provides comprehensive styling options:

```cpp
TextInputStyle style;
style.backgroundColor(0xFFFFFF)     // White background
     .borderColor(0x888888)         // Gray border
     .focusBorderColor(0x0066CC)    // Blue focus border
     .textColor(0x000000)           // Black text
     .cursorColor(0x000000)         // Black cursor
     .borderWidth(2)                // 2px border
     .padding(8)                    // 8px internal padding
     .fontSize(16);                 // 16px font size
```

### Font Configuration

```cpp
// Bitmap font (default)
style.useBitmapFont()
     .fontSize(2);  // Scale factor for bitmap font

// TTF font
style.useTTFFont("Arial")
     .fontSize(18);  // Point size for TTF font
```

### Style Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `backgroundColor` | `uint32_t` | `0xFFFFFF` | Background color |
| `borderColor` | `uint32_t` | `0x888888` | Border color |
| `focusBorderColor` | `uint32_t` | `0x0066CC` | Focus border color |
| `textColor` | `uint32_t` | `0x000000` | Text color |
| `cursorColor` | `uint32_t` | `0x000000` | Cursor color |
| `borderWidth` | `int` | `1` | Border width in pixels |
| `padding` | `int` | `4` | Internal padding |
| `fontSize` | `int` | `2` | Font size |

### Color Examples

```cpp
// Using color constants
style.backgroundColor(Colors::White)
     .borderColor(Colors::Gray)
     .focusBorderColor(Colors::Blue)
     .textColor(Colors::Black);

// Using hex colors
style.backgroundColor(0xF5F5F5)     // Light gray
     .borderColor(0xDDDDDD)         // Light border
     .focusBorderColor(0x4CAF50)    // Green focus
     .textColor(0x333333);          // Dark text
```

## Events and Signals

### Available Signals

```cpp
// Text changed signal
textInput->onTextChanged.connect([](const std::string& text) {
    // Called whenever text changes
});

// Enter key pressed
textInput->onEnterPressed.connect([](const std::string& text) {
    // Called when user presses Enter
});

// Focus changed
textInput->onFocusChanged.connect([](bool hasFocus) {
    // Called when focus state changes
});
```

### Event Handling Example

```cpp
void setupTextInput() {
    auto textInput = TextInput(TextInputConfig(100, 100, 300, 40)
        .placeholder("Search...")
        .maxLength(50));
    
    // Real-time search
    textInput->onTextChanged.connect([](const std::string& text) {
        if (text.length() > 2) {
            performSearch(text);
        }
    });
    
    // Submit search
    textInput->onEnterPressed.connect([](const std::string& text) {
        if (!text.empty()) {
            submitSearch(text);
        }
    });
    
    // Visual feedback on focus
    textInput->onFocusChanged.connect([textInput](bool focused) {
        if (focused) {
            // Highlight or show help text
        }
    });
}
```

## Focus Management

### Focus Control

```cpp
// Set focus programmatically
textInput->setFocus(true);

// Check focus state
if (textInput->isFocused()) {
    // Handle focused state
}

// Remove focus
textInput->setFocus(false);
```

### Focus Styling

```cpp
// Different colors for focused/unfocused states
TextInputStyle style;
style.borderColor(Colors::Gray)
     .focusBorderColor(Colors::Blue);  // Changes when focused

// The widget automatically switches between these colors
```

## Advanced Features

### Text Manipulation

```cpp
// Set text programmatically
textInput->setText("Hello, World!");

// Get current text
std::string currentText = textInput->getText();

// Clear text
textInput->clear();

// Update placeholder
textInput->setPlaceholder("New placeholder...");
```

### Validation Example

```cpp
textInput->onTextChanged.connect([](const std::string& text) {
    // Email validation
    if (text.find('@') == std::string::npos && !text.empty()) {
        // Show error state
    } else {
        // Show normal state
    }
});
```

### Character Limits

```cpp
// Set maximum length
auto textInput = TextInput(
    TextInputConfig(100, 100, 300, 40)
        .maxLength(20)  // Maximum 20 characters
        .placeholder("Max 20 chars")
);

// Check length in handler
textInput->onTextChanged.connect([](const std::string& text) {
    if (text.length() >= 20) {
        // Handle max length reached
    }
});
```

## Examples

### Login Form

```cpp
void createLoginForm() {
    // Username input
    auto usernameInput = TextInput(
        TextInputConfig(100, 100, 300, 40)
            .placeholder("Username")
            .style(TextInputStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .focusBorderColor(Colors::Blue)
                .padding(10)
                .fontSize(16))
    );
    
    // Password input (conceptual - would need custom implementation)
    auto passwordInput = TextInput(
        TextInputConfig(100, 150, 300, 40)
            .placeholder("Password")
            .style(TextInputStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .focusBorderColor(Colors::Blue)
                .padding(10)
                .fontSize(16))
    );
    
    // Login button
    auto loginButton = Button(
        ButtonConfig(100, 200, 300, 40)
            .text("Login")
            .style(ButtonStyle()
                .backgroundColor(Colors::Blue)
                .textColor(Colors::White))
    );
    
    // Handle login
    loginButton->onClick.connect([=]() {
        std::string username = usernameInput->getText();
        std::string password = passwordInput->getText();
        
        if (!username.empty() && !password.empty()) {
            // Process login
        }
    });
}
```

### Search Bar

```cpp
void createSearchBar() {
    auto searchInput = TextInput(
        TextInputConfig(50, 20, 400, 35)
            .placeholder("Search...")
            .style(TextInputStyle()
                .backgroundColor(0xF8F8F8)
                .borderColor(0xDDDDDD)
                .focusBorderColor(0x4CAF50)
                .textColor(0x333333)
                .borderWidth(1)
                .padding(12))
    );
    
    // Real-time search
    searchInput->onTextChanged.connect([](const std::string& query) {
        if (query.length() >= 2) {
            // Perform search with debouncing
            performSearch(query);
        } else {
            // Clear search results
            clearSearchResults();
        }
    });
    
    // Search on Enter
    searchInput->onEnterPressed.connect([](const std::string& query) {
        if (!query.empty()) {
            performFullSearch(query);
        }
    });
}
```

### Multi-Input Form

```cpp
void createContactForm() {
    std::vector<std::shared_ptr<TextInputWidget>> inputs;
    
    // Create multiple inputs
    inputs.push_back(TextInput(
        TextInputConfig(100, 100, 300, 40)
            .placeholder("First Name")
    ));
    
    inputs.push_back(TextInput(
        TextInputConfig(100, 150, 300, 40)
            .placeholder("Last Name")
    ));
    
    inputs.push_back(TextInput(
        TextInputConfig(100, 200, 300, 40)
            .placeholder("Email")
    ));
    
    // Submit button
    auto submitButton = Button(
        ButtonConfig(100, 250, 300, 40)
            .text("Submit")
    );
    
    // Handle form submission
    submitButton->onClick.connect([=]() {
        std::vector<std::string> values;
        for (const auto& input : inputs) {
            values.push_back(input->getText());
        }
        
        // Validate and process form
        if (validateForm(values)) {
            submitForm(values);
        }
    });
}
```

## Best Practices

### 1. Responsive Design

```cpp
// Use relative positioning for different screen sizes
int screenWidth = Fern::getWidth();
int inputWidth = std::min(400, screenWidth - 100);

auto textInput = TextInput(
    TextInputConfig(50, 100, inputWidth, 40)
        .placeholder("Responsive input")
);
```

### 2. Accessibility

```cpp
// Provide clear placeholders
.placeholder("Enter your email address")

// Use appropriate font sizes
.fontSize(16)  // Readable size

// Provide visual feedback
.focusBorderColor(Colors::Blue)
```

### 3. Performance

```cpp
// Limit text length for performance
.maxLength(1000)

// Use efficient event handlers
textInput->onTextChanged.connect([](const std::string& text) {
    // Avoid heavy operations in real-time handlers
    if (text.length() > 3) {
        // Debounce or throttle expensive operations
    }
});
```

### 4. User Experience

```cpp
// Clear visual states
TextInputStyle style;
style.backgroundColor(Colors::White)
     .borderColor(Colors::Gray)
     .focusBorderColor(Colors::Blue)  // Clear focus indication
     .padding(10);                    // Adequate padding

// Helpful placeholders
.placeholder("Enter email (e.g., user@example.com)")
```

### 5. Error Handling

```cpp
// Validate input
textInput->onTextChanged.connect([](const std::string& text) {
    if (!isValidEmail(text) && !text.empty()) {
        // Show error state
        // Could change border color to red
    }
});
```

## Troubleshooting

### Common Issues

1. **Text Input Not Receiving Focus**
   ```cpp
   // Ensure widget is properly added to manager
   addWidget(textInput);
   
   // Check if other widgets are blocking input
   ```

2. **Cursor Not Visible**
   ```cpp
   // Ensure cursor color contrasts with background
   style.cursorColor(Colors::Black)
        .backgroundColor(Colors::White);
   ```

3. **Text Overflow**
   ```cpp
   // Set appropriate width and max length
   TextInputConfig(100, 100, 300, 40)  // Wide enough
       .maxLength(50)                   // Reasonable limit
   ```

4. **Font Size Issues**
   ```cpp
   // For bitmap fonts, use scale factors
   style.useBitmapFont().fontSize(2);
   
   // For TTF fonts, use point sizes
   style.useTTFFont("Arial").fontSize(16);
   ```

### Performance Issues

1. **Slow Text Updates**
   ```cpp
   // Avoid heavy operations in onTextChanged
   textInput->onTextChanged.connect([](const std::string& text) {
       // Use debouncing for expensive operations
       static auto lastUpdate = std::chrono::steady_clock::now();
       auto now = std::chrono::steady_clock::now();
       
       if (now - lastUpdate > std::chrono::milliseconds(300)) {
           // Perform expensive operation
           lastUpdate = now;
       }
   });
   ```

2. **Memory Usage**
   ```cpp
   // Limit text length
   .maxLength(1000)
   
   // Clear unused text inputs
   textInput->clear();
   ```

## Related Documentation

- [Button Widget](button.md) - For form submission buttons
- [Text Widget](text.md) - For display text
- [Layout System](../layout/overview.md) - For positioning inputs
- [Input Handling](../input/input-handling.md) - For advanced input processing
- [Event System](../input/events.md) - For custom event handling

---

*This documentation covers the Fern Text Input Widget API. For more examples and advanced usage, see the examples directory.*
