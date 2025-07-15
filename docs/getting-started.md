# Getting Started with Fern UI Framework

Fern is a modern, cross-platform UI framework for C++ that provides a simple yet powerful way to build interactive applications. This guide will help you create your first Fern application.

## Table of Contents
- [Quick Start](#quick-start)
- [Your First Application](#your-first-application)
- [Understanding the Basics](#understanding-the-basics)
- [Adding Interactivity](#adding-interactivity)
- [Layout System](#layout-system)
- [Next Steps](#next-steps)

## Quick Start

### Prerequisites
- C++ compiler (GCC 7+ or Clang 6+)
- CMake 3.10+
- Git

### Installation
1. Clone the Fern repository:
```bash
git clone https://github.com/fernkit/fern.git
cd fern
```

2. Build the framework:
```bash
./build.sh linux  # For Linux
./build.sh web    # For Web (requires Emscripten)
```

3. Test your installation:
```bash
python3 ./cli/terra_cli.py fire examples/cpp/new/01_hello_text.cpp -p linux
```

## Your First Application

Let's create a simple "Hello, World!" application:

### Create the File
Create a new file `hello_world.cpp`:

```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Get canvas dimensions
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create centered text
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Text(Point(0, 0), "Hello, Fern!", 3, Colors::White));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Run the Application
```bash
python3 ./cli/terra_cli.py fire hello_world.cpp -p linux
```

You should see a window with "Hello, Fern!" displayed in the center.

## Understanding the Basics

### Core Concepts

#### 1. Initialization
Every Fern application starts with `Fern::initialize()`:
```cpp
int main() {
    Fern::initialize();  // Initialize the framework
    setupUI();          // Set up your UI
    Fern::setDrawCallback(draw);  // Set drawing function
    Fern::startRenderLoop();      // Start the main loop
    return 0;
}
```

#### 2. Drawing Function
The `draw()` function is called every frame:
```cpp
void draw() {
    Draw::fill(Colors::DarkBlue);  // Clear with dark blue
    // Additional drawing code goes here
}
```

#### 3. Widget System
Fern uses a widget-based architecture:
- **Widgets** are UI elements (buttons, text, etc.)
- **Layout widgets** arrange other widgets
- **Container widgets** group related widgets

#### 4. Adding Widgets
Use `addWidget()` to add widgets to the screen:
```cpp
auto myButton = Button(ButtonConfig(100, 100, 200, 50, "Click Me"));
addWidget(myButton);
```

## Adding Interactivity

### Button Example
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Create button style
    ButtonStyle style;
    style.normalColor(Colors::Blue)
        .hoverColor(Colors::LightBlue)
        .pressColor(Colors::DarkBlue)
        .textColor(Colors::White)
        .textScale(2);
    
    // Create button
    auto button = Button(ButtonConfig(300, 250, 200, 60, "Click Me!").style(style));
    
    // Add click handler
    button->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    addWidget(button);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Text Input Example
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Create text input
    auto textInput = TextInput(TextInputConfig(200, 200, 300, 40, "Type here..."));
    
    // Handle text changes
    textInput->onTextChange.connect([](const std::string& text) {
        std::cout << "Text changed to: " << text << std::endl;
    });
    
    addWidget(textInput);
}

void draw() {
    Draw::fill(Colors::Gray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

## Layout System

Fern provides powerful layout widgets for organizing your UI:

### Column Layout
```cpp
void setupUI() {
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Title", 4, Colors::White),
        SizedBox(0, 20),  // Spacing
        Text(Point(0, 0), "Subtitle", 2, Colors::Gray),
        SizedBox(0, 30),
        Button(ButtonConfig(0, 0, 150, 40, "Action").style(ButtonStyle()))
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    
    addWidget(centerWidget);
}
```

### Row Layout
```cpp
void setupUI() {
    std::vector<std::shared_ptr<Widget>> buttons = {
        Button(ButtonConfig(0, 0, 100, 40, "OK").style(ButtonStyle())),
        SizedBox(20, 0),  // Horizontal spacing
        Button(ButtonConfig(0, 0, 100, 40, "Cancel").style(ButtonStyle()))
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Row(buttons));
    
    addWidget(centerWidget);
}
```

## Common Patterns

### 1. Responsive Design
```cpp
void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Button takes 50% of screen width
    int buttonWidth = width * 0.5;
    int buttonHeight = 50;
    
    auto button = Button(ButtonConfig(0, 0, buttonWidth, buttonHeight, "Responsive"));
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(button);
    addWidget(centerWidget);
}
```

### 2. State Management
```cpp
static int counter = 0;
static std::shared_ptr<TextWidget> counterText;

void setupUI() {
    counterText = Text(Point(0, 0), "Count: 0", 3, Colors::White);
    
    auto incrementButton = Button(ButtonConfig(0, 0, 120, 40, "Increment"));
    incrementButton->onClick.connect([]() {
        counter++;
        counterText->setText("Count: " + std::to_string(counter));
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        counterText,
        SizedBox(0, 20),
        incrementButton
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}
```

## Next Steps

Now that you understand the basics, explore these topics:

1. **Widgets**: Learn about specific widgets
   - [Button Widget](widgets/button.md)
   - [Text Widget](widgets/text.md)
   - [Text Input Widget](widgets/text-input.md)

2. **Layout System**: Master layout management
   - [Layout Overview](layout/overview.md)
   - [Column Layout](layout/column.md)
   - [Row Layout](layout/row.md)

3. **Styling**: Customize your UI
   - [Colors System](graphics/colors.md)
   - [Styling Guide](graphics/styling.md)

4. **Advanced Topics**: 
   - [Input Handling](input/input-handling.md)
   - [Event System](input/events.md)
   - [Cross-Platform Development](platform/cross-platform.md)

## Troubleshooting

### Common Issues

#### Build Errors
- Ensure you have the correct compiler version
- Check that all dependencies are installed
- Try cleaning and rebuilding: `rm -rf build && ./build.sh linux`

#### Runtime Errors
- Check console output for error messages
- Verify all widgets are properly initialized
- Ensure proper order of initialization calls

#### Layout Issues
- Remember to set proper dimensions for container widgets
- Check that child widgets have appropriate sizes
- Use `SizedBox` for spacing between widgets

### Getting Help
- Check the [API Reference](api-reference.md)
- Look at [Example Projects](examples/basic.md)
- Visit the project repository for issues and discussions

---

**Next**: [Button Widget Guide](widgets/button.md)
