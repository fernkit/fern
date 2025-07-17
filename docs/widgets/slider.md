# Slider Widget Guide

Sliders represent one of the most intuitive ways humans interact with continuous values - from the volume knob on a radio to the brightness control on your screen. In this guide, you'll learn how to create responsive, visually appealing sliders in Fern while understanding the complex systems behind translating mouse movements into precise numerical values.

## Understanding Slider Interactions

Creating an effective slider involves orchestrating several sophisticated systems:

1. **Value Mapping**: Converting screen coordinates to numerical ranges and back
2. **Input Handling**: Detecting mouse clicks, drags, and hover states
3. **Visual Feedback**: Providing immediate, clear indication of current value and interaction state  
4. **Constraint Management**: Ensuring values stay within defined bounds
5. **Smooth Interpolation**: Making value changes feel natural and responsive

Fern handles these complexities while exposing a clean, intuitive API that makes slider creation straightforward.

## Slider Philosophy in Fern

Fern's sliders embody the principle of **direct manipulation**:

- **Visual Correspondence**: The visual position directly represents the numerical value
- **Immediate Feedback**: Changes are reflected instantly in both appearance and value
- **Precise Control**: Users can achieve exact values through careful positioning
- **Range Flexibility**: Any numerical range can be mapped to the slider track
- **Consistent Behavior**: Predictable interaction patterns across all slider instances

This approach ensures sliders feel natural and responsive while maintaining the educational transparency that makes Fern special.

## Your First Slider

Let's start with the simplest possible slider:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Create a basic slider for volume control (0-100)
    auto volumeSlider = Slider(SliderConfig(100, 100, 200, 20)
        .range(0.0f, 100.0f)
        .initialValue(50.0f));
    
    // Handle value changes
    volumeSlider->onValueChanged.connect([](float value) {
        std::cout << "Volume: " << (int)value << "%" << std::endl;
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
- `SliderConfig(100, 100, 200, 20)`: Position (100,100) with size 200x20 pixels
- `.range(0.0f, 100.0f)`: Values from 0 to 100 (typical percentage range)
- `.initialValue(50.0f)`: Starts at 50% (middle position)
- `onValueChanged`: Signal emitted whenever the user changes the slider value

Click and drag the slider thumb - you'll see immediate visual feedback and console output showing the current value.

## Understanding Slider Components

A Fern slider consists of several visual elements working together:

### The Track
The background bar that represents the full range of possible values:

```cpp
// The track shows the entire value range
SliderStyle style;
style.trackColor(Colors::DarkGray);  // Unfilled portion

auto slider = Slider(SliderConfig(100, 100, 250, 25)
    .range(0.0f, 10.0f)
    .style(style));
```

### The Fill
The colored portion showing the current value relative to the minimum:

```cpp
// Fill color indicates "how much" of the range is selected
SliderStyle style;
style.trackColor(Colors::DarkGray)
     .fillColor(Colors::Blue);  // Filled portion

auto progressSlider = Slider(SliderConfig(100, 150, 250, 25)
    .range(0.0f, 100.0f)
    .initialValue(75.0f)  // 75% filled
    .style(style));
```

### The Thumb
The draggable control element that users interact with:

```cpp
// Thumb appearance affects usability
SliderStyle style;
style.thumbColor(Colors::White)
     .thumbHoverColor(Colors::LightGray)  // Visual feedback on hover
     .thumbRadius(12);  // Larger thumb = easier to grab

auto precisionSlider = Slider(SliderConfig(100, 200, 300, 30)
    .range(-1.0f, 1.0f)  // Fine control range
    .initialValue(0.0f)
    .style(style));
```

### Value Display
Optional text showing the current numerical value:

```cpp
// Show actual numbers to users
SliderStyle style;
style.showValue(true)
     .textColor(Colors::White)
     .textScale(2);

auto temperatureSlider = Slider(SliderConfig(100, 250, 200, 25)
    .range(15.0f, 30.0f)  // Celsius range
    .initialValue(22.0f)
    .style(style));
```

## Value Mapping: The Math Behind Sliders

Understanding how screen coordinates map to values helps with advanced slider usage:

### Linear Interpolation
```cpp
// How Fern converts mouse position to value:
float screenToValue(int mouseX, int sliderX, int sliderWidth, float minVal, float maxVal) {
    // Clamp mouse position to slider bounds
    int relativeX = std::clamp(mouseX - sliderX, 0, sliderWidth);
    
    // Convert to 0-1 range
    float percentage = (float)relativeX / sliderWidth;
    
    // Map to value range
    return minVal + percentage * (maxVal - minVal);
}

// Example: Mouse at X=150 on slider from X=100, width=200, range 0-100
// relativeX = 150 - 100 = 50
// percentage = 50 / 200 = 0.25
// value = 0 + 0.25 * (100 - 0) = 25
```

### Precision and Granularity
```cpp
// Fine-grained control for precise values
auto precisionSlider = Slider(SliderConfig(100, 100, 400, 25)  // Wider = more precise
    .range(0.0f, 1.0f)
    .initialValue(0.5f));

// Coarse control for large ranges  
auto coarseSlider = Slider(SliderConfig(100, 150, 200, 25)     // Narrower = less precise
    .range(0.0f, 10000.0f)
    .initialValue(5000.0f));
```

## Advanced Slider Styling

Fern provides comprehensive styling options for creating professional-looking sliders:

### Color Schemes
```cpp
// Dark theme slider
SliderStyle darkStyle;
darkStyle.trackColor(0xFF333333)        // Dark gray track
         .fillColor(0xFF0080FF)          // Blue fill
         .thumbColor(0xFFFFFFFF)         // White thumb
         .thumbHoverColor(0xFFE0E0E0)    // Light gray on hover
         .textColor(0xFFFFFFFF);         // White text

// Light theme slider
SliderStyle lightStyle;
lightStyle.trackColor(0xFFE0E0E0)       // Light gray track
          .fillColor(0xFF007ACC)         // Darker blue fill
          .thumbColor(0xFF333333)        // Dark thumb
          .thumbHoverColor(0xFF666666)   // Medium gray on hover
          .textColor(0xFF333333);        // Dark text
```

### Size Variations
```cpp
// Compact slider for tight spaces
auto compactSlider = Slider(SliderConfig(100, 100, 150, 15)
    .style(SliderStyle().thumbRadius(6).textScale(1)));

// Large slider for easy interaction
auto largeSlider = Slider(SliderConfig(100, 150, 300, 35)
    .style(SliderStyle().thumbRadius(15).textScale(3)));
```

### Custom Thumb Styling
```cpp
// High-contrast thumb for accessibility
SliderStyle accessibleStyle;
accessibleStyle.thumbColor(Colors::Yellow)
               .thumbHoverColor(Colors::Orange)
               .thumbRadius(20)  // Large, easy to see and click
               .fillColor(Colors::Green)
               .trackColor(Colors::DarkGray);

auto accessibleSlider = Slider(SliderConfig(100, 200, 250, 40)
    .style(accessibleStyle));
```

## Slider Events and Interactivity

Sliders communicate through Fern's signal system:

### Value Change Events
```cpp
auto slider = Slider(SliderConfig(100, 100, 200, 25)
    .range(0.0f, 100.0f));

// Triggered every time the value changes
slider->onValueChanged.connect([](float value) {
    std::cout << "New value: " << value << std::endl;
    
    // Update other UI elements based on slider value
    updateSystemVolume(value);
    updateVolumeDisplay(value);
});
```

### Drag State Events
```cpp
// Know when user starts/stops dragging
slider->onDragging.connect([](bool isDragging) {
    if (isDragging) {
        std::cout << "User started dragging" << std::endl;
        // Maybe show more detailed feedback
    } else {
        std::cout << "User finished dragging" << std::endl;
        // Maybe save the final value
    }
});
```

### Real-Time vs Final Value Updates
```cpp
class VolumeControl {
public:
    VolumeControl() {
        slider_ = Slider(SliderConfig(100, 100, 200, 25)
            .range(0.0f, 100.0f)
            .initialValue(50.0f));
        
        // Update display in real-time
        slider_->onValueChanged.connect([this](float value) {
            updateDisplay(value);
        });
        
        // Apply settings only when user finishes
        slider_->onDragging.connect([this](bool dragging) {
            if (!dragging) {
                float finalValue = slider_->getValue();
                applyVolumeSettings(finalValue);
                saveUserPreference(finalValue);
            }
        });
    }
    
private:
    std::shared_ptr<SliderWidget> slider_;
    
    void updateDisplay(float value) {
        std::cout << "Volume: " << (int)value << "%" << std::endl;
    }
    
    void applyVolumeSettings(float value) {
        // Actually change system volume
    }
    
    void saveUserPreference(float value) {
        // Save to config file
    }
};
```

## Slider Presets and Common Patterns

Fern provides convenient presets for typical use cases:

### Volume Control
```cpp
auto volumeSlider = Slider(SliderPresets::Volume(100, 100));
// Pre-configured: 0-100 range, appropriate sizing, volume-style colors
```

### Brightness Control
```cpp
auto brightnessSlider = Slider(SliderPresets::Brightness(100, 150));
// Pre-configured: 0-100 range, brightness-appropriate colors
```

### Color Component Sliders
```cpp
// RGB color picker using three sliders
auto redSlider = Slider(SliderPresets::ColorComponent(100, 100)
    .range(0.0f, 255.0f)
    .style(SliderStyle().fillColor(Colors::Red)));

auto greenSlider = Slider(SliderPresets::ColorComponent(100, 140)
    .range(0.0f, 255.0f)
    .style(SliderStyle().fillColor(Colors::Green)));

auto blueSlider = Slider(SliderPresets::ColorComponent(100, 180)
    .range(0.0f, 255.0f)
    .style(SliderStyle().fillColor(Colors::Blue)));

// Update color preview when any slider changes
auto updateColor = [=]() {
    uint8_t r = (uint8_t)redSlider->getValue();
    uint8_t g = (uint8_t)greenSlider->getValue();
    uint8_t b = (uint8_t)blueSlider->getValue();
    uint32_t color = (r << 16) | (g << 8) | b;
    updateColorPreview(color);
};

redSlider->onValueChanged.connect([updateColor](float) { updateColor(); });
greenSlider->onValueChanged.connect([updateColor](float) { updateColor(); });
blueSlider->onValueChanged.connect([updateColor](float) { updateColor(); });
```

## Advanced Slider Techniques

### Logarithmic Scaling
```cpp
// For values that span multiple orders of magnitude (like frequency)
class LogarithmicSlider {
public:
    LogarithmicSlider(SliderConfig config, float minValue, float maxValue) 
        : minLog_(log10(minValue)), maxLog_(log10(maxValue)) {
        
        slider_ = Slider(config.range(0.0f, 1.0f));  // Linear 0-1 internally
        
        slider_->onValueChanged.connect([this](float linearValue) {
            // Convert linear 0-1 to logarithmic scale
            float logValue = minLog_ + linearValue * (maxLog_ - minLog_);
            float actualValue = pow(10, logValue);
            
            std::cout << "Frequency: " << actualValue << " Hz" << std::endl;
        });
    }
    
private:
    std::shared_ptr<SliderWidget> slider_;
    float minLog_, maxLog_;
};

// Usage: Frequency slider from 20Hz to 20kHz
LogarithmicSlider frequencySlider(
    SliderConfig(100, 100, 300, 25),
    20.0f,      // 20 Hz
    20000.0f    // 20 kHz
);
```

### Stepped/Discrete Values
```cpp
// Slider that snaps to specific values
class SteppedSlider {
public:
    SteppedSlider(SliderConfig config, std::vector<float> steps) 
        : steps_(steps) {
        
        slider_ = Slider(config.range(0.0f, (float)(steps.size() - 1)));
        
        slider_->onValueChanged.connect([this](float value) {
            int index = (int)round(value);
            index = std::clamp(index, 0, (int)steps_.size() - 1);
            
            float actualValue = steps_[index];
            std::cout << "Selected: " << actualValue << std::endl;
        });
    }
    
private:
    std::shared_ptr<SliderWidget> slider_;
    std::vector<float> steps_;
};

// Usage: Preset zoom levels
SteppedSlider zoomSlider(
    SliderConfig(100, 100, 200, 25),
    {0.25f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f}
);
```

### Bidirectional Sliders
```cpp
// Slider with center point (like balance controls)
class BalanceSlider {
public:
    BalanceSlider(SliderConfig config) {
        slider_ = Slider(config.range(-100.0f, 100.0f).initialValue(0.0f));
        
        // Custom styling for center-based slider
        SliderStyle style;
        style.trackColor(Colors::DarkGray)
             .fillColor(Colors::Transparent)  // No fill, we'll show balance differently
             .thumbColor(Colors::White);
        
        slider_->onValueChanged.connect([](float value) {
            if (value < 0) {
                std::cout << "Left bias: " << abs(value) << "%" << std::endl;
            } else if (value > 0) {
                std::cout << "Right bias: " << value << "%" << std::endl;
            } else {
                std::cout << "Centered" << std::endl;
            }
        });
    }
    
private:
    std::shared_ptr<SliderWidget> slider_;
};
```

## Slider Performance Considerations

### Update Frequency
```cpp
// High-frequency updates can impact performance
auto slider = Slider(SliderConfig(100, 100, 200, 25));

// Throttle updates for expensive operations
class ThrottledUpdater {
public:
    ThrottledUpdater(int maxUpdatesPerSecond = 30) 
        : interval_(1000 / maxUpdatesPerSecond), lastUpdate_(0) {}
    
    void update(float value) {
        uint32_t now = getCurrentTime();
        if (now - lastUpdate_ >= interval_) {
            performExpensiveOperation(value);
            lastUpdate_ = now;
        }
    }
    
private:
    uint32_t interval_;
    uint32_t lastUpdate_;
    
    void performExpensiveOperation(float value) {
        // Expensive graphics update, file I/O, etc.
    }
};

ThrottledUpdater updater(30);  // Max 30 updates per second
slider->onValueChanged.connect([&updater](float value) {
    updater.update(value);
});
```

### Memory Management
```cpp
// Sliders are automatically managed
{
    auto tempSlider = Slider(SliderConfig(100, 100, 200, 25));
    // Automatically cleaned up when scope ends
}

// For collections of sliders
class SliderPanel {
public:
    void addSlider(const SliderConfig& config) {
        sliders_.push_back(Slider(config));
    }
    
    void clearSliders() {
        sliders_.clear();  // Automatic cleanup
    }
    
private:
    std::vector<std::shared_ptr<SliderWidget>> sliders_;
};
```

## Troubleshooting Common Issues

### Slider Not Responding

**Check these common problems:**

1. **Slider outside clickable area**:
```cpp
// Ensure slider is positioned within screen bounds
int screenWidth = getScreenWidth();
int screenHeight = getScreenHeight();

auto slider = Slider(SliderConfig(50, 50, 200, 25));  // Safe position
```

2. **No event handlers connected**:
```cpp
auto slider = Slider(SliderConfig(100, 100, 200, 25));

// Make sure to connect to signals
slider->onValueChanged.connect([](float value) {
    // This will actually run when slider changes
});
```

3. **Range issues**:
```cpp
// Avoid invalid ranges
auto badSlider = Slider(SliderConfig(100, 100, 200, 25)
    .range(100.0f, 0.0f));  // BAD: max < min

auto goodSlider = Slider(SliderConfig(100, 100, 200, 25)
    .range(0.0f, 100.0f));  // GOOD: min < max
```

### Visual Issues

**Thumb not visible:**
```cpp
// Ensure thumb color contrasts with track
SliderStyle style;
style.trackColor(Colors::DarkGray)
     .thumbColor(Colors::White);  // High contrast
```

**Values not updating:**
```cpp
// Check if range is appropriate for your use case
auto slider = Slider(SliderConfig(100, 100, 200, 25)
    .range(0.0f, 1.0f));  // Small range needs careful handling

slider->onValueChanged.connect([](float value) {
    // Use appropriate precision for display
    std::cout << std::fixed << std::setprecision(3) << value << std::endl;
});
```

### Performance Issues

**Slider updates too frequent:**
```cpp
// Use throttling for expensive operations
auto slider = Slider(SliderConfig(100, 100, 200, 25));

// Instead of immediate updates
slider->onValueChanged.connect([](float value) {
    expensiveOperation(value);  // BAD: called constantly while dragging
});

// Use drag end for final updates
slider->onDragging.connect([slider](bool dragging) {
    if (!dragging) {
        expensiveOperation(slider->getValue());  // GOOD: called once when done
    }
});
```

## Summary

Sliders in Fern provide an intuitive, powerful way to handle continuous value input while maintaining the transparency and educational value that makes Fern special. Whether you're building audio controls, settings panels, or data visualization tools, understanding value mapping, event handling, and styling options will help you create responsive, professional interfaces.

Key takeaways:
- **Direct manipulation**: Visual position directly corresponds to numerical value
- **Flexible ranges**: Any numerical range can be mapped to slider dimensions
- **Rich styling**: Comprehensive appearance customization for any design aesthetic
- **Event-driven**: Clean signal-based communication for value changes and interaction states
- **Performance aware**: Consider update frequency and expensive operations
- **User-friendly**: Proper sizing, contrast, and feedback improve usability

Sliders exemplify Fern's philosophy: sophisticated functionality exposed through clean APIs, with full visibility into the underlying systems. Master sliders, and you'll have powerful tools for creating intuitive, responsive interfaces that users can control with precision and confidence.

