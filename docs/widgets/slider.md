# Slider Widget

The Slider Widget provides an interactive control for selecting values from a continuous range. It features a draggable thumb on a track, with customizable styling and optional value display.

## Overview

The Slider Widget consists of:
- **Track**: The background bar showing the full range
- **Fill**: The colored portion indicating the current value
- **Thumb**: The draggable control element
- **Value Display**: Optional text showing the current value

## Basic Usage

```cpp
#include <fern/fern.hpp>

// Create a basic slider
auto slider = Slider(SliderConfig(100, 100, 200, 20)
    .range(0.0f, 100.0f)
    .initialValue(50.0f), true);

// Connect to value changes
slider->onValueChanged.connect([](float value) {
    std::cout << "Slider value: " << value << std::endl;
});
```

## Configuration System

### SliderConfig

The `SliderConfig` class defines the slider's properties:

```cpp
// Complete configuration example
auto customSlider = Slider(SliderConfig(150, 200, 300, 25)
    .range(0.0f, 1.0f)           // Value range
    .initialValue(0.5f)          // Starting value
    .style(SliderStyle()
        .trackColor(Colors::DarkGray)
        .fillColor(Colors::Blue)
        .thumbColor(Colors::White)
        .thumbHoverColor(Colors::LightGray)
        .thumbRadius(12)
        .showValue(true)
        .textColor(Colors::White)
        .textScale(2)), true);
```

### SliderStyle

The `SliderStyle` class controls the visual appearance:

```cpp
SliderStyle style;
style.trackColor(Colors::Gray)          // Track background color
     .fillColor(Colors::Blue)           // Fill color (progress)
     .thumbColor(Colors::White)         // Thumb color
     .thumbHoverColor(Colors::LightGray) // Thumb hover color
     .thumbRadius(10)                   // Thumb size
     .showValue(true)                   // Show value text
     .textColor(Colors::Black)          // Text color
     .textScale(2);                     // Text size
```

## Methods

### Value Control

```cpp
// Set the current value
slider->setValue(75.0f);

// Get the current value
float currentValue = slider->getValue();

// Change the value range
slider->setRange(0.0f, 255.0f);
```

### Widget Interface

```cpp
// Position and size
slider->setPosition(200, 150);
slider->resize(350, 30);

// Get dimensions
int width = slider->getWidth();
int height = slider->getHeight();
```

## Events and Signals

### Value Changes

```cpp
// Respond to value changes
slider->onValueChanged.connect([](float value) {
    updateVolumeLevel(value);
});
```

### Drag State

```cpp
// Track dragging state
slider->onDragging.connect([](bool isDragging) {
    if (isDragging) {
        showTooltip("Adjusting value...");
    } else {
        hideTooltip();
    }
});
```

## Preset Configurations

Fern provides several preset configurations for common use cases:

### Default Slider

```cpp
auto slider = Slider(SliderPresets::Default(100, 100), true);
```

### Volume Control

```cpp
auto volumeSlider = Slider(SliderPresets::Volume(50, 100, 250, 25), true);
// Range: 0-100, themed for audio controls
```

### Brightness Control

```cpp
auto brightnessSlider = Slider(SliderPresets::Brightness(50, 150, 250, 25), true);
// Range: 0-100, themed for brightness controls
```

### Color Component

```cpp
auto redSlider = Slider(SliderPresets::ColorComponent(50, 200, 200, 20), true);
// Range: 0-255, themed for color values
```

## Interactive Examples

### Volume Control System

```cpp
void createVolumeControl() {
    auto volumeSlider = Slider(SliderConfig(100, 100, 250, 25)
        .range(0.0f, 100.0f)
        .initialValue(50.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Green)
            .thumbColor(Colors::White)
            .showValue(true)
            .textColor(Colors::White)), true);
    
    // Update audio system
    volumeSlider->onValueChanged.connect([](float volume) {
        // setAudioVolume(volume / 100.0f);
        std::cout << "Volume: " << volume << "%" << std::endl;
    });
    
    // Visual feedback during adjustment
    volumeSlider->onDragging.connect([](bool dragging) {
        if (dragging) {
            std::cout << "Adjusting volume..." << std::endl;
        }
    });
}
```

### Color Picker Component

```cpp
void createColorPicker() {
    // RGB sliders
    auto redSlider = Slider(SliderConfig(100, 100, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Red)
            .thumbColor(Colors::White)), true);
    
    auto greenSlider = Slider(SliderConfig(100, 130, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Green)
            .thumbColor(Colors::White)), true);
    
    auto blueSlider = Slider(SliderConfig(100, 160, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Blue)
            .thumbColor(Colors::White)), true);
    
    // Update color when any slider changes
    auto updateColor = [=]() {
        int r = static_cast<int>(redSlider->getValue());
        int g = static_cast<int>(greenSlider->getValue());
        int b = static_cast<int>(blueSlider->getValue());
        uint32_t color = (0xFF << 24) | (r << 16) | (g << 8) | b;
        
        std::cout << "Color: RGB(" << r << ", " << g << ", " << b << ")" << std::endl;
    };
    
    redSlider->onValueChanged.connect([=](float) { updateColor(); });
    greenSlider->onValueChanged.connect([=](float) { updateColor(); });
    blueSlider->onValueChanged.connect([=](float) { updateColor(); });
}
```

### Settings Panel

```cpp
void createSettingsPanel() {
    // Master volume
    auto masterVolume = Slider(SliderConfig(150, 100, 300, 25)
        .range(0.0f, 100.0f)
        .initialValue(75.0f)
        .style(SliderStyle()
            .trackColor(Colors::Gray)
            .fillColor(Colors::Blue)
            .showValue(true)), true);
    
    // Effects volume
    auto effectsVolume = Slider(SliderConfig(150, 140, 300, 25)
        .range(0.0f, 100.0f)
        .initialValue(50.0f)
        .style(SliderStyle()
            .trackColor(Colors::Gray)
            .fillColor(Colors::Purple)
            .showValue(true)), true);
    
    // Music volume
    auto musicVolume = Slider(SliderConfig(150, 180, 300, 25)
        .range(0.0f, 100.0f)
        .initialValue(80.0f)
        .style(SliderStyle()
            .trackColor(Colors::Gray)
            .fillColor(Colors::Orange)
            .showValue(true)), true);
    
    // Connect to settings system
    masterVolume->onValueChanged.connect([](float value) {
        // updateMasterVolume(value);
    });
    
    effectsVolume->onValueChanged.connect([](float value) {
        // updateEffectsVolume(value);
    });
    
    musicVolume->onValueChanged.connect([](float value) {
        // updateMusicVolume(value);
    });
}
```

## Advanced Usage

### Custom Styling

```cpp
void createCustomSlider() {
    auto slider = Slider(SliderConfig(100, 100, 250, 30)
        .range(0.0f, 100.0f)
        .initialValue(25.0f)
        .style(SliderStyle()
            .trackColor(0xFF2C3E50)     // Dark blue-gray track
            .fillColor(0xFF3498DB)      // Bright blue fill
            .thumbColor(0xFFECF0F1)     // Light gray thumb
            .thumbHoverColor(0xFFBDC3C7) // Darker gray on hover
            .thumbRadius(15)            // Larger thumb
            .showValue(true)
            .textColor(0xFFFFFFFF)      // White text
            .textScale(2)), true);
    
    // Custom value formatting
    slider->onValueChanged.connect([](float value) {
        std::cout << "Custom value: " << std::fixed << std::setprecision(1) 
                  << value << "%" << std::endl;
    });
}
```

### Multi-Slider Control

```cpp
void createEqualizerSliders() {
    std::vector<std::shared_ptr<SliderWidget>> eqSliders;
    std::vector<std::string> frequencies = {"60Hz", "170Hz", "310Hz", "600Hz", "1kHz", "3kHz", "6kHz", "12kHz"};
    
    for (int i = 0; i < frequencies.size(); ++i) {
        auto slider = Slider(SliderConfig(100 + i * 40, 100, 30, 150)
            .range(-12.0f, 12.0f)    // dB range
            .initialValue(0.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Cyan)
                .thumbColor(Colors::White)
                .thumbRadius(8)
                .showValue(true)
                .textColor(Colors::White)
                .textScale(1)), true);
        
        slider->onValueChanged.connect([i, frequencies](float value) {
            std::cout << frequencies[i] << ": " << value << "dB" << std::endl;
        });
        
        eqSliders.push_back(slider);
    }
}
```

## Best Practices

### 1. Appropriate Ranges

Choose meaningful ranges for your sliders:

```cpp
// Good - Clear, meaningful ranges
auto volumeSlider = Slider(SliderConfig(x, y)
    .range(0.0f, 100.0f)    // Percentage
    .initialValue(50.0f));

auto temperatureSlider = Slider(SliderConfig(x, y)
    .range(-20.0f, 40.0f)   // Celsius
    .initialValue(20.0f));
```

### 2. Visual Feedback

Provide clear visual feedback:

```cpp
slider->onDragging.connect([](bool isDragging) {
    if (isDragging) {
        // Show tooltip or highlight
        showValueTooltip();
    } else {
        hideValueTooltip();
    }
});
```

### 3. Reasonable Thumb Sizes

Use appropriate thumb sizes for the slider dimensions:

```cpp
// For normal sliders
.thumbRadius(8)     // Standard size

// For precision controls
.thumbRadius(12)    // Larger for easier interaction

// For compact interfaces
.thumbRadius(6)     // Smaller for tight spaces
```

### 4. Consistent Styling

Maintain consistent styling across related sliders:

```cpp
SliderStyle audioStyle;
audioStyle.trackColor(Colors::DarkGray)
          .fillColor(Colors::Blue)
          .thumbColor(Colors::White)
          .showValue(true);

// Use the same style for all audio sliders
auto masterSlider = Slider(SliderConfig(x, y).style(audioStyle), true);
auto effectsSlider = Slider(SliderConfig(x, y).style(audioStyle), true);
```

## Performance Tips

- Use value change events sparingly for expensive operations
- Consider debouncing rapid value changes if needed
- Cache slider references when using multiple sliders
- Use appropriate precision for your use case

## Integration with Other Widgets

Sliders work well with other widgets:

```cpp
// Slider with text display
auto valueLabel = Text(TextConfig(x, y, "50%"));
slider->onValueChanged.connect([=](float value) {
    valueLabel->setText(std::to_string(static_cast<int>(value)) + "%");
});

// Slider with button controls
auto decreaseBtn = Button(ButtonConfig(x, y, "-"));
auto increaseBtn = Button(ButtonConfig(x, y, "+"));

decreaseBtn->onClick.connect([=]() {
    slider->setValue(slider->getValue() - 1.0f);
});

increaseBtn->onClick.connect([=]() {
    slider->setValue(slider->getValue() + 1.0f);
});
```

## See Also

- [Button Widget](button.md) - For discrete controls
- [Text Widget](text.md) - For displaying slider values
- [Progress Bar Widget](progress-bar.md) - For displaying progress
- [Input Handling](../input/input-handling.md) - For advanced input processing
