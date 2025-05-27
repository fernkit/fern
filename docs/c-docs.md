# Fern Graphics C Library Documentation

## Introduction

Fern Graphics C is a lightweight graphics and UI library designed for creating simple interactive applications, visualizations, and games. Built with plain C and WebAssembly compatibility in mind, it provides a minimal API for rendering graphics primitives and basic UI components.

> **⚠️ Migration Notice**: This C implementation is being maintained for legacy purposes and has limited features. For new projects, we recommend using our C++ implementation which offers significantly more capabilities including a layout system, responsive components, event handling, and more.

## Key Features

- **Simple drawing API** for basic graphics primitives (rectangles, circles, lines)
- **Basic text rendering** with a built-in bitmap font
- **Simple widget system** with buttons and containers
- **Mouse input handling**
- **WebAssembly compatibility** for web deployment
- **Minimal dependencies** - only requires standard C libraries

## Getting Started

### Installation

```bash
git clone https://github.com/username/fern-graphics.git
cd fern-graphics
mkdir build && cd build
emcmake cmake ..
make
```

### Basic Structure of a Fern Application

```c
#include "fern.c"

#define WIDTH 800
#define HEIGHT 600
static uint32_t pixels[HEIGHT*WIDTH];

void draw_frame() {
    // Clear the screen with gray
    ffill(pixels, HEIGHT, WIDTH, Colors_gray);
    
    // Draw a rectangle
    frect(pixels, HEIGHT, WIDTH, Colors_blue, 100, 100, 200, 150);
    
    // Draw a circle
    fcircle(pixels, HEIGHT, WIDTH, Colors_red, 400, 300, 50);
    
    // Draw text
    ftext(pixels, WIDTH, HEIGHT, "HELLO WORLD", 300, 50, 2, Colors_white);
}

int main() {
    // Initialize canvas
    FernCanvas canvas = {pixels, HEIGHT, WIDTH};
    runApp(canvas);
    
    // Set the drawing callback
    fern_set_draw_callback(draw_frame);
    
    // Start the render loop
    fern_start_render_loop();
    return 0;
}
```

## Core Concepts

### Canvas and Pixels

The C implementation of Fern Graphics works directly with a pixel buffer that you must declare and manage:

```c
#define WIDTH 800
#define HEIGHT 600
static uint32_t pixels[HEIGHT*WIDTH];

// Create canvas from the buffer
FernCanvas canvas = {pixels, HEIGHT, WIDTH};
runApp(canvas);
```

### Coordinate System

Fern uses a standard 2D coordinate system where:
- (0,0) is at the top-left corner
- X increases to the right
- Y increases downward

### Colors

Colors are represented as 32-bit RGBA values (0xAARRGGBB format). Several predefined colors are available:

```c
Colors_green  // 0xFF00FF00
Colors_blue   // 0xFF0000FF
Colors_red    // 0xFFFF0000
Colors_gray   // 0xFF202020
Colors_black  // 0xFF000000
Colors_white  // 0xFFFFFFFF
```

### Points and Geometry

```c
// Create a point
Point p = Point_create(100, 100);

// Use with drawing functions
LineWidget(
    start(Point_create(10, 10)), 
    end(Point_create(200, 200)),
    thickness(2), 
    color(Colors_white)
);
```

## Drawing API

Fern C provides basic drawing primitives:

```c
// Fill the entire screen
ffill(pixels, HEIGHT, WIDTH, color);

// Draw a rectangle
frect(pixels, HEIGHT, WIDTH, color, x, y, width, height);

// Draw a circle
fcircle(pixels, HEIGHT, WIDTH, color, center_x, center_y, radius);

// Draw a line
fline(pixels, HEIGHT, WIDTH, color, x1, y1, x2, y2, thickness);

// Draw text
ftext(pixels, WIDTH, HEIGHT, text, x, y, scale, color);
```

### Example: Drawing a simple scene

```c
void draw_frame() {
    // Clear background to black
    ffill(pixels, HEIGHT, WIDTH, Colors_black);
    
    // Draw sky (blue rectangle at the top)
    frect(pixels, HEIGHT, WIDTH, Colors_blue, 0, 0, WIDTH, HEIGHT/2);
    
    // Draw ground (green rectangle at the bottom)
    frect(pixels, HEIGHT, WIDTH, Colors_green, 0, HEIGHT/2, WIDTH, HEIGHT/2);
    
    // Draw sun (yellow circle)
    fcircle(pixels, HEIGHT, WIDTH, 0xFFFFFF00, WIDTH-100, 100, 50);
    
    // Draw house (brown rectangle)
    frect(pixels, HEIGHT, WIDTH, 0xFF8B4513, WIDTH/2-75, HEIGHT/2-75, 150, 150);
    
    // Draw roof (red triangle using lines)
    fline(pixels, HEIGHT, WIDTH, Colors_red, WIDTH/2-75, HEIGHT/2-75, WIDTH/2+75, HEIGHT/2-75, 5);
    fline(pixels, HEIGHT, WIDTH, Colors_red, WIDTH/2-75, HEIGHT/2-75, WIDTH/2, HEIGHT/2-150, 5);
    fline(pixels, HEIGHT, WIDTH, Colors_red, WIDTH/2+75, HEIGHT/2-75, WIDTH/2, HEIGHT/2-150, 5);
    
    // Draw title
    ftext(pixels, WIDTH, HEIGHT, "MY HOUSE", WIDTH/2-50, 50, 2, Colors_white);
}
```

## Widget System

Fern C includes a simple widget system with basic UI components:

### Container Widget

Creates a solid color rectangle:

```c
// Simple container
Container(
    color(Colors_blue),
    x(100), y(100),
    width(200), height(150)
);

// Centered container
CenteredContainer(300, 200, Colors_red);
```

### Circle Widget

Draws a filled circle:

```c
CircleWidget(
    radius(50),
    position(Point_create(400, 300)),
    color(Colors_green)
);
```

### Line Widget

Draws a line between two points:

```c
LineWidget(
    start(Point_create(100, 100)),
    end(Point_create(300, 200)),
    thickness(3),
    color(Colors_white)
);
```

### Text Widget

Displays text:

```c
TextWidget(
    position(Point_create(100, 100)),
    text("HELLO WORLD"),
    scale(2),
    color(Colors_white)
);
```

### Button Widget

Creates an interactive button:

```c
void handle_button_click() {
    // Button click handler
    printf("Button clicked!\n");
}

// Button definition
ButtonConfig button_config = {
    .x = 100,
    .y = 100,
    .width = 200,
    .height = 50,
    .normal_color = Colors_blue,
    .hover_color = 0xFF4444AA,  // Lighter blue
    .press_color = 0xFF222288,  // Darker blue
    .label = "CLICK ME",
    .text_scale = 2,
    .text_color = Colors_white,
    .on_click = handle_button_click
};

// Create the button
ButtonWidget(button_config);
```

### Gradient Container

Creates a rectangle with a linear gradient:

```c
// Define gradient stops
GradientStop stops[3] = {
    {Colors_blue, 0.0f},   // Start color (position 0.0)
    {Colors_green, 0.5f},  // Middle color (position 0.5)
    {Colors_red, 1.0f}     // End color (position 1.0)
};

// Create gradient definition
LinearGradient gradient = {
    .stops = stops,
    .stop_count = 3,
    .direction = GRADIENT_HORIZONTAL  // or GRADIENT_VERTICAL
};

// Draw gradient container
LinearGradientContainer(100, 100, 400, 200, gradient);
```

## Input Handling

### Mouse Input

Access mouse state through the `current_input` global variable:

```c
void handle_input() {
    // Mouse position
    int mouse_x = current_input.mouse_x;
    int mouse_y = current_input.mouse_y;
    
    // Mouse button state
    bool is_down = current_input.mouse_down;
    bool was_clicked = current_input.mouse_clicked;
    
    // React to mouse click
    if (was_clicked && 
        mouse_x > 100 && mouse_x < 300 && 
        mouse_y > 100 && mouse_y < 200) {
        printf("Area clicked!\n");
    }
}
```

## Example Application: Particle Life Simulation

One of the showcase examples for Fern Graphics C is a particle life simulation that demonstrates emergent behaviors through simple interaction rules.

**[Live Demo: Particle Life Simulation](https://fern-life.web.app/)**

The simulation features:
- Multiple particle types with different interaction rules
- Attraction and repulsion forces between particles
- Customizable parameters and presets
- Interactive UI for controlling the simulation

### Key Components of the Simulation:

```c
// Initialize particles
void init_simulation() {
    // Set default parameters
    state.particle_count = 800;
    state.world_friction = 0.1f;
    state.force_strength = 0.5f;
    
    // Initialize particles with random positions and types
    for (int i = 0; i < state.particle_count; i++) {
        state.particles[i].position.x = WORLD_MARGIN + (float)rand() / RAND_MAX * (WIDTH - 2 * WORLD_MARGIN);
        state.particles[i].position.y = WORLD_MARGIN + (float)rand() / RAND_MAX * (HEIGHT - 2 * WORLD_MARGIN);
        state.particles[i].velocity.x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        state.particles[i].velocity.y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        state.particles[i].type = rand() % PARTICLE_TYPES;
        state.particles[i].size = 3.0f + ((float)rand() / RAND_MAX) * 2.0f;
        state.particles[i].active = true;
    }
}

// Update particle positions based on forces
void update_simulation() {
    // Calculate forces between particles
    for (int i = 0; i < state.particle_count; i++) {
        // Apply forces from other particles
        // Update velocity and position
        // Handle collisions and boundaries
    }
}

// Draw everything
void draw_frame() {
    // Clear screen
    ffill(pixels, HEIGHT, WIDTH, Colors_black);
    
    // Draw all particles
    for (int i = 0; i < state.particle_count; i++) {
        fcircle(pixels, HEIGHT, WIDTH, state.type_colors[state.particles[i].type],
                state.particles[i].position.x, state.particles[i].position.y, 
                state.particles[i].size);
    }
    
    // Draw UI elements
    // (buttons, status text, controls)
}
```

You can find the complete source code for this example in life_sim.c.

## Limitations of the C Implementation

While the C implementation is functional for simple applications, it has several limitations compared to the C++ implementation:

1. **No Layout System** - All positioning must be done manually with explicit coordinates
2. **Limited Widget Types** - Only basic widgets are supported
3. **No Signal/Slot System** - Event handling is limited to direct function pointers
4. **No Responsive Design** - No built-in facilities for handling window resizing
5. **Manual Memory Management** - No automatic cleanup or resource management
6. **No Widget Hierarchy** - Widgets cannot contain other widgets
7. **Limited Text Support** - Only uppercase letters and numbers are supported
8. **No Animation System** - Animations must be implemented manually

## Why Choose the C Implementation?

Despite its limitations, the C implementation may still be appropriate in these scenarios:

1. **Educational Purposes** - Simpler to understand for beginners
2. **Minimalist Projects** - When you need absolute minimal overhead
3. **Legacy Support** - For maintaining older projects
4. **Size Constraints** - When every byte counts (produces smaller WASM outputs)
5. **Simplicity** - When you don't need complex UI features

## Migrating to the C++ Implementation

For more complex projects or new development, we strongly recommend using the C++ implementation which offers:

- Object-oriented design
- Powerful layout system
- Signal/slot event mechanism
- More widget types
- Responsive design capabilities
- Memory safety
- Richer text rendering

See our Migration Guide for detailed instructions on transitioning from the C to C++ implementation.

## Performance Tips

### Minimize Redrawing

```c
// Bad: Redraw everything every frame
void draw_frame() {
    ffill(pixels, HEIGHT, WIDTH, Colors_black);
    
    // Draw everything from scratch
}

// Better: Only redraw what changed
void draw_frame() {
    static bool first_draw = true;
    
    if (first_draw) {
        ffill(pixels, HEIGHT, WIDTH, Colors_black);
        // Draw static elements
        first_draw = false;
    }
    
    // Only redraw dynamic elements
}
```

### Batch Similar Operations

```c
// Bad: Alternating between different drawing operations
frect(pixels, HEIGHT, WIDTH, Colors_red, 10, 10, 100, 50);
fcircle(pixels, HEIGHT, WIDTH, Colors_blue, 200, 200, 30);
frect(pixels, HEIGHT, WIDTH, Colors_green, 300, 10, 100, 50);
fcircle(pixels, HEIGHT, WIDTH, Colors_yellow, 400, 200, 30);

// Better: Batch similar operations
frect(pixels, HEIGHT, WIDTH, Colors_red, 10, 10, 100, 50);
frect(pixels, HEIGHT, WIDTH, Colors_green, 300, 10, 100, 50);
fcircle(pixels, HEIGHT, WIDTH, Colors_blue, 200, 200, 30);
fcircle(pixels, HEIGHT, WIDTH, Colors_yellow, 400, 200, 30);
```

## Troubleshooting

### Common Issues and Solutions

#### Nothing Displays on Screen

- Verify canvas dimensions match your pixel buffer size
- Check that your draw callback is registered properly
- Make sure colors include the alpha component (0xFF000000 for opaque black)

#### UI Not Responding to Input

- Check that event listeners are properly set up with `setup_event_listeners()`
- Ensure your HTML canvas element has the correct ID ("canvas")
- Verify mouse position calculations are correct

#### Text Rendering Issues

- Remember that only uppercase letters and numbers are supported
- Check text positioning and ensure it's within screen bounds
- Verify text color has proper alpha channel value

## Contributing to Fern Graphics

We welcome contributions to the Fern Graphics library! While our main development focuses on the C++ implementation, we still accept improvements to the C version, especially:

1. **Bug fixes** and stability improvements
2. **Performance optimizations**
3. **Documentation** enhancements
4. **Examples** showing how to use the library

Please see CONTRIBUTING.md for more detailed instructions.

## Future Plans

While the C implementation will continue to be maintained for compatibility, most new features will be added to the C++ version. We recommend using the C++ implementation for all new projects.