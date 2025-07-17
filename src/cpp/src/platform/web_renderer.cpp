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
        std::function<void(KeyCode, bool)> keyCallback_;
        std::function<void(const std::string&)> textInputCallback_;
        
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
        
        void setKeyCallback(std::function<void(KeyCode, bool)> callback) override {
            keyCallback_ = callback;
        }
        
        void setTextInputCallback(std::function<void(const std::string&)> callback) override {
            textInputCallback_ = callback;
        }
        
        void setSize(int width, int height) override {
            width_ = width;
            height_ = height;
            EM_ASM({
                var canvas = document.getElementById('canvas');
                if (canvas) {
                    canvas.width = $0;
                    canvas.height = $1;
                    
                    // Setting canvas.width automatically clears the canvas
                    // But let's also explicitly clear it to ensure clean state
                    var ctx = canvas.getContext('2d');
                    ctx.clearRect(0, 0, $0, $1);
                    
                    console.log("Canvas resized to: " + $0 + "x" + $1);
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
            
            // Update the HTML canvas element size
            setSize(width, height);
            
            if (resizeCallback_) {
                resizeCallback_(width, height);
            }
        }
        
        void onKeyEvent(int keyCode, bool isPressed) {
            KeyCode key = static_cast<KeyCode>(keyCode);
            if (keyCallback_) keyCallback_(key, isPressed);
        }
        
        void onTextInput(const char* text) {
            if (textInputCallback_) textInputCallback_(std::string(text));
        }
        
    private:
        void setupEventListeners() {
            EM_ASM({
                var canvas = document.getElementById('canvas');
                var renderer = canvas.fernRenderer;
                
                // Mouse events
                canvas.addEventListener('mousemove', function(e) {
                    var rect = canvas.getBoundingClientRect();
                    var x = Math.floor((e.clientX - rect.left) * (canvas.width / rect.width));
                    var y = Math.floor((e.clientY - rect.top) * (canvas.height / rect.height));
                    Module._webRendererMouseMove(renderer, x, y);
                });
                
                canvas.addEventListener('mousedown', function(e) {
                    canvas.focus(); // Ensure canvas has focus for keyboard events
                    Module._webRendererMouseClick(renderer, 1);
                });
                
                canvas.addEventListener('mouseup', function(e) {
                    Module._webRendererMouseClick(renderer, 0);
                });
                
                // Keyboard events - listen on both canvas and document
                canvas.setAttribute('tabindex', '0'); // Make canvas focusable
                canvas.style.outline = 'none'; // Remove focus outline
                
                // Canvas keyboard events (when canvas has focus)
                canvas.addEventListener('keydown', function(e) {
                    console.log('Canvas keydown:', e.keyCode);
                    Module._webRendererKeyEvent(renderer, e.keyCode, 1);
                    
                    // Prevent default for special keys
                    if (e.keyCode === 8 || e.keyCode === 9 || e.keyCode === 13 || 
                        e.keyCode === 27 || (e.keyCode >= 37 && e.keyCode <= 40)) {
                        e.preventDefault();
                    }
                });
                
                canvas.addEventListener('keyup', function(e) {
                    console.log('Canvas keyup:', e.keyCode);
                    Module._webRendererKeyEvent(renderer, e.keyCode, 0);
                });
                
                // Text input events for character input
                canvas.addEventListener('keypress', function(e) {
                    console.log('Canvas keypress:', e.charCode);
                    if (e.charCode >= 32 && e.charCode <= 126) { // Printable ASCII characters
                        var char = String.fromCharCode(e.charCode);
                        // Use a simpler approach - pass the character code directly
                        Module._webRendererTextInput(renderer, e.charCode);
                    }
                });
                
                // Also listen on document as fallback (but avoid duplicate events)
                document.addEventListener('keydown', function(e) {
                    if (document.activeElement === canvas) {
                        // Don't process if canvas already handled it
                        return;
                    }
                });
                
                document.addEventListener('keyup', function(e) {
                    if (document.activeElement === canvas) {
                        // Don't process if canvas already handled it  
                        return;
                    }
                });
                
                // Text input events for character input
                document.addEventListener('keypress', function(e) {
                    if (document.activeElement === canvas) {
                        // Don't process if canvas already handled it
                        return;
                    }
                });
                
                // Window resize
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