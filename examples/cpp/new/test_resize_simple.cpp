#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Simple text to display
    auto text = Text(Point(0, 0), "Resize Test - Try resizing this window!", 2, Colors::White);
    auto instruction = Text(Point(0, 0), "It should NOT crash with SIGSEGV", 2, Colors::Yellow);
    
    // Layout
    std::vector<std::shared_ptr<Widget>> children = {
        text,
        SizedBox(0, 30),
        instruction,
    };
    
    auto layout = Column(children);
    
    // Center the layout
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, getWidth(), getHeight());
    centerWidget->add(layout);
    
    addWidget(centerWidget);
}

void draw() {
    // Fill background
    Draw::fill(Colors::DarkGray);
}

int main() {
    std::cout << "Testing Fern Linux Resize Functionality" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Instructions:" << std::endl;
    std::cout << "1. Window will open and show a simple message" << std::endl;
    std::cout << "2. Try resizing the window by dragging the edges" << std::endl;
    std::cout << "3. The app should NOT crash with SIGSEGV" << std::endl;
    std::cout << "4. Press Ctrl+C to exit when done testing" << std::endl;
    std::cout << std::endl;
    
    try {
        Fern::initialize();
        setupUI();
        Fern::setDrawCallback(draw);
        Fern::startRenderLoop();
        
        std::cout << "✓ Test completed successfully - no crashes!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
