# Fern Graphics Library

<p align="center">
  <img src="assets/logo.png" alt="Fern Graphics Logo" width="200"/>
</p>

A lightweight graphics library for creating visual interactive applications using C or C++.

## Important Notices

> **C++ Implementation Available**: Fern is now available in both C and C++ implementations. The C++ version offers more features including layout systems, responsive design, and a modern event architecture.
>
> **Choose your implementation**:
> - [C Implementation](docs/c-docs.md) - Lightweight and simple
> - [C++ Implementation](docs/cpp-docs.md) - Feature-rich with modern design patterns
> - [Migration Guide](docs/migration-guide.md) - How to transition from C to C++

## Overview

Fern is a minimalist graphics library designed for simplicity, performance, and ease of use. It provides a declarative API for rendering graphics to HTML canvas via WebAssembly, enabling developers to create visual applications that run in any modern web browser.

### Live Examples

- [Particle Life Simulation](https://fern-life.web.app/) - An example of what you can create with Fern

## Key Features

- WebAssembly-powered rendering for near-native performance
- Available in both C (stable) and C++ (feature-rich) implementations
- Support for shapes, lines, gradients, text, and pixel manipulation
- TTF (TrueType Font) support for custom fonts (experimental, C++ only)
- Interactive UI elements with event handling
- Mouse input capture and processing
- Layout system for responsive design (C++ only)
- Signal/Slot event system for flexible event handling (C++ only)

## Choosing an Implementation

### C Implementation
- **Advantages**: Minimal size, simpler API, fewer dependencies
- **Best for**: Small projects, learning purposes, minimal applications
- **Limitations**: Manual positioning, limited widget types, no layout system
- [**Read the C documentation**](docs/c-docs.md)

### C++ Implementation
- **Advantages**: Layout system, responsive design, event system, OOP design, TTF font support
- **Best for**: Larger applications, UI-heavy projects, responsive designs, custom fonts
- **Features**: Widget hierarchy, signal/slots, smart pointers, easy positioning, experimental TTF rendering
- [**Read the C++ documentation**](docs/cpp-docs.md)

## Installation

```bash
# Install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Clone Fern
git clone https://github.com/RishiAhuja/fern.git
cd fern

# Make the CLI script executable
chmod +x fern-cli.sh

# Create a symbolic link to make it available system-wide
sudo ln -s $(pwd)/fern-cli.sh /usr/local/bin/fern

# Optional: Install the man page
sudo install -m 644 fern.1 /usr/local/share/man/man1/
sudo mandb
```

## Quick Start

### Using the C Implementation

```c
// main.c
#include "fern.c"

#define WIDTH 800
#define HEIGHT 600
static uint32_t pixels[HEIGHT*WIDTH];

void draw_frame() {
    ffill(pixels, HEIGHT, WIDTH, Colors_gray);
    frect(pixels, HEIGHT, WIDTH, Colors_blue, 100, 100, 200, 150);
    fcircle(pixels, HEIGHT, WIDTH, Colors_red, 400, 300, 50);
    ftext(pixels, WIDTH, HEIGHT, "HELLO WORLD", 300, 50, 2, Colors_white);
}

int main() {
    FernCanvas canvas = {pixels, HEIGHT, WIDTH};
    runApp(canvas);
    fern_set_draw_callback(draw_frame);
    fern_start_render_loop();
    return 0;
}
```

### Using the C++ Implementation

```cpp
// main.cpp
#include <fern/fern.hpp>
#include <memory>

using namespace Fern;

void setupUI() {
    // Load custom TTF font (experimental)
    TTF::load("customFont", "fonts/MyFont.ttf");
    TTF::setDefault("customFont");
    
    auto mainLayout = Column({
        Text(Point(0, 0), "FERN C++ DEMO", 3, Colors::White, false),
        Text(Point(0, 0), "Custom TTF Font", 24, Colors::Blue, true, FontType::TTF),
        Circle(50, Point(0, 0), Colors::Blue, false),
        Button({
            .width = 200,
            .height = 60,
            .normalColor = Colors::Green,
            .label = "CLICK ME"
        }, false)
    });
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

## Building and Running

```bash
# C implementation
fern main.c

# C++ implementation
fern --cpp main.cpp
```

## Fern CLI Tool

The Fern CLI is a convenient wrapper for building and running your Fern Graphics applications. It handles the Emscripten compilation process and serves your project for testing.

### Basic Usage

```bash
# Basic usage pattern
fern [options] <source_file>
```

### Options

| Option | Description |
|--------|-------------|
| `--cpp` | Use C++ implementation (default: C implementation) |
| `--release` | Build in release mode with optimizations (default: debug mode) |
| `--serve` | Serve the application after building (default: true) |
| `--no-serve` | Build without serving the application |
| `--port <number>` | Specify server port (default: 8080) |
| `--out <directory>` | Specify output directory (default: ./build) |
| `--assets <directory>` | Include assets from specified directory |
| `--embed-file <directory>` | Embed files/directories into WebAssembly (useful for fonts) |
| `--template <file>` | Use custom HTML template file |
| `--help` | Show help information |

### Examples

#### Build and run a C project
```bash
fern examples/c/simple_shapes.c
```

#### Build and run a C++ project
```bash
fern --cpp examples/cpp/simple_layout.cpp
```

#### Build a release version without serving
```bash
fern --release --no-serve --out ./dist main.c
```

#### Include assets directory
```bash
fern --assets ./images game.c
```

#### Embed fonts for TTF support (C++ only)
```bash
fern --cpp --embed-file fonts font_demo.cpp
```

#### Use a custom port
```bash
fern --port 3000 app.c
```

### Project Structure

When running the CLI, Fern will:

1. Create the output directory if it doesn't exist
2. Compile your code using Emscripten
3. Copy the HTML template and any assets
4. Start a local server if `--serve` is enabled

### Compilation Details

The CLI handles these Emscripten compilation flags for you:

- `-s WASM=1` - Compile to WebAssembly
- `-s ALLOW_MEMORY_GROWTH=1` - Allow dynamic memory allocation
- `-s EXPORTED_FUNCTIONS='[_main, _fernUpdateMousePosition, _fernUpdateMouseButton]'` - Export required functions
- `-s EXPORTED_RUNTIME_METHODS='[ccall, cwrap]'` - Export JS interop methods

For C++ projects, additional flags are used:
- `-std=c++14` - Use C++14 standard
- Additional optimizations for object-oriented code

### Advanced Usage

#### Multiple Source Files

For projects with multiple source files:

```bash
fern main.c utils.c graphics.c
```

For C++ projects with multiple files:
```bash
fern --cpp main.cpp widgets.cpp layout.cpp
```

### Troubleshooting

#### Common Issues

- **"Command not found"**: Make sure you've made the script executable with `chmod +x fern-cli.sh`
  
- **Emscripten not found**: Ensure you've sourced the Emscripten environment with `source ./emsdk_env.sh`

- **Compilation errors**: Check the console output for specific error messages. Common issues include missing headers or incorrect syntax

- **Canvas not displaying**: Ensure your HTML template has a canvas element with id="canvas"

Then serve the result with your preferred web server.

## Documentation

For detailed API reference, examples and more information, please refer to:

- **C Implementation Documentation**
- **C++ Implementation Documentation**
- **Migration Guide**

## Contributing

We welcome contributions to the Fern Graphics library! To contribute:

1. Fork the repository
2. Create a feature branch
3. Make your changes with appropriate tests
4. Submit a pull request

Please see CONTRIBUTING.md for more detailed instructions.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
