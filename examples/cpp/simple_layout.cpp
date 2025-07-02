#include <fern/fern.hpp>
#include <emscripten.h>

using namespace Fern;

void setupUI() {
    // Clear any previous widgets
    WidgetManager::getInstance().clear();
    
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
            0xFF1E1E1E,        
            0, 0, width, height,
            

            Row({
                Text(Point(0, 0), "Fern Graphics", 200, Colors::White, false, FontType::TTF),
                Container(Colors::Red, 0, 0, 100, 40),
                Container(Colors::Green, 0, 0, 100, 40),
                Container(Colors::Blue, 0, 0, 100, 40)
            }, false, MainAxisAlignment::SpaceBetween)

        )
        );
    
}

void draw() {
    Draw::fill(0xFF121212);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}