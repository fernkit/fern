# Circle Widget

The `CircleWidget` provides a simple way to display circular shapes with customizable radius, position, and color. It supports click and hover events, making it useful for buttons, indicators, and interactive elements.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Events](#events)
- [Styling](#styling)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating a Circle

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic circle
auto circle = Circle(50, Point(100, 100), Colors::Red);

// Without adding to manager
auto circle = Circle(30, Point(200, 200), Colors::Blue, false);
```

### Using CircleWidget Class

```cpp
// Create with constructor
auto circle = std::make_shared<CircleWidget>(
    40,                    // radius
    Point(150, 150),      // position
    Colors::Green         // color
);

// Add to widget manager
addWidget(circle);
```

## Configuration

### Basic Properties

```cpp
// Create circle
auto circle = Circle(50, Point(100, 100), Colors::Red);

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
