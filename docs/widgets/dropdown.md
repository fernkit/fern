# Dropdown Widget

The Dropdown Widget provides a compact way to select one option from a list of items. It displays a collapsed view by default and expands to show all available options when activated. This widget is ideal for forms, settings panels, and any interface where space is limited but multiple options need to be available.

## Overview

The Dropdown Widget consists of:
- **Main Button**: The collapsed view showing the current selection
- **Dropdown Arrow**: Visual indicator that the widget can be expanded
- **Dropdown List**: The expanded view showing all available options
- **Items**: Individual selectable options in the dropdown

## Basic Usage

```cpp
#include <fern/fern.hpp>

// Create a basic dropdown
auto dropdown = Dropdown(DropdownConfig(100, 100, 200, 35)
    .placeholder("Select an option...")
    .items({
        {"Option 1", "value1"},
        {"Option 2", "value2"},
        {"Option 3", "value3"}
    })
    .selectedIndex(0), true);

// Connect to selection changes
dropdown->onSelectionChanged.connect([](int index) {
    std::cout << "Selected index: " << index << std::endl;
});

dropdown->onItemSelected.connect([](const DropdownItem& item) {
    std::cout << "Selected: " << item.text << " (value: " << item.value << ")" << std::endl;
});
```

## Configuration System

### DropdownConfig

The `DropdownConfig` class defines the dropdown's properties:

```cpp
// Complete configuration example
auto customDropdown = Dropdown(DropdownConfig(150, 200, 250, 40)
    .placeholder("Choose language...")
    .items({
        {"English", "en"},
        {"Spanish", "es"},
        {"French", "fr"},
        {"German", "de"}
    })
    .selectedIndex(0)               // Default selection
    .style(DropdownStyle()
        .backgroundColor(Colors::White)
        .borderColor(Colors::Gray)
        .selectedColor(Colors::Blue)
        .textColor(Colors::Black)
        .hoverColor(Colors::LightGray)
        .dropdownBackgroundColor(Colors::White)
        .borderWidth(1)
        .padding(8)
        .maxVisibleItems(5)
        .fontSize(2)
        .useBitmapFont()), true);
```

### DropdownStyle

The `DropdownStyle` class controls the visual appearance:

```cpp
DropdownStyle style;
style.backgroundColor(Colors::White)            // Main button background
     .borderColor(Colors::Gray)                 // Border color
     .selectedColor(Colors::Blue)               // Selected item highlight
     .textColor(Colors::Black)                  // Text color
     .hoverColor(Colors::LightGray)             // Hover background
     .dropdownBackgroundColor(Colors::White)    // Dropdown list background
     .borderWidth(1)                            // Border thickness
     .padding(8)                                // Internal padding
     .maxVisibleItems(5)                        // Max items before scrolling
     .fontSize(2)                               // Text size
     .useBitmapFont();                          // Use bitmap font
```

### DropdownItem

Items in the dropdown can have both display text and values:

```cpp
// Create items with text and values
std::vector<DropdownItem> items = {
    {"Small", "s"},
    {"Medium", "m"},
    {"Large", "l"},
    {"Extra Large", "xl"}
};

// Or create items with just text (value defaults to text)
std::vector<DropdownItem> simpleItems = {
    {"Red"},
    {"Green"},
    {"Blue"}
};
```

## Methods

### Selection Control

```cpp
// Set the selected index
dropdown->setSelectedIndex(2);

// Get the selected index
int selectedIndex = dropdown->getSelectedIndex();

// Get the selected item
const DropdownItem* selectedItem = dropdown->getSelectedItem();
if (selectedItem) {
    std::cout << "Selected: " << selectedItem->text << std::endl;
}
```

### Item Management

```cpp
// Add a single item
dropdown->addItem("New Option", "new_value");

// Set all items at once
dropdown->setItems({
    {"Option A", "a"},
    {"Option B", "b"},
    {"Option C", "c"}
});

// Clear all items
dropdown->clearItems();

// Get all items
const auto& items = dropdown->getItems();
```

### State Management

```cpp
// Open the dropdown programmatically
dropdown->open();

// Close the dropdown
dropdown->close();

// Check if the dropdown is open
bool isOpen = dropdown->isOpen();
```

### Widget Interface

```cpp
// Position and size
dropdown->setPosition(200, 150);
dropdown->resize(300, 40);

// Get dimensions
int width = dropdown->getWidth();
int height = dropdown->getHeight();
```

## Events and Signals

### Selection Changes

```cpp
// Respond to selection changes (by index)
dropdown->onSelectionChanged.connect([](int index) {
    std::cout << "Selection changed to index: " << index << std::endl;
});

// Respond to item selection (with item data)
dropdown->onItemSelected.connect([](const DropdownItem& item) {
    std::cout << "Item selected: " << item.text << " (value: " << item.value << ")" << std::endl;
});
```

### Open/Close State

```cpp
// Track dropdown open/close state
dropdown->onOpenStateChanged.connect([](bool isOpen) {
    if (isOpen) {
        std::cout << "Dropdown opened" << std::endl;
    } else {
        std::cout << "Dropdown closed" << std::endl;
    }
});
```

## Preset Configurations

Fern provides several preset configurations for common use cases:

### Default Dropdown

```cpp
auto dropdown = Dropdown(DropdownPresets::Default(100, 100), true);
```

### Modern Dropdown

```cpp
auto modernDropdown = Dropdown(DropdownPresets::Modern(100, 150, 250, 40), true);
// Modern styling with larger size
```

### Compact Dropdown

```cpp
auto compactDropdown = Dropdown(DropdownPresets::Compact(100, 200, 150, 25), true);
// Smaller size, suitable for tight spaces
```

## Interactive Examples

### Language Selection

```cpp
void createLanguageDropdown() {
    auto languageDropdown = Dropdown(DropdownConfig(100, 100, 200, 35)
        .placeholder("Select language...")
        .items({
            {"English", "en"},
            {"Spanish", "es"},
            {"French", "fr"},
            {"German", "de"},
            {"Italian", "it"},
            {"Japanese", "ja"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)
            .hoverColor(Colors::LightGray)
            .maxVisibleItems(4)), true);
    
    languageDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Language changed to: " << item.text << " (" << item.value << ")" << std::endl;
    });
}
```

### Settings Panel

```cpp
void createSettingsDropdowns() {
    // Graphics quality dropdown
    auto qualityDropdown = Dropdown(DropdownConfig(100, 150, 180, 30)
        .placeholder("Graphics Quality")
        .items({
            {"Low", "low"},
            {"Medium", "medium"},
            {"High", "high"},
            {"Ultra", "ultra"}
        })
        .selectedIndex(1)  // Default to Medium
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Resolution dropdown
    auto resolutionDropdown = Dropdown(DropdownConfig(100, 200, 180, 30)
        .placeholder("Resolution")
        .items({
            {"1280x720", "720p"},
            {"1920x1080", "1080p"},
            {"2560x1440", "1440p"},
            {"3840x2160", "4k"}
        })
        .selectedIndex(1)  // Default to 1080p
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Audio output dropdown
    auto audioDropdown = Dropdown(DropdownConfig(100, 250, 180, 30)
        .placeholder("Audio Output")
        .items({
            {"Speakers", "speakers"},
            {"Headphones", "headphones"},
            {"USB Audio", "usb"},
            {"Bluetooth", "bluetooth"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)), true);
    
    // Handle setting changes
    qualityDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Graphics quality set to: " << item.text << std::endl;
    });
    
    resolutionDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Resolution set to: " << item.text << std::endl;
    });
    
    audioDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Audio output set to: " << item.text << std::endl;
    });
}
```

### Form with Multiple Dropdowns

```cpp
void createFormDropdowns() {
    // Country dropdown
    auto countryDropdown = Dropdown(DropdownConfig(300, 100, 200, 35)
        .placeholder("Select country...")
        .items({
            {"United States", "us"},
            {"Canada", "ca"},
            {"United Kingdom", "uk"},
            {"France", "fr"},
            {"Germany", "de"},
            {"Japan", "jp"},
            {"Australia", "au"}
        })
        .selectedIndex(-1)  // No default selection
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)
            .maxVisibleItems(5)), true);
    
    // State/Province dropdown (initially empty)
    auto stateDropdown = Dropdown(DropdownConfig(300, 150, 200, 35)
        .placeholder("Select state...")
        .items({})  // Empty initially
        .selectedIndex(-1)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)), true);
    
    // Update states based on country selection
    countryDropdown->onItemSelected.connect([=](const DropdownItem& country) {
        std::cout << "Country selected: " << country.text << std::endl;
        
        // Update state dropdown based on country
        if (country.value == "us") {
            stateDropdown->setItems({
                {"California", "ca"},
                {"New York", "ny"},
                {"Texas", "tx"},
                {"Florida", "fl"}
            });
        } else if (country.value == "ca") {
            stateDropdown->setItems({
                {"Ontario", "on"},
                {"Quebec", "qc"},
                {"British Columbia", "bc"},
                {"Alberta", "ab"}
            });
        } else {
            stateDropdown->clearItems();
        }
        
        stateDropdown->setSelectedIndex(-1);  // Reset selection
    });
    
    stateDropdown->onItemSelected.connect([](const DropdownItem& state) {
        std::cout << "State selected: " << state.text << std::endl;
    });
}
```

### Product Configuration

```cpp
void createProductConfiguration() {
    // Size dropdown
    auto sizeDropdown = Dropdown(DropdownConfig(100, 320, 150, 30)
        .placeholder("Size")
        .items({
            {"Small", "s"},
            {"Medium", "m"},
            {"Large", "l"},
            {"Extra Large", "xl"}
        })
        .selectedIndex(1)  // Default to Medium
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Purple)
            .textColor(Colors::Black)), true);
    
    // Color dropdown
    auto colorDropdown = Dropdown(DropdownConfig(270, 320, 150, 30)
        .placeholder("Color")
        .items({
            {"Red", "red"},
            {"Blue", "blue"},
            {"Green", "green"},
            {"Black", "black"},
            {"White", "white"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Purple)
            .textColor(Colors::Black)), true);
    
    // Material dropdown
    auto materialDropdown = Dropdown(DropdownConfig(440, 320, 150, 30)
        .placeholder("Material")
        .items({
            {"Cotton", "cotton"},
            {"Polyester", "polyester"},
            {"Wool", "wool"},
            {"Silk", "silk"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Purple)
            .textColor(Colors::Black)), true);
    
    // Update product configuration
    auto updateConfiguration = [=]() {
        const auto* size = sizeDropdown->getSelectedItem();
        const auto* color = colorDropdown->getSelectedItem();
        const auto* material = materialDropdown->getSelectedItem();
        
        if (size && color && material) {
            std::cout << "Product: " << size->text << " " << color->text << " " << material->text << std::endl;
        }
    };
    
    sizeDropdown->onItemSelected.connect([=](const DropdownItem&) { updateConfiguration(); });
    colorDropdown->onItemSelected.connect([=](const DropdownItem&) { updateConfiguration(); });
    materialDropdown->onItemSelected.connect([=](const DropdownItem&) { updateConfiguration(); });
}
```

## Advanced Usage

### Custom Styling

```cpp
void createCustomStyledDropdown() {
    auto customDropdown = Dropdown(DropdownConfig(100, 400, 250, 35)
        .placeholder("Custom Styled Dropdown")
        .items({
            {"Option Alpha", "alpha"},
            {"Option Beta", "beta"},
            {"Option Gamma", "gamma"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(0xFFF8F9FA)       // Light gray background
            .borderColor(0xFF6C757D)           // Gray border
            .selectedColor(0xFF007BFF)         // Blue selection
            .textColor(0xFF212529)             // Dark text
            .hoverColor(0xFFE9ECEF)            // Light hover
            .dropdownBackgroundColor(0xFFFFFFFF) // White dropdown
            .borderWidth(2)
            .padding(10)
            .maxVisibleItems(3)
            .fontSize(2)
            .useBitmapFont()), true);
    
    customDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Custom dropdown: " << item.text << " selected" << std::endl;
    });
}
```

### Dynamic Item Management

```cpp
void createDynamicDropdown() {
    auto dynamicDropdown = Dropdown(DropdownConfig(400, 400, 200, 35)
        .placeholder("Dynamic Options")
        .items({{"Initial Option", "initial"}})
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Orange)
            .textColor(Colors::Black)), true);
    
    // Simulate adding items dynamically
    dynamicDropdown->addItem("Option 1", "opt1");
    dynamicDropdown->addItem("Option 2", "opt2");
    dynamicDropdown->addItem("Option 3", "opt3");
    
    dynamicDropdown->onItemSelected.connect([](const DropdownItem& item) {
        std::cout << "Dynamic selection: " << item.text << std::endl;
    });
}
```

## Best Practices

### 1. Meaningful Placeholders

Use descriptive placeholders that clearly indicate what the dropdown is for:

```cpp
// Good - Clear purpose
.placeholder("Select payment method...")
.placeholder("Choose file format...")

// Avoid - Generic placeholders
.placeholder("Select option...")
.placeholder("Choose...")
```

### 2. Appropriate Item Limits

Consider using `maxVisibleItems` for long lists:

```cpp
// For long lists, limit visible items
.maxVisibleItems(5)

// For short lists, show all items
.maxVisibleItems(10)  // or don't set if list is short
```

### 3. Logical Default Selections

Choose sensible default selections:

```cpp
// Good - Reasonable defaults
.selectedIndex(0)     // First option is most common
.selectedIndex(1)     // Medium/Normal option

// Consider - No default for required choices
.selectedIndex(-1)    // Force user to make a choice
```

### 4. Consistent Styling

Use consistent styling across related dropdowns:

```cpp
DropdownStyle formStyle;
formStyle.backgroundColor(Colors::White)
         .borderColor(Colors::Gray)
         .selectedColor(Colors::Blue)
         .textColor(Colors::Black);

// Use the same style for all form dropdowns
auto dropdown1 = Dropdown(DropdownConfig(x, y).style(formStyle), true);
auto dropdown2 = Dropdown(DropdownConfig(x, y).style(formStyle), true);
```

## Performance Tips

- Use reasonable `maxVisibleItems` values for long lists
- Handle selection events efficiently
- Consider using item values for internal logic instead of text
- Cache dropdown references when managing multiple dropdowns

## Integration with Other Widgets

Dropdowns work well with other widgets:

```cpp
// Dropdown with apply button
auto applyButton = Button(ButtonConfig(x, y, "Apply"));
applyButton->onClick.connect([=]() {
    const auto* selectedItem = dropdown->getSelectedItem();
    if (selectedItem) {
        applySettings(selectedItem->value);
    }
});

// Dropdown with descriptive text
auto descriptionText = Text(TextConfig(x, y, "Select an option"));
dropdown->onItemSelected.connect([=](const DropdownItem& item) {
    descriptionText->setText("Selected: " + item.text);
});
```

## See Also

- [Radio Button Widget](radio-button.md) - For mutually exclusive options
- [Button Widget](button.md) - For single-action controls
- [Text Widget](text.md) - For dropdown labels
- [Input Handling](../input/input-handling.md) - For advanced input processing
