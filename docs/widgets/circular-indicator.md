# Circular Indicator Widget

The Circular Indicator Widget displays progress or status information in a circular format. It's ideal for showing completion percentages, loading states, health bars, battery levels, or any metric that can be represented as a circular progress indicator.

## Overview

The Circular Indicator Widget consists of:
- **Background Circle**: The base circle showing the full range
- **Progress Arc**: The colored arc indicating current progress
- **Border**: Optional border around the indicator
- **Center Text**: Optional percentage or value display in the center

## Basic Usage

```cpp
#include <fern/fern.hpp>

// Create a basic circular indicator
auto indicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
    .value(75.0f)                   // 75% complete
    .range(0.0f, 100.0f), true);

// Update progress
indicator->setValue(85.0f);

// Connect to progress events
indicator->onValueChanged.connect([](float value) {
    std::cout << "Progress: " << value << "%" << std::endl;
});
```

## Configuration System

### CircularIndicatorConfig

The `CircularIndicatorConfig` class defines the indicator's properties:

```cpp
// Complete configuration example
auto customIndicator = CircularIndicator(CircularIndicatorConfig(150, 100, 60)
    .value(45.0f)                   // Initial progress
    .range(0.0f, 100.0f)           // Value range
    .style(CircularIndicatorStyle()
        .backgroundColor(Colors::DarkGray)
        .fillColor(Colors::Blue)
        .borderColor(Colors::Black)
        .borderWidth(2)
        .thickness(8)               // Arc thickness
        .showPercentage(true)       // Show percentage text
        .textColor(Colors::White)
        .fontSize(2)
        .clockwise(true)            // Progress direction
        .startAngle(-90.0f)         // Start from top
        .useBitmapFont()), true);
```

### CircularIndicatorStyle

The `CircularIndicatorStyle` class controls the visual appearance:

```cpp
CircularIndicatorStyle style;
style.backgroundColor(Colors::DarkGray)     // Background color
     .fillColor(Colors::Blue)               // Progress arc color
     .borderColor(Colors::Black)            // Border color
     .borderWidth(2)                        // Border thickness
     .thickness(8)                          // Arc thickness
     .showPercentage(true)                  // Show percentage text
     .textColor(Colors::White)              // Text color
     .fontSize(2)                           // Text size
     .clockwise(true)                       // Progress direction
     .startAngle(-90.0f)                    // Start angle (top)
     .useBitmapFont();                      // Use bitmap font
```

## Methods

### Progress Control

```cpp
// Set the current progress value
indicator->setValue(65.0f);

// Get the current progress value
float currentValue = indicator->getValue();

// Get progress as percentage (0-100)
float percentage = indicator->getPercentage();

// Change the value range
indicator->setRange(0.0f, 200.0f);
```

### Widget Interface

```cpp
// Position and size
indicator->setPosition(200, 150);
indicator->resize(120, 120);  // Diameter

// Get dimensions
int width = indicator->getWidth();
int height = indicator->getHeight();
```

## Events and Signals

### Progress Changes

```cpp
// Respond to progress changes
indicator->onValueChanged.connect([](float value) {
    updateProgressLabel(value);
});
```

### Completion Events

```cpp
// Detect when progress reaches 100%
indicator->onComplete.connect([]() {
    std::cout << "Task completed!" << std::endl;
    showCompletionAnimation();
});
```

## Preset Configurations

Fern provides several preset configurations for common use cases:

### Default Circular Indicator

```cpp
auto indicator = CircularIndicator(CircularIndicatorPresets::Default(100, 100, 50), true);
```

### Loading Indicator

```cpp
auto loadingIndicator = CircularIndicator(CircularIndicatorPresets::Loading(200, 150, 40), true);
// Blue theme with percentage display, suitable for loading operations
```

### Health Indicator

```cpp
auto healthIndicator = CircularIndicator(CircularIndicatorPresets::Health(150, 100, 30), true);
// Green theme without percentage, suitable for health/status displays
```

### Battery Indicator

```cpp
auto batteryIndicator = CircularIndicator(CircularIndicatorPresets::Battery(250, 200, 25), true);
// Compact design with percentage, suitable for battery levels
```

## Interactive Examples

### Loading Progress

```cpp
void createLoadingIndicator() {
    auto loadingIndicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Blue)
            .borderColor(Colors::Gray)
            .borderWidth(1)
            .thickness(8)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(2)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    // Simulate loading progress
    loadingIndicator->onValueChanged.connect([](float progress) {
        std::cout << "Loading: " << progress << "%" << std::endl;
    });
    
    loadingIndicator->onComplete.connect([]() {
        std::cout << "Loading completed!" << std::endl;
    });
    
    // Simulate progress updates
    // loadingIndicator->setValue(25.0f);
    // loadingIndicator->setValue(50.0f);
    // loadingIndicator->setValue(100.0f);
}
```

### Health/Status Display

```cpp
void createHealthIndicator() {
    auto healthIndicator = CircularIndicator(CircularIndicatorConfig(150, 100, 40)
        .value(80.0f)               // 80% health
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkRed)
            .fillColor(Colors::Green)
            .borderColor(Colors::Black)
            .borderWidth(2)
            .thickness(6)
            .showPercentage(false)  // No percentage for health
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    // Health change logic
    healthIndicator->onValueChanged.connect([](float health) {
        if (health < 25.0f) {
            std::cout << "Health critical!" << std::endl;
        } else if (health < 50.0f) {
            std::cout << "Health low!" << std::endl;
        } else {
            std::cout << "Health: " << health << "%" << std::endl;
        }
    });
}
```

### Battery Level Display

```cpp
void createBatteryIndicator() {
    auto batteryIndicator = CircularIndicator(CircularIndicatorConfig(250, 200, 30)
        .value(65.0f)               // 65% battery
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::Black)
            .fillColor(Colors::Green)
            .borderColor(Colors::White)
            .borderWidth(1)
            .thickness(4)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    // Battery level monitoring
    batteryIndicator->onValueChanged.connect([](float level) {
        if (level < 20.0f) {
            std::cout << "Battery low: " << level << "%" << std::endl;
        } else if (level < 50.0f) {
            std::cout << "Battery medium: " << level << "%" << std::endl;
        } else {
            std::cout << "Battery: " << level << "%" << std::endl;
        }
    });
}
```

### Download Progress

```cpp
void createDownloadIndicator() {
    auto downloadIndicator = CircularIndicator(CircularIndicatorConfig(100, 300, 45)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::LightGray)
            .fillColor(Colors::Cyan)
            .borderColor(Colors::DarkGray)
            .borderWidth(2)
            .thickness(6)
            .showPercentage(true)
            .textColor(Colors::Black)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    downloadIndicator->onValueChanged.connect([](float progress) {
        std::cout << "Download: " << progress << "%" << std::endl;
    });
    
    downloadIndicator->onComplete.connect([]() {
        std::cout << "Download completed!" << std::endl;
    });
}
```

### Skill/Experience Progress

```cpp
void createSkillIndicator() {
    auto skillIndicator = CircularIndicator(CircularIndicatorConfig(300, 300, 50)
        .value(75.0f)               // 75% to next level
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(0xFF4A4A4A)   // Dark gray
            .fillColor(0xFFFFD700)         // Gold
            .borderColor(0xFF8B4513)       // Brown border
            .borderWidth(2)
            .thickness(8)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(2)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    skillIndicator->onValueChanged.connect([](float xp) {
        std::cout << "Experience: " << xp << "%" << std::endl;
    });
    
    skillIndicator->onComplete.connect([]() {
        std::cout << "Level up!" << std::endl;
    });
}
```

## Advanced Usage

### Custom Styling

```cpp
void createCustomIndicator() {
    auto customIndicator = CircularIndicator(CircularIndicatorConfig(200, 400, 60)
        .value(40.0f)
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(0xFF2C3E50)   // Dark blue-gray
            .fillColor(0xFF3498DB)         // Bright blue
            .borderColor(0xFF34495E)       // Darker border
            .borderWidth(3)
            .thickness(10)                 // Thick arc
            .showPercentage(true)
            .textColor(0xFFFFFFFF)         // White text
            .fontSize(3)                   // Large text
            .clockwise(true)
            .startAngle(-90.0f)            // Start from top
            .useBitmapFont()), true);
    
    customIndicator->onValueChanged.connect([](float value) {
        std::cout << "Custom indicator: " << value << "%" << std::endl;
    });
}
```

### Multiple Indicators Dashboard

```cpp
void createIndicatorDashboard() {
    // CPU usage
    auto cpuIndicator = CircularIndicator(CircularIndicatorConfig(100, 100, 30)
        .value(45.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Blue)
            .thickness(6)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Memory usage
    auto memoryIndicator = CircularIndicator(CircularIndicatorConfig(200, 100, 30)
        .value(67.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Orange)
            .thickness(6)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Disk usage
    auto diskIndicator = CircularIndicator(CircularIndicatorConfig(300, 100, 30)
        .value(32.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Green)
            .thickness(6)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Network usage
    auto networkIndicator = CircularIndicator(CircularIndicatorConfig(400, 100, 30)
        .value(78.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Purple)
            .thickness(6)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Update system info
    cpuIndicator->onValueChanged.connect([](float cpu) {
        std::cout << "CPU: " << cpu << "%" << std::endl;
    });
    
    memoryIndicator->onValueChanged.connect([](float memory) {
        std::cout << "Memory: " << memory << "%" << std::endl;
    });
    
    diskIndicator->onValueChanged.connect([](float disk) {
        std::cout << "Disk: " << disk << "%" << std::endl;
    });
    
    networkIndicator->onValueChanged.connect([](float network) {
        std::cout << "Network: " << network << "%" << std::endl;
    });
}
```

### Counter-clockwise Progress

```cpp
void createCounterClockwiseIndicator() {
    auto indicator = CircularIndicator(CircularIndicatorConfig(150, 250, 40)
        .value(60.0f)
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Red)
            .thickness(8)
            .showPercentage(true)
            .textColor(Colors::White)
            .clockwise(false)           // Counter-clockwise
            .startAngle(-90.0f)), true);
    
    indicator->onValueChanged.connect([](float value) {
        std::cout << "Counter-clockwise: " << value << "%" << std::endl;
    });
}
```

## Best Practices

### 1. Appropriate Sizes

Choose indicator sizes based on content and context:

```cpp
// Small indicators for status displays
auto statusIndicator = CircularIndicator(CircularIndicatorConfig(x, y, 20), true);

// Medium indicators for progress displays
auto progressIndicator = CircularIndicator(CircularIndicatorConfig(x, y, 40), true);

// Large indicators for prominent displays
auto mainIndicator = CircularIndicator(CircularIndicatorConfig(x, y, 80), true);
```

### 2. Color Conventions

Use conventional colors for different types of indicators:

```cpp
// Health/Success - Green
.fillColor(Colors::Green)

// Warning/Caution - Yellow/Orange
.fillColor(Colors::Orange)

// Error/Critical - Red
.fillColor(Colors::Red)

// Information/Progress - Blue
.fillColor(Colors::Blue)
```

### 3. Appropriate Thickness

Choose arc thickness based on indicator size:

```cpp
// Small indicators (radius < 30)
.thickness(4)

// Medium indicators (radius 30-60)
.thickness(6)

// Large indicators (radius > 60)
.thickness(8)
```

### 4. Text Display

Show percentage text when appropriate:

```cpp
// Show percentage for progress indicators
.showPercentage(true)

// Hide percentage for status indicators
.showPercentage(false)
```

## Performance Tips

- Update indicators only when values actually change
- Use appropriate precision for your use case
- Consider using onComplete for cleanup operations
- Avoid too frequent updates for better performance

## Integration with Other Widgets

Circular indicators work well with other widgets:

```cpp
// Indicator with progress bar
auto progressBar = ProgressBar(ProgressBarConfig(x, y, 200, 20));
auto circularIndicator = CircularIndicator(CircularIndicatorConfig(x, y, 40));

// Sync both indicators
auto updateProgress = [=](float value) {
    progressBar->setValue(value);
    circularIndicator->setValue(value);
};

// Indicator with status text
auto statusText = Text(TextConfig(x, y, "Initializing..."));
circularIndicator->onValueChanged.connect([=](float value) {
    if (value < 25.0f) {
        statusText->setText("Starting...");
    } else if (value < 75.0f) {
        statusText->setText("Processing...");
    } else {
        statusText->setText("Finishing...");
    }
});
```

## Animation and Effects

While the current implementation doesn't include built-in animations, you can create smooth progress updates:

```cpp
// Smooth progress animation
void animateProgress(CircularIndicatorWidget* indicator, float targetValue) {
    float currentValue = indicator->getValue();
    float step = (targetValue - currentValue) / 10.0f;
    
    // This would be implemented with a timer in a real application
    // for (int i = 0; i < 10; ++i) {
    //     currentValue += step;
    //     indicator->setValue(currentValue);
    //     // Wait for animation frame
    // }
}
```

## See Also

- [Progress Bar Widget](progress-bar.md) - For linear progress displays
- [Slider Widget](slider.md) - For interactive value selection
- [Circle Widget](circle.md) - For basic circular shapes
- [Styling Guide](../graphics/styling.md) - For advanced styling options
