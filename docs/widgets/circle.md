# Circle Widget Guide

Circles are fundamental geometric shapes that appear everywhere in user interfaces - from buttons and icons to decorative elements and interactive indicators. In this guide, you'll learn how to create, style, and make circles interactive in Fern, while understanding the mathematical principles behind drawing perfect circles on pixel-based displays.

## Understanding Circles in Digital Graphics

Drawing a circle on a pixel grid is more complex than it first appears. A perfect mathematical circle is a continuous curve, but pixels are discrete squares. To create the illusion of a smooth circle, we need to:

1. **Calculate which pixels to fill**: Using mathematical formulas to determine which pixels best approximate the circle
2. **Handle anti-aliasing**: Making edges appear smooth despite the pixel grid
3. **Manage the center point**: Defining where the circle is positioned
4. **Control the radius**: Determining the size accurately

Fern handles all this complexity while exposing a simple, intuitive API.

## Circle Philosophy in Fern

Circles in Fern are **center-positioned and radius-defined**. This means:

- **Position** refers to the center point, not a corner
- **Radius** determines size, making scaling intuitive
- **Color** is solid fill (no outline-only circles)
- **Interactivity** is built-in through signals

This approach mirrors how we think about circles naturally - as shapes with a center and a distance extending outward.

## Your First Circle

Let's start with the simplest possible circle:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

int main() {
    // Initialize Fern
    setupFern();
    
    // Create a red circle with radius 50, centered at (200, 150)
    auto circle = Circle(50, Point(200, 150), Colors::Red);
    
    // Start the game loop
    while (shouldKeepRunning()) {
        processInput();
        render();
    }
    
    return 0;
}
```

**What's happening here?**
- `50`: Radius in pixels (the circle extends 50 pixels from its center in all directions)
- `Point(200, 150)`: Center position (200 pixels from left, 150 pixels from top)
- `Colors::Red`: Fill color using Fern's color system

The resulting circle will be 100x100 pixels total (diameter = radius × 2).

## Circle Positioning: Center vs Corner

Unlike rectangles that are often positioned by their top-left corner, circles are positioned by their center. This makes positioning intuitive:

```cpp
// Screen center (assuming 800x600 screen)
auto centerCircle = Circle(30, Point(400, 300), Colors::Blue);

// Top-left area (circle will be partially visible)
auto cornerCircle = Circle(25, Point(25, 25), Colors::Green);

// The center of cornerCircle is at (25,25), so the circle extends from (0,0) to (50,50)
```

**Visual Understanding:**
```
Screen coordinates (0,0) at top-left

    25 pixels
    |
    v
25--●────────────────
    |░░░░░            ← Circle extends 25px in all directions
    |░░░░░            from center point (25,25)
    |░░░░░
    |░░░░░
    |
```

## Making Circles Interactive

One of Fern's strengths is built-in interactivity. Circles can respond to mouse events through Fern's signal system:

```cpp
// Create an interactive button-like circle
auto button = Circle(40, Point(150, 100), Colors::Blue);

// Handle clicks
button->onClick.connect([]() {
    std::cout << "Circle was clicked!" << std::endl;
});

// Handle hover events  
button->onHover.connect([](bool isHovered) {
    if (isHovered) {
        std::cout << "Mouse entered circle" << std::endl;
    } else {
        std::cout << "Mouse left circle" << std::endl;
    }
});
```

**Understanding Signal-Slot Communication:**
- `onClick` is emitted when the circle is clicked
- `onHover` is emitted when mouse enters or leaves the circle boundary
- You can connect multiple functions to the same signal
- Signals provide type-safe, decoupled communication

### Advanced Hover Effects

```cpp
auto hoverCircle = Circle(50, Point(250, 200), Colors::DarkBlue);

// Store original color for restoration
uint32_t originalColor = Colors::DarkBlue;
uint32_t hoverColor = Colors::LightBlue;

hoverCircle->onHover.connect([hoverCircle, originalColor, hoverColor](bool isHovered) {
    if (isHovered) {
        hoverCircle->setColor(hoverColor);
    } else {
        hoverCircle->setColor(originalColor);
    }
});
```

## Dynamic Circles: Animation and Updates

Circles can be modified after creation, enabling animations and dynamic interfaces:

```cpp
// Create a circle that grows over time
auto growingCircle = Circle(10, Point(300, 200), Colors::Green);

// In your game loop:
double time = getCurrentTime();
int newRadius = 10 + (sin(time * 2) + 1) * 20; // Pulsing between 10 and 50
growingCircle->setRadius(newRadius);
```

### Moving Circles

```cpp
// Create a circle that follows the mouse
auto follower = Circle(25, Point(0, 0), Colors::Yellow);

// In your input handling:
if (input.mousePosition.x != -1) { // Mouse is on screen
    follower->setPosition(input.mousePosition);
}
```

### Color Transitions

```cpp
// Circle that changes color based on some state
auto statusCircle = Circle(30, Point(100, 100), Colors::Green);

void updateStatus(int healthPercent) {
    if (healthPercent > 75) {
        statusCircle->setColor(Colors::Green);
    } else if (healthPercent > 25) {
        statusCircle->setColor(Colors::Yellow);
    } else {
        statusCircle->setColor(Colors::Red);
    }
}
```

## Circles in Layouts

Circles work seamlessly with Fern's layout system:

```cpp
// Row of indicator circles
auto indicatorRow = Row({
    Circle(15, Point(0, 0), Colors::Green),   // Active
    Circle(15, Point(0, 0), Colors::Gray),    // Inactive
    Circle(15, Point(0, 0), Colors::Gray),    // Inactive  
    Circle(15, Point(0, 0), Colors::Gray)     // Inactive
});

// The layout system will position these circles automatically
// The Point(0, 0) positions become relative to the row container
```

### Circle Grids

```cpp
// Create a grid of clickable circles
std::vector<std::shared_ptr<Widget>> circles;

for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
        auto circle = Circle(20, Point(0, 0), Colors::LightGray);
        
        // Add click handler
        circle->onClick.connect([row, col, circle]() {
            std::cout << "Clicked circle at (" << row << ", " << col << ")" << std::endl;
            circle->setColor(Colors::Blue); // Mark as selected
        });
        
        circles.push_back(circle);
    }
}

// Arrange in a grid layout (you'd use Fern's grid layout widget)
```

## Common Circle Patterns

### Toggle Buttons

```cpp
class ToggleCircle {
public:
    ToggleCircle(Point position, uint32_t activeColor, uint32_t inactiveColor) 
        : isActive_(false), activeColor_(activeColor), inactiveColor_(inactiveColor) {
        
        circle_ = Circle(25, position, inactiveColor);
        circle_->onClick.connect([this]() { toggle(); });
    }
    
    void toggle() {
        isActive_ = !isActive_;
        circle_->setColor(isActive_ ? activeColor_ : inactiveColor_);
    }
    
    bool isActive() const { return isActive_; }
    
private:
    std::shared_ptr<CircleWidget> circle_;
    bool isActive_;
    uint32_t activeColor_, inactiveColor_;
};

// Usage
ToggleCircle powerButton(Point(100, 100), Colors::Green, Colors::Red);
```

### Progress Indicators

```cpp
class CircularProgress {
public:
    CircularProgress(Point position, int maxRadius) 
        : position_(position), maxRadius_(maxRadius), progress_(0.0f) {
        
        // Background circle
        background_ = Circle(maxRadius, position, Colors::DarkGray);
        
        // Progress circle (starts small)
        progress_ = Circle(1, position, Colors::Blue);
    }
    
    void setProgress(float percent) { // 0.0 to 1.0
        progress_ = std::clamp(percent, 0.0f, 1.0f);
        int newRadius = (int)(progress_ * maxRadius_);
        progressCircle_->setRadius(newRadius);
    }
    
private:
    std::shared_ptr<CircleWidget> background_;
    std::shared_ptr<CircleWidget> progressCircle_;
    Point position_;
    int maxRadius_;
    float progress_;
};
```

### Notification Dots

```cpp
// Small notification indicators
auto notificationDot = Circle(8, Point(iconX + 30, iconY - 5), Colors::Red);

// Hide/show based on notifications
void updateNotifications(int count) {
    if (count > 0) {
        notificationDot->setColor(Colors::Red);
    } else {
        notificationDot->setColor(Colors::Transparent); // Hide
    }
}
```

## Circle Math: Understanding the Implementation

While Fern handles the complexity, understanding circle math helps with positioning and sizing:

### Radius and Diameter
```cpp
// Relationship between radius and size
int radius = 25;
int diameter = radius * 2;    // 50
int area = 3.14159 * radius * radius; // ~1963 pixels

// For UI spacing, consider the diameter when positioning multiple circles
int spacing = 10;
auto circle1 = Circle(25, Point(50, 100), Colors::Red);
auto circle2 = Circle(25, Point(50 + 50 + spacing, 100), Colors::Blue);
//                          └─ x + diameter + spacing
```

### Distance Calculations
```cpp
// Check if two circles overlap
bool circlesOverlap(Point center1, int radius1, Point center2, int radius2) {
    int dx = center1.x - center2.x;
    int dy = center1.y - center2.y;
    double distance = sqrt(dx*dx + dy*dy);
    return distance < (radius1 + radius2);
}
```

## Performance Considerations

### Circle Rendering Cost

Circles require more computation than rectangles:
- Each pixel must be tested against the circle equation
- Larger circles = more pixels to calculate
- Many circles = multiplicative performance impact

**Optimization strategies:**
```cpp
// Use smaller circles when possible
auto efficientDot = Circle(5, position, color);  // Better than Circle(50, ...)

// Cache circles that don't change
static auto cachedIcon = Circle(20, Point(100, 100), Colors::Blue);

// Consider using rectangular approximations for very small circles
// Rectangle widgets are faster to render
```

### Memory Management

```cpp
// Circles are automatically managed with shared_ptr
auto circle = Circle(30, Point(200, 200), Colors::Green);
// No manual cleanup needed - automatic when last reference goes away

// For temporary circles, consider:
{
    auto tempCircle = Circle(10, Point(50, 50), Colors::White);
    // Automatically cleaned up when scope ends
}
```

## Advanced Circle Techniques

### Custom Circle Behaviors

```cpp
// Circles that react to global state
class LiveCircle {
public:
    LiveCircle(Point pos, std::function<uint32_t()> colorFunc) {
        circle_ = Circle(20, pos, Colors::White);
        getColor_ = colorFunc;
    }
    
    void update() {
        circle_->setColor(getColor_());
    }
    
private:
    std::shared_ptr<CircleWidget> circle_;
    std::function<uint32_t()> getColor_;
};

// Usage - circle color reflects system state
LiveCircle systemStatus(Point(50, 50), []() {
    return isSystemHealthy() ? Colors::Green : Colors::Red;
});
```

### Circle Chains and Networks

```cpp
// Connected circles with lines between them
struct CircleNode {
    std::shared_ptr<CircleWidget> circle;
    std::vector<CircleNode*> connections;
    
    CircleNode(Point pos, uint32_t color) {
        circle = Circle(15, pos, color);
    }
};

// Create network of connected circles
std::vector<CircleNode> nodes;
// ... add nodes and connections
// ... draw lines between connected nodes (using LineWidget)
```

## Troubleshooting Common Issues

### Circle Not Appearing

**Check these common issues:**

1. **Position outside screen bounds**:
```cpp
// Bad: Circle center at (50, 50) with radius 100 extends off-screen
auto offScreen = Circle(100, Point(50, 50), Colors::Red);

// Good: Account for radius in positioning
auto onScreen = Circle(50, Point(100, 100), Colors::Red);
```

2. **Color matches background**:
```cpp
// If background is black, black circles won't be visible
auto invisible = Circle(30, Point(100, 100), Colors::Black); // On black background
auto visible = Circle(30, Point(100, 100), Colors::White);   // Contrasts with background
```

3. **Radius too small**:
```cpp
// Radius 1 might be too small to see clearly
auto tinyCircle = Circle(1, Point(100, 100), Colors::Red);
auto visibleCircle = Circle(5, Point(100, 100), Colors::Red); // Better minimum
```

### Click Detection Issues

**Events not triggering:**

1. **Circle outside clickable area**: Remember the click area is the full circular region
2. **Overlapping widgets**: Higher widgets might capture clicks first
3. **Signal not connected**: Ensure you've connected to the signal

```cpp
auto circle = Circle(30, Point(100, 100), Colors::Blue);

// Make sure to connect the signal
circle->onClick.connect([]() {
    std::cout << "This will actually run!" << std::endl;
});

// Missing connection means no response:
// auto circle = Circle(30, Point(100, 100), Colors::Blue);
// // No onClick.connect() call = no click response
```

### Performance Issues

**Too many circles causing lag:**

```cpp
// Instead of many small circles:
std::vector<std::shared_ptr<CircleWidget>> manyCircles;
for (int i = 0; i < 1000; i++) {
    manyCircles.push_back(Circle(2, Point(i, 100), Colors::White));
}

// Consider: Single widget with custom rendering, or sprites
```

## Summary

Circles in Fern provide a perfect foundation for creating interactive, dynamic interfaces. Whether you're building button systems, progress indicators, or decorative elements, understanding circle positioning, interactivity, and performance characteristics will help you create beautiful, responsive user interfaces.

Key takeaways:
- **Center-positioned**: Circles are positioned by their center point, making placement intuitive
- **Built-in interactivity**: onClick and onHover signals provide easy event handling
- **Dynamic updates**: Radius, position, and color can be changed during runtime
- **Layout integration**: Circles work seamlessly with Fern's layout system  
- **Performance awareness**: Consider rendering cost with many or large circles
- **Mathematical foundation**: Understanding radius, diameter, and distance helps with positioning

The circle widget showcases Fern's philosophy: simple concepts that can be combined in sophisticated ways. Master circles, and you'll have a powerful tool for creating engaging, interactive interfaces that feel natural and responsive to users.

// Modify properties
circle->setRadius(75);
circle->setPosition(Point(200, 200));
circle->setColor(Colors::Blue);

// Get properties
int radius = circle->getRadius();
Point position = circle->getPosition();
uint32_t color = circle->getColor();
int width = circle->getWidth();    // Returns radius * 2
int height = circle->getHeight();  // Returns radius * 2
```

### Position and Size

```cpp
// Position is the center of the circle
auto circle = Circle(30, Point(100, 100), Colors::Red);

// Width and height are automatic based on radius
int totalWidth = circle->getWidth();   // 60 (radius * 2)
int totalHeight = circle->getHeight(); // 60 (radius * 2)

// Update position
circle->setPosition(Point(200, 150));
```

## Events

### Click Events

```cpp
auto circle = Circle(40, Point(100, 100), Colors::Green);

// Handle clicks
circle->onClick.connect([]() {
    std::cout << "Circle clicked!" << std::endl;
});

// Click handling returns true if circle was clicked
bool wasClicked = circle->handleInput(inputState);
```

### Hover Events

```cpp
auto circle = Circle(40, Point(100, 100), Colors::Blue);

// Handle hover state changes
circle->onHover.connect([](bool isHovered) {
    if (isHovered) {
        std::cout << "Mouse entered circle" << std::endl;
    } else {
        std::cout << "Mouse left circle" << std::endl;
    }
});
```

### Interactive Circle Example

```cpp
void createInteractiveCircle() {
    auto circle = Circle(50, Point(200, 200), Colors::Red);
    
    // Change color on hover
    circle->onHover.connect([circle](bool isHovered) {
        if (isHovered) {
            circle->setColor(Colors::Yellow);  // Highlight color
        } else {
            circle->setColor(Colors::Red);     // Normal color
        }
    });
    
    // Handle clicks
    circle->onClick.connect([circle]() {
        // Toggle between red and blue on click
        static bool isRed = true;
        if (isRed) {
            circle->setColor(Colors::Blue);
        } else {
            circle->setColor(Colors::Red);
        }
        isRed = !isRed;
    });
}
```

## Styling

### Color Options

```cpp
// Using color constants
auto redCircle = Circle(30, Point(100, 100), Colors::Red);
auto blueCircle = Circle(30, Point(200, 100), Colors::Blue);
auto greenCircle = Circle(30, Point(300, 100), Colors::Green);

// Using hex colors
auto customCircle = Circle(40, Point(150, 200), 0xFF6B35);  // Orange
auto transparentCircle = Circle(40, Point(250, 200), 0x80FF0000);  // Semi-transparent red
```

### Size Variations

```cpp
// Small circle (radius 15)
auto smallCircle = Circle(15, Point(100, 100), Colors::Red);

// Medium circle (radius 30)
auto mediumCircle = Circle(30, Point(200, 100), Colors::Green);

// Large circle (radius 60)
auto largeCircle = Circle(60, Point(350, 100), Colors::Blue);
```

### Dynamic Styling

```cpp
auto circle = Circle(30, Point(200, 200), Colors::Gray);

// Animate size
void animateCircle(int frame) {
    int radius = 30 + (int)(10 * sin(frame * 0.1));  // Oscillate between 20-40
    circle->setRadius(radius);
}

// Animate color
void animateColor(int frame) {
    uint32_t red = (uint32_t)(128 + 127 * sin(frame * 0.05));
    uint32_t green = (uint32_t)(128 + 127 * sin(frame * 0.05 + 2.0));
    uint32_t blue = (uint32_t)(128 + 127 * sin(frame * 0.05 + 4.0));
    uint32_t color = (red << 16) | (green << 8) | blue;
    circle->setColor(color);
}
```

## Examples

### Simple Button

```cpp
void createCircleButton() {
    auto button = Circle(40, Point(200, 200), Colors::Blue);
    
    // Visual feedback
    button->onHover.connect([button](bool isHovered) {
        if (isHovered) {
            button->setColor(Colors::LightBlue);
        } else {
            button->setColor(Colors::Blue);
        }
    });
    
    // Action on click
    button->onClick.connect([]() {
        std::cout << "Circle button pressed!" << std::endl;
        // Perform action here
    });
}
```

### Status Indicator

```cpp
void createStatusIndicator() {
    auto statusCircle = Circle(20, Point(50, 50), Colors::Green);
    
    // Function to update status
    auto updateStatus = [statusCircle](bool isOnline) {
        if (isOnline) {
            statusCircle->setColor(Colors::Green);   // Online
        } else {
            statusCircle->setColor(Colors::Red);     // Offline
        }
    };
    
    // Simulate status changes
    updateStatus(true);   // Start online
    
    // Add tooltip behavior
    statusCircle->onHover.connect([](bool isHovered) {
        if (isHovered) {
            // Show tooltip: "Status: Online"
        }
    });
}
```

### Progress Indicator

```cpp
void createProgressIndicator() {
    std::vector<std::shared_ptr<CircleWidget>> dots;
    
    // Create 5 dots
    for (int i = 0; i < 5; ++i) {
        auto dot = Circle(10, Point(50 + i * 30, 100), Colors::Gray);
        dots.push_back(dot);
    }
    
    // Function to update progress
    auto updateProgress = [&dots](int progress) {
        for (int i = 0; i < dots.size(); ++i) {
            if (i < progress) {
                dots[i]->setColor(Colors::Green);  // Completed
            } else {
                dots[i]->setColor(Colors::Gray);   // Pending
            }
        }
    };
    
    // Example usage
    updateProgress(3);  // 3 out of 5 steps completed
}
```

### Color Palette

```cpp
void createColorPalette() {
    std::vector<uint32_t> colors = {
        Colors::Red, Colors::Green, Colors::Blue,
        Colors::Yellow, Colors::Magenta, Colors::Cyan,
        Colors::Orange, Colors::Purple, Colors::Pink
    };
    
    std::vector<std::shared_ptr<CircleWidget>> palette;
    
    for (int i = 0; i < colors.size(); ++i) {
        int x = 50 + (i % 3) * 80;
        int y = 50 + (i / 3) * 80;
        
        auto colorCircle = Circle(25, Point(x, y), colors[i]);
        palette.push_back(colorCircle);
        
        // Handle color selection
        colorCircle->onClick.connect([colors, i]() {
            std::cout << "Selected color: " << std::hex << colors[i] << std::endl;
            // Use selected color
        });
        
        // Highlight on hover
        colorCircle->onHover.connect([colorCircle, colors, i](bool isHovered) {
            if (isHovered) {
                colorCircle->setRadius(30);  // Slightly larger
            } else {
                colorCircle->setRadius(25);  // Normal size
            }
        });
    }
}
```

### Interactive Game Element

```cpp
void createGameBall() {
    auto ball = Circle(25, Point(200, 200), Colors::Red);
    
    // Game state
    static Point velocity = {2, 3};
    static int score = 0;
    
    // Click to change direction
    ball->onClick.connect([ball]() {
        velocity.x = -velocity.x;  // Reverse horizontal direction
        score += 10;
        std::cout << "Score: " << score << std::endl;
    });
    
    // Hover effect
    ball->onHover.connect([ball](bool isHovered) {
        if (isHovered) {
            ball->setColor(Colors::Yellow);  // Highlight when hoverable
        } else {
            ball->setColor(Colors::Red);     // Normal color
        }
    });
    
    // Update function for game loop
    auto updateBall = [ball]() {
        Point currentPos = ball->getPosition();
        Point newPos = {currentPos.x + velocity.x, currentPos.y + velocity.y};
        
        // Bounce off screen edges
        if (newPos.x <= 25 || newPos.x >= Fern::getWidth() - 25) {
            velocity.x = -velocity.x;
        }
        if (newPos.y <= 25 || newPos.y >= Fern::getHeight() - 25) {
            velocity.y = -velocity.y;
        }
        
        ball->setPosition(newPos);
    };
    
    // Call updateBall() in your game loop
}
```

### Navigation Dots

```cpp
void createNavigationDots() {
    std::vector<std::shared_ptr<CircleWidget>> navDots;
    int totalPages = 5;
    int currentPage = 0;
    
    // Create navigation dots
    for (int i = 0; i < totalPages; ++i) {
        auto dot = Circle(8, Point(200 + i * 25, 400), Colors::Gray);
        navDots.push_back(dot);
        
        // Click to navigate
        dot->onClick.connect([&currentPage, &navDots, i]() {
            currentPage = i;
            updateNavDots(navDots, currentPage);
            std::cout << "Navigated to page " << (i + 1) << std::endl;
        });
        
        // Hover effect
        dot->onHover.connect([dot](bool isHovered) {
            if (isHovered) {
                dot->setRadius(10);  // Slightly larger
            } else {
                dot->setRadius(8);   // Normal size
            }
        });
    }
    
    // Function to update active dot
    auto updateNavDots = [](std::vector<std::shared_ptr<CircleWidget>>& dots, int activePage) {
        for (int i = 0; i < dots.size(); ++i) {
            if (i == activePage) {
                dots[i]->setColor(Colors::Blue);   // Active page
            } else {
                dots[i]->setColor(Colors::Gray);   // Inactive page
            }
        }
    };
    
    // Set initial active page
    updateNavDots(navDots, currentPage);
}
```

### Animated Loading Spinner

```cpp
void createLoadingSpinner() {
    std::vector<std::shared_ptr<CircleWidget>> spinnerDots;
    
    // Create 8 dots in a circle
    for (int i = 0; i < 8; ++i) {
        double angle = (i * 2.0 * M_PI) / 8.0;
        int x = 200 + (int)(40 * cos(angle));
        int y = 200 + (int)(40 * sin(angle));
        
        auto dot = Circle(5, Point(x, y), Colors::Gray);
        spinnerDots.push_back(dot);
    }
    
    // Animation function
    int animationFrame = 0;
    auto animateSpinner = [&spinnerDots, &animationFrame]() {
        for (int i = 0; i < spinnerDots.size(); ++i) {
            // Calculate opacity based on animation frame
            int opacity = 255 - ((animationFrame - i * 4) % 32) * 8;
            if (opacity < 64) opacity = 64;
            
            // Create fading effect
            uint32_t color = (opacity << 24) | 0xFFFFFF;  // White with varying alpha
            spinnerDots[i]->setColor(color);
        }
        animationFrame++;
    };
    
    // Call animateSpinner() in your render loop
}
```

## Best Practices

### 1. Appropriate Sizing

```cpp
// Choose radius based on use case
auto smallIndicator = Circle(8, Point(x, y), color);    // Small indicator
auto mediumButton = Circle(25, Point(x, y), color);     // Medium button
auto largeTarget = Circle(50, Point(x, y), color);      // Large interactive target
```

### 2. Color and Contrast

```cpp
// Ensure good contrast for visibility
auto visibleCircle = Circle(30, Point(100, 100), Colors::White);  // On dark background
auto darkCircle = Circle(30, Point(200, 100), Colors::Black);     // On light background

// Use semantic colors
auto errorIndicator = Circle(15, Point(x, y), Colors::Red);
auto successIndicator = Circle(15, Point(x, y), Colors::Green);
auto warningIndicator = Circle(15, Point(x, y), Colors::Yellow);
```

### 3. Interactive Feedback

```cpp
auto interactiveCircle = Circle(30, Point(200, 200), Colors::Blue);

// Provide visual feedback
interactiveCircle->onHover.connect([interactiveCircle](bool isHovered) {
    if (isHovered) {
        interactiveCircle->setColor(Colors::LightBlue);  // Hover state
    } else {
        interactiveCircle->setColor(Colors::Blue);       // Normal state
    }
});

// Visual feedback on click
interactiveCircle->onClick.connect([interactiveCircle]() {
    // Temporarily change color or size to show click
    interactiveCircle->setColor(Colors::DarkBlue);
    // Reset after a short delay
});
```

### 4. Responsive Design

```cpp
// Adapt to screen size
int screenWidth = Fern::getWidth();
int screenHeight = Fern::getHeight();

// Scale radius based on screen size
int baseRadius = 30;
int scaledRadius = std::max(20, (screenWidth * baseRadius) / 800);

auto responsiveCircle = Circle(scaledRadius, Point(screenWidth/2, screenHeight/2), Colors::Green);
```

### 5. Performance Considerations

```cpp
// Reuse circles when possible
std::vector<std::shared_ptr<CircleWidget>> circlePool;

// Initialize pool
for (int i = 0; i < 10; ++i) {
    auto circle = Circle(20, Point(0, 0), Colors::Gray, false);  // Don't add to manager
    circlePool.push_back(circle);
}

// Use from pool
auto getCircle = [&circlePool]() -> std::shared_ptr<CircleWidget> {
    if (!circlePool.empty()) {
        auto circle = circlePool.back();
        circlePool.pop_back();
        return circle;
    }
    return Circle(20, Point(0, 0), Colors::Gray, false);
};
```

## Troubleshooting

### Common Issues

1. **Circle Not Visible**
   ```cpp
   // Check if circle is within screen bounds
   int screenWidth = Fern::getWidth();
   int screenHeight = Fern::getHeight();
   
   // Ensure position is visible
   auto circle = Circle(30, Point(100, 100), Colors::Red);  // Visible position
   
   // Check color contrast
   auto visibleCircle = Circle(30, Point(100, 100), Colors::White);  // On dark background
   ```

2. **Click Events Not Working**
   ```cpp
   // Ensure circle is added to widget manager
   auto circle = Circle(30, Point(100, 100), Colors::Red);  // Automatically added
   
   // Or manually add
   auto circle = Circle(30, Point(100, 100), Colors::Red, false);
   addWidget(circle);
   
   // Check if position is correct
   circle->setPosition(Point(validX, validY));
   ```

3. **Hover Detection Issues**
   ```cpp
   // Ensure radius is appropriate for hover area
   auto circle = Circle(25, Point(100, 100), Colors::Blue);  // Adequate hover area
   
   // Check if other widgets are blocking input
   ```

4. **Performance Issues with Many Circles**
   ```cpp
   // Consider using object pooling
   // Or limit the number of active circles
   const int MAX_CIRCLES = 100;
   
   // Use efficient update patterns
   for (auto& circle : activeCircles) {
       circle->render();  // Only render visible circles
   }
   ```

### Debugging

```cpp
// Debug circle properties
#ifdef DEBUG
    auto circle = Circle(30, Point(100, 100), Colors::Red);
    std::cout << "Circle - Radius: " << circle->getRadius() 
              << ", Position: (" << circle->getPosition().x << ", " << circle->getPosition().y << ")"
              << ", Color: 0x" << std::hex << circle->getColor() << std::endl;
#endif
```

## Related Documentation

- [Button Widget](button.md) - For more complex interactive elements
- [Line Widget](line.md) - For drawing lines
- [Graphics Primitives](../graphics/primitives.md) - For custom drawing
- [Colors System](../graphics/colors.md) - For color management
- [Input Handling](../input/input-handling.md) - For advanced input processing

---

*This documentation covers the Fern Circle Widget. For more examples and advanced usage, see the examples directory.*
