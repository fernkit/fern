#include <fern/fern.hpp>
#include <emscripten.h>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    EM_ASM({
        console.log("🌿 Simple Font Demo: Starting...");
    });
    
    // Simple font loading test - use embedded path
    bool ttfLoaded = false;
    try {
        // When using --embed-file fonts, the path is relative to the embedded filesystem
        // ttfLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
        ttfLoaded = TTF::load("roboto", "fonts/SpaceMono-Bold.ttf");
        if (ttfLoaded) {
            TTF::setDefault("roboto");
            EM_ASM({
                console.log("🌿 ✅ TTF Font loaded successfully!");
            });
        } else {
            EM_ASM({
                console.log("🌿 ❌ TTF Font failed to load - file not found");
            });
        }
    } catch (...) {
        EM_ASM({
            console.log("🌿 ❌ TTF Font loading threw exception");
        });
        ttfLoaded = false;
    }
    
    // Test direct widget addition without container/layout
    addWidget(Text(Point(200, 200), "RISHI", 150, Colors::Red, true, FontType::TTF));

    
    EM_ASM({
        console.log("🌿 Simple Font Demo: UI setup complete");
    });
}


void draw() {
    Draw::fill(0xFFF0F0F0); // Light gray background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
