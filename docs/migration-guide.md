# Fern Graphics: C to C++ Migration Guide

This guide provides comprehensive instructions for transitioning from the C implementation of Fern Graphics to the newer, more powerful C++ implementation.

## Introduction

Fern Graphics C++ is a complete rewrite of the original C library, designed with modern software engineering practices. This guide will help you transition your existing C-based projects to take advantage of the new features and improvements.

## Advantages of the C++ Implementation

- **Object-oriented design**: Classes, inheritance, and encapsulation for better code organization
- **Memory safety**: RAII principles for automatic resource management
- **Modern C++ features**: Lambdas, smart pointers, and standard library containers
- **Type safety**: Stronger type checking and fewer implicit conversions
- **Component-based architecture**: More modular and maintainable codebase
- **Layout system**: Flutter-inspired responsive UI components
- **Event handling**: Signal-based event system for better UI interactions
- **Extensibility**: Easier to extend through inheritance and composition

## File Structure and Organization

The C++ implementation uses a more organized directory structure:

```
include/fern/                   # Public API headers
├── core/                       # Core functionality
│   ├── canvas.hpp              # Canvas management
│   ├── input.hpp               # Input handling
│   ├── signal.hpp              # Event system
│   ├── types.hpp               # Common types
│   └── widget_manager.hpp      # Widget management
├── graphics/                   # Graphics primitives
│   ├── colors.hpp              # Color definitions
│   └── primitives.hpp          # Basic drawing functions
├── text/                       # Text rendering
│   └── font.hpp                # Font system
├── ui/                         # User interface components
│   ├── widget.hpp              # Base widget class
│   ├── widgets/                # Basic widgets
│   │   ├── button_widget.hpp   # Button widget
│   │   ├── circle_widget.hpp   # Circle widget
│   │   ├── line_widget.hpp     # Line widget
│   │   └── text_widget.hpp     # Text widget
│   ├── layout/                 # Layout system
│   │   └── layout.hpp          # Layout widgets (Column, Row, etc.)
│   └── containers/             # Container widgets
│       └── container.hpp       # Container widget
└── fern.hpp                    # Main include file
```

## Step-by-Step Migration Process

### 1. Update Include Statements

**C Implementation:**
```c
#include "fern.h"  // Or "fern.c" if using single-file approach
```

**C++ Implementation:**
```cpp
#include <fern/fern.hpp>  // Main include
using namespace Fern;     // Optional, for convenience
```

### 2. Initialize the Library

**C Implementation:**
```c
uint32_t pixels[WIDTH * HEIGHT];
FernCanvas canvas = {pixels, WIDTH, HEIGHT};
runApp(canvas);
```

**C++ Implementation:**
```cpp
// Option 1: Manual buffer management
uint32_t pixels[WIDTH * HEIGHT];
Fern::initialize(pixels, WIDTH, HEIGHT);

// Option 2: Automatic buffer management (recommended)
Fern::initialize();  // Auto-detect canvas size from browser
```

### 3. Set Up the Render Loop

**C Implementation:**
```c
void draw() {
    // Drawing code here
}

int main() {
    // Setup code...
    fern_set_draw_callback(draw);
    fern_start_render_loop();
    return 0;
}
```

**C++ Implementation:**
```cpp
void draw() {
    // Drawing code here
}

int main() {
    Fern::initialize();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### 4. Update Drawing Code

**C Implementation:**
```c
void draw() {
    frect(pixels, HEIGHT, WIDTH, Colors_blue, 10, 10, 100, 50);
    fcircle(pixels, HEIGHT, WIDTH, 150, 100, 30, Colors_red);
}
```

**C++ Implementation:**
```cpp
void draw() {
    Draw::rect(10, 10, 100, 50, Colors::Blue);
    Draw::circle(150, 100, 30, Colors::Red);
}
```

### 5. Update Widget Creation

**C Implementation:**
```c
CircleWidget(radius(20), position(Point_create(100, 100)), color(Colors_red));
TextWidget(position(Point_create(50, 50)), text("Hello"), scale(2), color(Colors_white));
```

**C++ Implementation:**
```cpp
Circle(20, Point(100, 100), Colors::Red);
Text(Point(50, 50), "Hello", 2, Colors::White);
```

### 6. Update Event Handling

**C Implementation:**
```c
ButtonConfig btn = {
    .x = 100, .y = 100,
    .width = 200, .height = 50,
    .normal_color = Colors_blue,
    .label = "Click Me",
    .on_click = myButtonHandler
};
ButtonWidget(btn);

void myButtonHandler() {
    // Handle click
}
```

**C++ Implementation:**
```cpp
ButtonConfig btn = {
    .x = 100, .y = 100,
    .width = 200, .height = 50,
    .normalColor = Colors::Blue,
    .label = "Click Me"
};
auto button = Button(btn);

// Connect event handler with lambda
button->onClick.connect([]() {
    // Handle click
});
```

## API Changes and Equivalents

| C Function/Type | C++ Equivalent | Notes |
|-----------------|----------------|-------|
| `frect()` | `Draw::rect()` | Namespace-based drawing functions |
| `Colors_red` | `Colors::Red` | Constants in Colors namespace |
| `Point_create(x, y)` | `Point(x, y)` | Constructor-based creation |
| `ButtonWidget()` | `Button()` | Returns shared_ptr to widget |
| `current_input` | `Input::getState()` | Access through function |
| `button_click_fn(fn)` | `button->onClick.connect(fn)` | Signal-based events |

## New Layout System

The C++ implementation introduces a powerful layout system inspired by Flutter:

```cpp
// Create a column layout
auto mainLayout = Column({
    // Add title at top
    Text(Point(0, 0), "LAYOUT EXAMPLE", 3, Colors::White, false),
    
    // Center a circle
    Circle(50, Point(0, 0), Colors::Blue, false),
    
    // Add a button
    Button({
        .width = 200,
        .height = 50,
        .normalColor = Colors::Green,
        .label = "CLICK ME"
    }, false)
});
```

This layout system handles:

- **Automatic positioning** of elements relative to each other
- **Responsive resizing** when the window size changes
- **Proper alignment** of child elements within their parent container

## Working with Widgets

All widgets in the C++ implementation return smart pointers, allowing you to store references to modify them later:

```cpp
// Create widgets and store references
auto statusText = Text(Point(50, 50), "Ready", 2, Colors::White);
auto mainCircle = Circle(30, Point(100, 100), Colors::Blue);

// Modify them later
void onButtonClick() {
    statusText->setText("Button clicked!");
    mainCircle->setRadius(mainCircle->getRadius() + 10);
}
```

## Responsive Design

The C++ implementation makes responsive design much easier:

```cpp
void setupUI() {
    int width = Fern::getWidth();   // Get current width
    int height = Fern::getHeight(); // Get current height
    
    // Position elements relative to canvas size
    Text(Point(width/2, height/10), "TITLE", 3, Colors::White);
    Circle(height/5, Point(width/2, height/2), Colors::Blue);
}

void draw() {
    static int lastWidth = 0;
    static int lastHeight = 0;
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Rebuild UI when size changes
    if (width != lastWidth || height != lastHeight) {
        WidgetManager::getInstance().clear();
        setupUI();
        
        lastWidth = width;
        lastHeight = height;
    }
}
```

## Common Patterns and Examples

### Creating a Button with Click Handler

```cpp
auto button = Button({
    .x = 100,
    .y = 100,
    .width = 200,
    .height = 50,
    .normalColor = Colors::Blue,
    .hoverColor = Colors::LightBlue,
    .pressColor = Colors::DarkBlue,
    .label = "CLICK ME",
    .textScale = 2,
    .textColor = Colors::White
});

button->onClick.connect([]() {
    std::cout << "Button clicked!" << std::endl;
});
```

### Creating a Simple Game Loop

```cpp
struct GameState {
    float playerX = 100;
    float playerY = 100;
    float velocity = 0;
} game;

void update() {
    // Get input state
    const auto& input = Input::getState();
    
    // Update game logic
    if (input.keyPressed[Keys::Right]) game.playerX += 5;
    if (input.keyPressed[Keys::Left]) game.playerX -= 5;
    
    // Handle physics
    game.velocity += 0.5;  // Gravity
    game.playerY += game.velocity;
    
    // Ground collision
    if (game.playerY > Fern::getHeight() - 50) {
        game.playerY = Fern::getHeight() - 50;
        game.velocity = 0;
    }
}

void draw() {
    // Clear screen
    Draw::fill(Colors::Black);
    
    // Update game state
    update();
    
    // Draw player
    Draw::circle(game.playerX, game.playerY, 20, Colors::Red);
    
    // Draw ground
    Draw::rect(0, Fern::getHeight() - 20, Fern::getWidth(), 20, Colors::Green);
}
```

## Troubleshooting

### Common Issues and Solutions

#### Widgets Not Showing Up

- Check that you're adding them to the WidgetManager
- Verify positions are within canvas bounds
- Ensure draw callback is properly registered

#### Layout Issues

- Make sure all child widgets specify `false` for the `addToManager` parameter
- Verify that the layout widget itself is added to the manager
- Check that widget size calculations are correct

#### Event Handlers Not Working

- Ensure event connections are made after widget creation
- Check that widgets are properly positioned for input detection
- After window resize, reconnect event handlers if widgets are recreated

## Future Development

The C++ implementation is actively being developed with these upcoming features:

- More layout options (Grid, Stack, etc.)
- Animation system for transitions
- Theme support for consistent styling
- Advanced input handling (multi-touch, gestures)
- Enhanced text rendering capabilities
- More widget types (sliders, dropdowns, etc.)

## Contributing

We welcome contributions to Fern Graphics! To contribute:

1. Fork the repository
2. Create a feature branch
3. Make your changes with appropriate tests
4. Submit a pull request

Please see CONTRIBUTING.md for more detailed instructions.

---

By following this guide, you should be able to successfully migrate your projects from the C to C++ implementation of Fern Graphics and take advantage of all the new features and improvements.