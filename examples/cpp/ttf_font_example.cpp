#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    

    
    // Load TTF font (requires --embed-file fonts when building)
    bool fontLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
        TTF::setDefault("roboto");

    
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


