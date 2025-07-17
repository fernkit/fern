# Project Structure

This document explains the organization of the Fern UI Framework codebase, helping developers understand where to find different components and how they relate to each other.

## Table of Contents
- [Overview](#overview)
- [Directory Structure](#directory-structure)
- [Source Code Organization](#source-code-organization)
- [Header Files](#header-files)
- [Documentation Structure](#documentation-structure)
- [Examples and Tests](#examples-and-tests)
- [Build System Files](#build-system-files)
- [Asset Management](#asset-management)

## Overview

Fern follows a modular architecture with clear separation between:
- **Core framework** (`src/cpp/`) - Main UI framework implementation
- **C bindings** (`src/c/`) - C language bindings and WASM interface
- **Documentation** (`docs/`) - Comprehensive user and developer guides
- **Examples** (`examples/`) - Sample applications and tutorials
- **Build system** - CMake configuration and build scripts

## Directory Structure

```
fern/
├── src/                          # Source code
│   ├── c/                        # C language bindings
│   │   ├── fern.c               # Main C API implementation
│   │   └── fern_wasm.c          # WebAssembly specific code
│   └── cpp/                     # C++ framework implementation
│       ├── CMakeLists.txt       # C++ build configuration
│       ├── include/             # Public header files
│       │   └── fern/            # Framework headers
│       │       ├── fern.hpp     # Main include file
│       │       ├── core/        # Core framework components
│       │       ├── font/        # Font system
│       │       ├── graphics/    # Graphics primitives and colors
│       │       ├── platform/    # Platform abstraction
│       │       ├── text/        # Text rendering
│       │       └── ui/          # UI components
│       │           ├── containers/  # Container widgets
│       │           ├── layout/      # Layout system
│       │           └── widgets/     # UI widgets
│       └── src/                 # Implementation files
│           ├── fern.cpp         # Main framework entry point
│           ├── core/            # Core implementations
│           ├── font/            # Font system implementation
│           ├── graphics/        # Graphics implementations
│           ├── platform/        # Platform-specific code
│           ├── text/            # Text rendering implementation
│           └── ui/              # UI implementations
├── docs/                        # Documentation
│   ├── README.md               # Documentation overview
│   ├── getting-started.md      # Quick start guide
│   ├── installation.md         # Installation instructions
│   ├── api-reference.md        # Complete API reference
│   ├── widgets/                # Widget documentation
│   ├── layout/                 # Layout system docs
│   ├── graphics/               # Graphics and styling
│   ├── input/                  # Input and events
│   ├── platform/               # Platform guides
│   ├── examples/               # Example documentation
│   └── development/            # Developer guides
├── examples/                   # Example applications
│   ├── c/                      # C examples
│   └── cpp/                    # C++ examples
├── assets/                     # Static assets
│   ├── logo.png               # Project logo
│   ├── example_scene.png      # Screenshots
│   └── example_scene2.png     # More screenshots
├── CMakeLists.txt             # Root CMake file
├── build.sh                   # Build script
├── fern-cli.sh               # CLI tool script
├── template.html             # HTML template for web builds
├── README.md                 # Project overview
├── CONTRIBUTING.md           # Contribution guidelines
└── LICENCE                   # Project license
```

## Source Code Organization

### Core Framework (`src/cpp/`)

The C++ framework is organized into logical modules:

#### Core Components (`core/`)
- `canvas.hpp/cpp` - Drawing surface and rendering context
- `input.hpp/cpp` - Input handling and event management
- `signal.hpp` - Event signaling system (header-only)
- `types.hpp` - Common types and structures
- `widget_manager.hpp` - Widget lifecycle management

#### UI System (`ui/`)
- `widgets/` - Individual widget implementations
  - `widget.hpp` - Base widget class
  - `button_widget.hpp/cpp` - Interactive buttons
  - `text_widget.hpp/cpp` - Text display
  - `text_input_widget.hpp/cpp` - Text input fields
  - `circle_widget.hpp/cpp` - Circular shapes
  - `line_widget.hpp/cpp` - Line drawing
  - Plus specialized widgets (slider, dropdown, etc.)
- `layout/` - Layout management system
  - `layout.hpp` - Layout base classes and algorithms
- `containers/` - Container widgets
  - `container.hpp` - Container widget implementations

#### Graphics System (`graphics/`)
- `colors.hpp/cpp` - Color definitions and utilities
- `primitives.hpp/cpp` - Basic drawing primitives

#### Font System (`font/`)
- `font.hpp/cpp` - Font management
- `font_manager.hpp` - Font loading and caching
- `ttf_font_renderer.hpp/cpp` - TrueType font rendering
- `ttf_reader.hpp/cpp` - TTF file parsing

#### Platform Layer (`platform/`)
- `renderer.hpp` - Platform abstraction for rendering
- `linux_renderer.cpp` - Linux-specific implementation
- `web_renderer.cpp` - WebAssembly implementation
- `platform_factory.cpp` - Platform detection and factory

### C Bindings (`src/c/`)

The C API provides a simplified interface for:
- C applications that need UI functionality
- Language bindings for other languages
- WebAssembly builds with minimal overhead

## Header Files

### Public API Structure

```cpp
// Main include - brings in the entire framework
#include <fern/fern.hpp>

// Or include specific modules
#include <fern/core/canvas.hpp>
#include <fern/ui/widgets/button_widget.hpp>
#include <fern/graphics/colors.hpp>
```

### Header Organization Principles

1. **Minimal dependencies** - Headers include only what they need
2. **Forward declarations** - Reduce compilation dependencies
3. **Namespace organization** - All framework code in `Fern` namespace
4. **Clear interfaces** - Public API separated from implementation details

### Include Patterns

```cpp
// Widget headers follow this pattern:
#pragma once
#include "widget.hpp"              // Base class
#include "../../core/signal.hpp"   // For events
#include <memory>                  // Standard includes

namespace Fern {
    class SpecificWidget : public Widget {
        // Implementation...
    };
    
    // Factory function
    std::shared_ptr<SpecificWidget> CreateWidget(/* parameters */);
}
```

## Documentation Structure

### Organization by Audience

- **Users** - Getting started, widget guides, examples
- **Developers** - API reference, architecture, contributing
- **Platform specific** - Linux, Web, cross-platform development

### Documentation Types

1. **Tutorials** - Step-by-step learning paths
2. **Guides** - Task-oriented documentation
3. **Reference** - Complete API documentation
4. **Explanations** - Architecture and design decisions

## Examples and Tests

### Example Categories

- **Basic examples** (`examples/c/`, `examples/cpp/`)
  - Simple widget usage
  - Layout demonstrations
  - Input handling
  - Basic graphics

- **Advanced examples**
  - Complex applications
  - Performance demonstrations
  - Platform-specific features

### Example Naming Convention

```
examples/
├── cpp/
│   ├── basic_counter_example.cpp      # Simple functionality
│   ├── complex_layout.cpp             # Layout features
│   ├── font_demo.cpp                  # Font system
│   ├── responsive_example.cpp         # Responsive design
│   └── text_input_example.cpp         # Input handling
└── c/
    ├── cyberpunk.c                    # Themed example
    ├── fractal_explorer.c             # Graphics demo
    ├── life_sim.c                     # Game-like example
    └── white_board.c                  # Drawing application
```

## Build System Files

### CMake Organization

- `CMakeLists.txt` (root) - Main build configuration
- `src/cpp/CMakeLists.txt` - C++ framework build rules
- Platform-specific configurations embedded in CMake files

### Build Artifacts

```
build/                    # Build directory (not in repo)
├── lib/                 # Compiled libraries
├── bin/                 # Example executables
├── docs/                # Generated documentation
└── tests/               # Test executables
```

## Asset Management

### Asset Types

- **Images** - Screenshots, logos, example assets
- **Fonts** - Embedded or referenced font files
- **Templates** - HTML templates for web builds
- **Resources** - Any files needed by examples

### Asset Organization

```
assets/
├── images/              # Screenshots and graphics
├── fonts/               # Font files (if any)
├── templates/           # Build templates
└── examples/            # Assets used by examples
```

## Development Workflow

### Adding New Components

1. **Create header** in appropriate `include/fern/` subdirectory
2. **Create implementation** in corresponding `src/` subdirectory
3. **Add to CMakeLists.txt** if needed
4. **Write documentation** in `docs/`
5. **Create example** demonstrating usage
6. **Add tests** if applicable

### File Naming Conventions

- **Headers**: `component_name.hpp`
- **Implementations**: `component_name.cpp`
- **Documentation**: `component-name.md` (kebab-case)
- **Examples**: `descriptive_example.cpp`

### Code Organization Guidelines

1. **Single responsibility** - Each file has a clear purpose
2. **Consistent naming** - Follow established patterns
3. **Proper namespacing** - Use `Fern` namespace consistently
4. **Include guards** - Use `#pragma once`
5. **Documentation** - Document public APIs with docstrings

## Integration Points

### How Components Connect

```cpp
// Typical application structure:
Application
├── WidgetManager        // Manages widget lifecycle
├── Canvas              // Provides drawing context
├── Input               // Handles user input
├── Renderer            // Platform-specific rendering
└── Widgets             // UI components
    ├── Layout widgets  // Arrange other widgets
    ├── Container widgets // Group widgets
    └── Leaf widgets    // Individual UI elements
```

### Data Flow

1. **Input** → Input system → Widget event handlers
2. **Layout** → Layout system → Widget positioning
3. **Rendering** → Widget render methods → Canvas → Platform renderer
4. **Events** → Signal system → Application callbacks

## Conclusion

This project structure promotes:
- **Modularity** - Clear separation of concerns
- **Maintainability** - Logical organization and naming
- **Extensibility** - Easy to add new components
- **Cross-platform** - Platform abstraction where needed
- **Documentation** - Comprehensive guides and examples

For questions about the project structure or suggestions for improvements, see the [Contributing Guide](development/contributing.md).
