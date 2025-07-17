# Radio Button Guide

Radio buttons represent one of the most fundamental concepts in human decision-making: mutually exclusive choice. Named after the mechanical push buttons on car radios where pressing one button would release all others, radio buttons embody the principle that sometimes we must choose exactly one option from a set of possibilities.

This guide explores not just how to create radio buttons in Fern, but the cognitive psychology of choice presentation, the design principles that make radio button groups immediately understandable, and the subtle interface patterns that guide users toward confident decisions.

## Understanding Choice Architecture

```cpp
// Radio buttons make these exclusive relationships visually clear
auto genderGroup = RadioGroup();

auto maleOption = RadioButton(RadioButtonConfig(100, 100, "Male", "gender"), true);
auto femaleOption = RadioButton(RadioButtonConfig(100, 130, "Female", "gender"), true);
auto nonBinaryOption = RadioButton(RadioButtonConfig(100, 160, "Non-binary", "gender"), true);

// The group ensures only one can be selected
genderGroup->addButton(maleOption);
genderGroup->addButton(femaleOption);
genderGroup->addButton(nonBinaryOption);
```

### Cognitive Load and Decision Making

Radio buttons reduce cognitive load by:

- **Constraining Options**: Users know exactly how many choices they have
- **Showing Current State**: The selected option is always visible
- **Preventing Errors**: Impossible to have invalid state (no selection or multiple selections)
- **Enabling Comparison**: All options are visible simultaneously for easy comparison

## Radio Button Philosophy in Fern

Fern's radio buttons follow several design principles:

- **Automatic Grouping**: Radio buttons automatically manage mutual exclusion
- **Visual Clarity**: Selection state is immediately obvious
- **Consistent Interaction**: Click anywhere (button or label) to select
- **Group Coordination**: Groups handle all inter-button communication
- **Event-Driven**: Rich event system for reactive programming
- **Flexible Styling**: Appearance adapts to different design contexts

## Your First Radio Button Group

Let's start with a simple example that demonstrates the core concepts:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    setupFern();
    
    // Create a radio button group for theme selection
    auto themeGroup = RadioGroup();
    
    // Create individual radio buttons
    auto lightTheme = RadioButton(RadioButtonConfig(100, 100, "Light Theme", "theme")
        .selected(true)  // Default selection
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto darkTheme = RadioButton(RadioButtonConfig(100, 140, "Dark Theme", "theme")
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto autoTheme = RadioButton(RadioButtonConfig(100, 180, "Auto Theme", "theme")
        .style(RadioButtonStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    // Add buttons to group for mutual exclusion
    themeGroup->addButton(lightTheme);
    themeGroup->addButton(darkTheme);
    themeGroup->addButton(autoTheme);
    
    // React to selection changes
    themeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selected) {
        std::cout << "Theme changed to: " << selected->getText() << std::endl;
        
        // Apply the selected theme
        if (selected->getText() == "Light Theme") {
            applyLightTheme();
        } else if (selected->getText() == "Dark Theme") {
            applyDarkTheme();
        } else {
            applyAutoTheme();
        }
    });
    
    while (shouldKeepRunning()) {
        processInput();
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `RadioButtonConfig(x, y, text, groupName)`: Creates button at position with label and group identifier
- `.selected(true)`: Sets the initial selection (only one should be true per group)
- `RadioButtonStyle()`: Controls visual appearance (colors, sizing, spacing)
- `RadioGroup()`: Manages mutual exclusion between buttons
- `themeGroup->addButton()`: Adds buttons to the group for coordination
- `onSelectionChanged`: Event fired when selection changes, providing the newly selected button

This creates an intuitive interface where users can see all theme options and select exactly one.

## Understanding the Configuration System

### RadioButtonConfig: Individual Button Setup

Each radio button is configured independently but coordinates with its group:

```cpp
// Basic configuration
RadioButtonConfig config(x, y, "Button Text", "group_name");

// Full configuration with all options
auto advancedConfig = RadioButtonConfig(150, 200, "Advanced Option", "settings")
    .selected(false)                    // Initial selection state
    .style(RadioButtonStyle()           // Visual styling
        .backgroundColor(Colors::White)
        .borderColor(Colors::Gray)
        .selectedColor(Colors::Green)
        .textColor(Colors::Black)
        .hoverColor(Colors::LightGray)
        .radius(10)                     // Circle radius in pixels
        .borderWidth(2)                 // Border thickness
        .spacing(12)                    // Space between circle and text
        .fontSize(2)                    // Text size
        .useBitmapFont());              // Font type

// Different group names create separate radio groups
auto option1 = RadioButtonConfig(100, 100, "Option 1", "group_a");  // Group A
auto option2 = RadioButtonConfig(100, 130, "Option 2", "group_a");  // Group A
auto option3 = RadioButtonConfig(300, 100, "Choice X", "group_b");  // Group B
auto option4 = RadioButtonConfig(300, 130, "Choice Y", "group_b");  // Group B
```

### RadioButtonStyle: Visual Customization

The style system provides complete control over appearance:

```cpp
RadioButtonStyle style;
style.backgroundColor(Colors::White)        // Circle background color
     .borderColor(Colors::DarkGray)         // Circle border color
     .selectedColor(Colors::Blue)           // Inner dot color when selected
     .textColor(Colors::Black)              // Label text color
     .hoverColor(Colors::LightBlue)         // Hover state color
     .radius(8)                             // Circle radius (affects overall size)
     .borderWidth(2)                        // Border thickness around circle
     .spacing(10)                           // Pixels between circle and text
     .fontSize(2)                           // Text size multiplier
     .useBitmapFont();                      // Use bitmap fonts (or useTTFFont())

// For high-quality text with TTF fonts:
style.useTTFFont("arial")                  // Specify TTF font
     .fontSize(16);                        // TTF font size (minimum 16)
```

**Understanding Style Properties:**

- **backgroundColor**: The main circle color (usually light for contrast)
- **borderColor**: Outline around the circle (provides definition)
- **selectedColor**: The inner dot that appears when selected
- **hoverColor**: Feedback color when mouse hovers over button
- **radius**: Controls overall button size (larger = easier to click)
- **spacing**: Visual separation between circle and text (affects layout)

### RadioButtonGroup: Coordination and Management

Groups handle the mutual exclusion logic automatically:

```cpp
// Create a group for managing related radio buttons
auto priorityGroup = RadioGroup();

// Create buttons with same group name
auto lowPriority = RadioButton(RadioButtonConfig(100, 100, "Low Priority", "priority"), true);
auto mediumPriority = RadioButton(RadioButtonConfig(100, 130, "Medium Priority", "priority"), true);
auto highPriority = RadioButton(RadioButtonConfig(100, 160, "High Priority", "priority"), true);

// Add to group for coordination
priorityGroup->addButton(lowPriority);
priorityGroup->addButton(mediumPriority);
priorityGroup->addButton(highPriority);

// Set initial selection
priorityGroup->selectButton(mediumPriority);  // Automatically deselects others

// React to group-level changes
priorityGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selected) {
    std::cout << "Priority set to: " << selected->getText() << std::endl;
    updateTaskPriority(selected->getText());
});

// Query current selection
auto currentSelection = priorityGroup->getSelected();
if (currentSelection) {
    std::cout << "Currently selected: " << currentSelection->getText() << std::endl;
}
```
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
