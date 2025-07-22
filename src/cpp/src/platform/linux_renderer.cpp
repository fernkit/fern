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
        
        XIM xim_;                   // Input Method
        XIC xic_;                   // Input Context
        Atom wmTakeFocus_;
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
                default: return KeyCode::Unknown;
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
            
            display_ = XOpenDisplay(nullptr);
            if (!display_) {
                throw std::runtime_error("Cannot connect to X server. Is DISPLAY set?");
            }
            
            int screen = DefaultScreen(display_);
            Window rootWindow = RootWindow(display_, screen);
            
            window_ = XCreateSimpleWindow(
                display_, rootWindow, 100, 100, width, height, 2,
                BlackPixel(display_, screen), WhitePixel(display_, screen)
            );
            
            if (!window_) {
                throw std::runtime_error("Failed to create X11 window");
            }
            
            XStoreName(display_, window_, "Fern Application - Linux");
            
            XSelectInput(display_, window_, 
                ExposureMask | KeyPressMask | KeyReleaseMask |
                ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                StructureNotifyMask | FocusChangeMask);
            
            xim_ = XOpenIM(display_, nullptr, nullptr, nullptr);
            if (!xim_) {
                throw std::runtime_error("Failed to open X Input Method");
            }

            xic_ = XCreateIC(xim_,
                            XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                            XNClientWindow, window_,
                            XNFocusWindow, window_,
                            nullptr);
            if (!xic_) {
                throw std::runtime_error("Failed to create X Input Context");
            }
            
            wmDeleteMessage_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
            wmTakeFocus_     = XInternAtom(display_, "WM_TAKE_FOCUS", False);

            Atom protocols[] = {wmDeleteMessage_, wmTakeFocus_};
            XSetWMProtocols(display_, window_, protocols, 2);

            gc_ = XCreateGC(display_, window_, 0, nullptr);
            if (!gc_) {
                throw std::runtime_error("Failed to create graphics context");
            }
            
            setupPixelBuffer();
            
            // CORRECTED STARTUP SEQUENCE
            // Step 9: Make window visible
            XMapWindow(display_, window_);

            // Step 10: Wait for the window to be mapped before setting focus
            // This loop waits for the first Expose event, which confirms the
            // window is visible and ready to receive focus, preventing a BadMatch error.
            bool windowIsReady = false;
            while (!windowIsReady) {
                XEvent event;
                XNextEvent(display_, &event);
                if (event.type == Expose) {
                    windowIsReady = true;
                }
            }

            // Step 11: Now that the window is confirmed to be visible, set the input focus.
            XSetInputFocus(display_, window_, RevertToParent, CurrentTime);
            XFlush(display_); // Ensure the focus request is sent
        }
        
        void present(uint32_t* pixelBuffer, int width, int height) override {
            if (!ximage_ || !pixelBuffer_ || !display_ || !pixelBuffer) {
                return;
            }
            
            if (width != width_ || height != height_) {
                return; 
            }
            
            size_t bufferSize = width * height * sizeof(uint32_t);
            memcpy(pixelBuffer_, pixelBuffer, bufferSize);
            
            XPutImage(
                display_, window_, gc_, ximage_,
                0, 0, 0, 0, width, height
            );
            
            XFlush(display_);
        }
        
        void setTitle(const std::string& title) override {
            if (display_ && window_) {
                XStoreName(display_, window_, title.c_str());
                XFlush(display_);
            }
        }
        
        bool shouldClose() override {
            return shouldClose_;
        }
        
        // RESTORED, FUNCTIONAL pollEvents()
        void pollEvents() override {
            XEvent event;
            while (XPending(display_) > 0) {
                XNextEvent(display_, &event);
                if (XFilterEvent(&event, None)) {
                    continue; // The input method handled this event, so we skip it.
                }
                
                switch (event.type) {
                    case ButtonPress:
                        if (clickCallback_) { clickCallback_(true); }
                        break;
                        
                    case ButtonRelease:
                        if (clickCallback_) { clickCallback_(false); }
                        break;
                        
                    case MotionNotify:
                        if (mouseCallback_) { mouseCallback_(event.xmotion.x, event.xmotion.y); }
                        break;

                    case KeyPress: {
                        KeySym keysym = NoSymbol;
                        char buffer[32] = {0};
                        Status status = 0;

                        int len = XmbLookupString(xic_, &event.xkey, buffer, 31, &keysym, &status);

                        if (keyCallback_) {
                            keyCallback_(translateXKeyToFernKey(keysym), true);
                        }
                        if (textInputCallback_ && len > 0) {
                            textInputCallback_(std::string(buffer, len));
                        }
                        break;
                    }

                    case KeyRelease:
                        if (keyCallback_) {
                            KeySym keysym = XLookupKeysym(&event.xkey, 0);
                            keyCallback_(translateXKeyToFernKey(keysym), false);
                        }
                        break;
                        
                    case ClientMessage:
                        if (event.xclient.data.l[0] == (long)wmDeleteMessage_) {
                            shouldClose_ = true;
                        }
                        else if (event.xclient.data.l[0] == (long)wmTakeFocus_) {
                            XSetInputFocus(display_, window_, RevertToParent, event.xclient.data.l[1]);
                        }
                        break;
                        
                    case ConfigureNotify:
                        if (event.xconfigure.width != width_ || event.xconfigure.height != height_) {
                            int newWidth = event.xconfigure.width;
                            int newHeight = event.xconfigure.height;
                           
                            if (newWidth > 0 && newHeight > 0) {
                                resizePixelBuffer(newWidth, newHeight);
                                width_ = newWidth;
                                height_ = newHeight;
                                if (resizeCallback_) {
                                    resizeCallback_(width_, height_);
                                }
                            }
                        }
                        break;
                        
                    case FocusIn:
                        // Optional: Handle focus gain
                        break;
                        
                    case FocusOut:
                        // Optional: Handle focus loss
                        break;
                        
                    default:
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
        
        void setMouseCallback(std::function<void(int, int)> callback) override { mouseCallback_ = callback; }
        void setClickCallback(std::function<void(bool)> callback) override { clickCallback_ = callback; }
        void setResizeCallback(std::function<void(int, int)> callback) override { resizeCallback_ = callback; }
        void setKeyCallback(std::function<void(KeyCode, bool)> callback) override { keyCallback_ = callback; }
        void setTextInputCallback(std::function<void(const std::string&)> callback) override { textInputCallback_ = callback; }
        
        void setSize(int width, int height) override {
            if (width > 0 && height > 0 && display_ && window_) {
                XResizeWindow(display_, window_, width, height);
                XFlush(display_);
            }
        }
        
    private:
        void setupPixelBuffer() {
            if (width_ <= 0 || height_ <= 0) {
                throw std::runtime_error("Invalid window dimensions for pixel buffer");
            }
            
            try {
                pixelBuffer_ = new uint32_t[width_ * height_];
                memset(pixelBuffer_, 0, width_ * height_ * sizeof(uint32_t));
            } catch (const std::bad_alloc& e) {
                throw std::runtime_error("Failed to allocate pixel buffer: " + std::string(e.what()));
            }
            
            int screen = DefaultScreen(display_);
            ximage_ = XCreateImage(
                display_, DefaultVisual(display_, screen), DefaultDepth(display_, screen),
                ZPixmap, 0, (char*)pixelBuffer_, width_, height_, 32, 0
            );
            
            if (!ximage_) {
                delete[] pixelBuffer_;
                pixelBuffer_ = nullptr;
                throw std::runtime_error("Failed to create XImage");
            }
        }
        
        void resizePixelBuffer(int newWidth, int newHeight) {
            if (newWidth <= 0 || newHeight <= 0) return;
            
            XImage* oldXImage = ximage_;
            uint32_t* oldPixelBuffer = pixelBuffer_;
            
            try {
                pixelBuffer_ = new uint32_t[newWidth * newHeight];
            } catch (const std::bad_alloc& e) {
                std::cerr << "Error: Failed to allocate new pixel buffer: " << e.what() << std::endl;
                pixelBuffer_ = oldPixelBuffer; // Restore old buffer on failure
                return;
            }
            
            int screen = DefaultScreen(display_);
            ximage_ = XCreateImage(
                display_, DefaultVisual(display_, screen), DefaultDepth(display_, screen),
                ZPixmap, 0, (char*)pixelBuffer_, newWidth, newHeight, 32, 0
            );
            
            if (!ximage_) {
                std::cerr << "Error: Failed to create resized XImage" << std::endl;
                delete[] pixelBuffer_;
                pixelBuffer_ = oldPixelBuffer;
                ximage_ = oldXImage;
                return;
            }
            
            if (oldXImage) {
                oldXImage->data = nullptr;
                XDestroyImage(oldXImage);
            }
            if (oldPixelBuffer) {
                delete[] oldPixelBuffer;
            }
        }
        
        void cleanup() {
            if (xic_) { XDestroyIC(xic_); xic_ = nullptr; }
            if (xim_) { XCloseIM(xim_); xim_ = nullptr; }
            
            if (ximage_) {
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