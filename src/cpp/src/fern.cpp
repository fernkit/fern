#include "../include/fern/fern.hpp"
#include "../include/fern/platform/renderer.hpp"
#include "../include/fern/core/input.hpp"
#include "../include/fern/core/widget_manager.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Fern {
    std::unique_ptr<PlatformRenderer> createRenderer();
    
    static std::function<void()> drawCallback = nullptr;
    static std::unique_ptr<PlatformRenderer> renderer = nullptr;
    static std::unique_ptr<uint32_t[]> managedBuffer = nullptr;
    static bool usingManagedBuffer = false;
    static int lastWidth = 800;
    static int lastHeight = 600;

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
            
            console.log("Fern C++: Auto-detected canvas size: " + availWidth + "x" + availHeight);
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
        
        renderer->setResizeCallback([](int width, int height) {
            if (usingManagedBuffer && (width != lastWidth || height != lastHeight)) {
                managedBuffer.reset(new uint32_t[width * height]);
                globalCanvas = new Canvas(managedBuffer.get(), width, height);
                lastWidth = width;
                lastHeight = height;
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
}
