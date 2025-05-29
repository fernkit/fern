#ifdef __EMSCRIPTEN__

#include "fern/platform/renderer.hpp"
#include <emscripten.h>
#include <emscripten/html5.h>

namespace Fern {
    
    class WebRenderer : public PlatformRenderer {
    private:
        int width_, height_;
        std::function<void(int, int)> mouseCallback_;
        std::function<void(bool)> clickCallback_;
        std::function<void(int, int)> resizeCallback_;
        
    public:
        void initialize(int width, int height) override {
            width_ = width;
            height_ = height;
            
            // Setup canvas and event listeners
            EM_ASM({
                var canvas = document.getElementById('canvas');
                if (!canvas) {
                    canvas = document.createElement('canvas');
                    canvas.id = 'canvas';
                    document.body.appendChild(canvas);
                }
                canvas.width = $0;
                canvas.height = $1;
                
                // Store renderer pointer for callbacks
                canvas.fernRenderer = $2;
            }, width, height, this);
            
            setupEventListeners();
        }
        
        void present(uint32_t* pixelBuffer, int width, int height) override {
            EM_ASM({
                var canvas = document.getElementById('canvas');
                var ctx = canvas.getContext('2d');
                
                var imageData = ctx.createImageData($0, $1);
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
            }, width, height, pixelBuffer);
        }
        
        void setTitle(const std::string& title) override {
            EM_ASM({
                document.title = UTF8ToString($0);
            }, title.c_str());
        }
        
        bool shouldClose() override { return false; }
        std::string getPlatformName() override { return "Web (Emscripten)"; }
        void pollEvents() override {}
        
        void setMouseCallback(std::function<void(int, int)> callback) override {
            mouseCallback_ = callback;
        }
        
        void setClickCallback(std::function<void(bool)> callback) override {
            clickCallback_ = callback;
        }
        
        void setResizeCallback(std::function<void(int, int)> callback) override {
            resizeCallback_ = callback;
        }
        
        void setSize(int width, int height) override {
            width_ = width;
            height_ = height;
            EM_ASM({
                var canvas = document.getElementById('canvas');
                if (canvas) {
                    canvas.width = $0;
                    canvas.height = $1;
                }
            }, width, height);
        }
        
        void shutdown() override {}
        
        // Public event handlers
        void onMouseMove(int x, int y) {
            if (mouseCallback_) mouseCallback_(x, y);
        }
        
        void onMouseClick(bool down) {
            if (clickCallback_) clickCallback_(down);
        }
        
        void onResize(int width, int height) {
            width_ = width;
            height_ = height;
            if (resizeCallback_) {
                resizeCallback_(width, height);
            }
        }
        
    private:
        void setupEventListeners() {
            EM_ASM({
                var canvas = document.getElementById('canvas');
                var renderer = canvas.fernRenderer;
                
                canvas.addEventListener('mousemove', function(e) {
                    var rect = canvas.getBoundingClientRect();
                    var x = Math.floor((e.clientX - rect.left) * (canvas.width / rect.width));
                    var y = Math.floor((e.clientY - rect.top) * (canvas.height / rect.height));
                    Module._webRendererMouseMove(renderer, x, y);
                });
                
                canvas.addEventListener('mousedown', function(e) {
                    Module._webRendererMouseClick(renderer, 1);
                });
                
                canvas.addEventListener('mouseup', function(e) {
                    Module._webRendererMouseClick(renderer, 0);
                });
                
                window.addEventListener('resize', function() {
                    var container = document.getElementById('canvas-container') || document.body;
                    var availWidth = container.clientWidth || window.innerWidth;
                    var availHeight = container.clientHeight || window.innerHeight;
                    
                    Module._webRendererResize(renderer, availWidth, availHeight);
                });
            }, this);
        }
    };
}

#endif // __EMSCRIPTEN__