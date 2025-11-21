#ifdef __APPLE__
#include "fern/platform/renderer.hpp"
#import <Cocoa/Cocoa.h>
#include <iostream>
#include <cstring>

// Custom NSView for rendering our pixel buffer (MUST be outside namespace)
@interface FernView : NSView {
    uint32_t* pixelBuffer;
    int bufferWidth;
    int bufferHeight;
}
- (void)setPixelBuffer:(uint32_t*)buffer width:(int)w height:(int)h;
@end

@implementation FernView

- (id)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        pixelBuffer = nullptr;
        bufferWidth = 0;
        bufferHeight = 0;
    }
    return self;
}

- (void)setPixelBuffer:(uint32_t*)buffer width:(int)w height:(int)h {
    pixelBuffer = buffer;
    bufferWidth = w;
    bufferHeight = h;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
    if (!pixelBuffer || bufferWidth == 0 || bufferHeight == 0) {
        return;
    }
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    
    // Create CGImage from pixel buffer
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef bitmapContext = CGBitmapContextCreate(
        pixelBuffer,
        bufferWidth,
        bufferHeight,
        8,                              // bits per component
        bufferWidth * 4,                // bytes per row
        colorSpace,
        kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little
    );
    
    CGImageRef image = CGBitmapContextCreateImage(bitmapContext);
    
    // Draw the image (flipped vertically for correct orientation)
    CGContextSaveGState(context);
    CGContextTranslateCTM(context, 0, self.bounds.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextDrawImage(context, self.bounds, image);
    CGContextRestoreGState(context);
    
    // Cleanup
    CGImageRelease(image);
    CGContextRelease(bitmapContext);
    CGColorSpaceRelease(colorSpace);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end

// Window delegate to handle events (MUST be outside namespace)
@interface FernWindowDelegate : NSObject <NSWindowDelegate> {
    @public
    std::function<void()> closeCallback;
    std::function<void(int, int)> resizeCallback;
}
@end

@implementation FernWindowDelegate

- (BOOL)windowShouldClose:(NSWindow*)sender {
    if (closeCallback) {
        closeCallback();
    }
    return YES;
}

- (void)windowDidResize:(NSNotification*)notification {
    if (resizeCallback) {
        NSWindow* window = [notification object];
        NSRect frame = [[window contentView] bounds];
        resizeCallback((int)frame.size.width, (int)frame.size.height);
    }
}

@end

namespace Fern {
    class MacOSRenderer : public PlatformRenderer {
    private:
        NSWindow* window_;
        FernView* view_;
        FernWindowDelegate* delegate_;
        uint32_t* pixelBuffer_;
        int width_, height_;
        bool shouldClose_;
        
        // Input callbacks
        std::function<void(int, int)> mouseCallback_;
        std::function<void(bool)> clickCallback_;
        std::function<void(int, int)> resizeCallback_;
        std::function<void(KeyCode, bool)> keyCallback_;
        std::function<void(const std::string&)> textInputCallback_;
        
        // Event monitor
        id eventMonitor_;
        
        // Key translation helper
        KeyCode translateCocoaKeyToFernKey(unsigned short keyCode) {
            // macOS virtual key codes
            switch (keyCode) {
                case 0x24: return KeyCode::Enter;     // Return
                case 0x35: return KeyCode::Escape;    // Escape
                case 0x31: return KeyCode::Space;     // Space
                case 0x33: return KeyCode::Backspace; // Delete
                case 0x30: return KeyCode::Tab;       // Tab
                case 0x7B: return KeyCode::ArrowLeft; // Left Arrow
                case 0x7C: return KeyCode::ArrowRight;// Right Arrow
                case 0x7E: return KeyCode::ArrowUp;   // Up Arrow
                case 0x7D: return KeyCode::ArrowDown; // Down Arrow
                case 0x00: return KeyCode::A;
                case 0x0B: return KeyCode::B;
                case 0x08: return KeyCode::C;
                case 0x02: return KeyCode::D;
                case 0x0E: return KeyCode::E;
                case 0x03: return KeyCode::F;
                case 0x05: return KeyCode::G;
                case 0x04: return KeyCode::H;
                case 0x22: return KeyCode::I;
                case 0x26: return KeyCode::J;
                case 0x28: return KeyCode::K;
                case 0x25: return KeyCode::L;
                case 0x2E: return KeyCode::M;
                case 0x2D: return KeyCode::N;
                case 0x1F: return KeyCode::O;
                case 0x23: return KeyCode::P;
                case 0x0C: return KeyCode::Q;
                case 0x0F: return KeyCode::R;
                case 0x01: return KeyCode::S;
                case 0x11: return KeyCode::T;
                case 0x20: return KeyCode::U;
                case 0x09: return KeyCode::V;
                case 0x0D: return KeyCode::W;
                case 0x07: return KeyCode::X;
                case 0x10: return KeyCode::Y;
                case 0x06: return KeyCode::Z;
                case 0x1D: return KeyCode::Num0;
                case 0x12: return KeyCode::Num1;
                case 0x13: return KeyCode::Num2;
                case 0x14: return KeyCode::Num3;
                case 0x15: return KeyCode::Num4;
                case 0x17: return KeyCode::Num5;
                case 0x16: return KeyCode::Num6;
                case 0x1A: return KeyCode::Num7;
                case 0x1C: return KeyCode::Num8;
                case 0x19: return KeyCode::Num9;
                default: return KeyCode::Unknown;
            }
        }
        
    public:
        MacOSRenderer() 
            : window_(nullptr)
            , view_(nullptr)
            , delegate_(nullptr)
            , pixelBuffer_(nullptr)
            , width_(0)
            , height_(0)
            , shouldClose_(false)
            , eventMonitor_(nullptr) {
        }
        
        ~MacOSRenderer() {
            shutdown();
        }
        
        void initialize(int width, int height) override {
            @autoreleasepool {
                width_ = width;
                height_ = height;
                shouldClose_ = false;
                
                // Initialize NSApplication if needed
                [NSApplication sharedApplication];
                [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
                
                // Create window
                NSRect frame = NSMakeRect(0, 0, width, height);
                NSUInteger styleMask = NSWindowStyleMaskTitled | 
                                      NSWindowStyleMaskClosable | 
                                      NSWindowStyleMaskMiniaturizable | 
                                      NSWindowStyleMaskResizable;
                
                window_ = [[NSWindow alloc] initWithContentRect:frame
                                                     styleMask:styleMask
                                                       backing:NSBackingStoreBuffered
                                                         defer:NO];
                
                [window_ setTitle:@"Fern Application"];
                [window_ center];
                [window_ makeKeyAndOrderFront:nil];
                
                // Create custom view
                view_ = [[FernView alloc] initWithFrame:frame];
                [window_ setContentView:view_];
                
                // Setup delegate
                delegate_ = [[FernWindowDelegate alloc] init];
                delegate_->closeCallback = [this]() {
                    shouldClose_ = true;
                };
                delegate_->resizeCallback = [this](int w, int h) {
                    if (resizeCallback_) {
                        resizeCallback_(w, h);
                    }
                };
                [window_ setDelegate:delegate_];
                
                // Allocate pixel buffer
                pixelBuffer_ = new uint32_t[width * height];
                std::memset(pixelBuffer_, 0, width * height * sizeof(uint32_t));
                
                // Setup event monitoring for mouse and keyboard
                setupEventMonitoring();
                
                [NSApp activateIgnoringOtherApps:YES];
                
                std::cout << "🍎 macOS Renderer initialized (" << width << "x" << height << ")" << std::endl;
            }
        }
        
        void setupEventMonitoring() {
            // Mouse move tracking
            NSEventMask eventMask = NSEventMaskMouseMoved | NSEventMaskLeftMouseDragged;
            eventMonitor_ = [NSEvent addLocalMonitorForEventsMatchingMask:eventMask handler:^NSEvent*(NSEvent* event) {
                if (mouseCallback_) {
                    NSPoint location = [event locationInWindow];
                    NSRect frame = [view_ bounds];
                    int x = (int)location.x;
                    int y = (int)(frame.size.height - location.y); // Flip Y coordinate
                    mouseCallback_(x, y);
                }
                return event;
            }];
        }
        
        void present(uint32_t* pixelBuffer, int width, int height) override {
            @autoreleasepool {
                if (width != width_ || height != height_) {
                    // Resize buffer if needed
                    delete[] pixelBuffer_;
                    pixelBuffer_ = new uint32_t[width * height];
                    width_ = width;
                    height_ = height;
                }
                
                // Copy pixel data
                std::memcpy(pixelBuffer_, pixelBuffer, width * height * sizeof(uint32_t));
                
                // Update view
                [view_ setPixelBuffer:pixelBuffer_ width:width height:height];
            }
        }
        
        void shutdown() override {
            @autoreleasepool {
                if (eventMonitor_) {
                    [NSEvent removeMonitor:eventMonitor_];
                    eventMonitor_ = nullptr;
                }
                
                if (pixelBuffer_) {
                    delete[] pixelBuffer_;
                    pixelBuffer_ = nullptr;
                }
                
                if (window_) {
                    [window_ close];
                    window_ = nullptr;
                }
                
                view_ = nullptr;
                delegate_ = nullptr;
            }
        }
        
        void setTitle(const std::string& title) override {
            @autoreleasepool {
                if (window_) {
                    NSString* nsTitle = [NSString stringWithUTF8String:title.c_str()];
                    [window_ setTitle:nsTitle];
                }
            }
        }
        
        void setSize(int width, int height) override {
            @autoreleasepool {
                if (window_) {
                    NSRect frame = [window_ frame];
                    frame.size.width = width;
                    frame.size.height = height;
                    [window_ setFrame:frame display:YES];
                    
                    width_ = width;
                    height_ = height;
                }
            }
        }
        
        bool shouldClose() override {
            return shouldClose_;
        }
        
        void pollEvents() override {
            @autoreleasepool {
                NSEvent* event;
                while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                   untilDate:nil
                                                      inMode:NSDefaultRunLoopMode
                                                     dequeue:YES])) {
                    
                    switch ([event type]) {
                        case NSEventTypeLeftMouseDown:
                            if (clickCallback_) {
                                clickCallback_(true);
                            }
                            break;
                            
                        case NSEventTypeLeftMouseUp:
                            if (clickCallback_) {
                                clickCallback_(false);
                            }
                            break;
                            
                        case NSEventTypeKeyDown: {
                            unsigned short keyCode = [event keyCode];
                            KeyCode fernKey = translateCocoaKeyToFernKey(keyCode);
                            if (keyCallback_ && fernKey != KeyCode::Unknown) {
                                keyCallback_(fernKey, true);
                            }
                            
                            // Handle text input
                            if (textInputCallback_) {
                                NSString* characters = [event characters];
                                if ([characters length] > 0) {
                                    std::string text = [characters UTF8String];
                                    textInputCallback_(text);
                                }
                            }
                            break;
                        }
                            
                        case NSEventTypeKeyUp: {
                            unsigned short keyCode = [event keyCode];
                            KeyCode fernKey = translateCocoaKeyToFernKey(keyCode);
                            if (keyCallback_ && fernKey != KeyCode::Unknown) {
                                keyCallback_(fernKey, false);
                            }
                            break;
                        }
                            
                        default:
                            break;
                    }
                    
                    [NSApp sendEvent:event];
                }
            }
        }
        
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
        
        std::string getPlatformName() override {
            return "macOS";
        }
    };
    
    // Factory function for creating macOS renderer
    std::unique_ptr<PlatformRenderer> createMacOSRenderer() {
        return std::make_unique<MacOSRenderer>();
    }
}

#endif // __APPLE__
