# Radio Button Widget

The Radio Button Widget provides a control for selecting one option from a set of mutually exclusive choices. Radio buttons are grouped together, and only one button in each group can be selected at a time.

## Overview

The Radio Button Widget consists of:
- **Radio Circle**: The circular indicator showing selection state
- **Selection Indicator**: The inner circle shown when selected
- **Text Label**: The descriptive text next to the radio button
- **Group Management**: Automatic mutual exclusion within groups

## Basic Usage

```cpp
#include <fern/fern.hpp>

// Create radio buttons in a group
auto option1 = RadioButton(RadioButtonConfig(100, 100, "Option 1", "group1"), true);
auto option2 = RadioButton(RadioButtonConfig(100, 130, "Option 2", "group1"), true);
auto option3 = RadioButton(RadioButtonConfig(100, 160, "Option 3", "group1"), true);

// Set initial selection
option1->setSelected(true);

// Connect to selection events
option1->onSelectionChanged.connect([](bool selected) {
    if (selected) {
        std::cout << "Option 1 selected" << std::endl;
    }
});
```

## Configuration System

### RadioButtonConfig

The `RadioButtonConfig` class defines the radio button's properties:

```cpp
// Complete configuration example
auto customRadio = RadioButton(RadioButtonConfig(150, 200, "Custom Option", "settings")
    .selected(false)                // Initial selection state
    .style(RadioButtonStyle()
        .backgroundColor(Colors::White)
        .borderColor(Colors::Gray)
        .selectedColor(Colors::Blue)
        .textColor(Colors::Black)
        .hoverColor(Colors::LightGray)
        .borderWidth(2)
        .radius(10)
        .spacing(12)
        .fontSize(2)
        .useBitmapFont()), true);
```

### RadioButtonStyle

The `RadioButtonStyle` class controls the visual appearance:

```cpp
RadioButtonStyle style;
style.backgroundColor(Colors::White)        // Background color
     .borderColor(Colors::Gray)             // Border color
     .selectedColor(Colors::Blue)           // Selection indicator color
     .textColor(Colors::Black)              // Text color
     .hoverColor(Colors::LightGray)         // Hover background color
     .borderWidth(1)                        // Border thickness
     .radius(8)                             // Radio button radius
     .spacing(8)                            // Space between button and text
     .fontSize(2)                           // Text size
     .useBitmapFont();                      // Use bitmap font
```

## Group Management

### RadioButtonGroup

Radio buttons are automatically grouped by their group name, but you can also manage groups explicitly:

```cpp
// Create a group
auto group = RadioGroup();

// Add buttons to the group
group->addButton(option1);
group->addButton(option2);
group->addButton(option3);

// Select a button programmatically
group->selectButton(option2);

// Get the currently selected button
auto selected = group->getSelected();
if (selected) {
    std::cout << "Selected: " << selected->getText() << std::endl;
}
```

## Methods

### Selection Control

```cpp
// Set selection state
radioButton->setSelected(true);

// Get selection state
bool isSelected = radioButton->isSelected();

// Change the text
radioButton->setText("New Option Text");

// Get the text
std::string text = radioButton->getText();

// Get the group name
std::string groupName = radioButton->getGroupName();
```

### Widget Interface

```cpp
// Position and size
radioButton->setPosition(200, 150);
radioButton->resize(200, 25);

// Get dimensions
int width = radioButton->getWidth();
int height = radioButton->getHeight();
```

## Events and Signals

### Selection Changes

```cpp
// Respond to selection changes
radioButton->onSelectionChanged.connect([](bool selected) {
    if (selected) {
        std::cout << "Radio button selected" << std::endl;
    } else {
        std::cout << "Radio button deselected" << std::endl;
    }
});
```

### Selection Events

```cpp
// Respond only when this button is selected
radioButton->onSelected.connect([]() {
    std::cout << "This specific button was selected" << std::endl;
});
```

## Preset Configurations

Fern provides several preset configurations for common use cases:

### Default Radio Button

```cpp
auto radioButton = RadioButton(RadioButtonPresets::Default(100, 100, "Option", "group"), true);
```

### Modern Radio Button

```cpp
auto modernRadio = RadioButton(RadioButtonPresets::Modern(100, 130, "Modern Option", "group"), true);
// Blue theme with modern styling
```

### Compact Radio Button

```cpp
auto compactRadio = RadioButton(RadioButtonPresets::Compact(100, 160, "Compact", "group"), true);
// Smaller size, suitable for tight spaces
```

## Interactive Examples

### Settings Panel

```cpp
void createSettingsPanel() {
    // Graphics quality options
    auto lowQuality = RadioButton(RadioButtonConfig(100, 100, "Low Quality", "graphics")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    auto mediumQuality = RadioButton(RadioButtonConfig(100, 130, "Medium Quality", "graphics")
        .selected(true)  // Default selection
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    auto highQuality = RadioButton(RadioButtonConfig(100, 160, "High Quality", "graphics")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Handle selection changes
    lowQuality->onSelected.connect([]() {
        std::cout << "Graphics set to Low Quality" << std::endl;
    });
    
    mediumQuality->onSelected.connect([]() {
        std::cout << "Graphics set to Medium Quality" << std::endl;
    });
    
    highQuality->onSelected.connect([]() {
        std::cout << "Graphics set to High Quality" << std::endl;
    });
}
```

### User Preferences

```cpp
void createUserPreferences() {
    // Theme selection
    auto lightTheme = RadioButton(RadioButtonConfig(100, 200, "Light Theme", "theme")
        .selected(true)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    auto darkTheme = RadioButton(RadioButtonConfig(100, 230, "Dark Theme", "theme")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Language selection
    auto english = RadioButton(RadioButtonConfig(300, 200, "English", "language")
        .selected(true)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    auto spanish = RadioButton(RadioButtonConfig(300, 230, "Español", "language")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    auto french = RadioButton(RadioButtonConfig(300, 260, "Français", "language")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    // Theme change handling
    lightTheme->onSelected.connect([]() {
        std::cout << "Theme changed to Light" << std::endl;
    });
    
    darkTheme->onSelected.connect([]() {
        std::cout << "Theme changed to Dark" << std::endl;
    });
    
    // Language change handling
    english->onSelected.connect([]() {
        std::cout << "Language changed to English" << std::endl;
    });
    
    spanish->onSelected.connect([]() {
        std::cout << "Language changed to Spanish" << std::endl;
    });
    
    french->onSelected.connect([]() {
        std::cout << "Language changed to French" << std::endl;
    });
}
```

### Survey Form

```cpp
void createSurveyForm() {
    // Satisfaction rating
    auto verySatisfied = RadioButton(RadioButtonConfig(100, 300, "Very Satisfied", "satisfaction")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    auto satisfied = RadioButton(RadioButtonConfig(100, 330, "Satisfied", "satisfaction")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    auto neutral = RadioButton(RadioButtonConfig(100, 360, "Neutral", "satisfaction")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Yellow)
            .textColor(Colors::Black)), true);
    
    auto dissatisfied = RadioButton(RadioButtonConfig(100, 390, "Dissatisfied", "satisfaction")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Red)
            .textColor(Colors::Black)), true);
    
    // Handle survey responses
    verySatisfied->onSelected.connect([]() {
        std::cout << "Survey response: Very Satisfied" << std::endl;
    });
    
    satisfied->onSelected.connect([]() {
        std::cout << "Survey response: Satisfied" << std::endl;
    });
    
    neutral->onSelected.connect([]() {
        std::cout << "Survey response: Neutral" << std::endl;
    });
    
    dissatisfied->onSelected.connect([]() {
        std::cout << "Survey response: Dissatisfied" << std::endl;
    });
}
```

### Game Options

```cpp
void createGameOptions() {
    // Difficulty selection
    auto easy = RadioButton(RadioButtonConfig(400, 100, "Easy", "difficulty")
        .selected(true)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto normal = RadioButton(RadioButtonConfig(400, 130, "Normal", "difficulty")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Yellow)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto hard = RadioButton(RadioButtonConfig(400, 160, "Hard", "difficulty")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Red)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    // Game mode selection
    auto singlePlayer = RadioButton(RadioButtonConfig(400, 200, "Single Player", "mode")
        .selected(true)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    auto multiPlayer = RadioButton(RadioButtonConfig(400, 230, "Multiplayer", "mode")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Handle game option changes
    easy->onSelected.connect([]() {
        std::cout << "Difficulty set to Easy" << std::endl;
    });
    
    normal->onSelected.connect([]() {
        std::cout << "Difficulty set to Normal" << std::endl;
    });
    
    hard->onSelected.connect([]() {
        std::cout << "Difficulty set to Hard" << std::endl;
    });
    
    singlePlayer->onSelected.connect([]() {
        std::cout << "Game mode: Single Player" << std::endl;
    });
    
    multiPlayer->onSelected.connect([]() {
        std::cout << "Game mode: Multiplayer" << std::endl;
    });
}
```

## Advanced Usage

### Custom Styling

```cpp
void createCustomStyledRadios() {
    auto customRadio = RadioButton(RadioButtonConfig(100, 450, "Custom Style", "custom")
        .selected(false)
        .style(RadioButtonStyle()
            .backgroundColor(0xFFF8F9FA)   // Light gray background
            .borderColor(0xFF6C757D)       // Gray border
            .selectedColor(0xFF007BFF)     // Blue selection
            .textColor(0xFF212529)         // Dark text
            .hoverColor(0xFFE9ECEF)        // Light hover
            .borderWidth(2)
            .radius(12)                    // Larger radius
            .spacing(15)                   // More spacing
            .fontSize(2)                   // Larger text
            .useBitmapFont()), true);
    
    customRadio->onSelected.connect([]() {
        std::cout << "Custom styled radio selected" << std::endl;
    });
}
```

### Dynamic Radio Button Groups

```cpp
void createDynamicRadioGroups() {
    std::vector<std::string> options = {"Option A", "Option B", "Option C", "Option D"};
    std::vector<std::shared_ptr<RadioButtonWidget>> radioButtons;
    
    for (int i = 0; i < options.size(); ++i) {
        auto radio = RadioButton(RadioButtonConfig(500, 300 + i * 30, options[i], "dynamic")
            .selected(i == 0)  // First option selected by default
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::Purple)
                .textColor(Colors::Black)), true);
        
        radio->onSelected.connect([i, options]() {
            std::cout << "Dynamic option selected: " << options[i] << std::endl;
        });
        
        radioButtons.push_back(radio);
    }
}
```

## Best Practices

### 1. Meaningful Groups

Use descriptive group names:

```cpp
// Good - Clear group purposes
auto option1 = RadioButton(RadioButtonConfig(x, y, "Option 1", "graphics_quality"), true);
auto option2 = RadioButton(RadioButtonConfig(x, y, "Option 2", "audio_format"), true);

// Avoid - Generic group names
auto option3 = RadioButton(RadioButtonConfig(x, y, "Option 3", "group1"), true);
```

### 2. Clear Labels

Use descriptive and concise labels:

```cpp
// Good - Clear and descriptive
auto highQuality = RadioButton(RadioButtonConfig(x, y, "High Quality", "graphics"), true);
auto mediumQuality = RadioButton(RadioButtonConfig(x, y, "Medium Quality", "graphics"), true);

// Avoid - Unclear labels
auto option1 = RadioButton(RadioButtonConfig(x, y, "Option 1", "graphics"), true);
```

### 3. Logical Default Selection

Always have a reasonable default selection:

```cpp
// Good - Sensible default
auto normalDifficulty = RadioButton(RadioButtonConfig(x, y, "Normal", "difficulty")
    .selected(true), true);  // Normal is a good default
```

### 4. Consistent Styling

Use consistent styling within related groups:

```cpp
RadioButtonStyle settingsStyle;
settingsStyle.backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black);

// Use the same style for all settings options
auto option1 = RadioButton(RadioButtonConfig(x, y, "Option 1", "group")
    .style(settingsStyle), true);
auto option2 = RadioButton(RadioButtonConfig(x, y, "Option 2", "group")
    .style(settingsStyle), true);
```

## Performance Tips

- Use group names efficiently to avoid unnecessary group management
- Handle selection events only when needed
- Consider using onSelected instead of onSelectionChanged for better performance
- Cache radio button references when managing multiple groups

## Integration with Other Widgets

Radio buttons work well with other widgets:

```cpp
// Radio buttons with apply button
auto applyButton = Button(ButtonConfig(x, y, "Apply"));
applyButton->onClick.connect([=]() {
    // Apply the selected radio button settings
    if (highQuality->isSelected()) {
        applyHighQualitySettings();
    } else if (mediumQuality->isSelected()) {
        applyMediumQualitySettings();
    }
});

// Radio buttons with descriptive text
auto descriptionText = Text(TextConfig(x, y, "Select quality level"));
highQuality->onSelected.connect([=]() {
    descriptionText->setText("High quality: Best visuals, slower performance");
});
```

## See Also

- [Button Widget](button.md) - For single-action controls
- [Text Widget](text.md) - For radio button labels
- [Dropdown Widget](dropdown.md) - For single selection from many options
- [Input Handling](../input/input-handling.md) - For advanced input processing
