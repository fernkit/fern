#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
 
    
    // Simple font loading test - use embedded path
    bool ttfLoaded = false;
    try {
        // When using --embed-file fonts, the path is relative to the embedded filesystem
        // ttfLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
        ttfLoaded = TTF::load("roboto", "fonts/SpaceMono-Bold.ttf");
            TTF::setDefault("roboto");

    } catch (...) {

        ttfLoaded = false;
    }
    
    // Test direct widget addition without container/layout
    addWidget(Text(Point(200, 200), "RISHI", 150, Colors::Red, true, FontType::TTF));

    

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
