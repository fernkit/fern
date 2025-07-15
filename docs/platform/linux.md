# Linux Platform Guide

## Overview

The Fern UI Framework provides native Linux support through the X11 windowing system, offering high-performance rendering and complete access to system resources. This guide covers Linux-specific features, setup, and deployment.

## Table of Contents

1. [System Requirements](#system-requirements)
2. [Installation](#installation)
3. [Linux-Specific Features](#linux-specific-features)
4. [Build Configuration](#build-configuration)
5. [Window Management](#window-management)
6. [Input Handling](#input-handling)
7. [Performance Optimization](#performance-optimization)
8. [Troubleshooting](#troubleshooting)
9. [Best Practices](#best-practices)

## System Requirements

### Required Dependencies

- **X11 Development Libraries**: Core windowing system
- **GCC/Clang**: C++17 compatible compiler
- **CMake**: Build system (3.10+)
- **pkg-config**: Package configuration tool

### Ubuntu/Debian Installation

```bash
# Install core dependencies
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libx11-dev \
    pkg-config \
    git

# Optional: Additional X11 extensions
sudo apt-get install -y \
    libxi-dev \
    libxext-dev \
    libxrandr-dev
```

### Fedora/RHEL Installation

```bash
# Install core dependencies
sudo dnf groupinstall "Development Tools"
sudo dnf install -y \
    cmake \
    libX11-devel \
    pkg-config \
    git

# Optional: Additional X11 extensions
sudo dnf install -y \
    libXi-devel \
    libXext-devel \
    libXrandr-devel
```

### Arch Linux Installation

```bash
# Install core dependencies
sudo pacman -S \
    base-devel \
    cmake \
    libx11 \
    pkg-config \
    git

# Optional: Additional X11 extensions
sudo pacman -S \
    libxi \
    libxext \
    libxrandr
```

## Installation

### Building from Source

```bash
# Clone the repository
git clone https://github.com/fernkit/fern.git
cd fern

# Configure for Linux
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
make -j$(nproc)

# Install system-wide (optional)
sudo make install
```

### Using the Build Script

```bash
# Simple Linux build
./build.sh linux

# Debug build
./build.sh linux debug

# Release build with optimization
./build.sh linux release
```

## Linux-Specific Features

### X11 Integration

The Linux implementation uses X11 for windowing:

```cpp
#include <fern/fern.hpp>

int main() {
    // Initialize with X11 backend
    Fern::initialize(800, 600);
    
    // Window automatically integrates with X11 window manager
    Fern::setDrawCallback([]() {
        // Your rendering code here
    });
    
    // Start the X11 event loop
    Fern::startRenderLoop();
    
    return 0;
}
```

### Native Performance

- **Zero-copy rendering**: Direct pixel buffer access
- **Hardware acceleration**: Utilizes GPU when available
- **Efficient event handling**: Direct X11 event processing
- **Memory management**: Optimized buffer allocation

### System Integration

```cpp
// Access to Linux-specific features
#include <fern/platform/renderer.hpp>

// Get platform information
auto renderer = Fern::createRenderer();
std::string platform = renderer->getPlatformName(); // "Linux (X11)"

// Window management
renderer->setTitle("My Fern Application");
renderer->setSize(1024, 768);
```

## Build Configuration

### CMake Configuration

```cmake
# CMakeLists.txt for Linux application
cmake_minimum_required(VERSION 3.10)
project(MyFernApp)

set(CMAKE_CXX_STANDARD 17)

# Find Fern
find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11)

# Add your application
add_executable(myapp main.cpp)

# Link Fern and X11
target_link_libraries(myapp 
    fern 
    ${X11_LIBRARIES}
)

target_include_directories(myapp PRIVATE 
    ${X11_INCLUDE_DIRS}
)
```

### Compiler Flags

```bash
# Manual compilation
g++ -std=c++17 \
    -I/usr/local/include \
    -L/usr/local/lib \
    -lfern \
    -lX11 \
    -o myapp main.cpp

# With optimization
g++ -std=c++17 -O3 -march=native \
    -I/usr/local/include \
    -L/usr/local/lib \
    -lfern \
    -lX11 \
    -o myapp main.cpp
```

## Window Management

### Basic Window Operations

```cpp
#include <fern/fern.hpp>

int main() {
    // Create window
    Fern::initialize(800, 600);
    
    // Set window properties
    auto renderer = Fern::createRenderer();
    renderer->setTitle("My Linux App");
    
    // Handle window events
    renderer->setResizeCallback([](int width, int height) {
        std::cout << "Window resized to: " << width << "x" << height << std::endl;
        // Resize your canvas if needed
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

### Advanced Window Features

```cpp
// Custom window setup
class LinuxWindow {
private:
    std::unique_ptr<Fern::PlatformRenderer> renderer_;
    
public:
    LinuxWindow(int width, int height) {
        renderer_ = Fern::createRenderer();
        renderer_->initialize(width, height);
        
        // Configure window
        renderer_->setTitle("Advanced Fern App");
        
        // Set up event handlers
        setupEventHandlers();
    }
    
    void setupEventHandlers() {
        // Mouse events
        renderer_->setMouseCallback([this](int x, int y) {
            handleMouseMove(x, y);
        });
        
        // Keyboard events
        renderer_->setKeyCallback([this](Fern::KeyCode key, bool pressed) {
            handleKeyEvent(key, pressed);
        });
        
        // Window management
        renderer_->setResizeCallback([this](int w, int h) {
            handleResize(w, h);
        });
    }
    
    void handleMouseMove(int x, int y) {
        // Handle mouse movement
    }
    
    void handleKeyEvent(Fern::KeyCode key, bool pressed) {
        // Handle keyboard input
        if (key == Fern::KeyCode::Escape && pressed) {
            // Exit application
            renderer_->shutdown();
        }
    }
    
    void handleResize(int width, int height) {
        // Handle window resize
        // Update canvas dimensions
    }
};
```

## Input Handling

### Keyboard Input

```cpp
#include <fern/core/input.hpp>

void handleInput() {
    auto& input = Fern::Input::getState();
    
    // Check key states
    if (input.isKeyPressed(Fern::KeyCode::W)) {
        // Move forward
    }
    
    if (input.isKeyPressed(Fern::KeyCode::Space)) {
        // Jump
    }
    
    // Handle text input
    if (!input.textInput.empty()) {
        for (const auto& text : input.textInput) {
            std::cout << "Text input: " << text << std::endl;
        }
    }
}
```

### Mouse Input

```cpp
void handleMouse() {
    auto& input = Fern::Input::getState();
    
    // Mouse position
    int mouseX = input.mouseX;
    int mouseY = input.mouseY;
    
    // Mouse buttons
    if (input.mousePressed) {
        std::cout << "Mouse clicked at: " << mouseX << ", " << mouseY << std::endl;
    }
    
    // Mouse events
    if (input.hasMouseEvent()) {
        // Handle mouse movement
        processMouseMovement(mouseX, mouseY);
    }
}
```

## Performance Optimization

### Rendering Optimization

```cpp
// Optimized rendering loop
class OptimizedRenderer {
private:
    std::unique_ptr<uint32_t[]> backBuffer_;
    bool needsRedraw_;
    
public:
    void initialize(int width, int height) {
        backBuffer_ = std::make_unique<uint32_t[]>(width * height);
        needsRedraw_ = true;
    }
    
    void render() {
        if (!needsRedraw_) return;
        
        // Only redraw when necessary
        renderToBuffer();
        needsRedraw_ = false;
    }
    
    void invalidate() {
        needsRedraw_ = true;
    }
    
private:
    void renderToBuffer() {
        // Render to back buffer
        // Use hardware acceleration when available
    }
};
```

### Memory Management

```cpp
// Efficient buffer management
class BufferManager {
private:
    std::vector<std::unique_ptr<uint32_t[]>> bufferPool_;
    size_t bufferSize_;
    
public:
    BufferManager(size_t size) : bufferSize_(size) {
        // Pre-allocate buffers
        for (int i = 0; i < 3; ++i) {
            bufferPool_.push_back(
                std::make_unique<uint32_t[]>(bufferSize_)
            );
        }
    }
    
    uint32_t* getBuffer() {
        // Return available buffer from pool
        // Implement buffer recycling
    }
};
```

## Troubleshooting

### Common Issues

#### X11 Connection Failed
```bash
# Check DISPLAY environment variable
echo $DISPLAY

# Ensure X11 server is running
ps aux | grep X

# Test X11 connection
xdpyinfo
```

#### Missing Dependencies
```bash
# Check for X11 development files
pkg-config --cflags --libs x11

# Verify installation
dpkg -l | grep libx11-dev  # Ubuntu/Debian
rpm -qa | grep libX11-devel  # Fedora/RHEL
```

#### Performance Issues
```bash
# Check graphics driver
lspci | grep VGA
glxinfo | grep "direct rendering"

# Monitor system resources
htop
nvidia-smi  # For NVIDIA GPUs
```

### Debugging

```cpp
// Debug build configuration
#ifdef DEBUG
    // Enable detailed logging
    renderer->setDebugMode(true);
    
    // Performance monitoring
    auto startTime = std::chrono::high_resolution_clock::now();
    // ... rendering code ...
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        endTime - startTime
    );
    
    std::cout << "Render time: " << duration.count() << " µs" << std::endl;
#endif
```

## Best Practices

### Resource Management

```cpp
// RAII for X11 resources
class XResourceManager {
private:
    Display* display_;
    Window window_;
    
public:
    XResourceManager() : display_(nullptr), window_(0) {}
    
    ~XResourceManager() {
        cleanup();
    }
    
    void cleanup() {
        if (window_) {
            XDestroyWindow(display_, window_);
            window_ = 0;
        }
        
        if (display_) {
            XCloseDisplay(display_);
            display_ = nullptr;
        }
    }
};
```

### Error Handling

```cpp
// Robust error handling
try {
    Fern::initialize(800, 600);
    
    // Application logic
    runApplication();
    
} catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return 1;
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
```

### Threading

```cpp
// Thread-safe rendering
class ThreadSafeRenderer {
private:
    std::mutex renderMutex_;
    std::condition_variable renderCondition_;
    
public:
    void render() {
        std::lock_guard<std::mutex> lock(renderMutex_);
        
        // Safe rendering operations
        performRendering();
        
        renderCondition_.notify_all();
    }
    
    void waitForRender() {
        std::unique_lock<std::mutex> lock(renderMutex_);
        renderCondition_.wait(lock);
    }
};
```

## Complete Example

```cpp
#include <fern/fern.hpp>
#include <iostream>

class LinuxFernApp {
private:
    bool running_;
    
public:
    LinuxFernApp() : running_(true) {}
    
    int run() {
        try {
            // Initialize Fern for Linux
            Fern::initialize(800, 600);
            
            // Set up callbacks
            setupCallbacks();
            
            // Start the application
            Fern::startRenderLoop();
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        
        return 0;
    }
    
private:
    void setupCallbacks() {
        // Set render callback
        Fern::setDrawCallback([this]() {
            render();
        });
        
        // Set up input handling
        auto renderer = Fern::createRenderer();
        renderer->setKeyCallback([this](Fern::KeyCode key, bool pressed) {
            if (key == Fern::KeyCode::Escape && pressed) {
                running_ = false;
            }
        });
    }
    
    void render() {
        if (!running_) return;
        
        // Clear screen
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw UI elements
        drawUI();
    }
    
    void drawUI() {
        // Example UI rendering
        Fern::Canvas::drawText("Linux Fern Application", 10, 10, 
                              Fern::Colors::WHITE);
        
        Fern::Canvas::drawRect(100, 100, 200, 100, 
                              Fern::Colors::BLUE);
    }
};

int main() {
    LinuxFernApp app;
    return app.run();
}
```

## Related Documentation

- [Getting Started Guide](../getting-started.md)
- [Cross-Platform Development](cross-platform.md)
- [Web Platform Guide](web.md)
- [Input Handling](../input/input-handling.md)
- [Performance Guide](../development/performance.md)

---

*This documentation covers Fern UI Framework v1.0 for Linux platforms. For updates and more information, visit the [project repository](https://github.com/fernkit/fern).*
