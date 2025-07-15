#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void draw() {
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Draw title
    DrawText::drawText("Welcome to web_template_demo!", 50, 50, 3, Colors::White);
    
    // Draw subtitle
    DrawText::drawText("Your Fern project is ready!", 50, 100, 2, Colors::LightGray);
    
    // Draw instructions
    DrawText::drawText("Edit lib/main.cpp to start coding", 50, 150, 1, Colors::Cyan);
    DrawText::drawText("Run 'fern fire' to see changes", 50, 170, 1, Colors::Cyan);
}

int main() {
    std::cout << "🌿 Starting web_template_demo..." << std::endl;
    
    // Initialize Fern
    Fern::initialize();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start the application
    Fern::startRenderLoop();
    
    return 0;
}
