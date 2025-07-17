#ifdef __linux__
#include "fern/platform/renderer.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <iostream>
#include <cstring>

namespace Fern {
    
    class LinuxRenderer : public PlatformRenderer {
    private:
        // Core X11 components
        Display* display_;          // Connection to X server
        Window window_;             // Our application window
        GC gc_;                     // Graphics context for drawing
        XImage* ximage_;            // Image structure for pixel data
        uint32_t* pixelBuffer_;     // Our local pixel buffer
        int width_, height_;        // Window dimensions
        bool shouldClose_;          // Application running state
        Atom wmDeleteMessage_;      // Window manager delete message
        
        // Input callbacks
        std::function<void(int, int)> mouseCallback_;
        std::function<void(bool)> clickCallback_;
        std::function<void(int, int)> resizeCallback_;
        std::function<void(KeyCode, bool)> keyCallback_;
        std::function<void(const std::string&)> textInputCallback_;
        
        // Key translation helper
        KeyCode translateXKeyToFernKey(KeySym keysym) {
            switch (keysym) {
                case XK_Return: return KeyCode::Enter;
                case XK_Escape: return KeyCode::Escape;
                case XK_space: return KeyCode::Space;
                case XK_BackSpace: return KeyCode::Backspace;
                case XK_Tab: return KeyCode::Tab;
                case XK_Left: return KeyCode::ArrowLeft;
                case XK_Right: return KeyCode::ArrowRight;
                case XK_Up: return KeyCode::ArrowUp;
                case XK_Down: return KeyCode::ArrowDown;
                case XK_a: case XK_A: return KeyCode::A;
                case XK_b: case XK_B: return KeyCode::B;
                case XK_c: case XK_C: return KeyCode::C;
                case XK_d: case XK_D: return KeyCode::D;
                case XK_e: case XK_E: return KeyCode::E;
                case XK_f: case XK_F: return KeyCode::F;
                case XK_g: case XK_G: return KeyCode::G;
                case XK_h: case XK_H: return KeyCode::H;
                case XK_i: case XK_I: return KeyCode::I;
                case XK_j: case XK_J: return KeyCode::J;
                case XK_k: case XK_K: return KeyCode::K;
                case XK_l: case XK_L: return KeyCode::L;
                case XK_m: case XK_M: return KeyCode::M;
                case XK_n: case XK_N: return KeyCode::N;
                case XK_o: case XK_O: return KeyCode::O;
                case XK_p: case XK_P: return KeyCode::P;
                case XK_q: case XK_Q: return KeyCode::Q;
                case XK_r: case XK_R: return KeyCode::R;
                case XK_s: case XK_S: return KeyCode::S;
                case XK_t: case XK_T: return KeyCode::T;
                case XK_u: case XK_U: return KeyCode::U;
                case XK_v: case XK_V: return KeyCode::V;
                case XK_w: case XK_W: return KeyCode::W;
                case XK_x: case XK_X: return KeyCode::X;
                case XK_y: case XK_Y: return KeyCode::Y;
                case XK_z: case XK_Z: return KeyCode::Z;
                case XK_0: return KeyCode::Num0;
                case XK_1: return KeyCode::Num1;
                case XK_2: return KeyCode::Num2;
                case XK_3: return KeyCode::Num3;
                case XK_4: return KeyCode::Num4;
                case XK_5: return KeyCode::Num5;
                case XK_6: return KeyCode::Num6;
                case XK_7: return KeyCode::Num7;
                case XK_8: return KeyCode::Num8;
                case XK_9: return KeyCode::Num9;
                // default: return KeyCode::Unknown;
            }
        }
        
    public:
        LinuxRenderer() : display_(nullptr), window_(0), gc_(nullptr), 
                         ximage_(nullptr), pixelBuffer_(nullptr), shouldClose_(false) {
        }
        
        ~LinuxRenderer() {
            cleanup();
        }
        
        void initialize(int width, int height) override {
            width_ = width;
            height_ = height;
            
            
            // Step 1: Connect to X Server
            display_ = XOpenDisplay(nullptr);  // nullptr means use DISPLAY environment variable
            if (!display_) {
                throw std::runtime_error("Cannot connect to X server. Is DISPLAY set?");
            }
            
            // Step 2: Get screen information
            int screen = DefaultScreen(display_);              // Usually 0 for single monitor
            Window rootWindow = RootWindow(display_, screen);   // Desktop background window
            
            
            // Step 3: Create our application window
            window_ = XCreateSimpleWindow(
                display_,                                    // X server connection
                rootWindow,                                  // Parent window (desktop)
                100, 100,                                    // Initial position (x, y)
                width, height,                               // Window size
                2,                                          // Border width
                BlackPixel(display_, screen),               // Border color
                WhitePixel(display_, screen)                // Background color
            );
            
            if (!window_) {
                throw std::runtime_error("Failed to create X11 window");
            }
            
            
            // Step 4: Set window properties
            XStoreName(display_, window_, "Fern Application - Linux");
            
            // Step 5: Enable input events
            XSelectInput(display_, window_, 
                        ExposureMask | KeyPressMask | KeyReleaseMask |
                        ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                        StructureNotifyMask);
            
            // Step 6: Set up window close event
            wmDeleteMessage_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(display_, window_, &wmDeleteMessage_, 1);
            
            // Step 7: Create Graphics Context (like a "drawing pen")
            gc_ = XCreateGC(display_, window_, 0, nullptr);
            if (!gc_) {
                throw std::runtime_error("Failed to create graphics context");
            }
            
            // Step 8: Prepare pixel buffer and XImage
            setupPixelBuffer();
            
            // Step 9: Make window visible
            XMapWindow(display_, window_);     // Show the window
            XFlush(display_);                  // Force X server to process requests
            
        }
        
        void present(uint32_t* pixelBuffer, int width, int height) override {
            if (!ximage_ || !pixelBuffer_ || !display_) {
                std::cerr << "Error: Invalid renderer state in present() - ximage_=" << ximage_ 
                         << ", pixelBuffer_=" << pixelBuffer_ << ", display_=" << display_ << std::endl;
                return;
            }
            
            // Check input parameters
            if (!pixelBuffer) {
                std::cerr << "Error: Input pixelBuffer is null in present()" << std::endl;
                return;
            }
            
            if (width <= 0 || height <= 0) {
                std::cerr << "Error: Invalid dimensions in present(): " << width << "x" << height << std::endl;
                return;
            }
            
            // Check if dimensions match our current buffer
            if (width != width_ || height != height_) {
                std::cerr << "Warning: Present called with dimensions " << width << "x" << height 
                         << " but renderer is " << width_ << "x" << height_ << std::endl;
                return; // Skip this frame to prevent crashes
            }
            
            // Step 1: Copy our pixel data to X11's format
            // Note: X11 might have different byte ordering, but we'll keep it simple for now
            size_t bufferSize = width * height * sizeof(uint32_t);
            memcpy(pixelBuffer_, pixelBuffer, bufferSize);
            
            // Step 2: Tell X11 to draw our image to the window
            XPutImage(
                display_,        // X server connection
                window_,         // Target window
                gc_,             // Graphics context
                ximage_,         // Our image data
                0, 0,            // Source position in image (x, y)
                0, 0,            // Destination position in window (x, y)  
                width, height    // Size to copy
            );
            
            // Step 3: Force X11 to actually draw it (X11 batches operations)
            XFlush(display_);
        }
        
        // Minimal implementations for interface compliance
        void setTitle(const std::string& title) override {
            if (display_ && window_) {
                XStoreName(display_, window_, title.c_str());
                XFlush(display_);
            }
        }
        
        bool shouldClose() override {
            return shouldClose_;
        }
        
        void pollEvents() override {
            XEvent event;
            while (XPending(display_) > 0) {
                XNextEvent(display_, &event);
                
                switch (event.type) {
                    case ButtonPress:
                        if (clickCallback_) {
                            clickCallback_(true);
                        }
                        break;
                        
                    case ButtonRelease:
                        if (clickCallback_) {
                            clickCallback_(false);
                        }
                        break;
                        
                    case MotionNotify:
                        if (mouseCallback_) {
                            mouseCallback_(event.xmotion.x, event.xmotion.y);
                        }
                        break;
                        
                    case KeyPress:
                        if (keyCallback_) {
                            KeySym keysym = XLookupKeysym(&event.xkey, 0);
                            KeyCode keycode = translateXKeyToFernKey(keysym);
                            keyCallback_(keycode, true);
                        }
                        break;
                        
                    case KeyRelease:
                        if (keyCallback_) {
                            KeySym keysym = XLookupKeysym(&event.xkey, 0);
                            KeyCode keycode = translateXKeyToFernKey(keysym);
                            keyCallback_(keycode, false);
                        }
                        break;
                        
                    case ClientMessage:
                        // Handle window close event
                        if (event.xclient.data.l[0] == (long)wmDeleteMessage_) {
                            shouldClose_ = true;
                        }
                        break;
                        
                    case ConfigureNotify:
                        // Handle window resize
                        if (event.xconfigure.width != width_ || event.xconfigure.height != height_) {
                            int newWidth = event.xconfigure.width;
                            int newHeight = event.xconfigure.height;
                           
                            
                            // Reallocate pixel buffer and recreate XImage for new dimensions
                            if (newWidth > 0 && newHeight > 0) {
                                try {
                                    resizePixelBuffer(newWidth, newHeight);
                                    width_ = newWidth;
                                    height_ = newHeight;
                                    
                                    if (resizeCallback_) {
                                        resizeCallback_(width_, height_);
                                    }
                                    
                                } catch (const std::exception& e) {
                                    std::cerr << "Error during window resize: " << e.what() << std::endl;
                                }
                            } else {
                                std::cerr << "Warning: Invalid resize dimensions ignored: " 
                                         << newWidth << "x" << newHeight << std::endl;
                            }
                        }
                        break;
                }
            }
        }
        
        std::string getPlatformName() override {
            return "Linux (X11)";
        }
        
        void shutdown() override {
            cleanup();
            shouldClose_ = true;
        }
        
        // Input callback implementations
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
            if (width <= 0 || height <= 0) return;
            
            // Resize the X11 window
            if (display_ && window_) {
                XResizeWindow(display_, window_, width, height);
                XFlush(display_);
            }
            
            // The actual buffer resize will be handled by the ConfigureNotify event
            // when X11 confirms the resize
        }
        
    private:
        void setupPixelBuffer() {
            
            // Validate dimensions
            if (width_ <= 0 || height_ <= 0) {
                throw std::runtime_error("Invalid window dimensions for pixel buffer");
            }
            
            // Allocate our local pixel buffer
            try {
                pixelBuffer_ = new uint32_t[width_ * height_];
                // Initialize buffer to black
                memset(pixelBuffer_, 0, width_ * height_ * sizeof(uint32_t));
            } catch (const std::bad_alloc& e) {
                throw std::runtime_error("Failed to allocate pixel buffer: " + std::string(e.what()));
            }
            
            // Create XImage structure - this is X11's way of handling image data
            int screen = DefaultScreen(display_);
            
            ximage_ = XCreateImage(
                display_,                           // X server connection
                DefaultVisual(display_, screen),    // Color format info
                DefaultDepth(display_, screen),     // Bits per pixel (usually 24 or 32)
                ZPixmap,                            // Image format (raw pixels)
                0,                                  // Offset in data
                (char*)pixelBuffer_,                // Our pixel data
                width_, height_,                    // Image dimensions
                32,                                 // Bitmap padding (32-bit alignment)
                0                                   // Bytes per line (0 = auto-calculate)
            );
            
            if (!ximage_) {
                delete[] pixelBuffer_;
                pixelBuffer_ = nullptr;
                throw std::runtime_error("Failed to create XImage");
            }
            
        }
        
        void resizePixelBuffer(int newWidth, int newHeight) {
            
            // Validate dimensions
            if (newWidth <= 0 || newHeight <= 0) {
                std::cerr << "Error: Invalid resize dimensions: " << newWidth << "x" << newHeight << std::endl;
                return;
            }
            
            // Store old values for cleanup
            XImage* oldXImage = ximage_;
            uint32_t* oldPixelBuffer = pixelBuffer_;
            
            // Allocate new pixel buffer first
            try {
                pixelBuffer_ = new uint32_t[newWidth * newHeight];
            } catch (const std::bad_alloc& e) {
                std::cerr << "Error: Failed to allocate new pixel buffer: " << e.what() << std::endl;
                return;
            }
            
            // Create new XImage with new dimensions
            int screen = DefaultScreen(display_);
            
            ximage_ = XCreateImage(
                display_,                           // X server connection
                DefaultVisual(display_, screen),    // Color format info
                DefaultDepth(display_, screen),     // Bits per pixel (usually 24 or 32)
                ZPixmap,                            // Image format (raw pixels)
                0,                                  // Offset in data
                (char*)pixelBuffer_,                // Our pixel data
                newWidth, newHeight,                // New image dimensions
                32,                                 // Bitmap padding (32-bit alignment)
                0                                   // Bytes per line (0 = auto-calculate)
            );
            
            if (!ximage_) {
                std::cerr << "Error: Failed to create resized XImage" << std::endl;
                // Restore old state
                delete[] pixelBuffer_;
                pixelBuffer_ = oldPixelBuffer;
                ximage_ = oldXImage;
                return;
            }
            
            // Now it's safe to clean up old resources
            if (oldXImage) {
                // Important: Set data to nullptr so XDestroyImage doesn't free our buffer
                oldXImage->data = nullptr;
                XDestroyImage(oldXImage);
            }
            
            if (oldPixelBuffer) {
                delete[] oldPixelBuffer;
            }
            
        }
        
        void cleanup() {
            
            if (ximage_) {
                // Important: Set data to nullptr so XDestroyImage doesn't free our buffer
                ximage_->data = nullptr;
                XDestroyImage(ximage_);
                ximage_ = nullptr;
            }
            
            if (pixelBuffer_) {
                delete[] pixelBuffer_;
                pixelBuffer_ = nullptr;
            }
            
            if (gc_) {
                XFreeGC(display_, gc_);
                gc_ = nullptr;
            }
            
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
}
#endif