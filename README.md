<p align="center">
  <img src="assets/logo.png" alt="Fern Graphics Logo" width="200"/>
</p>

<h1 align="center">Fern UI Framework</h1>

<p align="center">
  A fast, minimal and cross-platform UI toolkit written in C++ with modern design principles.
</p>


<p align="center">
  <a href="https://github.com/fernkit/fern/blob/main/LICENCE">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="License" />
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/status-beta-orange" alt="Status: Beta" />
  </a>
</p>

A cross-platform graphics library designed for building beautiful user interfaces with simplicity and performance in mind. Fern provides a comprehensive widget system, flexible layout management, and seamless cross-platform rendering capabilities.

## Overview

Fern Graphics offers developers a powerful yet intuitive framework for creating desktop applications with rich graphical interfaces. Built with modern C++ and featuring C bindings, Fern emphasizes clean APIs, efficient rendering, and cross-platform compatibility.

**Key Features:**
- Comprehensive widget system with buttons, text inputs, shapes, and custom components
- Flexible layout management with containers, spacing, and responsive design
- Cross-platform rendering (Linux native, Web via WebAssembly)
- Font rendering with TTF support
- Signal-slot event system for reactive programming
- Canvas-based drawing with primitive graphics support
- Memory-efficient design with minimal dependencies

## Quick Start

Get started with Fern in minutes by following our comprehensive [Getting Started Guide](docs/getting-started.md), which walks you through:

- Setting up your development environment
- Creating your first Fern application
- Understanding the core concepts
- Building and running examples

## Architecture

Fern is built around several core subsystems that work together to provide a cohesive development experience:

- **Widget System**: Hierarchical component architecture with built-in and custom widgets
- **Layout Engine**: Flexible positioning and sizing with containers and constraints
- **Rendering Pipeline**: Platform-abstracted graphics with efficient drawing operations
- **Event System**: Signal-slot based event handling for user interactions
- **Font Management**: TTF font loading and text rendering capabilities

## Widget System

Fern provides a rich set of built-in widgets and the tools to create custom components:

### Core Widgets
- **Text**: Styled text rendering with font customization
- **Button**: Interactive buttons with click handling
- **Text Input**: Single and multi-line text entry fields
- **Shapes**: Circles, lines, and custom geometric shapes

### Layout Containers
- **Row/Column**: Linear layout arrangement
- **Padding**: Spacing and margins around widgets
- **Center**: Widget centering and alignment
- **Expanded**: Flexible space distribution
- **Sized Box**: Constrain a widget to a fixed width and height.
- **Center**: Align a single child widget at the center of its parent.
  
### Advanced Components
- **Slider**: Value selection with customizable ranges
- **Progress Bar**: Visual progress indication
- **Radio Buttons**: Single-selection option groups
- **Dropdown**: Expandable selection menus
- **Circular indicator**: Spinning or static indicator for loading or progress feedback.

Learn more in the [Widget System Documentation](docs/core/widget-system.md).

## Cross-Platform Support

Fern targets multiple platforms with consistent APIs and behavior:

- **Linux**: Native rendering with X11/Wayland support
- **Web**: WebAssembly compilation for browser deployment
- **Windows**: Planned support with native Windows APIs
- **macOS**: Planned support with Cocoa integration

## Documentation

Comprehensive documentation is available in the `docs/` directory:

- [Getting Started Guide](docs/getting-started.md) - Your first steps with Fern
- [Widget Documentation](docs/widgets/) - Individual widget guides and examples
- [Core Systems](docs/core/) - Deep dives into Fern's architecture
- [Graphics and Styling](docs/graphics/) - Colors, fonts, and visual customization
- [Layout System](docs/layout/) - Positioning and arrangement strategies
- [Cross-Platform Guide](docs/platform/) - Platform-specific considerations

## Contributing

We welcome contributions to Fern Graphics. Please read our [Contributing Guidelines](CONTRIBUTING.md) for details on:

- Code style and conventions
- Submitting pull requests
- Reporting issues and bugs
- Development setup and testing

## Requirements

- C++17 or later
- CMake 3.10+
- Platform-specific graphics libraries (varies by target)

## License

Fern Graphics is released under the [License](LICENCE). See the license file for complete terms and conditions.

---

**Note**: For developers seeking more direct control over graphics operations, we also provide comprehensive [C-only documentation](docs/c-docs.md) that covers the lower-level C API and manual memory management approaches.
