#ifdef __linux__
#include "fern/platform/renderer.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
        
    public:
        LinuxRenderer() : display_(nullptr), window_(0), gc_(nullptr), 
                         ximage_(nullptr), pixelBuffer_(nullptr), shouldClose_(false) {
            std::cout << "Creating Linux X11 Renderer..." << std::endl;
        }
        
        ~LinuxRenderer() {
            cleanup();
        }
        
        void initialize(int width, int height) override {
            width_ = width;
            height_ = height;
            
            std::cout << "Initializing X11 display..." << std::endl;
            
            // Step 1: Connect to X Server
            display_ = XOpenDisplay(nullptr);  // nullptr means use DISPLAY environment variable
            if (!display_) {
                throw std::runtime_error("Cannot connect to X server. Is DISPLAY set?");
            }
            
            // Step 2: Get screen information
            int screen = DefaultScreen(display_);              // Usually 0 for single monitor
            Window rootWindow = RootWindow(display_, screen);   // Desktop background window
            
            std::cout << "Connected to X server on screen " << screen << std::endl;
            
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
            
            std::cout << "Created window with ID: " << window_ << std::endl;
            
            // Step 4: Set window properties
            XStoreName(display_, window_, "Fern Application - Linux");
            
            // Step 5: Create Graphics Context (like a "drawing pen")
            gc_ = XCreateGC(display_, window_, 0, nullptr);
            if (!gc_) {
                throw std::runtime_error("Failed to create graphics context");
            }
            
            // Step 6: Prepare pixel buffer and XImage
            setupPixelBuffer();
            
            // Step 7: Make window visible
            XMapWindow(display_, window_);     // Show the window
            XFlush(display_);                  // Force X server to process requests
            
            std::cout << "Linux renderer initialized successfully!" << std::endl;
        }
        
        void present(uint32_t* pixelBuffer, int width, int height) override {
            if (!ximage_ || !pixelBuffer_ || !display_) return;
            
            // Step 1: Copy our pixel data to X11's format
            // Note: X11 might have different byte ordering, but we'll keep it simple for now
            memcpy(pixelBuffer_, pixelBuffer, width * height * sizeof(uint32_t));
            
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
                            // X11 button mapping: 1=left, 2=middle, 3=right
                            if (event.xbutton.button == 1) {
                                clickCallback_(true);
                            }
                        }
                        break;
                        
                    case ButtonRelease:
                        if (clickCallback_) {
                            if (event.xbutton.button == 1) {
                                clickCallback_(false);
                            }
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
                        
                        // Handle text input for printable characters
                        if (textInputCallback_) {
                            char buffer[32];
                            KeySym keysym;
                            int len = XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);
                            if (len > 0) {
                                buffer[len] = '\0';
                                textInputCallback_(std::string(buffer));
                            }
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
                            width_ = event.xconfigure.width;
                            height_ = event.xconfigure.height;
                            if (resizeCallback_) {
                                resizeCallback_(width_, height_);
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
            if (width_ == width && height_ == height) return;
            
            width_ = width;
            height_ = height;
            
            if (display_ && window_) {
                XResizeWindow(display_, window_, width, height);
                // Recreate pixel buffer with new dimensions
                setupPixelBuffer();
                XFlush(display_);
            }
        }
        
    private:
        void setupPixelBuffer() {
            std::cout << "Setting up pixel buffer..." << std::endl;
            
            // Allocate our local pixel buffer
            pixelBuffer_ = new uint32_t[width_ * height_];
            
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
                throw std::runtime_error("Failed to create XImage");
            }
            
            std::cout << "Pixel buffer setup complete. Depth: " << DefaultDepth(display_, DefaultScreen(display_)) << " bits" << std::endl;
        }
        
        void cleanup() {
            std::cout << "Cleaning up X11 resources..." << std::endl;
            
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
            
            std::cout << "X11 cleanup complete." << std::endl;
        }
    };
}
#endif