# Circular Indicator Guide

Progress visualization is one of the most psychologically powerful interface elements - it transforms uncertainty into understanding, anxiety into anticipation. The circular indicator represents the perfect marriage of mathematical precision and human intuition, using the ancient symbolism of the circle to represent completion and wholeness in digital interfaces.

This guide explores not just how to create circular indicators in Fern, but the fascinating psychology of progress visualization, the mathematics of circular arcs, and the design principles that make circular progress indicators so universally understood and emotionally engaging.

## Understanding Circular Progress

### The Psychology of Circular Progress

Circles have deep psychological significance in human perception:

1. **Completeness**: Circles represent wholeness, completion, and cycles in virtually every culture
2. **Natural Motion**: Circular movement mirrors natural patterns - sun/moon cycles, seasons, life cycles
3. **Visual Focus**: The circular shape naturally draws the eye to the center, creating focal hierarchy
4. **Progress Intuition**: The "filled" portion immediately communicates how much work is complete
5. **Anticipation Building**: Seeing progress approach completion creates positive anticipation

```cpp
// A circular indicator at 75% completion immediately communicates:
// - 3/4 of the task is done (mathematical understanding)
// - "Almost there!" (emotional response)
// - The remaining effort is manageable (psychological comfort)

auto indicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
    .value(75.0f)                   // 75% complete
    .range(0.0f, 100.0f));
```

### Mathematical Foundations

Circular indicators involve several fascinating mathematical concepts:

**Arc Mathematics**:
- A complete circle represents 360° or 2π radians
- Progress percentage maps to arc angle: `angle = (progress / 100) * 2π`
- Arc rendering requires trigonometric calculations for pixel placement

**Coordinate Transformation**:
```cpp
// Converting circular coordinates to screen pixels:
// For a circle at center (cx, cy) with radius r and angle θ:
int pixelX = cx + r * cos(θ);
int pixelY = cy + r * sin(θ);

// This transformation happens continuously as the indicator fills
```

## Circular Indicator Philosophy in Fern

Fern's circular indicators follow several design principles:

- **Immediate Clarity**: Progress should be instantly understandable at a glance
- **Smooth Animation**: Value changes should feel natural and continuous
- **Configurable Aesthetics**: Appearance should adapt to different design contexts
- **Text Integration**: Optional percentage display provides precise information
- **Direction Control**: Clockwise/counter-clockwise options for different cultural contexts
- **Accessibility**: Clear visual contrast and optional text for screen readers

## Your First Circular Indicator

Let's start with the most basic circular indicator to understand the core concepts:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    setupFern();
    
    // Create a simple loading indicator
    auto loadingIndicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
        .value(0.0f)                // Start at 0%
        .range(0.0f, 100.0f)       // Standard 0-100% range
        .style(CircularIndicatorStyle()
            .backgroundColor(Colors::DarkGray)   // Unfilled portion
            .fillColor(Colors::Blue)             // Progress portion
            .showPercentage(true)                // Show percentage text
            .thickness(8)                        // Arc width in pixels
            .startAngle(-90.0f)                 // Start from top (12 o'clock)
            .clockwise(true)),                   // Fill clockwise
        true);  // Add to widget manager
    
    // Simulate loading progress
    float progress = 0.0f;
    
    while (shouldKeepRunning()) {
        processInput();
        
        // Update progress (simulate work being done)
        progress += 0.5f; // Increase by 0.5% each frame
        if (progress > 100.0f) progress = 0.0f; // Reset for demo
        
        loadingIndicator->setValue(progress);
        
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `CircularIndicatorConfig(200, 150, 50)`: Creates indicator at position (200, 150) with radius 50 pixels
- `.value(0.0f)`: Sets initial progress to 0%
- `.range(0.0f, 100.0f)`: Defines the value range (0% to 100%)
- `.backgroundColor(Colors::DarkGray)`: Color for the unfilled background arc
- `.fillColor(Colors::Blue)`: Color for the progress arc
- `.thickness(8)`: Makes the arc 8 pixels thick
- `.startAngle(-90.0f)`: Positions start at top of circle (12 o'clock position)
- `.clockwise(true)`: Progress fills in clockwise direction

This creates a smooth, animated loading indicator that visually represents progress in an immediately understandable way.

## Understanding the Configuration System

### CircularIndicatorConfig: The Foundation

The `CircularIndicatorConfig` class defines the basic properties of your indicator:

```cpp
// Complete configuration breakdown
auto config = CircularIndicatorConfig(x, y, radius)
    .value(initialValue)           // Starting progress value
    .range(minValue, maxValue)     // Value range (not always 0-100!)
    .style(styleObject);           // Visual appearance

// Real-world examples of different ranges:

// Standard percentage (0-100%)
auto percentIndicator = CircularIndicatorConfig(100, 100, 40)
    .value(75.0f)
    .range(0.0f, 100.0f);

// Temperature gauge (-20°C to 40°C)
auto tempIndicator = CircularIndicatorConfig(200, 100, 40)
    .value(23.5f)                  // Current temperature
    .range(-20.0f, 40.0f);         // Temperature range

// Experience points (0 to next level)
auto xpIndicator = CircularIndicatorConfig(300, 100, 40)
    .value(850.0f)                 // Current XP
    .range(0.0f, 1000.0f);         // XP needed for next level

// Volume control (0-11, because this one goes to eleven!)
auto volumeIndicator = CircularIndicatorConfig(400, 100, 40)
    .value(7.5f)
    .range(0.0f, 11.0f);
```

### CircularIndicatorStyle: Visual Customization

The `CircularIndicatorStyle` class controls every aspect of the indicator's appearance:

```cpp
CircularIndicatorStyle style;
style.backgroundColor(Colors::DarkGray)     // Background arc color
     .fillColor(Colors::Blue)               // Progress arc color
     .borderColor(Colors::Black)            // Border around entire indicator
     .borderWidth(2)                        // Border thickness in pixels
     .thickness(8)                          // Arc thickness in pixels
     .showPercentage(true)                  // Display percentage text in center
     .textColor(Colors::White)              // Text color
     .fontSize(2)                           // Text size (bitmap font scaling)
     .clockwise(true)                       // Progress direction
     .startAngle(-90.0f)                    // Start position (degrees)
     .useBitmapFont();                      // Use bitmap fonts for text

// For higher quality text with TTF fonts:
style.useTTFFont("arial")                  // Use TTF font
     .fontSize(16);                        // TTF font size (minimum 16)
```

**Understanding the Style Properties:**

- **backgroundColor**: The "empty" portion of the circle that shows remaining work
- **fillColor**: The "filled" portion that represents completed work
- **thickness**: Thicker arcs are more prominent but take more space
- **startAngle**: -90° = top, 0° = right, 90° = bottom, 180° = left
- **clockwise**: true = fills like a clock, false = fills counter-clockwise
- **showPercentage**: When true, displays the percentage value in the center

## Core Methods and Real-Time Updates

### Progress Control Methods

```cpp
auto indicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50), true);

// Set the current progress value
indicator->setValue(65.0f);

// Get the current progress value  
float currentValue = indicator->getValue();
std::cout << "Current progress: " << currentValue << std::endl;

// Get progress as percentage (useful for any range)
float percentage = indicator->getPercentage();
std::cout << "Percentage complete: " << percentage << "%" << std::endl;

// Change the value range dynamically
indicator->setRange(0.0f, 200.0f);  // Now 65.0f represents 32.5% instead of 65%
```

**Understanding Value vs Percentage:**
- `getValue()` returns the raw value within your specified range
- `getPercentage()` always returns 0-100%, regardless of your range
- This distinction is crucial for displaying meaningful information to users

### Widget Interface Methods

```cpp
// Position and size management
indicator->setPosition(300, 200);           // Move the indicator
indicator->resize(120, 120);                // Change size (diameter)

// Query current dimensions
int width = indicator->getWidth();          // Returns diameter (2 * radius)
int height = indicator->getHeight();        // Same as width (circles are square)
int x = indicator->getX();                  // Current X position
int y = indicator->getY();                  // Current Y position
```

## Events and Reactive Programming

### Progress Change Events

Circular indicators support event-driven programming for responsive interfaces:

```cpp
auto downloadIndicator = CircularIndicator(CircularIndicatorConfig(200, 100, 40), true);

// React to any progress change
downloadIndicator->onValueChanged.connect([](float value) {
    std::cout << "Download progress: " << value << "%" << std::endl;
    
    // Update UI based on progress
    if (value < 25.0f) {
        updateStatusText("Download starting...");
    } else if (value < 75.0f) {
        updateStatusText("Download in progress...");
    } else if (value < 100.0f) {
        updateStatusText("Download almost complete...");
    }
});

// React to completion
downloadIndicator->onComplete.connect([]() {
    std::cout << "Download finished!" << std::endl;
    updateStatusText("Download complete!");
    showCompletionAnimation();
    playSuccessSound();
});

// Simulate download progress
void simulateDownload() {
    static float progress = 0.0f;
    progress += 0.3f; // Simulate download chunks
    
    if (progress > 100.0f) progress = 100.0f;
    downloadIndicator->setValue(progress);
}
```

### Event-Driven UI Updates

```cpp
class ProgressiveInterface {
public:
    ProgressiveInterface() {
        setupIndicators();
    }
    
private:
    void setupIndicators() {
        // Health indicator with color changes
        healthIndicator_ = CircularIndicator(CircularIndicatorConfig(100, 100, 30), true);
        healthIndicator_->onValueChanged.connect([this](float health) {
            updateHealthVisualization(health);
        });
        
        // Experience indicator with level-up detection
        xpIndicator_ = CircularIndicator(CircularIndicatorConfig(200, 100, 30), true);
        xpIndicator_->onComplete.connect([this]() {
            triggerLevelUp();
        });
        
        // Battery indicator with low-power warnings
        batteryIndicator_ = CircularIndicator(CircularIndicatorConfig(300, 100, 30), true);
        batteryIndicator_->onValueChanged.connect([this](float battery) {
            if (battery < 20.0f && !lowBatteryWarningShown_) {
                showLowBatteryWarning();
                lowBatteryWarningShown_ = true;
            } else if (battery >= 20.0f) {
                lowBatteryWarningShown_ = false;
            }
        });
    }
    
    void updateHealthVisualization(float health) {
        if (health > 75.0f) {
            healthIndicator_->getStyle().fillColor(Colors::Green);
        } else if (health > 50.0f) {
            healthIndicator_->getStyle().fillColor(Colors::Yellow);
        } else if (health > 25.0f) {
            healthIndicator_->getStyle().fillColor(Colors::Orange);
        } else {
            healthIndicator_->getStyle().fillColor(Colors::Red);
            if (health < 10.0f) {
                triggerCriticalHealthEffect();
            }
        }
    }
    
    void triggerLevelUp() {
        std::cout << "LEVEL UP!" << std::endl;
        // Reset XP indicator for next level
        xpIndicator_->setValue(0.0f);
        // Play level-up animation
        // Increase character stats
    }
    
    void showLowBatteryWarning() {
        std::cout << "Low battery warning!" << std::endl;
        // Show warning UI
        // Suggest power-saving mode
    }
    
    void triggerCriticalHealthEffect() {
        // Screen flash effect
        // Warning sounds
        // UI color changes
    }
    
    std::shared_ptr<CircularIndicatorWidget> healthIndicator_;
    std::shared_ptr<CircularIndicatorWidget> xpIndicator_;
    std::shared_ptr<CircularIndicatorWidget> batteryIndicator_;
    bool lowBatteryWarningShown_ = false;
};
```

## Preset Configurations: Quick Start Templates

Fern provides carefully designed presets for common use cases, embodying best practices for different types of progress indicators:

### Default Preset: Clean and Versatile

```cpp
auto indicator = CircularIndicator(CircularIndicatorPresets::Default(100, 100, 50), true);
```

**Design Characteristics:**
- Neutral gray background with blue progress
- 8-pixel thickness for good visibility
- Percentage display for precise feedback
- Starts from top (12 o'clock position)
- Clockwise progression (natural for most cultures)

### Loading Preset: User-Friendly Feedback

```cpp
auto loadingIndicator = CircularIndicator(CircularIndicatorPresets::Loading(200, 150, 40), true);
```

**Optimized for Loading Operations:**
- Slightly smaller radius (40px) for compact UI integration
- Blue theme suggests progress and activity
- Percentage display provides user reassurance
- Smooth animation-friendly design

### Health Preset: Status Visualization

```cpp
auto healthIndicator = CircularIndicator(CircularIndicatorPresets::Health(150, 100, 30), true);
```

**Designed for Status Display:**
- Compact size (30px radius) for UI dashboards
- Green theme suggests vitality and success
- No percentage display (status more important than exact numbers)
- Quick visual assessment at a glance

### Battery Preset: System Monitoring

```cpp
auto batteryIndicator = CircularIndicator(CircularIndicatorPresets::Battery(250, 200, 25), true);
```

**Tailored for Battery/Resource Display:**
- Very compact (25px radius) for status bars
- Includes percentage for precise remaining capacity
- Neutral styling that adapts to different themes
- Efficient rendering for always-visible elements

## Real-World Application Examples

### File Transfer with Dynamic Feedback

```cpp
class FileTransferIndicator {
public:
    FileTransferIndicator(const std::string& filename, size_t totalBytes) 
        : filename_(filename), totalBytes_(totalBytes), transferredBytes_(0) {
        
        indicator_ = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::LightGray)
                .fillColor(Colors::Cyan)
                .borderColor(Colors::DarkGray)
                .borderWidth(2)
                .thickness(8)
                .showPercentage(true)
                .textColor(Colors::Black)
                .fontSize(2)), true);
        
        // Connect to progress events for user feedback
        indicator_->onValueChanged.connect([this](float progress) {
            updateTransferStatus(progress);
        });
        
        indicator_->onComplete.connect([this]() {
            onTransferComplete();
        });
    }
    
    void updateProgress(size_t bytesTransferred) {
        transferredBytes_ = bytesTransferred;
        float percentage = (float(transferredBytes_) / totalBytes_) * 100.0f;
        indicator_->setValue(percentage);
    }
    
private:
    void updateTransferStatus(float progress) {
        size_t remainingBytes = totalBytes_ - transferredBytes_;
        std::cout << filename_ << ": " << progress << "% complete (" 
                  << remainingBytes << " bytes remaining)" << std::endl;
        
        // Estimate time remaining based on current transfer rate
        // Update UI status text
        // Adjust transfer priority if needed
    }
    
    void onTransferComplete() {
        std::cout << "Transfer complete: " << filename_ << std::endl;
        // Verify file integrity
        // Update file manager
        // Clean up resources
    }
    
    std::string filename_;
    size_t totalBytes_;
    size_t transferredBytes_;
    std::shared_ptr<CircularIndicatorWidget> indicator_;
};
```

### Game Character Status Dashboard

```cpp
class CharacterStatusDashboard {
public:
    CharacterStatusDashboard() {
        setupHealthIndicator();
        setupManaIndicator();
        setupExperienceIndicator();
        setupStaminaIndicator();
    }
    
    void updateCharacterStats(float health, float mana, float experience, float stamina) {
        // Each indicator uses appropriate range and styling
        healthIndicator_->setValue(health);      // 0-100 health points
        manaIndicator_->setValue(mana);          // 0-100 mana points  
        experienceIndicator_->setValue(experience); // 0-1000 XP to next level
        staminaIndicator_->setValue(stamina);    // 0-100 stamina points
    }
    
private:
    void setupHealthIndicator() {
        healthIndicator_ = CircularIndicator(CircularIndicatorConfig(100, 100, 30)
            .value(100.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkRed)
                .fillColor(Colors::Green)
                .thickness(6)
                .showPercentage(false)  // Visual status more important than numbers
                .startAngle(-90.0f)), true);
        
        healthIndicator_->onValueChanged.connect([this](float health) {
            updateHealthEffects(health);
        });
    }
    
    void setupManaIndicator() {
        manaIndicator_ = CircularIndicator(CircularIndicatorConfig(200, 100, 30)
            .value(100.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkBlue)
                .fillColor(Colors::Cyan)
                .thickness(6)
                .showPercentage(false)), true);
        
        manaIndicator_->onValueChanged.connect([this](float mana) {
            updateManaEffects(mana);
        });
    }
    
    void setupExperienceIndicator() {
        experienceIndicator_ = CircularIndicator(CircularIndicatorConfig(300, 100, 30)
            .value(0.0f)
            .range(0.0f, 1000.0f)  // XP needed for next level
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Gold)
                .thickness(6)
                .showPercentage(true)), true);  // Show percentage for progression feedback
        
        experienceIndicator_->onComplete.connect([this]() {
            triggerLevelUp();
        });
    }
    
    void setupStaminaIndicator() {
        staminaIndicator_ = CircularIndicator(CircularIndicatorConfig(400, 100, 30)
            .value(100.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGreen)
                .fillColor(Colors::Yellow)
                .thickness(6)
                .showPercentage(false)), true);
    }
    
    void updateHealthEffects(float health) {
        if (health < 25.0f) {
            // Screen edge flashing
            // Character movement penalties
            // Warning sounds
        } else if (health < 50.0f) {
            // Subtle visual effects
            // Slight movement reduction
        }
    }
    
    void updateManaEffects(float mana) {
        if (mana < 10.0f) {
            // Disable magic abilities
            // Visual mana depletion effects
        }
    }
    
    void triggerLevelUp() {
        // Level-up animation
        // Stat increases
        // Reset experience indicator
        experienceIndicator_->setValue(0.0f);
        // Increase next level requirement
        experienceIndicator_->setRange(0.0f, 1200.0f); // Harder next level
    }
    
    std::shared_ptr<CircularIndicatorWidget> healthIndicator_;
    std::shared_ptr<CircularIndicatorWidget> manaIndicator_;
    std::shared_ptr<CircularIndicatorWidget> experienceIndicator_;
    std::shared_ptr<CircularIndicatorWidget> staminaIndicator_;
};
```

### System Performance Monitor

```cpp
class SystemMonitor {
public:
    SystemMonitor() {
        setupPerformanceIndicators();
        startMonitoring();
    }
    
private:
    void setupPerformanceIndicators() {
        // CPU usage (0-100%)
        cpuIndicator_ = CircularIndicator(CircularIndicatorConfig(100, 100, 35)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thickness(6)
                .showPercentage(true)
                .fontSize(1)), true);
        
        // Memory usage (0-100%)
        memoryIndicator_ = CircularIndicator(CircularIndicatorConfig(200, 100, 35)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Orange)
                .thickness(6)
                .showPercentage(true)
                .fontSize(1)), true);
        
        // Network usage (0-100 Mbps)
        networkIndicator_ = CircularIndicator(CircularIndicatorConfig(300, 100, 35)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .thickness(6)
                .showPercentage(true)
                .fontSize(1)), true);
        
        // Setup warning thresholds
        cpuIndicator_->onValueChanged.connect([](float cpu) {
            if (cpu > 90.0f) {
                std::cout << "WARNING: High CPU usage!" << std::endl;
            }
        });
        
        memoryIndicator_->onValueChanged.connect([](float memory) {
            if (memory > 85.0f) {
                std::cout << "WARNING: High memory usage!" << std::endl;
            }
        });
    }
    
    void startMonitoring() {
        // In a real application, this would be a separate thread
        // updating these indicators with actual system metrics
    }
    
    void updateSystemMetrics(float cpu, float memory, float network) {
        cpuIndicator_->setValue(cpu);
        memoryIndicator_->setValue(memory);
        networkIndicator_->setValue(network);
        
        // Change colors based on load levels
        updateIndicatorColor(cpuIndicator_, cpu);
        updateIndicatorColor(memoryIndicator_, memory);
        updateIndicatorColor(networkIndicator_, network);
    }
    
    void updateIndicatorColor(std::shared_ptr<CircularIndicatorWidget> indicator, float value) {
        if (value > 90.0f) {
            indicator->getStyle().fillColor(Colors::Red);      // Critical
        } else if (value > 75.0f) {
            indicator->getStyle().fillColor(Colors::Orange);   // High
        } else if (value > 50.0f) {
            indicator->getStyle().fillColor(Colors::Yellow);   // Medium
        } else {
            indicator->getStyle().fillColor(Colors::Green);    // Normal
        }
    }
    
    std::shared_ptr<CircularIndicatorWidget> cpuIndicator_;
    std::shared_ptr<CircularIndicatorWidget> memoryIndicator_;
    std::shared_ptr<CircularIndicatorWidget> networkIndicator_;
};
```

## Advanced Styling and Customization

### Color Psychology and Design Guidelines

Different colors evoke different psychological responses in users:

```cpp
// Success/Health: Green colors suggest completion, health, success
CircularIndicatorStyle healthStyle;
healthStyle.backgroundColor(0xFF2D4A2D)    // Dark green background
        .fillColor(0xFF4CAF50)             // Material Design green
        .textColor(Colors::White);

// Warning/Caution: Orange/Yellow for attention without alarm
CircularIndicatorStyle warningStyle;
warningStyle.backgroundColor(0xFF4A3D2D)   // Dark orange background
         .fillColor(0xFFFF9800)            // Material Design orange
         .textColor(Colors::Black);

// Critical/Error: Red for urgent attention
CircularIndicatorStyle criticalStyle;
criticalStyle.backgroundColor(0xFF4A2D2D)  // Dark red background
          .fillColor(0xFFF44336)           // Material Design red
          .textColor(Colors::White);

// Information/Progress: Blue for neutral information
CircularIndicatorStyle infoStyle;
infoStyle.backgroundColor(0xFF2D3A4A)      // Dark blue background
      .fillColor(0xFF2196F3)               // Material Design blue
      .textColor(Colors::White);

// Apply styles based on context
void updateIndicatorStyle(std::shared_ptr<CircularIndicatorWidget> indicator, float value) {
    if (value < 25.0f) {
        indicator->setStyle(criticalStyle);
    } else if (value < 50.0f) {
        indicator->setStyle(warningStyle);
    } else if (value < 90.0f) {
        indicator->setStyle(infoStyle);
    } else {
        indicator->setStyle(healthStyle);
    }
}
```

### Custom Start Angles and Direction

Different starting positions create different emotional contexts:

```cpp
// Traditional clock position (12 o'clock) - most intuitive
CircularIndicatorStyle clockwiseTop;
clockwiseTop.startAngle(-90.0f).clockwise(true);

// Right side start (3 o'clock) - suggests moving forward
CircularIndicatorStyle clockwiseRight;
clockwiseRight.startAngle(0.0f).clockwise(true);

// Bottom start (6 o'clock) - suggests building up
CircularIndicatorStyle clockwiseBottom;
clockwiseBottom.startAngle(90.0f).clockwise(true);

// Counter-clockwise from top - less common, draws attention
CircularIndicatorStyle counterClockwiseTop;
counterClockwiseTop.startAngle(-90.0f).clockwise(false);

// Example: Countdown timer (counter-clockwise suggests time running out)
auto countdownTimer = CircularIndicator(CircularIndicatorConfig(200, 150, 40)
    .value(60.0f)                          // 60 seconds remaining
    .range(0.0f, 60.0f)
    .style(CircularIndicatorStyle()
        .backgroundColor(Colors::DarkGray)
        .fillColor(Colors::Red)            // Red for urgency
        .startAngle(-90.0f)                // Start at top
        .clockwise(false)                  // Counter-clockwise for countdown
        .showPercentage(false)), true);
```

### Multi-Layer Indicators

Create complex visualizations by layering multiple indicators:

```cpp
class MultiLayerIndicator {
public:
    MultiLayerIndicator(int x, int y, int radius) {
        // Outer ring: Overall progress
        outerRing_ = CircularIndicator(CircularIndicatorConfig(x, y, radius)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thickness(8)
                .showPercentage(false)), true);
        
        // Middle ring: Sub-task progress
        middleRing_ = CircularIndicator(CircularIndicatorConfig(x, y, radius - 15)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .thickness(6)
                .showPercentage(false)), true);
        
        // Inner ring: Current operation
        innerRing_ = CircularIndicator(CircularIndicatorConfig(x, y, radius - 25)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Orange)
                .thickness(4)
                .showPercentage(true)
                .fontSize(1)), true);
    }
    
    void updateProgress(float overall, float subtask, float current) {
        outerRing_->setValue(overall);
        middleRing_->setValue(subtask);
        innerRing_->setValue(current);
    }
    
private:
    std::shared_ptr<CircularIndicatorWidget> outerRing_;
    std::shared_ptr<CircularIndicatorWidget> middleRing_;
    std::shared_ptr<CircularIndicatorWidget> innerRing_;
};

// Usage: Complex installation progress
MultiLayerIndicator installer(300, 200, 60);
installer.updateProgress(45.0f,    // Overall installation: 45%
                        78.0f,     // Current component: 78%
                        23.0f);    // Current file: 23%
```

### Animated Transitions and Effects

Create smooth, engaging animations:

```cpp
class AnimatedIndicator {
public:
    AnimatedIndicator(int x, int y, int radius) 
        : currentValue_(0.0f), targetValue_(0.0f), animationSpeed_(2.0f) {
        
        indicator_ = CircularIndicator(CircularIndicatorConfig(x, y, radius)
            .value(0.0f)
            .range(0.0f, 100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thickness(8)
                .showPercentage(true)), true);
    }
    
    void setTargetValue(float target) {
        targetValue_ = target;
    }
    
    void update(float deltaTime) {
        // Smooth animation towards target
        if (abs(currentValue_ - targetValue_) > 0.1f) {
            float direction = (targetValue_ > currentValue_) ? 1.0f : -1.0f;
            currentValue_ += direction * animationSpeed_ * deltaTime;
            
            // Clamp to target
            if (direction > 0 && currentValue_ > targetValue_) {
                currentValue_ = targetValue_;
            } else if (direction < 0 && currentValue_ < targetValue_) {
                currentValue_ = targetValue_;
            }
            
            indicator_->setValue(currentValue_);
        }
    }
    
    void setAnimationSpeed(float speed) { animationSpeed_ = speed; }
    
private:
    std::shared_ptr<CircularIndicatorWidget> indicator_;
    float currentValue_;
    float targetValue_;
    float animationSpeed_;
};

// Usage in game loop
AnimatedIndicator animatedHealth(200, 150, 40);

void gameLoop() {
    float deltaTime = getDeltaTime();
    
    // Health changed from combat
    if (playerTookDamage) {
        animatedHealth.setTargetValue(newHealthValue);
        playerTookDamage = false;
    }
    
    // Smooth animation
    animatedHealth.update(deltaTime);
}
```

## Performance Optimization and Best Practices

### Efficient Updates and Rendering

```cpp
class OptimizedIndicatorManager {
public:
    void addIndicator(std::shared_ptr<CircularIndicatorWidget> indicator) {
        indicators_.push_back(indicator);
    }
    
    void updateValues(const std::vector<float>& newValues) {
        // Batch update to minimize redraws
        for (size_t i = 0; i < indicators_.size() && i < newValues.size(); ++i) {
            float currentValue = indicators_[i]->getValue();
            
            // Only update if value actually changed
            if (abs(currentValue - newValues[i]) > 0.01f) {
                indicators_[i]->setValue(newValues[i]);
            }
        }
    }
    
    void setUpdateFrequency(float maxUpdatesPerSecond) {
        updateInterval_ = 1.0f / maxUpdatesPerSecond;
    }
    
private:
    std::vector<std::shared_ptr<CircularIndicatorWidget>> indicators_;
    float updateInterval_ = 1.0f / 60.0f; // 60 FPS max
    float lastUpdateTime_ = 0.0f;
};
```

### Memory Management and Lifecycle

```cpp
class IndicatorLifecycleManager {
public:
    std::shared_ptr<CircularIndicatorWidget> createTemporaryIndicator(
        int x, int y, int radius, float duration) {
        
        auto indicator = CircularIndicator(CircularIndicatorConfig(x, y, radius), true);
        
        // Auto-cleanup after duration
        temporaryIndicators_.push_back({indicator, getCurrentTime() + duration});
        
        return indicator;
    }
    
    void update() {
        float currentTime = getCurrentTime();
        
        // Remove expired temporary indicators
        temporaryIndicators_.erase(
            std::remove_if(temporaryIndicators_.begin(), temporaryIndicators_.end(),
                [currentTime](const TempIndicator& temp) {
                    if (currentTime > temp.expiryTime) {
                        // Indicator automatically cleaned up when shared_ptr goes out of scope
                        return true;
                    }
                    return false;
                }),
            temporaryIndicators_.end()
        );
    }
    
private:
    struct TempIndicator {
        std::shared_ptr<CircularIndicatorWidget> indicator;
        float expiryTime;
    };
    
    std::vector<TempIndicator> temporaryIndicators_;
    
    float getCurrentTime() { 
        return 0.0f; // Placeholder - use actual time function
    }
};
```

### Size and Positioning Guidelines

```cpp
namespace IndicatorDesignGuidelines {
    // Recommended sizes for different contexts
    constexpr int ICON_SIZE = 16;      // Tiny status indicators
    constexpr int COMPACT_SIZE = 24;   // Toolbar/status bar
    constexpr int STANDARD_SIZE = 40;  // General UI elements
    constexpr int PROMINENT_SIZE = 60; // Important progress displays
    constexpr int HERO_SIZE = 100;     // Main focal points
    
    // Thickness recommendations based on size
    int recommendedThickness(int radius) {
        if (radius <= 20) return 3;
        if (radius <= 40) return 6;
        if (radius <= 60) return 8;
        return 10;
    }
    
    // Spacing recommendations
    int recommendedSpacing(int radius) {
        return radius * 2 + 20; // Diameter plus padding
    }
    
    // Font size recommendations
    int recommendedFontSize(int radius) {
        if (radius <= 20) return 1;
        if (radius <= 40) return 2;
        return 3;
    }
}

// Apply design guidelines
auto createWellDesignedIndicator(int x, int y, int radius) {
    return CircularIndicator(CircularIndicatorConfig(x, y, radius)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(CircularIndicatorStyle()
            .thickness(IndicatorDesignGuidelines::recommendedThickness(radius))
            .fontSize(IndicatorDesignGuidelines::recommendedFontSize(radius))
            .backgroundColor(Colors::DarkGray)
            .fillColor(Colors::Blue)
            .showPercentage(radius >= IndicatorDesignGuidelines::STANDARD_SIZE)), true);
}
```

## Integration Patterns

### With Layout Systems

```cpp
// Indicators work well in layout containers
auto statusPanel = Column({
    Text(TextPresets::Heading(0, 0, "System Status")),
    
    Row({
        Text(TextPresets::Body(0, 0, "CPU:")),
        CircularIndicator(CircularIndicatorPresets::Default(0, 0, 20), true)
    }),
    
    Row({
        Text(TextPresets::Body(0, 0, "Memory:")),
        CircularIndicator(CircularIndicatorPresets::Default(0, 0, 20), true)
    }),
    
    Row({
        Text(TextPresets::Body(0, 0, "Disk:")),
        CircularIndicator(CircularIndicatorPresets::Default(0, 0, 20), true)
    })
});
```

### With State Management

```cpp
class StatefulProgressManager {
public:
    enum class ProgressState {
        Idle,
        Starting,
        InProgress,
        Completing,
        Done,
        Error
    };
    
    void setState(ProgressState state) {
        currentState_ = state;
        updateIndicatorAppearance();
    }
    
    void updateProgress(float value) {
        if (currentState_ == ProgressState::InProgress) {
            indicator_->setValue(value);
            
            if (value >= 100.0f) {
                setState(ProgressState::Completing);
            }
        }
    }
    
private:
    void updateIndicatorAppearance() {
        switch (currentState_) {
            case ProgressState::Idle:
                indicator_->getStyle()
                    .fillColor(Colors::Gray)
                    .backgroundColor(Colors::DarkGray);
                break;
                
            case ProgressState::Starting:
                indicator_->getStyle()
                    .fillColor(Colors::Yellow)
                    .backgroundColor(Colors::DarkGray);
                break;
                
            case ProgressState::InProgress:
                indicator_->getStyle()
                    .fillColor(Colors::Blue)
                    .backgroundColor(Colors::DarkGray);
                break;
                
            case ProgressState::Completing:
                indicator_->getStyle()
                    .fillColor(Colors::Green)
                    .backgroundColor(Colors::DarkGray);
                break;
                
            case ProgressState::Done:
                indicator_->getStyle()
                    .fillColor(Colors::Green)
                    .backgroundColor(Colors::DarkGreen);
                break;
                
            case ProgressState::Error:
                indicator_->getStyle()
                    .fillColor(Colors::Red)
                    .backgroundColor(Colors::DarkRed);
                break;
        }
    }
    
    ProgressState currentState_ = ProgressState::Idle;
    std::shared_ptr<CircularIndicatorWidget> indicator_;
};
```

## Troubleshooting Common Issues

### Indicator Not Visible

**Check these common problems:**

1. **Position outside screen bounds**:
```cpp
// Bad: Indicator positioned completely off-screen
auto offScreen = CircularIndicator(CircularIndicatorConfig(-100, -100, 50), true);

// Good: Ensure indicator is within visible area
auto onScreen = CircularIndicator(CircularIndicatorConfig(50, 50, 50), true);
```

2. **Colors match background**:
```cpp
// Invisible: Same color as background
auto invisible = CircularIndicatorConfig(200, 150, 50)
    .style(CircularIndicatorStyle()
        .backgroundColor(Colors::Black)    // On black background
        .fillColor(Colors::Black));        // Also black - invisible!

// Visible: Use contrasting colors
auto visible = CircularIndicatorConfig(200, 150, 50)
    .style(CircularIndicatorStyle()
        .backgroundColor(Colors::DarkGray)
        .fillColor(Colors::Blue));
```

3. **Zero thickness or radius**:
```cpp
// Won't show: Zero thickness
auto noThickness = CircularIndicatorConfig(200, 150, 50)
    .style(CircularIndicatorStyle().thickness(0));

// Won't show: Zero radius
auto noRadius = CircularIndicatorConfig(200, 150, 0);

// Visible: Adequate thickness and radius
auto visible = CircularIndicatorConfig(200, 150, 30)
    .style(CircularIndicatorStyle().thickness(6));
```

### Value and Range Issues

**Understanding value mapping:**

```cpp
// Common mistake: Misunderstanding value ranges
auto indicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50)
    .value(50.0f)
    .range(0.0f, 200.0f), true);  // Range is 0-200, not 0-100!

// This shows 25% progress (50/200), not 50%
std::cout << "Progress: " << indicator->getPercentage() << "%" << std::endl; // Prints 25%

// If you want 50% progress with 0-200 range:
indicator->setValue(100.0f);  // 100 out of 200 = 50%
```

### Text Display Problems

```cpp
// Text too small to read
auto tinyText = CircularIndicatorConfig(200, 150, 20)
    .style(CircularIndicatorStyle()
        .fontSize(1)               // Tiny on small indicator
        .showPercentage(true));

// Better: Scale font with indicator size
auto readableText = CircularIndicatorConfig(200, 150, 20)
    .style(CircularIndicatorStyle()
        .fontSize(1)               // Appropriate for size
        .showPercentage(false));   // Or hide text for small indicators

// For larger indicators, larger font
auto largeWithText = CircularIndicatorConfig(200, 150, 60)
    .style(CircularIndicatorStyle()
        .fontSize(3)               // Readable on large indicator
        .showPercentage(true));
```

### Performance Issues

```cpp
// Inefficient: Updating too frequently
void inefficientUpdate() {
    // Don't update every single frame unless necessary
    for (int i = 0; i < 1000; ++i) {
        indicator->setValue(i * 0.1f);  // 1000 updates per frame!
    }
}

// Efficient: Update only when value actually changes
void efficientUpdate(float newValue) {
    float currentValue = indicator->getValue();
    if (abs(currentValue - newValue) > 0.1f) {  // Only update if significant change
        indicator->setValue(newValue);
    }
}

// Efficient: Batch updates for multiple indicators
void batchUpdate(const std::vector<float>& values) {
    for (size_t i = 0; i < indicators.size() && i < values.size(); ++i) {
        if (abs(indicators[i]->getValue() - values[i]) > 0.1f) {
            indicators[i]->setValue(values[i]);
        }
    }
}
```

### Animation and Smoothness Issues

```cpp
// Choppy animation: Jumping values
void choppyAnimation() {
    // Bad: Large jumps
    indicator->setValue(10.0f);
    indicator->setValue(60.0f);
    indicator->setValue(90.0f);
}

// Smooth animation: Gradual changes
class SmoothProgressUpdater {
public:
    void setTargetValue(float target) { targetValue_ = target; }
    
    void update(float deltaTime) {
        float current = indicator_->getValue();
        if (abs(current - targetValue_) > 0.1f) {
            float step = (targetValue_ - current) * 3.0f * deltaTime; // 3x speed factor
            indicator_->setValue(current + step);
        }
    }
    
private:
    std::shared_ptr<CircularIndicatorWidget> indicator_;
    float targetValue_ = 0.0f;
};
```

### Event Handling Problems

```cpp
// Event not firing: Check value actually changed
auto indicator = CircularIndicator(CircularIndicatorConfig(200, 150, 50), true);

indicator->onValueChanged.connect([](float value) {
    std::cout << "Value changed: " << value << std::endl;
});

// This won't trigger event if value is already 50.0f
indicator->setValue(50.0f);
indicator->setValue(50.0f);  // No event - same value

// Force event trigger by setting different value first
indicator->setValue(49.9f);
indicator->setValue(50.0f);  // Now event fires

// Better: Check if you need to force updates
void forceValueUpdate(float newValue) {
    float current = indicator->getValue();
    if (current == newValue) {
        // Force change by setting slightly different value first
        indicator->setValue(newValue - 0.001f);
    }
    indicator->setValue(newValue);
}
```

## Integration with Other Systems

### Animation Framework Integration

```cpp
// Works well with animation systems
class ProgressAnimation {
public:
    void animateToValue(float target, float duration) {
        startValue_ = indicator_->getValue();
        targetValue_ = target;
        animationTime_ = 0.0f;
        animationDuration_ = duration;
        isAnimating_ = true;
    }
    
    void update(float deltaTime) {
        if (!isAnimating_) return;
        
        animationTime_ += deltaTime;
        float progress = animationTime_ / animationDuration_;
        
        if (progress >= 1.0f) {
            indicator_->setValue(targetValue_);
            isAnimating_ = false;
        } else {
            // Smooth easing function
            float easedProgress = easeInOutQuad(progress);
            float currentValue = startValue_ + (targetValue_ - startValue_) * easedProgress;
            indicator_->setValue(currentValue);
        }
    }
    
private:
    float easeInOutQuad(float t) {
        return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
    }
    
    std::shared_ptr<CircularIndicatorWidget> indicator_;
    float startValue_, targetValue_;
    float animationTime_, animationDuration_;
    bool isAnimating_ = false;
};
```

### Data Binding Integration

```cpp
// Automatic updates from data sources
template<typename T>
class DataBoundIndicator {
public:
    DataBoundIndicator(std::shared_ptr<CircularIndicatorWidget> indicator,
                      std::function<float()> valueGetter,
                      float minVal, float maxVal)
        : indicator_(indicator), getValue_(valueGetter) {
        
        indicator_->setRange(minVal, maxVal);
    }
    
    void update() {
        float newValue = getValue_();
        float currentValue = indicator_->getValue();
        
        if (abs(newValue - currentValue) > 0.01f) {
            indicator_->setValue(newValue);
        }
    }
    
private:
    std::shared_ptr<CircularIndicatorWidget> indicator_;
    std::function<float()> getValue_;
};

// Usage with game data
struct GameCharacter {
    float health = 100.0f;
    float mana = 100.0f;
    float experience = 0.0f;
};

GameCharacter player;

// Bind indicators to player data
DataBoundIndicator healthIndicator(
    CircularIndicator(CircularIndicatorPresets::Health(100, 100, 30), true),
    [&player]() { return player.health; },
    0.0f, 100.0f
);

DataBoundIndicator manaIndicator(
    CircularIndicator(CircularIndicatorPresets::Default(200, 100, 30), true),
    [&player]() { return player.mana; },
    0.0f, 100.0f
);

// Automatic updates in game loop
void gameLoop() {
    // Game logic updates player stats
    player.health -= damageThisFrame;
    player.mana -= spellCost;
    
    // Indicators automatically reflect changes
    healthIndicator.update();
    manaIndicator.update();
}
```

## Best Practices Summary

### Design Guidelines

1. **Size Appropriately**: Use design guidelines for consistent sizing
2. **Color Meaningfully**: Follow color psychology principles
3. **Animate Smoothly**: Gradual changes feel more natural
4. **Provide Context**: Use labels and descriptions when needed
5. **Test Accessibility**: Ensure visibility across different displays

### Performance Guidelines

1. **Update Efficiently**: Only update when values actually change
2. **Batch Operations**: Group multiple updates together
3. **Limit Frequency**: Don't update more than necessary
4. **Cache Values**: Store frequently accessed values
5. **Clean Up Events**: Disconnect event handlers when done

### User Experience Guidelines

1. **Immediate Feedback**: Show progress changes immediately
2. **Clear Purpose**: Make the indicator's meaning obvious
3. **Appropriate Precision**: Match precision to user needs
4. **Consistent Behavior**: Use similar patterns throughout your app
5. **Meaningful Events**: Connect progress to user actions

## Summary

Circular indicators in Fern represent the perfect fusion of mathematical precision, psychological understanding, and practical functionality. They transform abstract numerical values into intuitive visual representations that users understand instinctively.

Key takeaways:

- **Psychological Power**: Circles naturally represent completion and progress in human perception
- **Mathematical Foundation**: Arc rendering involves trigonometry and coordinate transformation
- **Flexible Configuration**: Extensive customization options for different use cases
- **Event-Driven Design**: Reactive programming support for responsive interfaces
- **Performance Optimized**: Efficient rendering and update mechanisms
- **Integration Ready**: Works seamlessly with layouts, animations, and data binding

Circular indicators exemplify Fern's philosophy: take a simple concept (showing progress), provide deep customization capabilities, but make the common cases effortless. Whether you're building a file transfer dialog, a game character sheet, or a system monitoring dashboard, circular indicators give you the tools to create engaging, informative user interfaces that feel both professional and intuitive.

Master circular indicators, and you'll have a powerful tool for transforming numerical data into compelling visual stories that engage users and provide immediate understanding of system state and progress.
