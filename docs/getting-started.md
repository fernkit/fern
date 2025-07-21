# Getting Started with Fern

Welcome to Fern, a interesting approach to building user interfaces. This guide will take you from complete beginner to creating your first interactive application, while teaching you fundamental concepts about how user interfaces actually work under the hood.

## What is Fern?

Fern is a zero-dependency UI library written in C++ that takes a fundamentally different approach to user interface development. Unlike traditional frameworks that rely on browser APIs, GPU frameworks, or existing graphics libraries, Fern manually paints every single pixel on the screen. It's part of a larger ecosystem called FernKit, which includes several interconnected tools designed to work together seamlessly.

Think of it like this: if you've ever wondered how text appears on your screen, how buttons know when you click them, or how layouts automatically arrange themselves, Fern exposes all of these low-level details while still providing a clean, modern API. It's educational by design, letting you see and understand the building blocks that other frameworks hide from you.

### The FernKit Ecosystem

Fern doesn't exist in isolation. It's part of FernKit, a collection of tools that work together like components of a natural ecosystem:

- **Terra**: The underlying orchestrator that manages the entire development experience. Think of Terra as the soil and bedrock - it provides the foundation that nurtures everything else. Terra handles project management, build processes, and coordinates between different parts of your application.

- **Fern**: The UI library itself, growing from Terra's foundation. Like a real fern plant, it starts small but can grow into complex, beautiful interfaces. It renders everything from basic shapes to sophisticated layouts.

- **Gleeb**: A Language Server Protocol (LSP) implementation that provides intelligent code completion, error checking, and development assistance. Like beneficial bacteria in soil that help plants grow, Gleeb works quietly in the background to enhance your development experience with Fern.

As the project motto says: "A fern is small. It grows. It renders from nothing."

### How Fern is Different

Fern doesn't use browser canvas APIs, WebGL, DirectX, OpenGL, or any other graphics frameworks. Instead, it implements everything manually:

- **Manual pixel-by-pixel rendering**: Every dot of color you see is explicitly calculated and placed by Fern's rendering engine
- **Custom drawing pipeline**: Lines, rectangles, circles, and text are all drawn using mathematical algorithms that manipulate individual pixels
- **Declarative layout engine**: Inspired by Flutter's widget tree, you describe what you want and Fern figures out how to arrange it
- **Cross-platform compatibility**: The same code runs natively on Linux and compiles to WebAssembly for browsers
- **Zero external dependencies**: No graphics libraries, no framework dependencies - just pure C++ and mathematical rendering

### Notable Features

- **Educational by Design**: See exactly how UI rendering works at the pixel level
- **Declarative Layouts**: Use familiar concepts like Rows, Columns, and Containers to build interfaces
- **Custom Drawing Pipeline**: Complete control over how everything looks, with no "black box" rendering
- **Cross-Platform**: Native performance on desktop, WebAssembly for web deployment
- **Lightweight**: No massive framework dependencies or complex build chains
- **Signal-Slot System**: Clean, type-safe event handling inspired by Qt and modern reactive programming

### Why Fern Exists

Most developers never get to see how user interfaces actually work. We use high-level frameworks that hide the complexity, but this also hides the understanding. Fern exists to bridge that gap - it's for developers who want to learn how text rendering works, how input events bubble through widget hierarchies, how layout algorithms calculate positions, and how all of this comes together to create the interactive applications we use every day.

It's a graphics engine for people who want to understand graphics engines, a UI framework for those curious about what happens under the hood of UI frameworks.

## Prerequisites

Before we begin, you'll need several tools and libraries installed on your system. Fern development currently works best on Linux systems, though web deployment works everywhere.

### Required System Tools

1. **C++ Compiler**: GCC 7+ or Clang 6+ for compiling Fern applications
2. **Build Tools**: Make and CMake for build system management
3. **Git**: For cloning repositories and version control
4. **Python 3**: Required for the Terra CLI tool
5. **pkg-config**: For managing library dependencies
6. **Node.js**: Needed for the Gleeb LSP server (optional, for enhanced development experience)

### Required System Libraries

Fern requires several system libraries for graphics and windowing support:

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config cmake make git python3
sudo apt-get install libx11-dev libxext-dev libfontconfig1-dev libfreetype6-dev
```

**CentOS/RHEL/Fedora:**
```bash
sudo dnf groupinstall 'Development Tools'
sudo dnf install cmake pkgconfig make git python3 libX11-devel libXext-devel fontconfig-devel freetype-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake pkg-config make git python3 libx11 libxext fontconfig freetype2
```

### Optional: Emscripten SDK (for Web Development)

Emscripten is crucial for web deployment as it compiles C++ code to WebAssembly. Install it if you plan to deploy your applications to the web:

```bash
# Clone the Emscripten repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest version
./emsdk install latest
./emsdk activate latest

# Set up environment variables for current session
source ./emsdk_env.sh
```

To make Emscripten available permanently, add it to your shell profile:

```bash
# Add this line to your ~/.bashrc or ~/.zshrc
echo 'source /path/to/emsdk/emsdk_env.sh' >> ~/.bashrc

# Reload your shell configuration
source ~/.bashrc
```

Replace `/path/to/emsdk` with the actual path where you cloned the emsdk repository.

### Platform Support

Currently, Fern development is optimized for Linux systems. While the compiled applications can run on both native X11 and browsers (WebAssembly), the development tools and build process work best on Linux distributions.

## Installation

Now that you have the prerequisites, let's install Fern and the FernKit tools:

### 1. Clone the Repository

```bash
git clone https://github.com/fernkit/fern.git
cd fern
```

### 2. Run the Installation Script

The Fern repository includes an installation script that sets up everything you need:

```bash
# Make the install script executable
chmod +x install.sh

# Run the installation
./install.sh
```

This installation script will:

- **Install Terra CLI**: The project management and build tool, typically installed to `~/.local/bin/terra`
- **Build Fern C++ Library**: Compiles the core Fern framework for your system
- **Install Gleeb LSP**: Sets up the language server for enhanced development experience
- **Configure Environment**: Sets up necessary environment variables and paths

The tools are usually installed to standard locations:
- Terra CLI: `~/.local/bin/` (make sure this is in your PATH)
- Fern library: Built locally in the project directory
- Gleeb LSP: Installed as a Node.js package

### 3. Verify Installation

After installation, you should be able to use the Terra CLI (in CLI, terra and fern can be used interchangeably):

```bash
fern --help
```

If you see the Terra help message, you're ready to start developing with Fern!

You can also run a comprehensive system health check:

```bash
fern bloom
```

This will verify that all dependencies are installed correctly and provide troubleshooting tips if needed.

## Creating Your First Project

Now that Fern is installed, let's create your first project to understand how everything works together.

### Project Creation

Use the Terra CLI to create a new Fern project:

```bash
# Create a new project directory and initialize it
fern sprout my_first_app

# Navigate into the project
cd my_first_app
```

This creates a new directory with a complete Fern project structure.

### Understanding the Project Structure

Let's examine what `fern sprout` created for you:

```
my_first_app/
├── assets/          # Static resources (images, fonts, data files)
├── build/           # Temporary build files and compiled executables
│   └── main         # Your compiled application (generated)
├── examples/        # Sample code and tutorials for reference
├── fern.yaml        # Project configuration (dependencies, build settings)
├── lib/             # Your main source code directory
│   └── main.cpp     # Application entry point (this is where you code!)
├── linux/           # Linux-specific configuration files
├── README.md        # Project documentation
└── web/             # Web deployment configuration
    └── template.html # HTML template for browser builds

6 directories, 5 files
```

**Key Directory Explanations:**

- **`lib/main.cpp`**: This is your main coding workspace. When you run `fern fire` without specifying a file, it automatically looks for and compiles this file.
- **`build/`**: Contains temporary compilation files and your final executable. Fern manages this automatically - you rarely need to touch it.
- **`web/template.html`**: The HTML shell used when compiling for web. You can customize this to change how your app appears in browsers.
- **`assets/`**: Store images, fonts, configuration files, or any other resources your app needs.
- **`fern.yaml`**: Project configuration including build settings, dependencies, and platform-specific options.
- **`examples/`**: Contains sample code demonstrating various Fern features - great for learning!

**Important**: The `lib/` directory is special - `fern fire` automatically looks here for your main source code, so you don't need to specify file paths when developing.

### Testing Your Setup

Let's make sure everything works by building and running the default project:

```bash
# Navigate to your project directory
cd my_first_app

# Build and run for Linux (default)
fern fire

# Or build and run for web browsers
fern fire -p web
```

You should see a simple "Hello Fern" application window open (or web page). If you encounter any issues, run `fern bloom --troubleshoot` for debugging help.

**Success!** Your Fern development environment is now ready. The project structure is in place, and you can start customizing `lib/main.cpp` to build your own applications.

## Understanding Terra CLI

Terra is your primary interface for working with Fern projects. It handles compilation, project management, and development workflow. Here are the main commands you'll use:

- **`fern fire`**: Compile and run a Fern application
- **`fern bloom`**: Performs system checks related to fern
- **`fern sprout`**: Initialize a new project in the current directory
- **`fern prepare`**: Builds project binaries for Linux or web

For example, to compile and run a Fern program:

```bash
fern fire my_app.cpp -p linux    # Compile and run for Linux
fern fire my_app.cpp -p web      # Compile and run for WebAssembly
```

The Terra CLI streamlines the entire development process, handling the complex build configurations so you can focus on creating great user interfaces.

Note: The framework also includes direct build scripts (`./build.sh`, `fern-cli.sh`) and manual compilation options, but these are primarily used for framework development itself, not for building applications with Fern.

## Your First Custom Application

Now that you have a working Fern project with the basic structure, let's modify the `lib/main.cpp` file to create your first custom application. This will help you understand the fundamentals of how Fern works.

Open `lib/main.cpp` in your favorite editor - this is where we'll write all our application code.

### The Basic Structure

Every Fern application follows a similar pattern. At its core, a Fern program is a loop: initialize the framework, set up your user interface, define what to draw each frame, and then start the main loop that handles events and rendering.

Here's the skeleton every Fern application uses:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Create and configure your widgets here
}

void draw() {
    // This function is called every frame
    // Clear the screen and draw background elements here
}

int main() {
    Fern::initialize();           // Initialize the framework
    setupUI();                   // Set up your user interface
    Fern::setDrawCallback(draw); // Tell Fern what to draw each frame
    Fern::startRenderLoop();     // Start the main application loop
    return 0;
}
```

Let's examine each part:

**`Fern::initialize()`**: This sets up the rendering system, allocates the pixel buffer, and prepares the framework for drawing. You can optionally specify dimensions like `Fern::initialize(800, 600)` to set a specific window size. If you don't specify dimensions, Fern will auto-detect appropriate sizes - typically 800x600 for native builds, and the full browser window for web builds.

**`setupUI()`**: This is where you create your widgets, configure their properties, and add them to the widget manager. This function typically runs once at startup.

**`setDrawCallback(draw)`**: This tells Fern which function to call every frame for custom drawing. The draw function is your opportunity to clear the screen, draw backgrounds, or add custom graphics that aren't handled by widgets.

**`startRenderLoop()`**: This begins the main application loop. Fern will now continuously update widgets, handle input events, call your draw function, and render everything to the screen. This function blocks until the application is closed.

### Understanding the Game Loop

Before we write code, it's important to understand what happens inside `startRenderLoop()`. Modern interactive applications use what's called a "game loop" or "main loop" - a continuous cycle that runs many times per second:

1. **Handle Input**: Check for mouse clicks, key presses, window events
2. **Update**: Let widgets update their state, handle animations, process logic
3. **Draw**: Clear the screen, call your draw function, render all widgets
4. **Present**: Show the final result to the user
5. **Wait**: Brief pause to maintain consistent frame rate, then repeat

This happens approximately 60 times per second, creating the illusion of smooth, responsive interfaces. Your `draw()` function is called during step 3 of every loop iteration.

### A Simple Example

Let's create a complete, runnable example. Since you've already created a project, open `lib/main.cpp` in your favorite editor and replace its contents with this modern Fern application:

```cpp
#include <fern/fern.hpp>

using namespace Fern;

// Modern color palette for a professional look
namespace ModernColors {
    const uint32_t BackgroundDark = 0xFF1A1A1A;    // Dark charcoal background
    const uint32_t SurfaceElevated = 0xFF2A2A2A;   // Elevated surfaces
    const uint32_t AccentBlue = 0xFF3B82F6;        // Modern blue accent
    const uint32_t TextPrimary = 0xFFFFFFFF;       // Pure white text
    const uint32_t TextSecondary = 0xFFE5E7EB;     // Light gray text
}

void setupUI() {
    // We'll add interactive widgets here soon
}

void draw() {
    // Clear with modern dark background
    Draw::fill(ModernColors::BackgroundDark);
    
    // Draw some elegant shapes with modern colors
    Draw::rect(50, 50, 200, 100, ModernColors::SurfaceElevated);  // Elevated surface
    Draw::rect(60, 60, 180, 80, ModernColors::AccentBlue);        // Blue accent rectangle
    Draw::circle(400, 150, 40, ModernColors::AccentBlue);         // Blue circle
    
    // Draw connecting lines with subtle styling
    Draw::line(250, 100, 360, 150, 2, ModernColors::TextSecondary);
}

int main() {
    Fern::initialize(800, 600);  // Set a nice window size
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

This example demonstrates Fern's low-level drawing primitives. Every frame, Fern:
1. Calls `draw()` which fills the screen with dark blue
2. Draws a red rectangle at position (50, 50) with size 100x100 pixels
3. Draws a yellow circle centered at (300, 100) with radius 50 pixels  
4. Draws a white line from (100, 200) to (400, 250) with thickness 3 pixels


Build and run this from your project directory:

```bash
fern fire             # Automatically finds and compiles lib/main.cpp
fern fire -p linux    # Compile for native X11
```
or
```
fern fire -p web      # Compile for web (browser)
```

You should see a window with simple geometric shapes on a dark blue background. These drawing primitives are the building blocks that all higher-level widgets use internally.

## Low-Level Drawing Primitives

Before we explore widgets, it's important to understand the drawing primitives that power everything in Fern. These functions directly manipulate pixels and form the foundation for all visual elements.

Fern provides several basic drawing operations in the `Draw` namespace:

- **`Draw::fill(color)`**: Fill the entire screen with a solid color
- **`Draw::rect(x, y, width, height, color)`**: Draw a filled rectangle
- **`Draw::circle(cx, cy, radius, color)`**: Draw a filled circle
- **`Draw::line(x1, y1, x2, y2, thickness, color)`**: Draw a line between two points

All colors in Fern use 32-bit RGBA format (0xAABBGGRR), and the `Colors` namespace provides many predefined constants like `Colors::Red`, `Colors::Blue`, `Colors::White`, etc.

These primitives might seem basic, but they're incredibly powerful. Every button, text label, and complex widget you'll create is ultimately composed of rectangles, circles, and lines drawn pixel by pixel.

Now let's move to the more exciting part - widgets that handle interaction and layout automatically.

## Introduction to Widgets

While drawing primitives give you complete control, manually positioning and drawing every element becomes tedious for complex interfaces. This is where Fern's widget system shines. Widgets are reusable UI components that handle their own rendering, input processing, and layout.

Think of widgets as smart drawing operations. A button widget knows how to draw itself as a rectangle with text, how to change appearance when you hover over it, and how to respond when clicked. A text widget knows how to render text with proper font handling and wrapping.

### Your First Widget

Let's create a beautiful, interactive button using the same modern color palette. Replace your `setupUI()` function with this:

```cpp
void setupUI() {
    // Create a modern button style
    ButtonStyle buttonStyle;
    buttonStyle.normalColor(ModernColors::AccentBlue)       // Default blue
              .hoverColor(0xFF4F46E5)                      // Indigo on hover  
              .pressColor(0xFF3730A3)                      // Darker indigo on press
              .textColor(ModernColors::TextPrimary)        // White text
              .textScale(2)                                // 2x text size
              .borderRadius(8);                            // Rounded corners

    // Create button with modern styling
    auto myButton = Button(ButtonConfig(0, 0, 200, 55, "Click Me!")
        .style(buttonStyle));
    
    // Add click handler to show interaction
    myButton->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    // Center the button on screen
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(myButton);
    
    addWidget(centerWidget);
}
```

Update your `draw()` function for a cleaner background:

```cpp
void draw() {
    Draw::fill(ModernColors::BackgroundDark);  // Modern dark background
}
```

Compile and run this updated version. You'll see a beautifully styled, centered button that responds to mouse hover and clicks with smooth visual feedback.

### Understanding Widget Management

When you call `addWidget(myButton)`, you're registering the button with Fern's widget manager. The widget manager is a centralized system that:

1. **Maintains Z-order**: Widgets added later appear on top of earlier widgets
2. **Handles input distribution**: Routes mouse and keyboard events to the appropriate widget
3. **Manages rendering**: Automatically calls each widget's render function every frame
4. **Coordinates updates**: Ensures widgets update in the correct order

This is why you don't need to manually draw the button in your `draw()` function - the widget manager handles it automatically.

## Adding Interactivity

Static interfaces are boring. Let's make our button actually do something when clicked. Fern uses a signal-slot system for handling events, which is a clean and type-safe way to connect user actions to your code.

### Understanding Signals and Slots

The signal-slot pattern is a powerful way to handle events without tight coupling between components. Here's how it works conceptually:

- A **signal** is something that can happen (like a button click)
- A **slot** is a function that responds to that signal
- You **connect** slots to signals to define what happens when events occur

This pattern is used throughout modern software development because it's flexible and maintainable. You can connect multiple slots to one signal, disconnect them later, and the signal source doesn't need to know what's listening to it.

### Creating an Interactive Counter

Let's build a simple interactive application that demonstrates multiple widgets working together, using manual positioning (we'll learn about automatic layouts later):

```cpp
#include <fern/fern.hpp>
#include <iostream>
#include <string>

using namespace Fern;

// Modern color palette for consistent styling
namespace ModernColors {
    const uint32_t BackgroundDark = 0xFF1A1A1A;
    const uint32_t SurfaceElevated = 0xFF2A2A2A;
    const uint32_t AccentBlue = 0xFF3B82F6;
    const uint32_t AccentGreen = 0xFF10B981;
    const uint32_t TextPrimary = 0xFFFFFFFF;
    const uint32_t TextSecondary = 0xFFE5E7EB;
}

// Global state for our counter
static int counter = 0;
static std::shared_ptr<TextWidget> counterDisplay;

void updateCounter() {
    counterDisplay->setText(std::to_string(counter));
}

void setupUI() {
    // Create title at the top of the screen
    TextStyle titleStyle;
    titleStyle.color(ModernColors::TextPrimary)
              .fontSize(3)
              .useBitmapFont().alignment(1);
    
    auto title = Text(TextConfig(200, 50, "Modern Counter").style(titleStyle));
    
    // Create counter display in the center
    TextStyle counterStyle;
    counterStyle.color(ModernColors::AccentBlue)
                .fontSize(6)
                .useBitmapFont()
                .backgroundColor(ModernColors::SurfaceElevated)
                .padding(20);
    
    counterDisplay = Text(TextConfig(320, 150, "0").style(counterStyle));
    
    // Create modern button style
    ButtonStyle buttonStyle;
    buttonStyle.normalColor(ModernColors::AccentGreen)
              .hoverColor(0xFF059669)              // Darker green on hover
              .pressColor(0xFF047857)              // Even darker on press
              .textColor(ModernColors::TextPrimary)
              .textScale(2)
              .borderRadius(8);
    
    // Create increment button positioned below counter
    auto incrementBtn = Button(ButtonConfig(280, 250, 120, 50, "Add +1")
        .style(buttonStyle));
    
    incrementBtn->onClick.connect([]() {
        counter++;
        updateCounter();
        std::cout << "Counter: " << counter << std::endl;
    });
    
    // Create reset button with different color, positioned below increment
    ButtonStyle resetStyle = buttonStyle;
    resetStyle.normalColor(ModernColors::SurfaceElevated)
              .hoverColor(0xFF4B5563);
    
    auto resetBtn = Button(ButtonConfig(292, 320, 100, 40, "Reset")
        .style(resetStyle));
    
    resetBtn->onClick.connect([]() {
        counter = 0;
        updateCounter();
        std::cout << "Counter reset!" << std::endl;
    });
    
    // Add widgets to the widget manager (manual positioning)
    addWidget(title);
    addWidget(counterDisplay);
    addWidget(incrementBtn);
    addWidget(resetBtn);
}

void draw() {
    Draw::fill(ModernColors::BackgroundDark);
}

int main() {
    Fern::initialize(680, 420);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

This example demonstrates several important concepts:

1. **Manual Widget Positioning**: Each widget specifies exact x,y coordinates (like `TextConfig(320, 150, "0")`)
2. **Multiple Widget Types**: Text widgets for display, buttons for interaction
3. **Global State Management**: Using static variables to track the counter value
4. **Event Handling**: Button clicks update the display through connected functions
5. **Modern Styling**: Professional color palette with hover effects and proper styling

Notice how we manually position each widget:
- Title at (250, 50) - top center
- Counter display at (320, 150) - middle center  
- Increment button at (280, 250) - below counter
- Reset button at (300, 320) - below increment button

This approach works for simple interfaces, but as you'll see later, manual positioning becomes difficult to maintain as your applications grow more complex.

```cpp
void draw() {
    Draw::fill(ModernColors::BackgroundDark);
}

int main() {
    Fern::initialize(600, 500);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```


### Understanding Widget Styling

The styling pattern you see above is fundamental to how Fern creates beautiful, professional interfaces. Each widget type has its own style class that follows a consistent builder pattern.

**The Style Pattern:**

```cpp
// All Fern widgets follow this pattern:
ButtonStyle style;
style.normalColor(ModernColors::AccentBlue)       // Default appearance
     .hoverColor(0xFF4F46E5)                      // State-based styling
     .pressColor(0xFF3730A3)                      // Multiple interaction states
     .textColor(ModernColors::TextPrimary)        // Text styling
     .textScale(2)                                // Sizing properties
     .borderRadius(8);                            // Visual effects

// Apply the style to a widget
auto button = Button(ButtonConfig(0, 0, 200, 50, "Styled Button").style(style));
```

**Key styling concepts:**

- **Method Chaining**: Style methods return references, enabling fluid `.method().method()` syntax
- **State-Based Styling**: Interactive widgets support multiple states (normal, hover, pressed, focused, disabled)
- **Consistent Properties**: Colors, sizing, borders, and effects work similarly across all widget types
- **Modern Color System**: Use carefully chosen color palettes for professional results
- **Responsive Behavior**: Styles automatically respond to user interactions without additional code

**Universal Style Properties:**

All Fern widgets support common styling approaches:

```cpp
TextStyle textStyle;
textStyle.color(ModernColors::TextPrimary)        // Text color
         .fontSize(3)                             // Size scaling
         .backgroundColor(ModernColors::Surface)  // Background
         .padding(10)                             // Spacing
         .alignment(1);                           // Center alignment

ContainerStyle containerStyle;
containerStyle.backgroundColor(ModernColors::SurfaceElevated)
              .borderRadius(12)                   // Rounded corners
              .shadow(true, 0xFF000000, 4)        // Drop shadows
              .padding(20);                       // Internal spacing
```

**Color Palette Best Practices:**

Following the examples in your project's `examples/` directory, use organized color namespaces:

```cpp
namespace ModernColors {
    // Backgrounds
    const uint32_t BackgroundDark = 0xFF1A1A1A;
    const uint32_t SurfaceElevated = 0xFF2A2A2A;
    
    // Accents
    const uint32_t AccentBlue = 0xFF3B82F6;
    const uint32_t AccentGreen = 0xFF10B981;
    const uint32_t AccentRed = 0xFFEF4444;
    
    // Text
    const uint32_t TextPrimary = 0xFFFFFFFF;
    const uint32_t TextSecondary = 0xFFE5E7EB;
    const uint32_t TextMuted = 0xFF9CA3AF;
}
```

**Exploring Widget-Specific Styles:**

Each widget type has specialized styling options. Check out the examples in your project's `examples/` directory to see:

- `ButtonStyle` - hover states, borders, text styling
- `TextStyle` - fonts, alignment, backgrounds, shadows  
- `ContainerStyle` - layouts, spacing, borders, backgrounds
- `InputStyle` - focus states, placeholder styling, validation

This declarative approach means you describe the appearance you want, and Fern handles all the complexity of rendering, state management, and smooth visual transitions.

**Key Takeaways from the Counter Example:**

- **Signal-Slot Pattern**: `button->onClick.connect()` creates clean event handling
- **Modern Styling**: Professional color palettes make applications look polished  
- **State Management**: Global variables track application state between interactions
- **Manual Positioning**: Each widget specifies exact pixel coordinates

While manual positioning works for simple interfaces, you'll quickly discover its limitations as your applications grow more complex.

## The Layout Problem

So far, we've been manually specifying exact pixel coordinates for our widgets (like `TextConfig(320, 150, "0")`). This works for simple examples, but becomes a nightmare for real applications. What happens when the window is resized? What if you want to add another widget? What about different screen sizes?

Manual positioning is fragile and doesn't scale. This is where Fern's layout system becomes essential.

### Understanding Layouts

Instead of specifying exact positions, layout widgets automatically arrange their children based on rules you define. This is similar to how HTML uses flexbox or CSS grid, or how Flutter uses Rows and Columns.

Key concepts:

- **Parent-Child Relationships**: Layout widgets (like Column or Row) contain other widgets as children
- **Automatic Positioning**: The layout widget calculates where each child should be placed
- **Responsive Design**: Layouts automatically adapt when the window size changes
- **Nested Layouts**: You can put layout widgets inside other layout widgets for complex arrangements

### Your First Layout

Let's rebuild our button example using layouts. When using layouts, you should set widget positions to (0, 0) because the layout will calculate the real positions:

```cpp
void setupUI() {
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Welcome to Fern", 4, Colors::White),
        SizedBox(0, 20),  // 20 pixels of vertical spacing
        Text(Point(0, 0), "A pixel-perfect UI framework", 2, Colors::Gray),
        SizedBox(0, 30),  // 30 pixels more spacing
        Button(ButtonConfig(0, 0, 200, 40, "Get Started"))
    };
    
    // Get current window dimensions
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create a center widget that fills the entire window
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    
    // Add a column layout to the center widget
    centerWidget->add(Column(children));
    
    addWidget(centerWidget);
}
```

This creates a vertical layout (Column) that's centered on the screen. The Column arranges its children vertically with automatic spacing, and the CenterWidget positions the entire column in the center of the window.

Notice several important things:

1. **Zero Coordinates**: All widgets use `Point(0, 0)` because layouts calculate positions
2. **SizedBox for Spacing**: `SizedBox(0, 20)` creates 20 pixels of vertical space between elements
3. **Parent-Child Structure**: `centerWidget` contains a `Column`, which contains the text and button
4. **Dynamic Sizing**: `Fern::getWidth()` and `Fern::getHeight()` get current window dimensions

### Layout Alignment

Layouts support different alignment options:

- **Main Axis**: For columns, this is vertical alignment (top, center, bottom)
- **Cross Axis**: For columns, this is horizontal alignment (left, center, right)

You can customize how children are arranged:

```cpp
auto column = Column(children);
column.setMainAxisAlignment(MainAxisAlignment::Center);    // Center vertically
column.setCrossAxisAlignment(CrossAxisAlignment::Start);   // Align left horizontally
```

Similar concepts apply to Row widgets, but with axes swapped - main axis is horizontal, cross axis is vertical.

## State Management

Interactive applications need to respond to user input by changing what's displayed. This requires managing state - data that can change over time and affects the UI.

However, there's an important concept to understand: your `setupUI()` and `draw()` functions run in a specific context, and variables inside them are recreated every time the function runs.

### The Static Variable Pattern

Consider this broken example:

```cpp
void setupUI() {
    int counter = 0;  // This gets reset every time setupUI runs!
    auto counterText = Text(Point(0, 0), "Count: 0", 3, Colors::White);
    
    auto button = Button(ButtonConfig(0, 0, 120, 40, "Increment"));
    button->onClick.connect([&]() {
        counter++;  // This won't work as expected
        counterText->setText("Count: " + std::to_string(counter));
    });
}
```

The problem is that `counter` and `counterText` are local variables that get destroyed when `setupUI()` finishes. When the button is clicked later, these variables no longer exist.

The solution is to use `static` variables, which persist for the entire program lifetime:

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

This pattern is similar to how React or other frameworks handle component state - you need persistent storage for data that changes over time.

### Why Static Variables Work

When you declare a variable as `static`, it's allocated once when the program starts and persists until the program ends. This means:

1. The `counter` variable maintains its value between button clicks
2. The `counterText` widget pointer remains valid so you can call `setText()` on it
3. The widget can update its display with the new counter value

This is a fundamental pattern in Fern applications - use static variables for any data that needs to persist and change over time.

## Building and Running

Now that you understand the basics, let's talk about how to compile and run your Fern applications.

### Using Terra CLI

The recommended way to build Fern applications is with the Terra CLI:

```bash
# Compile for Linux (native performance)
fern fire your_app.cpp -p linux

# Compile for web (WebAssembly)
fern fire your_app.cpp -p web
```

Terra handles all the complex build configuration, linking, and platform-specific details automatically.

### Understanding the Build Process

When you run `fern fire`, several things happen:

1. **Dependency Resolution**: Terra ensures all required libraries are available
2. **Compilation**: Your C++ code is compiled with the appropriate compiler (GCC/Clang for Linux, Emscripten for web)
3. **Linking**: Your code is linked with the Fern framework libraries
4. **Asset Processing**: Any resources like fonts are processed and embedded
5. **Execution**: For Linux builds, the program runs immediately. For web builds, Terra starts a local server using python

The entire process is designed to be fast and seamless, so you can focus on building your application rather than fighting with build systems.

## Next Steps

Congratulations! You now understand the fundamental concepts of Fern:

- How pixel-level rendering creates user interfaces
- The game loop and frame-based drawing
- Widget systems and automatic management
- Signal-slot event handling
- Layout systems for responsive design
- State management with static variables

### What to Explore Next

1. **More Widgets**: Explore TextInput, Slider, RadioButton, and other built-in widgets
2. **Advanced Layouts**: Learn about Row widgets, Expanded widgets for flexible sizing, and nested layouts
3. **Custom Widgets**: Create your own widget classes by inheriting from the base Widget class
4. **Scene Management**: For complex applications, use Fern's scene system to organize different screens
5. **Font System**: Integrate TTF fonts for beautiful typography
6. **Advanced Graphics**: Combine widgets with custom drawing for unique effects

### Getting Help

If you run into issues or have questions:

1. **Documentation**: Check the `docs/` directory for detailed guides on specific topics
2. **Examples**: Look at the example programs in `examples/cpp/` for inspiration and patterns
3. **Community**: Join the FernKit community for discussions and support
4. **Source Code**: Fern is open source - you can read the implementation to understand how everything works

### Troubleshooting Common Issues

**Build Errors**: Make sure you have all prerequisites installed, especially Emscripten for web builds.

**Window Not Appearing**: Verify that your `main()` function calls all four required functions: `initialize()`, `setupUI()`, `setDrawCallback()`, and `startRenderLoop()`.

**Widgets Not Responding**: Check that you're calling `addWidget()` to register widgets with the widget manager.

**Layout Issues**: Remember to use `Point(0, 0)` for widget positions when using layout widgets.

Remember, Fern is designed to be educational. When something doesn't work as expected, it's often an opportunity to understand how user interfaces actually function at a fundamental level. Don't hesitate to experiment, read the source code, and explore the low-level details that other frameworks hide from you.