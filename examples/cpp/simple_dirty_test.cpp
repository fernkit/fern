#include "fern/fern.hpp"
#include <iostream>

using namespace Fern;

int main() {
    try {
        std::cout << "=== Simple Dirty Checking Test ===" << std::endl;
        
        // Test TextWidget dirty checking
        auto text = Text(Point(100, 100), "Hello World", 2, Colors::White, false);
        
        std::cout << "Initial text dirty state: " << (text->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        // Clean it
        text->markClean();
        std::cout << "After markClean: " << (text->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        // Change text
        text->setText("New Text");
        std::cout << "After setText: " << (text->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        // Test ButtonWidget dirty checking
        ButtonConfig config = {
            .x = 300, .y = 250, .width = 200, .height = 50,
            .normalColor = Colors::Green,
            .hoverColor = Colors::LightGreen,
            .pressColor = Colors::DarkGreen,
            .label = "CLICK ME",
            .textScale = 2,
            .textColor = Colors::White
        };
        
        auto button = Button(config, false);
        
        std::cout << "Initial button dirty state: " << (button->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        // Clean it
        button->markClean();
        std::cout << "After markClean: " << (button->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        // Change position
        button->setPosition(400, 300);
        std::cout << "After setPosition: " << (button->isDirty() ? "DIRTY" : "clean") << std::endl;
        
        std::cout << "=== Test Complete ===" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
