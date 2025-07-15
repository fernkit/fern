# Center Layout

The `CenterWidget` centers a single child widget within its bounds, providing both horizontal and vertical centering. It's one of the most commonly used layout widgets for creating balanced, centered layouts.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Configuration](#configuration)
- [Centering Behavior](#centering-behavior)
- [Responsive Design](#responsive-design)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating a Center Widget

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Center a widget within specific bounds
auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
centerWidget->add(Text(Point(0, 0), "Centered Text", 3, Colors::White));

// Using helper function
auto centeredText = Center(Text(Point(0, 0), "Hello, World!", 2, Colors::White));
```

### Screen Centering

```cpp
void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Center content on the entire screen
    auto screenCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto content = Text(Point(0, 0), "Screen Centered", 3, Colors::White);
    screenCenter->add(content);
    
    addWidget(screenCenter);
}
```

## Configuration

### Basic Properties

```cpp
// Create center widget with explicit dimensions
auto centerWidget = std::make_shared<CenterWidget>(
    100, 100,  // x, y position
    400, 300   // width, height
);

// Add child widget
auto button = Button(ButtonConfig(0, 0, 200, 50).text("Centered Button"));
centerWidget->add(button);

// The button will be automatically centered within the 400x300 area
```

### Single Child Limitation

```cpp
// CenterWidget only supports one child
auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);

// This replaces any previous child
centerWidget->add(Text(Point(0, 0), "First Text", 2, Colors::White));
centerWidget->add(Text(Point(0, 0), "Second Text", 2, Colors::White)); // Replaces first

// To center multiple widgets, use a container
auto column = Column({
    Text(Point(0, 0), "First Line", 2, Colors::White),
    Text(Point(0, 0), "Second Line", 2, Colors::White)
});

centerWidget->add(column);  // Centers the entire column
```

## Centering Behavior

### Horizontal and Vertical Centering

```cpp
// The center widget calculates position as:
// childX = parentX + (parentWidth - childWidth) / 2
// childY = parentY + (parentHeight - childHeight) / 2

auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
auto button = Button(ButtonConfig(0, 0, 100, 40).text("Button"));

centerWidget->add(button);
// Button will be positioned at (150, 130) to center it in 400x300 area
```

### Automatic Sizing

```cpp
// If child has no explicit size, CenterWidget can provide dimensions
auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);

// Child without explicit size will use parent's dimensions
auto textWidget = Text(Point(0, 0), "Auto-sized", 2, Colors::White);
centerWidget->add(textWidget);
```

### Centering Different Widget Types

```cpp
// Center a button
auto buttonCenter = std::make_shared<CenterWidget>(0, 0, 400, 200);
buttonCenter->add(Button(ButtonConfig(0, 0, 150, 50).text("Centered Button")));

// Center a text widget
auto textCenter = std::make_shared<CenterWidget>(0, 0, 400, 100);
textCenter->add(Text(Point(0, 0), "Centered Text", 3, Colors::White));

// Center a complex layout
auto layoutCenter = std::make_shared<CenterWidget>(0, 0, 600, 400);
auto complexLayout = Column({
    Text(Point(0, 0), "Title", 3, Colors::Yellow),
    SizedBox(0, 20),
    Button(ButtonConfig(0, 0, 200, 40).text("Action"))
});
layoutCenter->add(complexLayout);
```

## Responsive Design

### Full Screen Centering

```cpp
void createResponsiveCenter() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Always center on full screen
    auto fullScreenCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    // Content that adapts to screen size
    int contentWidth = std::min(400, screenWidth - 40);
    int contentHeight = std::min(300, screenHeight - 40);
    
    auto content = Column({
        Text(Point(0, 0), "Responsive Content", 3, Colors::White),
        SizedBox(0, 20),
        Button(ButtonConfig(0, 0, contentWidth - 40, 50).text("Responsive Button"))
    });
    
    fullScreenCenter->add(content);
    addWidget(fullScreenCenter);
}
```

### Proportional Centering

```cpp
void createProportionalCenter() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Center within 80% of screen
    int centerWidth = (screenWidth * 80) / 100;
    int centerHeight = (screenHeight * 80) / 100;
    int centerX = (screenWidth - centerWidth) / 2;
    int centerY = (screenHeight - centerHeight) / 2;
    
    auto proportionalCenter = std::make_shared<CenterWidget>(
        centerX, centerY, centerWidth, centerHeight
    );
    
    auto content = Text(Point(0, 0), "80% Screen Center", 3, Colors::White);
    proportionalCenter->add(content);
    
    addWidget(proportionalCenter);
}
```

## Examples

### Welcome Screen

```cpp
void createWelcomeScreen() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto welcomeCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto welcomeContent = Column({
        Text(Point(0, 0), "Welcome to Fern UI", 4, Colors::White),
        SizedBox(0, 30),
        Text(Point(0, 0), "A modern C++ UI framework", 2, Colors::Gray),
        SizedBox(0, 40),
        Button(ButtonConfig(0, 0, 200, 50).text("Get Started")),
        SizedBox(0, 15),
        Button(ButtonConfig(0, 0, 200, 50).text("Learn More"))
    });
    
    welcomeCenter->add(welcomeContent);
    addWidget(welcomeCenter);
}
```

### Modal Dialog

```cpp
void createModalDialog() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Full screen overlay
    auto modalOverlay = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    // Dialog content
    auto dialogContent = Column({
        Text(Point(0, 0), "Confirm Action", 3, Colors::White),
        SizedBox(0, 20),
        Text(Point(0, 0), "Are you sure you want to proceed?", 2, Colors::Gray),
        SizedBox(0, 30),
        Row({
            Button(ButtonConfig(0, 0, 100, 40).text("Cancel")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 100, 40).text("Confirm"))
        })
    });
    
    modalOverlay->add(dialogContent);
    addWidget(modalOverlay);
}
```

### Loading Screen

```cpp
void createLoadingScreen() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto loadingCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto loadingContent = Column({
        // Loading spinner (using circles)
        Row({
            Circle(8, Point(0, 0), Colors::Blue),
            SizedBox(5, 0),
            Circle(8, Point(0, 0), Colors::Blue),
            SizedBox(5, 0),
            Circle(8, Point(0, 0), Colors::Blue)
        }),
        SizedBox(0, 20),
        Text(Point(0, 0), "Loading...", 2, Colors::White)
    });
    
    loadingCenter->add(loadingContent);
    addWidget(loadingCenter);
}
```

### Game Menu

```cpp
void createGameMenu() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto menuCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto menuContent = Column({
        Text(Point(0, 0), "GAME TITLE", 5, Colors::Yellow),
        SizedBox(0, 50),
        
        Button(ButtonConfig(0, 0, 250, 60).text("Start Game")),
        SizedBox(0, 15),
        
        Button(ButtonConfig(0, 0, 250, 60).text("Load Game")),
        SizedBox(0, 15),
        
        Button(ButtonConfig(0, 0, 250, 60).text("Options")),
        SizedBox(0, 15),
        
        Button(ButtonConfig(0, 0, 250, 60).text("Exit")),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "v1.0.0", 1, Colors::Gray)
    });
    
    menuCenter->add(menuContent);
    addWidget(menuCenter);
}
```

### Settings Panel

```cpp
void createSettingsPanel() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Center the settings panel
    auto settingsCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto settingsContent = Column({
        Text(Point(0, 0), "Settings", 3, Colors::White),
        SizedBox(0, 30),
        
        // Setting rows
        Row({
            Text(Point(0, 0), "Volume:", 2, Colors::White),
            SizedBox(50, 0),
            Text(Point(0, 0), "75%", 2, Colors::Gray)
        }),
        SizedBox(0, 20),
        
        Row({
            Text(Point(0, 0), "Quality:", 2, Colors::White),
            SizedBox(50, 0),
            Text(Point(0, 0), "High", 2, Colors::Gray)
        }),
        SizedBox(0, 20),
        
        Row({
            Text(Point(0, 0), "Fullscreen:", 2, Colors::White),
            SizedBox(50, 0),
            Text(Point(0, 0), "Yes", 2, Colors::Gray)
        }),
        SizedBox(0, 40),
        
        Row({
            Button(ButtonConfig(0, 0, 100, 40).text("Save")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 100, 40).text("Cancel"))
        })
    });
    
    settingsCenter->add(settingsContent);
    addWidget(settingsCenter);
}
```

### Error Dialog

```cpp
void createErrorDialog() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto errorCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto errorContent = Column({
        Text(Point(0, 0), "Error", 3, Colors::Red),
        SizedBox(0, 20),
        Text(Point(0, 0), "An unexpected error occurred.", 2, Colors::White),
        SizedBox(0, 10),
        Text(Point(0, 0), "Please try again later.", 2, Colors::Gray),
        SizedBox(0, 30),
        Button(ButtonConfig(0, 0, 150, 40).text("OK"))
    });
    
    errorCenter->add(errorContent);
    addWidget(errorCenter);
}
```

### Card Layout

```cpp
void createCenteredCard() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto cardCenter = std::make_shared<CenterWidget>(
        0, 0, screenWidth, screenHeight
    );
    
    auto cardContent = Column({
        // Card header
        Text(Point(0, 0), "User Profile", 3, Colors::White),
        SizedBox(0, 20),
        
        // Profile picture placeholder
        Circle(50, Point(0, 0), Colors::Blue),
        SizedBox(0, 20),
        
        // User info
        Text(Point(0, 0), "John Doe", 2, Colors::White),
        SizedBox(0, 5),
        Text(Point(0, 0), "john.doe@example.com", 2, Colors::Gray),
        SizedBox(0, 30),
        
        // Action buttons
        Row({
            Button(ButtonConfig(0, 0, 100, 40).text("Edit")),
            SizedBox(15, 0),
            Button(ButtonConfig(0, 0, 100, 40).text("Delete"))
        })
    });
    
    cardCenter->add(cardContent);
    addWidget(cardCenter);
}
```

## Best Practices

### 1. Use for Single Items

```cpp
// Good: Center a single complex widget
auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
auto complexContent = Column({
    Text(Point(0, 0), "Title", 3, Colors::White),
    Button(ButtonConfig(0, 0, 200, 40).text("Action"))
});
centerWidget->add(complexContent);

// Avoid: Trying to center multiple separate widgets
// Use a container widget instead
```

### 2. Responsive Centering

```cpp
// Adapt to screen size
int screenWidth = Fern::getWidth();
int screenHeight = Fern::getHeight();

auto responsiveCenter = std::make_shared<CenterWidget>(
    0, 0, screenWidth, screenHeight
);

// Content that scales with screen
int contentWidth = std::min(500, screenWidth - 40);
auto content = Button(ButtonConfig(0, 0, contentWidth, 50).text("Responsive"));
responsiveCenter->add(content);
```

### 3. Proper Sizing

```cpp
// Ensure center widget has proper dimensions
auto centerWidget = std::make_shared<CenterWidget>(
    0, 0, 
    400, 300  // Explicit dimensions for proper centering
);

// Child should have appropriate size
auto child = Button(ButtonConfig(0, 0, 200, 50).text("Properly Sized"));
centerWidget->add(child);
```

### 4. Nested Centering

```cpp
// Center within center for complex layouts
auto outerCenter = std::make_shared<CenterWidget>(0, 0, screenWidth, screenHeight);
auto innerCenter = std::make_shared<CenterWidget>(0, 0, 400, 300);

auto content = Text(Point(0, 0), "Nested Center", 2, Colors::White);
innerCenter->add(content);
outerCenter->add(innerCenter);
```

### 5. Performance Considerations

```cpp
// Reuse center widgets when possible
static std::shared_ptr<CenterWidget> dialogCenter = nullptr;

auto getDialogCenter = []() {
    if (!dialogCenter) {
        dialogCenter = std::make_shared<CenterWidget>(
            0, 0, Fern::getWidth(), Fern::getHeight()
        );
    }
    return dialogCenter;
};
```

## Troubleshooting

### Common Issues

1. **Child Not Appearing Centered**
   ```cpp
   // Ensure center widget has proper dimensions
   auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);  // Explicit size
   
   // Ensure child has valid dimensions
   auto child = Button(ButtonConfig(0, 0, 200, 50).text("Button"));  // Valid size
   centerWidget->add(child);
   ```

2. **Center Widget Not Visible**
   ```cpp
   // Check if center widget is added to manager
   auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
   centerWidget->add(child);
   addWidget(centerWidget);  // Don't forget this!
   ```

3. **Centering Not Working on Screen Resize**
   ```cpp
   // Recreate center widget with new dimensions
   void onScreenResize() {
       int newWidth = Fern::getWidth();
       int newHeight = Fern::getHeight();
       
       auto newCenter = std::make_shared<CenterWidget>(0, 0, newWidth, newHeight);
       newCenter->add(content);
       // Replace old center widget
   }
   ```

4. **Child Extends Beyond Center Bounds**
   ```cpp
   // Ensure child size is smaller than center widget
   auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
   auto child = Button(ButtonConfig(0, 0, 350, 250).text("Fits"));  // Smaller than 400x300
   centerWidget->add(child);
   ```

### Performance Issues

1. **Frequent Recentering**
   ```cpp
   // Avoid creating new center widgets frequently
   // Cache and reuse when possible
   static std::shared_ptr<CenterWidget> cachedCenter = nullptr;
   
   if (!cachedCenter) {
       cachedCenter = std::make_shared<CenterWidget>(0, 0, screenWidth, screenHeight);
   }
   ```

2. **Complex Child Layouts**
   ```cpp
   // Pre-calculate child dimensions for better performance
   auto child = Column({...});  // Complex layout
   // Let child calculate its size first
   centerWidget->add(child);
   ```

### Debugging

```cpp
// Debug center widget dimensions and child positioning
#ifdef DEBUG
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, 400, 300);
    auto child = Button(ButtonConfig(0, 0, 200, 50).text("Debug"));
    centerWidget->add(child);
    
    std::cout << "Center: " << centerWidget->getX() << ", " << centerWidget->getY() 
              << ", " << centerWidget->getWidth() << "x" << centerWidget->getHeight() << std::endl;
    std::cout << "Child: " << child->getX() << ", " << child->getY() 
              << ", " << child->getWidth() << "x" << child->getHeight() << std::endl;
#endif
```

## Related Documentation

- [Column Layout](column.md) - For vertical arrangements
- [Row Layout](row.md) - For horizontal arrangements
- [Layout System Overview](overview.md) - For general layout concepts
- [Padding Layout](padding.md) - For adding space around widgets
- [Button Widget](../widgets/button.md) - Common centered widget
- [Text Widget](../widgets/text.md) - Another common centered widget

---

*This documentation covers the Fern Center Layout system. For more examples and advanced usage, see the examples directory.*
