# Progress Bar Widget Guide

Progress bars are essential visual communication tools that transform abstract numerical progress into immediate, intuitive understanding. In this guide, you'll learn how to create compelling progress indicators in Fern while understanding the psychology of progress feedback and the technical challenges of smooth, responsive progress visualization.

## Understanding Progress Visualization

Progress bars address a fundamental human need: **knowing how much work remains**. They serve multiple psychological functions:

1. **Reducing Anxiety**: Clear progress indication reduces user stress about task completion
2. **Setting Expectations**: Users can estimate remaining time and plan accordingly
3. **Maintaining Engagement**: Visual progress encourages users to wait for completion
4. **Providing Feedback**: Immediate visual response confirms that systems are working
5. **Creating Satisfaction**: Watching progress advance triggers positive psychological responses

Effective progress bars require careful balance between accuracy, responsiveness, and visual appeal.

## Progress Bar Philosophy in Fern

Fern's progress bars embody the principle of **transparent progress**:

- **Accurate Representation**: Visual fill directly corresponds to actual completion percentage
- **Smooth Updates**: Progress changes feel natural and responsive, not jarring
- **Clear Boundaries**: Distinct visual separation between completed and remaining work
- **Contextual Information**: Optional text display provides precise numerical feedback
- **Flexible Ranges**: Any numerical range can be mapped to visual progress

This approach ensures progress bars feel trustworthy and informative while maintaining visual appeal.

## Your First Progress Bar

Let's start with the simplest possible progress bar:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Create a basic progress bar for file download
    auto downloadProgress = ProgressBar(ProgressBarConfig(100, 100, 300, 25)
        .range(0.0f, 100.0f)
        .value(0.0f));
    
    // Simulate download progress
    float progress = 0.0f;
    
    // Handle completion
    downloadProgress->onComplete.connect([]() {
        std::cout << "Download complete!" << std::endl;
    });
    
    // Start the game loop with progress simulation
    while (shouldKeepRunning() && progress < 100.0f) {
        progress += 0.5f; // Simulate gradual progress
        downloadProgress->setValue(progress);
        
        processInput();
        render();
        
        // Small delay to see progress
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    return 0;
}
```

**What's happening here?**
- `ProgressBarConfig(100, 100, 300, 25)`: Position (100,100) with size 300x25 pixels
- `.range(0.0f, 100.0f)`: Progress from 0% to 100% (typical percentage range)
- `.value(0.0f)`: Starts at 0% completion
- `onComplete`: Signal emitted when progress reaches maximum value
- `setValue()`: Updates progress in real-time

Watch as the blue fill gradually expands from left to right, providing clear visual feedback of the simulated download progress.

## Understanding Progress Bar Components

A Fern progress bar consists of several coordinated visual elements:

### The Background Track
The unfilled portion that shows the total work to be completed:

```cpp
// Background represents "work remaining"
ProgressBarStyle style;
style.backgroundColor(Colors::DarkGray);  // Subtle, unobtrusive background

auto taskProgress = ProgressBar(ProgressBarConfig(100, 100, 400, 30)
    .range(0.0f, 1000.0f)  // Large task with 1000 units of work
    .value(250.0f)         // 25% complete
    .style(style));
```

### The Fill Indicator
The colored portion that represents completed work:

```cpp
// Fill color conveys meaning through color psychology
ProgressBarStyle healthStyle;
healthStyle.backgroundColor(Colors::DarkRed)
           .fillColor(Colors::Green);  // Green = healthy/positive

ProgressBarStyle warningStyle;
warningStyle.backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Orange);  // Orange = caution

ProgressBarStyle errorStyle;
errorStyle.backgroundColor(Colors::DarkGray)
          .fillColor(Colors::Red);      // Red = danger/error
```

### Border and Definition
Optional borders that provide visual separation and definition:

```cpp
// Borders improve visual clarity
ProgressBarStyle definedStyle;
definedStyle.backgroundColor(Colors::LightGray)
            .fillColor(Colors::Blue)
            .borderColor(Colors::DarkGray)
            .borderWidth(2);  // Clear visual boundaries

auto definedProgress = ProgressBar(ProgressBarConfig(100, 200, 300, 25)
    .style(definedStyle));
```

### Text Display and Information
Optional numerical feedback for precise progress communication:

```cpp
// Text provides exact progress information
ProgressBarStyle informativeStyle;
informativeStyle.showPercentage(true)
                .textColor(Colors::White)
                .fontSize(2)
                .useBitmapFont();

auto informativeProgress = ProgressBar(ProgressBarConfig(100, 250, 350, 30)
    .range(0.0f, 100.0f)
    .value(67.5f)
    .style(informativeStyle));
// Displays "67%" inside the progress bar
```

## Progress Mathematics and Value Mapping

Understanding how values map to visual progress helps with advanced usage:

### Percentage Calculation
```cpp
// How Fern calculates visual progress:
float calculatePercentage(float value, float minVal, float maxVal) {
    // Clamp value to valid range
    value = std::clamp(value, minVal, maxVal);
    
    // Calculate percentage of range completed
    if (maxVal == minVal) return 100.0f;  // Prevent division by zero
    
    return ((value - minVal) / (maxVal - minVal)) * 100.0f;
}

// Example: Value 750 in range 0-1000
// percentage = ((750 - 0) / (1000 - 0)) * 100 = 75%
```

### Visual Width Calculation
```cpp
// How visual fill width is determined:
int calculateFillWidth(float percentage, int totalWidth) {
    return (int)((percentage / 100.0f) * totalWidth);
}

// Example: 75% progress on 300-pixel wide bar
// fillWidth = (75 / 100) * 300 = 225 pixels
```

### Non-Linear Progress Ranges
```cpp
// Progress bars can represent any numerical range
auto temperatureProgress = ProgressBar(ProgressBarConfig(100, 100, 250, 20)
    .range(-40.0f, 100.0f)  // Celsius temperature range
    .value(22.0f));         // Room temperature

// File size progress
auto fileSizeProgress = ProgressBar(ProgressBarConfig(100, 150, 300, 25)
    .range(0.0f, 2048.0f)   // 2GB file in MB
    .value(512.0f));        // 512MB downloaded = 25%
```

## Advanced Progress Bar Styling

Fern provides comprehensive styling for creating professional progress indicators:

### Color Psychology in Progress Design
```cpp
// Health/status indicators
ProgressBarStyle healthBar;
healthBar.backgroundColor(Colors::DarkRed)
         .fillColor(Colors::Green)
         .borderColor(Colors::Black)
         .borderWidth(1);

// Download/loading indicators  
ProgressBarStyle downloadBar;
downloadBar.backgroundColor(Colors::LightGray)
           .fillColor(Colors::Blue)
           .showPercentage(true)
           .textColor(Colors::White);

// Warning/critical progress
ProgressBarStyle warningBar;
warningBar.backgroundColor(Colors::DarkGray)
          .fillColor(Colors::Orange)
          .borderColor(Colors::Red)
          .borderWidth(2);
```

### Size and Proportion Guidelines
```cpp
// Compact progress for secondary information
auto compactProgress = ProgressBar(ProgressBarConfig(100, 100, 200, 12)
    .style(ProgressBarStyle().showPercentage(false)));

// Standard progress for primary information
auto standardProgress = ProgressBar(ProgressBarConfig(100, 150, 300, 25)
    .style(ProgressBarStyle().showPercentage(true)));

// Large progress for critical operations
auto prominentProgress = ProgressBar(ProgressBarConfig(100, 200, 400, 40)
    .style(ProgressBarStyle().showPercentage(true).fontSize(3)));
```

### Font Integration
```cpp
// Bitmap fonts for retro/game interfaces
ProgressBarStyle pixelStyle;
pixelStyle.useBitmapFont()
          .fontSize(2)
          .showPercentage(true)
          .fillColor(Colors::LimeGreen)
          .backgroundColor(Colors::DarkGreen);

// TTF fonts for modern interfaces (requires font loading)
ProgressBarStyle modernStyle;
modernStyle.useTTFFont("arial")  // Must be loaded first!
           .fontSize(16)
           .showPercentage(true)
           .textColor(Colors::DarkBlue);
```

## Progress Bar Events and Communication

Progress bars communicate through Fern's signal system:

### Value Change Events
```cpp
auto progress = ProgressBar(ProgressBarConfig(100, 100, 300, 25));

// Monitor all progress changes
progress->onValueChanged.connect([](float newValue) {
    std::cout << "Progress updated: " << newValue << std::endl;
    
    // Update related UI elements
    updateProgressLabel(newValue);
    
    // Change colors based on progress
    if (newValue > 90.0f) {
        // Almost complete - maybe change to green
    }
});
```

### Completion Events
```cpp
// Specific event for reaching 100%
progress->onComplete.connect([]() {
    std::cout << "Task completed!" << std::endl;
    
    // Trigger completion actions
    showCompletionMessage();
    enableNextStep();
    saveProgressToFile();
});
```

### Real-World Progress Tracking
```cpp
class FileDownloader {
public:
    FileDownloader(const std::string& filename, size_t totalBytes) 
        : filename_(filename), totalBytes_(totalBytes), downloadedBytes_(0) {
        
        progressBar_ = ProgressBar(ProgressBarConfig(100, 100, 400, 30)
            .range(0.0f, (float)totalBytes)
            .value(0.0f)
            .style(ProgressBarStyle()
                .fillColor(Colors::Blue)
                .showPercentage(true)
                .textColor(Colors::White)));
        
        progressBar_->onComplete.connect([this]() {
            std::cout << "Download of " << filename_ << " completed!" << std::endl;
        });
    }
    
    void updateProgress(size_t bytesDownloaded) {
        downloadedBytes_ = bytesDownloaded;
        progressBar_->setValue((float)downloadedBytes_);
        
        // Calculate and display speed, ETA, etc.
        updateDownloadStats();
    }
    
private:
    std::shared_ptr<ProgressBarWidget> progressBar_;
    std::string filename_;
    size_t totalBytes_;
    size_t downloadedBytes_;
    
    void updateDownloadStats() {
        float percentage = progressBar_->getPercentage();
        std::cout << filename_ << ": " << percentage << "% complete" << std::endl;
    }
};

// Usage
FileDownloader downloader("large_file.zip", 104857600);  // 100MB file
downloader.updateProgress(52428800);  // 50MB downloaded
```

## Progress Bar Presets and Common Patterns

Fern provides convenient presets for typical use cases:

### Loading Operations
```cpp
auto loadingBar = ProgressBar(ProgressBarPresets::Loading(100, 100));
// Pre-configured: compact size, loading-appropriate colors
```

### Health/Status Indicators
```cpp
auto healthBar = ProgressBar(ProgressBarPresets::Health(100, 150));
// Pre-configured: health bar colors (red background, green fill)
```

### File Operations
```cpp
auto downloadBar = ProgressBar(ProgressBarPresets::Download(100, 200));
// Pre-configured: larger size, download-appropriate styling
```

## Advanced Progress Techniques

### Smooth Progress Animation
```cpp
class SmoothProgress {
public:
    SmoothProgress(ProgressBarConfig config) 
        : targetValue_(0.0f), currentValue_(0.0f), animationSpeed_(50.0f) {
        
        progressBar_ = ProgressBar(config);
    }
    
    void setTargetValue(float target) {
        targetValue_ = std::clamp(target, 0.0f, 100.0f);
    }
    
    void update(float deltaTime) {
        if (abs(currentValue_ - targetValue_) > 0.1f) {
            // Smooth interpolation toward target
            float diff = targetValue_ - currentValue_;
            currentValue_ += diff * animationSpeed_ * deltaTime;
            
            progressBar_->setValue(currentValue_);
        }
    }
    
private:
    std::shared_ptr<ProgressBarWidget> progressBar_;
    float targetValue_;
    float currentValue_;
    float animationSpeed_;
};
```

## Performance Considerations

### Update Frequency Management
```cpp
// Avoid excessive updates that impact performance
class ThrottledProgress {
public:
    ThrottledProgress(ProgressBarConfig config, float updateThreshold = 0.1f) 
        : updateThreshold_(updateThreshold), lastValue_(0.0f) {
        
        progressBar_ = ProgressBar(config);
    }
    
    void setValue(float value) {
        // Only update if change is significant
        if (abs(value - lastValue_) >= updateThreshold_) {
            progressBar_->setValue(value);
            lastValue_ = value;
        }
    }
    
private:
    std::shared_ptr<ProgressBarWidget> progressBar_;
    float updateThreshold_;
    float lastValue_;
};

// Usage: Only updates when progress changes by 0.1% or more
ThrottledProgress efficientProgress(ProgressBarConfig(100, 100, 300, 25), 0.1f);
```

### Memory Management
```cpp
// Progress bars are automatically managed
{
    auto tempProgress = ProgressBar(ProgressBarConfig(100, 100, 200, 20));
    // Automatically cleaned up when scope ends
}

// For collections of progress bars
class ProgressManager {
public:
    void addProgress(const std::string& name, ProgressBarConfig config) {
        progressBars_[name] = ProgressBar(config);
    }
    
    void updateProgress(const std::string& name, float value) {
        auto it = progressBars_.find(name);
        if (it != progressBars_.end()) {
            it->second->setValue(value);
        }
    }
    
    void clearProgress() {
        progressBars_.clear();  // Automatic cleanup
    }
    
private:
    std::map<std::string, std::shared_ptr<ProgressBarWidget>> progressBars_;
};
```

## Troubleshooting Common Issues

### Progress Not Updating

**Check these common problems:**

1. **Value outside range**:
```cpp
auto progress = ProgressBar(ProgressBarConfig(100, 100, 300, 25)
    .range(0.0f, 100.0f));

// This won't show progress - value is outside range
progress->setValue(150.0f);  // BAD: exceeds maximum

// Fern will clamp to valid range
progress->setValue(75.0f);   // GOOD: within range
```

2. **No visible change for small updates**:
```cpp
// Very small increments might not be visually noticeable
for (int i = 0; i < 100; i++) {
    progress->setValue(i + 0.01f);  // 0.01% change might not be visible
}

// Use meaningful increments
for (int i = 0; i < 100; i++) {
    progress->setValue(i);  // 1% increments are clearly visible
}
```

3. **Range configuration issues**:
```cpp
// Invalid range
auto badProgress = ProgressBar(ProgressBarConfig(100, 100, 300, 25)
    .range(100.0f, 0.0f));  // BAD: max < min

// Valid range
auto goodProgress = ProgressBar(ProgressBarConfig(100, 100, 300, 25)
    .range(0.0f, 100.0f));  // GOOD: min < max
```

### Visual Issues

**Fill not appearing:**
```cpp
// Ensure fill color contrasts with background
ProgressBarStyle style;
style.backgroundColor(Colors::DarkGray)
     .fillColor(Colors::White);  // High contrast

// Avoid similar colors
// style.backgroundColor(Colors::Blue).fillColor(Colors::DarkBlue);  // BAD: hard to see
```

**Text not readable:**
```cpp
// Ensure text color contrasts with both background and fill
ProgressBarStyle style;
style.backgroundColor(Colors::Gray)
     .fillColor(Colors::Blue)
     .textColor(Colors::White)  // Contrasts with both colors
     .showPercentage(true);
```

### Performance Issues

**Frequent updates causing lag:**
```cpp
// Avoid updating every frame if not necessary
auto progress = ProgressBar(ProgressBarConfig(100, 100, 300, 25));

// BAD: Updates every frame
for (int frame = 0; frame < 10000; frame++) {
    progress->setValue(frame / 100.0f);  // Excessive updates
}

// GOOD: Update only when meaningful change occurs
float lastReportedProgress = 0.0f;
for (int frame = 0; frame < 10000; frame++) {
    float currentProgress = frame / 100.0f;
    if (currentProgress - lastReportedProgress >= 1.0f) {  // 1% threshold
        progress->setValue(currentProgress);
        lastReportedProgress = currentProgress;
    }
}
```

## Summary

Progress bars in Fern provide powerful tools for clear, immediate progress communication while maintaining the transparency and educational value that makes Fern special. Whether you're building file transfer interfaces, loading screens, or status indicators, understanding value mapping, visual design principles, and user psychology will help you create progress bars that inform and reassure users.

Key takeaways:
- **Clear communication**: Visual progress directly corresponds to actual completion
- **Psychological importance**: Progress bars reduce anxiety and maintain user engagement
- **Flexible ranges**: Any numerical range can be mapped to visual progress
- **Rich styling**: Comprehensive appearance options for any design aesthetic
- **Event-driven**: Clean signal-based communication for progress and completion events
- **Performance conscious**: Consider update frequency and visual significance

Progress bars showcase Fern's commitment to both functionality and understanding - they're simple to use but reveal the complex considerations behind effective progress communication. Master progress bars, and you'll have essential tools for creating interfaces that keep users informed and engaged during any duration of work.

