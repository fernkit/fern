#include <fern/fern.hpp>
#include <emscripten.h>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    EM_ASM({
        console.log("🌿 TTF Font Example: Starting...");
    });
    
    // Load TTF font (requires --embed-file fonts when building)
    bool fontLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
    if (fontLoaded) {
        TTF::setDefault("roboto");
        EM_ASM({
            console.log("🌿 ✅ TTF Font loaded successfully!");
        });
    } else {
        EM_ASM({
            console.log("🌿 ❌ TTF Font failed to load");
        });
    }
    
    addWidget(
        Container(
            Colors::White,
            0, 0, width, height,
            
            Column({
                // Title with default bitmap font
                // Text(Point(0, 20), "Fern Graphics TTF Font Demo", 3, Colors::DarkBlue, false),
                
                // TTF font examples at different sizes
                Text(Point(50, 100), "Large TTF Text (48pt)", 100, Colors::Red, true, FontType::TTF),
                Text(Point(50, 180), "Medium TTF Text (32pt)", 80, Colors::Green, true, FontType::TTF),
                Text(Point(50, 240), "Small TTF Text (24pt)", 60, Colors::Blue, true, FontType::TTF),
                
                // // Comparison with bitmap font
                // Text(Point(50, 320), "Bitmap Font for Comparison", 2, Colors::Purple, false),
                
                // More TTF examples
                Text(Point(50, 380), "Custom Font ABCDEFGHIJKLMNOPQRSTUVWXYZ", 60, Colors::DarkRed, true, FontType::TTF),
                Text(Point(50, 440), "Numbers 0123456789", 60, Colors::DarkGreen, true, FontType::TTF),
                Text(Point(50, 500), "Special !@#$%^&*()", 60, Colors::Orange, true, FontType::TTF)
                
            }, false)
        )
    );
    
    EM_ASM({
        console.log("🌿 TTF Font Example: UI setup complete");
    });
}

void draw() {
    Draw::fill(0xFFF5F5F5); // Very light gray background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}

/*
To build and run this example:

1. Make sure you have font files in a 'fonts' directory
2. Build with font embedding:
   fern --cpp --embed-file fonts ttf_font_example.cpp

This will:
- Compile the C++ code
- Embed the fonts directory into the WebAssembly binary
- Start a local server to view the result

Note: TTF font support is experimental. For production use,
consider using the default bitmap font system.
*/
