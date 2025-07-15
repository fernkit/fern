# Progress Bar Widget

The Progress Bar Widget provides a visual indicator of completion progress for tasks, downloads, loading operations, or any process that can be quantified as a percentage. It supports customizable styling, text display, and smooth animations.

## Overview

The Progress Bar Widget consists of:
- **Background**: The unfilled portion of the progress bar
- **Fill**: The colored portion indicating progress
- **Border**: Optional border around the progress bar
- **Text**: Optional percentage or custom text display

## Basic Usage

```cpp
#include <fern/fern.hpp>

// Create a basic progress bar
auto progressBar = ProgressBar(ProgressBarConfig(100, 100, 300, 20)
    .value(45.0f)                   // 45% complete
    .range(0.0f, 100.0f), true);

// Update progress
progressBar->setValue(75.0f);

// Connect to progress events
progressBar->onValueChanged.connect([](float value) {
    std::cout << "Progress: " << value << "%" << std::endl;
});
```

## Configuration System

### ProgressBarConfig

The `ProgressBarConfig` class defines the progress bar's properties:

```cpp
// Complete configuration example
auto customProgress = ProgressBar(ProgressBarConfig(100, 150, 400, 25)
    .value(60.0f)                   // Initial progress (0-100)
    .range(0.0f, 100.0f)           // Value range
    .style(ProgressBarStyle()
        .backgroundColor(Colors::DarkGray)
        .fillColor(Colors::Green)
        .borderColor(Colors::Black)
        .borderWidth(2)
        .showPercentage(true)       // Display percentage text
        .textColor(Colors::White)
        .fontSize(2)
        .useBitmapFont()), true);
```

### ProgressBarStyle

The `ProgressBarStyle` class controls the visual appearance:

```cpp
ProgressBarStyle style;
style.backgroundColor(Colors::LightGray)    // Background color
     .fillColor(Colors::Blue)               // Progress fill color
     .borderColor(Colors::Black)            // Border color
     .borderWidth(1)                        // Border thickness
     .showPercentage(true)                  // Show percentage text
     .textColor(Colors::White)              // Text color
     .fontSize(2)                           // Text size
     .useBitmapFont();                      // Use bitmap font
```

## Methods

### Progress Control

```cpp
// Set the current progress value
progressBar->setValue(65.0f);

// Get the current progress value
float currentValue = progressBar->getValue();

// Get progress as percentage (0-100)
float percentage = progressBar->getPercentage();

// Change the value range
progressBar->setRange(0.0f, 200.0f);
```

### Widget Interface

```cpp
// Position and size
progressBar->setPosition(200, 150);
progressBar->resize(450, 30);

// Get dimensions
int width = progressBar->getWidth();
int height = progressBar->getHeight();
```

## Events and Signals

### Progress Changes

```cpp
// Respond to progress changes
progressBar->onValueChanged.connect([](float value) {
    updateProgressLabel(value);
});
```

### Completion Events

```cpp
// Detect when progress reaches 100%
progressBar->onComplete.connect([]() {
    std::cout << "Task completed!" << std::endl;
    showCompletionMessage();
});
```

## Preset Configurations

Fern provides several preset configurations for common use cases:

### Default Progress Bar

```cpp
auto progressBar = ProgressBar(ProgressBarPresets::Default(100, 100), true);
```

### Loading Progress

```cpp
auto loadingBar = ProgressBar(ProgressBarPresets::Loading(50, 100, 250, 20), true);
// Blue theme, suitable for loading operations
```

### Health Bar

```cpp
auto healthBar = ProgressBar(ProgressBarPresets::Health(50, 150, 200, 15), true);
// Green theme, suitable for health/status displays
```

### Download Progress

```cpp
auto downloadBar = ProgressBar(ProgressBarPresets::Download(50, 200, 400, 30), true);
// Green theme with percentage display, suitable for downloads
```

## Interactive Examples

### File Download Progress

```cpp
void createDownloadProgress() {
    auto downloadBar = ProgressBar(ProgressBarConfig(100, 100, 400, 25)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::LightGray)
            .fillColor(Colors::Blue)
            .borderColor(Colors::DarkGray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(2)), true);
    
    // Simulate download progress
    downloadBar->onValueChanged.connect([](float progress) {
        std::cout << "Download: " << progress << "% complete" << std::endl;
    });
    
    downloadBar->onComplete.connect([]() {
        std::cout << "Download completed!" << std::endl;
    });
    
    // Simulate progress updates (in real app, this would be driven by actual download)
    // downloadBar->setValue(25.0f);  // 25% complete
    // downloadBar->setValue(50.0f);  // 50% complete
    // downloadBar->setValue(100.0f); // Complete - triggers onComplete
}
```

### Health/Status Display

```cpp
void createHealthBar() {
    auto healthBar = ProgressBar(ProgressBarConfig(100, 150, 200, 15)
        .value(80.0f)               // 80% health
        .range(0.0f, 100.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::DarkRed)
            .fillColor(Colors::Green)
            .borderColor(Colors::Black)
            .borderWidth(2)
            .showPercentage(false)  // No percentage for health
            .textColor(Colors::White)), true);
    
    // Health change logic
    healthBar->onValueChanged.connect([](float health) {
        if (health < 25.0f) {
            std::cout << "Health critical!" << std::endl;
        } else if (health < 50.0f) {
            std::cout << "Health low!" << std::endl;
        }
    });
}
```

### Multi-Step Process

```cpp
void createMultiStepProgress() {
    auto processBar = ProgressBar(ProgressBarConfig(100, 200, 350, 25)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::Gray)
            .fillColor(Colors::Purple)
            .borderColor(Colors::DarkGray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(2)), true);
    
    std::vector<std::string> steps = {
        "Initializing...",
        "Loading data...",
        "Processing...",
        "Saving results...",
        "Complete!"
    };
    
    int currentStep = 0;
    
    processBar->onValueChanged.connect([&](float progress) {
        int stepIndex = static_cast<int>(progress / 25.0f);
        if (stepIndex < steps.size() && stepIndex != currentStep) {
            currentStep = stepIndex;
            std::cout << steps[currentStep] << std::endl;
        }
    });
    
    processBar->onComplete.connect([&]() {
        std::cout << steps.back() << std::endl;
    });
}
```

### Battery Level Indicator

```cpp
void createBatteryIndicator() {
    auto batteryBar = ProgressBar(ProgressBarConfig(100, 250, 150, 20)
        .value(65.0f)               // 65% battery
        .range(0.0f, 100.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::Black)
            .fillColor(Colors::Green)
            .borderColor(Colors::White)
            .borderWidth(2)
            .showPercentage(true)
            .textColor(Colors::White)
            .fontSize(1)), true);
    
    // Battery level monitoring
    batteryBar->onValueChanged.connect([](float level) {
        if (level < 20.0f) {
            std::cout << "Battery low: " << level << "%" << std::endl;
        }
    });
    
    // Color changes based on battery level
    batteryBar->onValueChanged.connect([=](float level) {
        if (level < 20.0f) {
            // Change to red when low (would require style update method)
            std::cout << "Battery critically low!" << std::endl;
        } else if (level < 50.0f) {
            // Change to yellow when medium
            std::cout << "Battery level medium" << std::endl;
        }
    });
}
```

## Advanced Usage

### Custom Styling

```cpp
void createCustomStyledBar() {
    auto customBar = ProgressBar(ProgressBarConfig(100, 300, 300, 30)
        .value(40.0f)
        .range(0.0f, 100.0f)
        .style(ProgressBarStyle()
            .backgroundColor(0xFF2C3E50)   // Dark blue-gray
            .fillColor(0xFF3498DB)         // Bright blue
            .borderColor(0xFF34495E)       // Darker border
            .borderWidth(3)
            .showPercentage(true)
            .textColor(0xFFFFFFFF)         // White text
            .fontSize(3)                   // Large text
            .useBitmapFont()), true);
    
    customBar->onValueChanged.connect([](float value) {
        std::cout << "Custom progress: " << value << "%" << std::endl;
    });
}
```

### Multiple Progress Bars

```cpp
void createProgressDashboard() {
    // CPU usage
    auto cpuBar = ProgressBar(ProgressBarConfig(100, 100, 200, 15)
        .value(45.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Blue)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Memory usage
    auto memoryBar = ProgressBar(ProgressBarConfig(100, 130, 200, 15)
        .value(67.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Orange)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Disk usage
    auto diskBar = ProgressBar(ProgressBarConfig(100, 160, 200, 15)
        .value(32.0f)
        .style(ProgressBarStyle()
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Green)
            .showPercentage(true)
            .fontSize(1)), true);
    
    // Update system info
    cpuBar->onValueChanged.connect([](float cpu) {
        std::cout << "CPU: " << cpu << "%" << std::endl;
    });
    
    memoryBar->onValueChanged.connect([](float memory) {
        std::cout << "Memory: " << memory << "%" << std::endl;
    });
    
    diskBar->onValueChanged.connect([](float disk) {
        std::cout << "Disk: " << disk << "%" << std::endl;
    });
}
```

## Best Practices

### 1. Appropriate Ranges

Use meaningful ranges for your progress bars:

```cpp
// Good - Clear 0-100 percentage
auto progressBar = ProgressBar(ProgressBarConfig(x, y)
    .range(0.0f, 100.0f)
    .value(0.0f));

// Good - File size in bytes
auto downloadBar = ProgressBar(ProgressBarConfig(x, y)
    .range(0.0f, static_cast<float>(fileSize))
    .value(0.0f));
```

### 2. Visual Feedback

Provide clear progress indication:

```cpp
// Show percentage for long operations
.showPercentage(true)

// Use appropriate colors
.fillColor(Colors::Blue)        // For normal progress
.fillColor(Colors::Green)       // For success/health
.fillColor(Colors::Red)         // For critical/error states
```

### 3. Completion Handling

Always handle completion events:

```cpp
progressBar->onComplete.connect([]() {
    std::cout << "Operation completed!" << std::endl;
    // Hide progress bar, show completion message, etc.
});
```

### 4. Smooth Updates

Update progress smoothly for better user experience:

```cpp
// Update in reasonable increments
void updateProgress(float newValue) {
    progressBar->setValue(newValue);
    // Don't update too frequently (e.g., every frame)
}
```

## Performance Tips

- Update progress bars only when values actually change
- Use appropriate precision for your use case
- Consider using onComplete for cleanup operations
- Avoid too frequent updates for better performance

## Integration with Other Widgets

Progress bars work well with other widgets:

```cpp
// Progress bar with cancel button
auto cancelBtn = Button(ButtonConfig(x, y, "Cancel"));
cancelBtn->onClick.connect([=]() {
    // Cancel operation and reset progress
    progressBar->setValue(0.0f);
});

// Progress bar with status text
auto statusText = Text(TextConfig(x, y, "Initializing..."));
progressBar->onValueChanged.connect([=](float value) {
    if (value < 25.0f) {
        statusText->setText("Starting...");
    } else if (value < 75.0f) {
        statusText->setText("Processing...");
    } else {
        statusText->setText("Finishing...");
    }
});
```

## See Also

- [Circular Indicator Widget](circular-indicator.md) - For circular progress displays
- [Slider Widget](slider.md) - For interactive value selection
- [Text Widget](text.md) - For displaying progress labels
- [Styling Guide](../graphics/styling.md) - For advanced styling options
