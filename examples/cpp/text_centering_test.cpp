#include "fern/fern.hpp"
#include <iostream>

using namespace Fern;

void setupUI() {
    std::cout << "=== Text Centering Test ===" << std::endl;
    
    // Create a center widget to test text centering
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, 800, 600);
    
    // Test text in center
    auto centeredText = Text(Point(0, 0), "This should be centered", 2, Colors::White, false);
    centerWidget->add(centeredText);
    
    addWidget(centerWidget);
    
    // Test regular text positioning
    Text(Point(50, 50), "Top-left text", 2, Colors::Green);
    Text(Point(400, 300), "Middle text", 2, Colors::Blue);
    Text(Point(600, 550), "Bottom-right text", 2, Colors::Red);
    
    // Draw reference lines to help visualize centering
    addWidget(Line(Point(0, 300), Point(800, 300), 1, Colors::Gray)); // Horizontal center
    addWidget(Line(Point(400, 0), Point(400, 600), 1, Colors::Gray));  // Vertical center
}

void draw() {
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Draw title
    DrawText::drawText("Text Centering Test", 50, 20, 2, Colors::White);
    DrawText::drawText("Gray lines show center - white text should be centered", 50, 580, 1, Colors::LightGray);
}

int main() {
    std::cout << "=== Testing Text Centering Fix ===" << std::endl;
    
    // Initialize Fern
    Fern::initialize();
    
    // Set up UI
    setupUI();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start render loop
    Fern::startRenderLoop();
    
    return 0;
}
