#include "../include/fern/fern.hpp"
#include "../include/fern/platform/renderer.hpp"
#include "../include/fern/core/input.hpp"
#include "../include/fern/core/widget_manager.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <iostream>

namespace Fern {
    std::unique_ptr<PlatformRenderer> createRenderer();
    
    static std::function<void()> drawCallback = nullptr;
    static std::unique_ptr<PlatformRenderer> renderer = nullptr;
    static std::unique_ptr<uint32_t[]> managedBuffer = nullptr;
    static bool usingManagedBuffer = false;
    static int lastWidth = 800;
    static int lastHeight = 600;

    static std::function<void(int, int)> windowResizeCallback = nullptr;

    int getWidth() {
        return lastWidth;
    }
    
    int getHeight() {
        return lastHeight;
    }
    
    Point getCanvasSize() {
        return Point(lastWidth, lastHeight);
    }
    
    void initialize() {
        int width, height;
        
#ifdef __EMSCRIPTEN__
        EM_ASM({
            var container = document.getElementById('canvas-container') || document.body;
            var availWidth = container.clientWidth || window.innerWidth;
            var availHeight = container.clientHeight || window.innerHeight;
            
            setValue($0, availWidth, 'i32');
            setValue($1, availHeight, 'i32');
            
        }, &width, &height);
#else
        width = 800;
        height = 600;
#endif
        
        initialize(std::max(320, width), std::max(240, height));
    }
    
    void initialize(int width, int height) {
        lastWidth = width;
        lastHeight = height;
        
        renderer = createRenderer();
        
        managedBuffer.reset(new uint32_t[width * height]);
        usingManagedBuffer = true;
        
        globalCanvas = new Canvas(managedBuffer.get(), width, height);
        
        renderer->initialize(width, height);
        
        renderer->setMouseCallback([](int x, int y) {
            Input::updateMousePosition(x, y);
        });
        
        renderer->setClickCallback([](bool down) {
            Input::updateMouseButton(down);
        });
        
        renderer->setKeyCallback([](KeyCode key, bool isPressed) {
            if (isPressed) {
                Input::updateKeyPress(key);
            } else {
                Input::updateKeyRelease(key);
            }
        });
        
        renderer->setTextInputCallback([](const std::string& text) {
            Input::updateTextInput(text);
        });
        
        renderer->setResizeCallback([](int width, int height) {
            if (usingManagedBuffer && (width != lastWidth || height != lastHeight)) {
                // Create new buffer for new dimensions
                managedBuffer.reset(new uint32_t[width * height]);
                
                // Initialize the new buffer to opaque black (ARGB format: 0xAARRGGBB)
                std::fill_n(managedBuffer.get(), width * height, 0xFF000000);
                
                // Delete old canvas and create new one
                delete globalCanvas;
                globalCanvas = new Canvas(managedBuffer.get(), width, height);
                
                lastWidth = width;
                lastHeight = height;
                
                
                // Force a complete redraw by clearing everything and re-rendering
                // The draw callback will be called in the next frame
                
                // Notify widget manager about resize
                WidgetManager::getInstance().onWindowResize(width, height);
            }

            if (windowResizeCallback) {
                windowResizeCallback(width, height);
            }
        });
    }

    void initialize(uint32_t* pixelBuffer, int width, int height) {
        lastWidth = width;
        lastHeight = height;
        usingManagedBuffer = false;
        
        globalCanvas = new Canvas(pixelBuffer, width, height);
        
        renderer = createRenderer();
        renderer->initialize(width, height);
        
        renderer->setMouseCallback([](int x, int y) {
            Input::updateMousePosition(x, y);
        });
        
        renderer->setClickCallback([](bool down) {
            Input::updateMouseButton(down);
        });
    }
    
    void startRenderLoop() {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop([]() {
            renderer->pollEvents();
            
            if (drawCallback) {
                drawCallback();
            }

            WidgetManager::getInstance().updateAll(Input::getState());
            WidgetManager::getInstance().renderAll();
            
            renderer->present(globalCanvas->getBuffer(), lastWidth, lastHeight);
            
            Input::resetEvents();
        }, 0, 1);
#else
        while (!renderer->shouldClose()) {
            renderer->pollEvents();
            
            if (drawCallback) {
                drawCallback();
            }

            WidgetManager::getInstance().updateAll(Input::getState());
            WidgetManager::getInstance().renderAll();
            
            renderer->present(globalCanvas->getBuffer(), lastWidth, lastHeight);
            Input::resetEvents();
        }
        
        renderer->shutdown();
#endif
    }
    
    void setDrawCallback(std::function<void()> callback) {
        drawCallback = callback;
    }

    void setWindowResizeCallback(std::function<void(int, int)> callback) {
        windowResizeCallback = callback;
    }
}
