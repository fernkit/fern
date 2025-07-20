#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void draw() {
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Draw title
    DrawText::drawText("Colored Boxes Example", 50, 50, 3, Colors::White);
    
    // Draw subtitle  
    DrawText::drawText("This example is under construction", 50, 100, 2, Colors::LightGray);
    
    // Draw instructions
    DrawText::drawText("Check back later for colored boxes demonstration", 50, 150, 1, Colors::Cyan);
}

int main() {
    std::cout << "🌿 Starting Colored Boxes Example..." << std::endl;
    
    // Initialize Fern
    Fern::initialize();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start the application
    Fern::startRenderLoop();
    
    return 0;
}
