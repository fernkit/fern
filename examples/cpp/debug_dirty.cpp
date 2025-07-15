#include "fern/fern.hpp"
#include <iostream>

using namespace Fern;

int main() {
    std::cout << "=== Debug Dirty Checking ===" << std::endl;
    
    // Test 1: Create a text widget with addToManager=true
    auto text1 = Text(Point(100, 100), "Test Text 1", 2, Colors::White, true);
    std::cout << "Created text1, dirty: " << text1->isDirty() << std::endl;
    
    // Test 2: Check widget manager
    auto& manager = WidgetManager::getInstance();
    std::cout << "Widget manager has widgets, will try to render..." << std::endl;
    
    // Test 3: Manual render call 
    std::cout << "Before renderAll - text1 dirty: " << text1->isDirty() << std::endl;
    manager.renderAll();
    std::cout << "After renderAll - text1 dirty: " << text1->isDirty() << std::endl;
    
    // Test 4: Change text and test again
    text1->setText("Changed Text");
    std::cout << "After setText - text1 dirty: " << text1->isDirty() << std::endl;
    manager.renderAll();
    std::cout << "After renderAll - text1 dirty: " << text1->isDirty() << std::endl;
    
    return 0;
}
