# Web Platform Guide

## Overview

The Fern UI Framework provides comprehensive web support through WebAssembly (WASM) and Emscripten, enabling native-performance applications that run directly in modern web browsers. This guide covers web-specific features, deployment, and optimization techniques.

## Table of Contents

1. [Web Architecture](#web-architecture)
2. [System Requirements](#system-requirements)
3. [Installation & Setup](#installation--setup)
4. [Build Configuration](#build-configuration)
5. [Web-Specific Features](#web-specific-features)
6. [Canvas Integration](#canvas-integration)
7. [Event Handling](#event-handling)
8. [Performance Optimization](#performance-optimization)
9. [Deployment](#deployment)
10. [Browser Compatibility](#browser-compatibility)
11. [Troubleshooting](#troubleshooting)
12. [Best Practices](#best-practices)

## Web Architecture

### WebAssembly Integration

Fern compiles to WebAssembly for near-native performance in browsers:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Fern C++ App  │ -> │  Emscripten     │ -> │  WebAssembly    │
│                 │    │  Compiler       │    │  + JavaScript   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                                      │
                                                      v
                                              ┌─────────────────┐
                                              │  Browser        │
                                              │  (Canvas API)   │
                                              └─────────────────┘
```

### Key Components

- **WebAssembly Module**: Compiled C++ code
- **JavaScript Glue**: Event handling and DOM interaction
- **Canvas Element**: Rendering surface
- **Memory Management**: Shared heap between WASM and JS

## System Requirements

### Development Environment

- **Emscripten SDK**: Latest version (3.1.0+)
- **Python**: 3.6+ (for Emscripten tools)
- **Node.js**: 14+ (for development server)
- **Modern Browser**: Chrome 89+, Firefox 88+, Safari 14+

### Target Browser Requirements

- **WebAssembly Support**: All modern browsers
- **Canvas 2D Context**: Required for rendering
- **Typed Arrays**: For efficient memory access
- **ES6 Modules**: For modern JavaScript features

## Installation & Setup

### Install Emscripten

```bash
# Download and install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install latest SDK
./emsdk install latest
./emsdk activate latest

# Set up environment
source ./emsdk_env.sh
```

### Verify Installation

```bash
# Check Emscripten version
emcc --version

# Test WebAssembly compilation
echo '#include <stdio.h>
int main() { printf("Hello Web!\\n"); return 0; }' > test.c
emcc test.c -o test.html
```

### Build Fern for Web

```bash
# Clone Fern repository
git clone https://github.com/fernkit/fern.git
cd fern

# Build for web
./build.sh web

# Or using CMake directly
mkdir build-web && cd build-web
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make
```

## Build Configuration

### CMake Configuration

```cmake
# CMakeLists.txt for web builds
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGL2=1")
    
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
        -s WASM=1 \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s USE_WEBGL2=1 \
        -s EXPORTED_FUNCTIONS='[_main]' \
        -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' \
        --preload-file assets/")
endif()
```

### Manual Build Commands

```bash
# Basic web build
emcc -std=c++17 \
    -I src/cpp/include \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s USE_WEBGL2=1 \
    -o myapp.html \
    src/cpp/src/fern.cpp \
    main.cpp

# Optimized production build
emcc -std=c++17 -O3 \
    -I src/cpp/include \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s USE_WEBGL2=1 \
    -s EXPORTED_FUNCTIONS='[_main]' \
    -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' \
    --closure 1 \
    -o myapp.html \
    src/cpp/src/fern.cpp \
    main.cpp
```

## Web-Specific Features

### Canvas Integration

```cpp
#include <fern/fern.hpp>

int main() {
    // Initialize for web platform
    Fern::initialize(800, 600);
    
    // Web-specific setup
    Fern::setDrawCallback([]() {
        // Clear canvas
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw web-optimized UI
        drawWebUI();
    });
    
    // Start web render loop
    Fern::startRenderLoop();
    
    return 0;
}
```

### JavaScript Interop

```cpp
// Call JavaScript from C++
#include <emscripten.h>

void callJavaScript() {
    // Execute JavaScript code
    EM_ASM({
        console.log("Hello from C++!");
        document.title = "Fern Web App";
    });
    
    // Call with parameters
    EM_ASM({
        var message = UTF8ToString($0);
        alert(message);
    }, "Hello from Fern!");
}

// Export C++ functions to JavaScript
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void updateFromJavaScript(int value) {
        // Handle JavaScript callback
        std::cout << "Received from JS: " << value << std::endl;
    }
}
```

### HTML Integration

```html
<!DOCTYPE html>
<html>
<head>
    <title>Fern Web Application</title>
    <style>
        body {
            margin: 0;
            padding: 20px;
            font-family: Arial, sans-serif;
            background-color: #1a1a1a;
        }
        
        #canvas {
            border: 1px solid #333;
            display: block;
            margin: 20px auto;
        }
        
        #controls {
            text-align: center;
            color: white;
        }
    </style>
</head>
<body>
    <div id="controls">
        <h1>Fern Web Application</h1>
        <button onclick="resetApp()">Reset</button>
        <button onclick="saveState()">Save</button>
    </div>
    
    <canvas id="canvas"></canvas>
    
    <script>
        // JavaScript integration
        function resetApp() {
            if (Module && Module._updateFromJavaScript) {
                Module._updateFromJavaScript(0);
            }
        }
        
        function saveState() {
            // Save application state
            localStorage.setItem('fernAppState', JSON.stringify({
                timestamp: Date.now(),
                // Add your state data
            }));
        }
        
        // Module configuration
        var Module = {
            onRuntimeInitialized: function() {
                console.log("Fern WebAssembly module loaded");
                
                // Initialize canvas
                var canvas = document.getElementById('canvas');
                canvas.focus();
            },
            
            canvas: (function() {
                var canvas = document.getElementById('canvas');
                return canvas;
            })()
        };
    </script>
    
    <script src="myapp.js"></script>
</body>
</html>
```

## Canvas Integration

### Canvas Setup

```cpp
class WebCanvas {
private:
    int width_, height_;
    std::unique_ptr<uint32_t[]> pixelBuffer_;
    
public:
    WebCanvas(int width, int height) : width_(width), height_(height) {
        pixelBuffer_ = std::make_unique<uint32_t[]>(width * height);
        
        // Setup canvas element
        EM_ASM({
            var canvas = document.getElementById('canvas');
            if (!canvas) {
                canvas = document.createElement('canvas');
                canvas.id = 'canvas';
                document.body.appendChild(canvas);
            }
            
            canvas.width = $0;
            canvas.height = $1;
            canvas.style.imageRendering = 'pixelated'; // For pixel art
            
            // Make focusable for keyboard input
            canvas.setAttribute('tabindex', '0');
        }, width, height);
    }
    
    void present() {
        // Present pixel buffer to canvas
        EM_ASM({
            var canvas = document.getElementById('canvas');
            var ctx = canvas.getContext('2d');
            
            var imageData = ctx.createImageData($0, $1);
            var data = imageData.data;
            var buffer = $2;
            
            for (var i = 0; i < $0 * $1; i++) {
                var pixel = HEAP32[buffer/4 + i];
                var j = i * 4;
                data[j + 0] = (pixel >> 16) & 0xFF; // R
                data[j + 1] = (pixel >> 8) & 0xFF;  // G
                data[j + 2] = pixel & 0xFF;         // B
                data[j + 3] = (pixel >> 24) & 0xFF; // A
            }
            
            ctx.putImageData(imageData, 0, 0);
        }, width_, height_, pixelBuffer_.get());
    }
};
```

### High-DPI Support

```cpp
void setupHighDPI() {
    EM_ASM({
        var canvas = document.getElementById('canvas');
        var ctx = canvas.getContext('2d');
        var dpr = window.devicePixelRatio || 1;
        
        // Scale canvas for high-DPI displays
        var rect = canvas.getBoundingClientRect();
        canvas.width = rect.width * dpr;
        canvas.height = rect.height * dpr;
        
        ctx.scale(dpr, dpr);
        
        canvas.style.width = rect.width + 'px';
        canvas.style.height = rect.height + 'px';
    });
}
```

## Event Handling

### Mouse Events

```cpp
class WebEventHandler {
private:
    std::function<void(int, int)> mouseCallback_;
    std::function<void(bool)> clickCallback_;
    
public:
    void setupMouseEvents() {
        EM_ASM({
            var canvas = document.getElementById('canvas');
            var handler = $0;
            
            canvas.addEventListener('mousemove', function(e) {
                var rect = canvas.getBoundingClientRect();
                var x = Math.floor((e.clientX - rect.left) * 
                                 (canvas.width / rect.width));
                var y = Math.floor((e.clientY - rect.top) * 
                                 (canvas.height / rect.height));
                
                Module._webMouseMove(handler, x, y);
            });
            
            canvas.addEventListener('mousedown', function(e) {
                Module._webMouseClick(handler, 1);
                e.preventDefault();
            });
            
            canvas.addEventListener('mouseup', function(e) {
                Module._webMouseClick(handler, 0);
                e.preventDefault();
            });
        }, this);
    }
    
    // Export to JavaScript
    void onMouseMove(int x, int y) {
        if (mouseCallback_) mouseCallback_(x, y);
    }
    
    void onMouseClick(bool down) {
        if (clickCallback_) clickCallback_(down);
    }
};

// Export functions
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void webMouseMove(WebEventHandler* handler, int x, int y) {
        handler->onMouseMove(x, y);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void webMouseClick(WebEventHandler* handler, bool down) {
        handler->onMouseClick(down);
    }
}
```

### Keyboard Events

```cpp
void setupKeyboardEvents() {
    EM_ASM({
        var canvas = document.getElementById('canvas');
        
        // Focus management
        canvas.addEventListener('click', function() {
            canvas.focus();
        });
        
        // Keyboard events
        document.addEventListener('keydown', function(e) {
            if (document.activeElement === canvas) {
                Module._webKeyEvent(e.keyCode, 1);
                
                // Prevent default for game keys
                if ([8, 9, 13, 27, 37, 38, 39, 40].includes(e.keyCode)) {
                    e.preventDefault();
                }
            }
        });
        
        document.addEventListener('keyup', function(e) {
            if (document.activeElement === canvas) {
                Module._webKeyEvent(e.keyCode, 0);
            }
        });
        
        // Text input
        document.addEventListener('keypress', function(e) {
            if (document.activeElement === canvas && 
                e.charCode >= 32 && e.charCode <= 126) {
                Module._webTextInput(e.charCode);
            }
        });
    });
}
```

## Performance Optimization

### Memory Management

```cpp
class WebMemoryManager {
private:
    size_t heapSize_;
    bool allowGrowth_;
    
public:
    WebMemoryManager(size_t initialSize = 16 * 1024 * 1024) 
        : heapSize_(initialSize), allowGrowth_(true) {
        
        // Configure memory settings
        EM_ASM({
            Module.TOTAL_MEMORY = $0;
            Module.ALLOW_MEMORY_GROWTH = $1;
        }, heapSize_, allowGrowth_);
    }
    
    void optimizeMemory() {
        // Garbage collection hint
        EM_ASM({
            if (typeof gc !== 'undefined') {
                gc();
            }
        });
    }
    
    size_t getMemoryUsage() {
        return EM_ASM_INT({
            return Module.HEAP8.length;
        });
    }
};
```

### Rendering Optimization

```cpp
class WebRenderer {
private:
    bool needsRedraw_;
    std::chrono::steady_clock::time_point lastFrame_;
    
public:
    void render() {
        auto now = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastFrame_).count();
        
        // Limit frame rate
        if (deltaTime < 16) return; // ~60 FPS
        
        if (needsRedraw_) {
            performRender();
            needsRedraw_ = false;
        }
        
        lastFrame_ = now;
    }
    
    void invalidate() {
        needsRedraw_ = true;
    }
    
private:
    void performRender() {
        // Optimized rendering code
        // Use efficient drawing operations
        // Minimize canvas operations
    }
};
```

### Asset Loading

```cpp
class WebAssetLoader {
public:
    void loadAsset(const std::string& url) {
        EM_ASM({
            var url = UTF8ToString($0);
            
            fetch(url)
                .then(response => response.arrayBuffer())
                .then(data => {
                    var buffer = new Uint8Array(data);
                    var ptr = Module._malloc(buffer.length);
                    Module.HEAP8.set(buffer, ptr);
                    
                    // Notify C++ code
                    Module._assetLoaded(ptr, buffer.length);
                })
                .catch(error => {
                    console.error('Asset loading failed:', error);
                });
        }, url.c_str());
    }
};

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void assetLoaded(uint8_t* data, size_t size) {
        // Process loaded asset
        processAsset(data, size);
        
        // Free memory
        free(data);
    }
}
```

## Deployment

### Static File Hosting

```bash
# Build for production
emcc -O3 -std=c++17 \
    -I src/cpp/include \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --closure 1 \
    -o dist/myapp.html \
    src/cpp/src/fern.cpp \
    main.cpp

# Deploy to web server
cp dist/* /var/www/html/
```

### CDN Deployment

```html
<!-- Include from CDN -->
<script src="https://cdn.jsdelivr.net/npm/fern-ui@latest/dist/fern.js"></script>

<!-- Or self-hosted -->
<script src="./fern.js"></script>
```

### Progressive Web App

```json
// manifest.json
{
  "name": "Fern UI Application",
  "short_name": "FernApp",
  "start_url": "/",
  "display": "standalone",
  "background_color": "#1a1a1a",
  "theme_color": "#4a90e2",
  "icons": [
    {
      "src": "icon-192.png",
      "sizes": "192x192",
      "type": "image/png"
    }
  ]
}
```

## Browser Compatibility

### Feature Detection

```cpp
bool checkWebAssemblySupport() {
    return EM_ASM_INT({
        return typeof WebAssembly !== 'undefined';
    });
}

bool checkCanvasSupport() {
    return EM_ASM_INT({
        var canvas = document.createElement('canvas');
        return !!(canvas.getContext && canvas.getContext('2d'));
    });
}
```

### Fallback Handling

```javascript
// Fallback for older browsers
if (!window.WebAssembly) {
    document.body.innerHTML = `
        <div style="text-align: center; padding: 50px;">
            <h1>Browser Not Supported</h1>
            <p>This application requires WebAssembly support.</p>
            <p>Please update your browser or use a modern browser.</p>
        </div>
    `;
}
```

## Troubleshooting

### Common Issues

#### WASM Loading Failed
```javascript
// Debug WASM loading
Module.onAbort = function(what) {
    console.error('WASM aborted:', what);
};

Module.onRuntimeInitialized = function() {
    console.log('WASM initialized successfully');
};
```

#### Memory Issues
```cpp
// Monitor memory usage
void checkMemory() {
    size_t used = EM_ASM_INT({
        return Module.HEAP8.length;
    });
    
    std::cout << "Memory usage: " << used << " bytes" << std::endl;
}
```

#### Performance Problems
```javascript
// Performance monitoring
function monitorPerformance() {
    if (performance.memory) {
        console.log('JS Memory:', performance.memory.usedJSHeapSize);
    }
    
    requestAnimationFrame(monitorPerformance);
}
```

## Best Practices

### Code Organization

```cpp
// Modular web application
class WebApplication {
private:
    std::unique_ptr<WebRenderer> renderer_;
    std::unique_ptr<WebEventHandler> eventHandler_;
    std::unique_ptr<WebAssetLoader> assetLoader_;
    
public:
    void initialize() {
        renderer_ = std::make_unique<WebRenderer>();
        eventHandler_ = std::make_unique<WebEventHandler>();
        assetLoader_ = std::make_unique<WebAssetLoader>();
        
        setupComponents();
    }
    
    void run() {
        // Main application loop
        emscripten_set_main_loop_arg(mainLoop, this, 0, 1);
    }
    
private:
    static void mainLoop(void* userData) {
        static_cast<WebApplication*>(userData)->update();
    }
    
    void update() {
        // Update logic
        renderer_->render();
    }
};
```

### Error Handling

```cpp
void handleWebError(const std::string& message) {
    EM_ASM({
        var msg = UTF8ToString($0);
        console.error('Fern Error:', msg);
        
        // Show user-friendly error
        var errorDiv = document.createElement('div');
        errorDiv.innerHTML = 'Application Error: ' + msg;
        errorDiv.style.cssText = `
            position: fixed;
            top: 10px;
            right: 10px;
            background: #ff4444;
            color: white;
            padding: 10px;
            border-radius: 5px;
            z-index: 1000;
        `;
        document.body.appendChild(errorDiv);
    }, message.c_str());
}
```

## Complete Example

```cpp
#include <fern/fern.hpp>
#include <emscripten.h>

class WebFernApp {
private:
    bool running_;
    int frameCount_;
    
public:
    WebFernApp() : running_(true), frameCount_(0) {}
    
    void initialize() {
        // Initialize Fern for web
        Fern::initialize(800, 600);
        
        // Set up web-specific features
        setupWebFeatures();
        
        // Set render callback
        Fern::setDrawCallback([this]() {
            this->render();
        });
    }
    
    void run() {
        // Start web render loop
        Fern::startRenderLoop();
    }
    
private:
    void setupWebFeatures() {
        // Configure canvas
        EM_ASM({
            var canvas = document.getElementById('canvas');
            canvas.style.border = '2px solid #4a90e2';
            canvas.style.borderRadius = '8px';
            
            // Add loading indicator
            var loading = document.createElement('div');
            loading.innerHTML = 'Loading Fern Application...';
            loading.style.cssText = `
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
                color: white;
                font-size: 18px;
            `;
            document.body.appendChild(loading);
            
            // Remove loading after initialization
            setTimeout(() => {
                if (loading.parentNode) {
                    loading.parentNode.removeChild(loading);
                }
            }, 1000);
        });
    }
    
    void render() {
        if (!running_) return;
        
        // Clear canvas
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw UI
        drawWebUI();
        
        frameCount_++;
    }
    
    void drawWebUI() {
        // Draw title
        Fern::Canvas::drawText("Fern Web Application", 10, 10, 
                              Fern::Colors::WHITE);
        
        // Draw frame counter
        std::string frameText = "Frame: " + std::to_string(frameCount_);
        Fern::Canvas::drawText(frameText, 10, 30, 
                              Fern::Colors::GRAY);
        
        // Draw interactive elements
        Fern::Canvas::drawRect(100, 100, 200, 100, 
                              Fern::Colors::BLUE);
        
        Fern::Canvas::drawText("Click me!", 120, 140, 
                              Fern::Colors::WHITE);
    }
};

// Global app instance
WebFernApp* app = nullptr;

int main() {
    app = new WebFernApp();
    app->initialize();
    app->run();
    
    return 0;
}
```

## Related Documentation

- [Getting Started Guide](../getting-started.md)
- [Cross-Platform Development](cross-platform.md)
- [Linux Platform Guide](linux.md)
- [Build System](../build-system.md)
- [Performance Guide](../development/performance.md)

---

*This documentation covers Fern UI Framework v1.0 for web platforms. For updates and more information, visit the [project repository](https://github.com/fernkit/fern).*
