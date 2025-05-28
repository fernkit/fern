#include "../include/fern/fern.hpp"
#include "../include/fern/core/input.hpp"
#include "../include/fern/core/widget_manager.hpp"
#include <emscripten.h>
#include <functional>

namespace Fern {
    static std::function<void()> drawCallback = nullptr;
    
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
        
        EM_ASM({
            var container = document.getElementById('canvas-container') || document.body;
            var availWidth = container.clientWidth || window.innerWidth;
            var availHeight = container.clientHeight || window.innerHeight;
            
            setValue($0, availWidth, 'i32');
            setValue($1, availHeight, 'i32');
            
            console.log("Fern C++: Auto-detected canvas size: " + availWidth + "x" + availHeight);
        }, &width, &height);
        
        initialize(std::max(320, width), std::max(240, height));
    }
    
    void initialize(int width, int height) {
        managedBuffer.reset(new uint32_t[width * height]);
        usingManagedBuffer = true;
        lastWidth = width;
        lastHeight = height;
        
        initialize(managedBuffer.get(), width, height);
        
        EM_ASM({
            window.addEventListener('resize', function() {
                var container = document.getElementById('canvas-container') || document.body;
                var availWidth = container.clientWidth || window.innerWidth;
                var availHeight = container.clientHeight || window.innerHeight;
                
                _fernHandleResize(availWidth, availHeight);
            });
        });
    }

    // overload for backward compatibility
    void initialize(uint32_t* pixelBuffer, int width, int height) {
        globalCanvas = new Canvas(pixelBuffer, width, height);
        
        EM_ASM({
            var canvas = document.getElementById('canvas');
            
            canvas.addEventListener('mousemove', function(e) {
                var rect = canvas.getBoundingClientRect();
                var mouseX = Math.floor((e.clientX - rect.left) * (canvas.width / rect.width));
                var mouseY = Math.floor((e.clientY - rect.top) * (canvas.height / rect.height));
                
                Module._fernUpdateMousePosition(mouseX, mouseY);
            });
            
            canvas.addEventListener('mousedown', function(e) {
                Module._fernUpdateMouseButton(1);
            });
            
            canvas.addEventListener('mouseup', function(e) {
                Module._fernUpdateMouseButton(0);
            });
            
            console.log("Fern C++: Event listeners initialized");
        });
    }
    
    void startRenderLoop() {
        emscripten_set_main_loop([]() {
            if (drawCallback) {
                drawCallback();
            }

            WidgetManager::getInstance().updateAll(Input::getState());
            WidgetManager::getInstance().renderAll();
            
            EM_ASM({
                var canvas = document.getElementById('canvas');
                var ctx = canvas.getContext('2d');
                
                if (canvas.width !== $1 || canvas.height !== $0) {
                    canvas.width = $1;
                    canvas.height = $0;
                }
                
                var imageData = ctx.createImageData(canvas.width, canvas.height);
                var data = imageData.data;
                var buffer = $2;
                var size = $0 * $1;
                
                for (var i = 0; i < size; i++) {
                    var pixel = HEAP32[buffer/4 + i];
                    var b = pixel & 0xFF;         
                    var g = (pixel >> 8) & 0xFF;    
                    var r = (pixel >> 16) & 0xFF;   
                    var a = (pixel >> 24) & 0xFF;   
                    var j = i * 4;
                    data[j + 0] = r;
                    data[j + 1] = g;
                    data[j + 2] = b;
                    data[j + 3] = a;
                }
                
                ctx.putImageData(imageData, 0, 0);
            }, 
            globalCanvas->getHeight(), globalCanvas->getWidth(), 
            globalCanvas->getBuffer());
            
            Input::resetEvents();
        }, 0, 1);
    }
    
    void setDrawCallback(std::function<void()> callback) {
        drawCallback = callback;
    }
}

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void fernUpdateMousePosition(int x, int y) {
        Fern::Input::updateMousePosition(x, y);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void fernUpdateMouseButton(int down) {
        Fern::Input::updateMouseButton(down != 0);
    }

    EMSCRIPTEN_KEEPALIVE
    void fernHandleResize(int width, int height) {
        if (Fern::usingManagedBuffer && (width != Fern::lastWidth || height != Fern::lastHeight)) {
            Fern::managedBuffer.reset(new uint32_t[width * height]);
            Fern::globalCanvas = new Fern::Canvas(Fern::managedBuffer.get(), width, height);
            Fern::lastWidth = width;
            Fern::lastHeight = height;
        }
    }
}
