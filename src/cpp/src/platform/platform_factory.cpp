#include "fern/platform/renderer.hpp"

#ifdef __EMSCRIPTEN__
#include "web_renderer.cpp"
#include <emscripten.h>
#elif defined(__linux__)
#include "linux_renderer.cpp"
#elif defined(__APPLE__)
// Forward declare the MacOSRenderer class (defined in macos_renderer.mm)
namespace Fern {
    class MacOSRenderer;
}
#endif

namespace Fern {
    std::unique_ptr<PlatformRenderer> createRenderer() {
#ifdef __EMSCRIPTEN__
        return std::make_unique<WebRenderer>();
#elif defined(__APPLE__)
        // Extern function defined in macos_renderer.mm
        extern std::unique_ptr<PlatformRenderer> createMacOSRenderer();
        return createMacOSRenderer();
#elif defined(__linux__)
        return std::make_unique<LinuxRenderer>();
#else
        #error "Only Web, macOS, and Linux platforms supported currently"
#endif
    }
}

// Define extern "C" functions ONLY here for web
#ifdef __EMSCRIPTEN__
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void webRendererMouseMove(Fern::WebRenderer* renderer, int x, int y) {
        renderer->onMouseMove(x, y);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void webRendererMouseClick(Fern::WebRenderer* renderer, bool down) {
        renderer->onMouseClick(down);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void webRendererResize(Fern::WebRenderer* renderer, int width, int height) {
        renderer->onResize(width, height);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void webRendererKeyEvent(Fern::WebRenderer* renderer, int keyCode, bool isPressed) {
        renderer->onKeyEvent(keyCode, isPressed);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void webRendererTextInput(Fern::WebRenderer* renderer, int charCode) {
        char text[2] = {0};
        if (charCode >= 32 && charCode <= 126) {
            text[0] = static_cast<char>(charCode);
            text[1] = '\0';
            renderer->onTextInput(text);
        }
    }
}
#endif