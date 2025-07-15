#include "fern/fern.hpp"
#include <iostream>

using namespace Fern;

// Global widgets for testing
static std::shared_ptr<TextWidget> text1;
static std::shared_ptr<TextWidget> text2;
static std::shared_ptr<ButtonWidget> button;
static int frameCount = 0;

void setupUI() {
    std::cout << "=== Setting Up Dirty Checking Test ===" << std::endl;
    
    // Create widgets using the proper addWidget function 
    text1 = Text(Point(100, 100), "Hello World", 2, Colors::White);
    text2 = Text(Point(100, 200), "Dynamic Text", 2, Colors::Green);
    
    ButtonConfig config = {
        .x = 100, .y = 300, .width = 200, .height = 50,
        .normalColor = Colors::Blue,
        .hoverColor = Colors::LightBlue,
        .pressColor = Colors::DarkBlue,
        .label = "Click Me",
        .textScale = 2,
        .textColor = Colors::White
    };
    button = Button(config);
    
    // Set up button click handler
    button->onClick.connect([]() {
        std::cout << "Button clicked! Changing text..." << std::endl;
        text1->setText("Button was clicked!");
        text2->setText("Frame: " + std::to_string(frameCount));
    });
    
    // Test initial dirty states
    std::cout << "Initial state:" << std::endl;
    std::cout << "  text1 dirty: " << text1->isDirty() << std::endl;
    std::cout << "  text2 dirty: " << text2->isDirty() << std::endl;
    std::cout << "  button dirty: " << button->isDirty() << std::endl;
}

void draw() {
    frameCount++;
    
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Test dirty checking every 60 frames
    if (frameCount % 60 == 0) {
        std::cout << "\n=== Frame " << frameCount << " - Dirty Check ===" << std::endl;
        std::cout << "  text1 dirty: " << text1->isDirty() << std::endl;
        std::cout << "  text2 dirty: " << text2->isDirty() << std::endl;
        std::cout << "  button dirty: " << button->isDirty() << std::endl;
        
        // Make some changes to test dirty marking
        if (frameCount == 120) {
            std::cout << "Making programmatic changes..." << std::endl;
            text2->setText("Auto-updated at frame " + std::to_string(frameCount));
            button->setPosition(150, 350);
        }
    }
    
    // Draw instructions
    DrawText::drawText("Dirty Checking Test", 50, 50, 3, Colors::White);
    DrawText::drawText("Click the button to test interactive dirty checking", 50, 450, 1, Colors::LightGray);
    DrawText::drawText("Check console for dirty state logs", 50, 480, 1, Colors::LightGray);
}

// Test the dirty checking system properly
int main() {
    std::cout << "=== Proper Dirty Checking Test ===" << std::endl;
    
    // Initialize Fern with proper rendering context
    Fern::initialize();
    
    // Set up UI
    setupUI();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start render loop (this will show visual output)
    Fern::startRenderLoop();
    
    return 0;
}
