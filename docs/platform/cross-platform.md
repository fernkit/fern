# Cross-Platform Development Guide

## Overview

The Fern UI Framework is designed to support multiple platforms with a unified codebase. This guide covers best practices for developing cross-platform applications, handling platform differences, and optimizing for each target platform.

## Table of Contents

1. [Platform Support](#platform-support)
2. [Architecture Overview](#architecture-overview)
3. [Code Organization](#code-organization)
4. [Platform Abstractions](#platform-abstractions)
5. [Build Configuration](#build-configuration)
6. [Platform-Specific Features](#platform-specific-features)
7. [Testing Strategies](#testing-strategies)
8. [Deployment Considerations](#deployment-considerations)
9. [Best Practices](#best-practices)
10. [Future Platforms](#future-platforms)

## Platform Support

### Current Platforms

| Platform | Status | Renderer | Build System | Notes |
|----------|--------|----------|--------------|-------|
| Linux    | ✅ Stable | Native/OpenGL | CMake/GCC | Primary development platform |
| Web      | ✅ Stable | WebAssembly/Canvas | Emscripten | Full feature support |
| macOS    | 🚧 In Progress | Native/Metal | CMake/Clang | Limited testing |
| Windows  | 🚧 In Progress | DirectX/OpenGL | CMake/MSVC | Community support |
| iOS      | 📋 Planned | Metal | Xcode | Future release |
| Android  | 📋 Planned | Vulkan/OpenGL ES | Gradle/NDK | Future release |

### Feature Matrix

| Feature | Linux | Web | macOS | Windows | iOS | Android |
|---------|-------|-----|-------|---------|-----|---------|
| Basic Widgets | ✅ | ✅ | ✅ | ✅ | 📋 | 📋 |
| TTF Fonts | ✅ | ✅ | ✅ | ✅ | 📋 | 📋 |
| Layouts | ✅ | ✅ | ✅ | ✅ | 📋 | 📋 |
| Events | ✅ | ✅ | ✅ | ✅ | 📋 | 📋 |
| File I/O | ✅ | ⚠️ Limited | ✅ | ✅ | 📋 | 📋 |
| Audio | ✅ | ✅ | ✅ | ✅ | 📋 | 📋 |
| Networking | ✅ | ⚠️ Limited | ✅ | ✅ | 📋 | 📋 |

Legend: ✅ Supported, ⚠️ Limited, 🚧 In Progress, 📋 Planned

## Architecture Overview

### Platform Abstraction Layer

```cpp
// Platform-independent application code
class MyApp {
public:
    void initialize() {
        // Use Fern's cross-platform API
        Fern::initialize(800, 600);
        setupUI();
    }
    
    void setupUI() {
        // Widgets work the same on all platforms
        auto button = Button(ButtonConfig(100, 100, 200, 50, "Click Me"));
        button->onClick.connect([]() {
            std::cout << "Button clicked!" << std::endl;
        });
        addWidget(button);
    }
};

// Platform-specific entry points
#ifdef __EMSCRIPTEN__
    // Web platform
    int main() {
        MyApp app;
        app.initialize();
        Fern::startRenderLoop();  // Web-specific render loop
        return 0;
    }
#else
    // Native platforms
    int main() {
        MyApp app;
        app.initialize();
        
        while (!Fern::shouldClose()) {
            Fern::pollEvents();
            Fern::render();
        }
        
        Fern::shutdown();
        return 0;
    }
#endif
```

### Renderer Abstraction

```cpp
// Platform-independent renderer interface
class PlatformRenderer {
public:
    virtual void initialize(int width, int height) = 0;
    virtual void present(uint32_t* pixelBuffer, int width, int height) = 0;
    virtual void setTitle(const std::string& title) = 0;
    virtual bool shouldClose() = 0;
    virtual void pollEvents() = 0;
    virtual void shutdown() = 0;
    
    // Event handling
    virtual void setMouseCallback(std::function<void(int, int)> callback) = 0;
    virtual void setClickCallback(std::function<void(bool)> callback) = 0;
    virtual void setResizeCallback(std::function<void(int, int)> callback) = 0;
    virtual void setKeyCallback(std::function<void(KeyCode, bool)> callback) = 0;
};

// Platform-specific implementations
class LinuxRenderer : public PlatformRenderer { /* ... */ };
class WebRenderer : public PlatformRenderer { /* ... */ };
class WindowsRenderer : public PlatformRenderer { /* ... */ };
```

## Code Organization

### Directory Structure

```
src/
├── common/                    # Cross-platform code
│   ├── widgets/              # Widget implementations
│   ├── layout/               # Layout system
│   ├── graphics/             # Graphics utilities
│   └── core/                 # Core framework
├── platform/                 # Platform-specific code
│   ├── linux/               # Linux implementations
│   ├── web/                 # Web/Emscripten implementations
│   ├── windows/             # Windows implementations
│   └── macos/               # macOS implementations
└── include/                  # Public headers
    └── fern/                # Framework headers
```

### Conditional Compilation

```cpp
// Platform detection
#if defined(__EMSCRIPTEN__)
    #define FERN_PLATFORM_WEB
#elif defined(__linux__)
    #define FERN_PLATFORM_LINUX
#elif defined(__APPLE__)
    #define FERN_PLATFORM_MACOS
#elif defined(_WIN32)
    #define FERN_PLATFORM_WINDOWS
#endif

// Platform-specific includes
#ifdef FERN_PLATFORM_WEB
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#ifdef FERN_PLATFORM_LINUX
    #include <X11/Xlib.h>
    #include <GL/gl.h>
#endif

#ifdef FERN_PLATFORM_WINDOWS
    #include <windows.h>
    #include <d3d11.h>
#endif
```

### Configuration Management

```cpp
// config.hpp - Platform-specific configuration
namespace Fern {
    struct PlatformConfig {
        #ifdef FERN_PLATFORM_WEB
            static constexpr bool hasFileSystem = false;
            static constexpr bool hasNetworking = true;
            static constexpr int maxTextureSize = 4096;
        #else
            static constexpr bool hasFileSystem = true;
            static constexpr bool hasNetworking = true;
            static constexpr int maxTextureSize = 8192;
        #endif
    };
}
```

## Platform Abstractions

### File System Abstraction

```cpp
// Cross-platform file operations
namespace Fern::FileSystem {
    std::vector<uint8_t> readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
    bool fileExists(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& path);
    
    // Platform-specific implementations
    #ifdef FERN_PLATFORM_WEB
        // Use Emscripten's virtual file system
        std::vector<uint8_t> readFile(const std::string& path) {
            // Implementation using Emscripten FS API
        }
    #else
        // Use standard file I/O
        std::vector<uint8_t> readFile(const std::string& path) {
            std::ifstream file(path, std::ios::binary);
            // Standard implementation
        }
    #endif
}
```

### Input Handling Abstraction

```cpp
// Unified input handling across platforms
namespace Fern::Input {
    enum class KeyCode {
        A = 65, B = 66, /* ... */
        Enter = 13, Escape = 27,
        Space = 32, Tab = 9,
        /* Platform-specific mappings handled internally */
    };
    
    struct InputState {
        int mouseX, mouseY;
        bool mouseButtons[3];
        bool keys[256];
        std::string textInput;
        
        // Platform-specific touch input (mobile)
        #ifdef FERN_PLATFORM_MOBILE
            struct TouchPoint {
                int id, x, y;
                bool active;
            };
            std::vector<TouchPoint> touches;
        #endif
    };
}
```

### Graphics Abstraction

```cpp
// Platform-independent graphics operations
namespace Fern::Graphics {
    void drawRect(int x, int y, int w, int h, uint32_t color);
    void drawCircle(int x, int y, int radius, uint32_t color);
    void drawText(const std::string& text, int x, int y, uint32_t color);
    
    // Platform-specific optimizations
    #ifdef FERN_PLATFORM_GPU_ACCELERATED
        void enableGPUAcceleration();
        void uploadTexture(const ImageData& data);
    #endif
    
    #ifdef FERN_PLATFORM_WEB
        void setCanvasSize(int width, int height);
        void enableWebGLFeatures();
    #endif
}
```

## Build Configuration

### CMake Platform Detection

```cmake
# CMakeLists.txt - Platform detection and configuration
if(EMSCRIPTEN)
    set(FERN_PLATFORM "Web")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif(UNIX AND NOT APPLE)
    set(FERN_PLATFORM "Linux")
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
elseif(APPLE)
    set(FERN_PLATFORM "macOS")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
elseif(WIN32)
    set(FERN_PLATFORM "Windows")
    find_package(DirectX11 REQUIRED)
endif()

# Platform-specific source files
if(FERN_PLATFORM STREQUAL "Web")
    set(PLATFORM_SOURCES src/platform/web_renderer.cpp)
elseif(FERN_PLATFORM STREQUAL "Linux")
    set(PLATFORM_SOURCES src/platform/linux_renderer.cpp)
elseif(FERN_PLATFORM STREQUAL "Windows")
    set(PLATFORM_SOURCES src/platform/windows_renderer.cpp)
endif()

# Common sources for all platforms
set(COMMON_SOURCES
    src/core/canvas.cpp
    src/widgets/button_widget.cpp
    src/widgets/text_widget.cpp
    # ... other common sources
)

# Create library with appropriate sources
add_library(fern ${COMMON_SOURCES} ${PLATFORM_SOURCES})

# Platform-specific compile definitions
target_compile_definitions(fern PRIVATE FERN_PLATFORM_${FERN_PLATFORM})

# Platform-specific linking
if(FERN_PLATFORM STREQUAL "Linux")
    target_link_libraries(fern ${X11_LIBRARIES} ${OPENGL_LIBRARIES})
elseif(FERN_PLATFORM STREQUAL "macOS")
    target_link_libraries(fern ${COCOA_LIBRARY} ${METAL_LIBRARY})
elseif(FERN_PLATFORM STREQUAL "Windows")
    target_link_libraries(fern d3d11 dxgi)
endif()
```

### Build Scripts

```bash
#!/bin/bash
# build.sh - Cross-platform build script

PLATFORM=$1

case $PLATFORM in
    "linux")
        echo "Building for Linux..."
        mkdir -p build
        cd build
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make -j$(nproc)
        ;;
    "web")
        echo "Building for Web..."
        source /path/to/emsdk/emsdk_env.sh
        mkdir -p build-web
        cd build-web
        emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
        emmake make -j$(nproc)
        ;;
    "windows")
        echo "Building for Windows..."
        mkdir -p build-windows
        cd build-windows
        cmake .. -G "Visual Studio 16 2019" -A x64
        cmake --build . --config Release
        ;;
    "macos")
        echo "Building for macOS..."
        mkdir -p build-macos
        cd build-macos
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make -j$(sysctl -n hw.ncpu)
        ;;
    *)
        echo "Usage: $0 {linux|web|windows|macos}"
        exit 1
        ;;
esac
```

## Platform-Specific Features

### Web Platform Features

```cpp
#ifdef FERN_PLATFORM_WEB
namespace Fern::Web {
    // JavaScript interop
    void callJavaScript(const std::string& code);
    void setCanvasStyle(const std::string& css);
    
    // Web-specific events
    void onBeforeUnload(std::function<void()> callback);
    void onVisibilityChange(std::function<void(bool visible)> callback);
    
    // Local storage
    void setLocalStorage(const std::string& key, const std::string& value);
    std::string getLocalStorage(const std::string& key);
    
    // URL parameters
    std::map<std::string, std::string> getURLParameters();
}

// Usage example
void setupWebFeatures() {
    Fern::Web::setCanvasStyle("border: 2px solid #333; border-radius: 8px;");
    
    Fern::Web::onBeforeUnload([]() {
        // Save application state before page unload
        Fern::Web::setLocalStorage("appState", serializeAppState());
    });
}
#endif
```

### Mobile Platform Features

```cpp
#ifdef FERN_PLATFORM_MOBILE
namespace Fern::Mobile {
    // Touch input
    Signal<int, int, int> onTouchStart;  // id, x, y
    Signal<int, int, int> onTouchMove;   // id, x, y
    Signal<int> onTouchEnd;              // id
    
    // Device sensors
    struct AccelerometerData {
        float x, y, z;
    };
    Signal<AccelerometerData> onAccelerometerUpdate;
    
    // Device capabilities
    bool hasCamera();
    bool hasGPS();
    bool hasVibration();
    
    // Platform integration
    void showKeyboard();
    void hideKeyboard();
    void vibrate(int milliseconds);
}
#endif
```

### Desktop Platform Features

```cpp
#ifdef FERN_PLATFORM_DESKTOP
namespace Fern::Desktop {
    // File dialogs
    std::string openFileDialog(const std::vector<std::string>& filters);
    std::string saveFileDialog(const std::string& defaultName);
    std::string selectFolderDialog();
    
    // System integration
    void showNotification(const std::string& title, const std::string& message);
    void setSystemTrayIcon(const std::string& iconPath);
    
    // Window management
    void setWindowIcon(const std::string& iconPath);
    void setWindowAlwaysOnTop(bool onTop);
    void minimizeWindow();
    void maximizeWindow();
}
#endif
```

## Testing Strategies

### Cross-Platform Testing

```cpp
// test_cross_platform.cpp
#include <fern/fern.hpp>
#include <gtest/gtest.h>

class CrossPlatformTest : public ::testing::Test {
protected:
    void SetUp() override {
        Fern::initialize(800, 600);
    }
    
    void TearDown() override {
        Fern::shutdown();
    }
};

TEST_F(CrossPlatformTest, WidgetCreation) {
    // Test that widgets work the same on all platforms
    auto button = Button(ButtonConfig(0, 0, 100, 50, "Test"));
    EXPECT_EQ(button->getWidth(), 100);
    EXPECT_EQ(button->getHeight(), 50);
}

TEST_F(CrossPlatformTest, EventHandling) {
    // Test event handling across platforms
    bool clicked = false;
    auto button = Button(ButtonConfig(0, 0, 100, 50, "Test"));
    button->onClick.connect([&clicked]() { clicked = true; });
    
    // Simulate click (platform-independent)
    InputState input;
    input.mouseX = 50;
    input.mouseY = 25;
    input.mouseLeftPressed = true;
    
    EXPECT_TRUE(button->handleInput(input));
    EXPECT_TRUE(clicked);
}

// Platform-specific tests
#ifdef FERN_PLATFORM_WEB
TEST_F(CrossPlatformTest, WebSpecificFeatures) {
    // Test web-specific functionality
    Fern::Web::setLocalStorage("test", "value");
    EXPECT_EQ(Fern::Web::getLocalStorage("test"), "value");
}
#endif

#ifdef FERN_PLATFORM_DESKTOP
TEST_F(CrossPlatformTest, DesktopSpecificFeatures) {
    // Test desktop-specific functionality
    EXPECT_NO_THROW(Fern::Desktop::setWindowIcon("icon.png"));
}
#endif
```

### Automated Testing

```yaml
# .github/workflows/cross-platform.yml
name: Cross-Platform Tests

on: [push, pull_request]

jobs:
  test-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: sudo apt-get install build-essential cmake libx11-dev
      - name: Build
        run: ./build.sh linux
      - name: Test
        run: cd build && ctest

  test-web:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Setup Emscripten
        uses: mymindstorm/setup-emsdk@v11
      - name: Build
        run: ./build.sh web
      - name: Test
        run: cd build-web && node test.js

  test-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v2
      - name: Setup MSVC
        uses: microsoft/setup-msbuild@v1
      - name: Build
        run: .\build.bat windows
      - name: Test
        run: cd build-windows && ctest

  test-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: brew install cmake
      - name: Build
        run: ./build.sh macos
      - name: Test
        run: cd build-macos && ctest
```

## Deployment Considerations

### Web Deployment

```html
<!-- index.html template for web deployment -->
<!DOCTYPE html>
<html>
<head>
    <title>Fern Application</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            margin: 0;
            padding: 0;
            background: #1a1a1a;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            font-family: Arial, sans-serif;
        }
        
        #canvas {
            border: 1px solid #333;
            background: #000;
        }
        
        .loading {
            color: white;
            text-align: center;
        }
    </style>
</head>
<body>
    <div id="loading" class="loading">Loading Fern Application...</div>
    <canvas id="canvas" style="display: none;"></canvas>
    
    <script>
        var Module = {
            canvas: document.getElementById('canvas'),
            onRuntimeInitialized: function() {
                document.getElementById('loading').style.display = 'none';
                document.getElementById('canvas').style.display = 'block';
            }
        };
    </script>
    <script src="myapp.js"></script>
</body>
</html>
```

### Desktop Distribution

```bash
# Linux AppImage creation
#!/bin/bash
# create_appimage.sh

# Create AppDir structure
mkdir -p MyApp.AppDir/usr/bin
mkdir -p MyApp.AppDir/usr/lib
mkdir -p MyApp.AppDir/usr/share/applications
mkdir -p MyApp.AppDir/usr/share/icons

# Copy application
cp build/myapp MyApp.AppDir/usr/bin/
cp -r assets/ MyApp.AppDir/usr/share/myapp/

# Create desktop file
cat > MyApp.AppDir/usr/share/applications/myapp.desktop << EOF
[Desktop Entry]
Type=Application
Name=My Fern App
Exec=myapp
Icon=myapp
Categories=Utility;
EOF

# Create AppImage
appimagetool MyApp.AppDir
```

### Mobile Packaging

```xml
<!-- Android manifest template -->
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.fernapp">
    
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.INTERNET" />
    
    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme">
        
        <activity
            android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```

## Best Practices

### Cross-Platform Code Guidelines

1. **Use Platform Abstractions**: Always use Fern's cross-platform APIs
2. **Isolate Platform Code**: Keep platform-specific code in separate files
3. **Test on All Platforms**: Regular testing prevents platform-specific bugs
4. **Document Platform Differences**: Clearly document any platform limitations
5. **Use Feature Detection**: Check capabilities at runtime when possible

```cpp
// Good: Use abstraction
Fern::FileSystem::readFile("config.json");

// Bad: Platform-specific code in common areas
#ifdef _WIN32
    std::ifstream file("config.json");
#else
    FILE* file = fopen("config.json", "r");
#endif
```

### Performance Considerations

```cpp
// Platform-specific optimizations
void optimizeForPlatform() {
    #ifdef FERN_PLATFORM_WEB
        // Optimize for web: smaller textures, fewer draw calls
        Canvas::setMaxTextureSize(2048);
        Canvas::enableBatching(true);
    #endif
    
    #ifdef FERN_PLATFORM_MOBILE
        // Optimize for mobile: lower quality, battery efficiency
        Canvas::setRenderQuality(Canvas::Quality::Low);
        enablePowerSaving(true);
    #endif
    
    #ifdef FERN_PLATFORM_DESKTOP
        // Optimize for desktop: high quality, full features
        Canvas::setRenderQuality(Canvas::Quality::High);
        enableAdvancedFeatures(true);
    #endif
}
```

### Error Handling

```cpp
// Cross-platform error handling
namespace Fern::Error {
    enum class PlatformError {
        WindowCreationFailed,
        GraphicsInitFailed,
        AudioInitFailed,
        FileSystemUnavailable,
        NetworkUnavailable
    };
    
    void handlePlatformError(PlatformError error) {
        switch (error) {
            case PlatformError::WindowCreationFailed:
                #ifdef FERN_PLATFORM_WEB
                    // Show HTML error message
                    showWebError("Failed to initialize canvas");
                #else
                    // Show native error dialog
                    showNativeError("Window creation failed");
                #endif
                break;
                
            // Handle other errors...
        }
    }
}
```

## Future Platforms

### Planned Platform Support

#### iOS Platform
```cpp
// Future iOS implementation
#ifdef FERN_PLATFORM_IOS
namespace Fern::iOS {
    // iOS-specific features
    void setupMetalRenderer();
    void handleAppStateChanges();
    void integrateWithUIKit();
}
#endif
```

#### Android Platform
```cpp
// Future Android implementation
#ifdef FERN_PLATFORM_ANDROID
namespace Fern::Android {
    // Android-specific features
    void setupVulkanRenderer();
    void handleActivityLifecycle();
    void integrateWithAndroidUI();
}
#endif
```

#### Console Platforms
```cpp
// Future console support
#ifdef FERN_PLATFORM_CONSOLE
namespace Fern::Console {
    // Console-specific optimizations
    void setupConsoleRenderer();
    void handleControllerInput();
    void optimizeForConsole();
}
#endif
```

---

This cross-platform guide provides the foundation for developing applications that work consistently across all supported Fern platforms while taking advantage of platform-specific features when needed.

## Related Documentation

- [Platform-Specific Guides](../platform/)
- [Build System](../build-system.md)
- [Architecture Guide](../development/architecture.md)
- [Performance Guide](../development/performance.md)
- [Contributing Guide](../development/contributing.md)
