#include "fern/fern.hpp"
#include <iostream>
#include <chrono>

using namespace Fern;

class DirtyCheckingTest {
private:
    std::shared_ptr<TextWidget> staticText_;
    std::shared_ptr<TextWidget> changingText_;
    std::shared_ptr<ButtonWidget> button_;
    int frameCount_ = 0;
    int dirtyCount_ = 0;
    
public:
    void initialize() {
        std::cout << "=== Dirty Checking Test ===" << std::endl;
        ButtonConfig config = {
            .x = 300, .y = 250, .width = 200, .height = 50,
            .normalColor = Colors::Green,
            .hoverColor = Colors::LightGreen,
            .pressColor = Colors::DarkGreen,
            .label = "CLICK ME",
            .textScale = 2,
            .textColor = Colors::White
        };
        // Create test widgets
        staticText_ = Text(Point(100, 100), "Static Text - Should Not Change", 2, Colors::White, true);
        changingText_ = Text(Point(100, 200), "Frame: 0", 2, Colors::Green, true);
        button_ = Button(config, true);
        
        // Set up button click handler
        button_->onClick.connect([this]() {
            std::cout << "Button clicked - should mark button as dirty" << std::endl;
        });
        
        std::cout << "Initial state - all widgets should be dirty" << std::endl;
        checkDirtyStates();
    }
    
    void update() {
        frameCount_++;
        
        // Every 60 frames, update the changing text
        if (frameCount_ % 60 == 0) {
            changingText_->setText("Frame: " + std::to_string(frameCount_));
            std::cout << "Frame " << frameCount_ << " - Updated changing text" << std::endl;
        }
        
        // Every 10 frames, check dirty states
        if (frameCount_ % 10 == 0) {
            checkDirtyStates();
        }
        
        // Simulate rendering
        simulateRender();
    }
    
    void checkDirtyStates() {
        std::cout << "Dirty States Check:" << std::endl;
        std::cout << "  Static Text: " << (staticText_->isDirty() ? "DIRTY" : "clean") << std::endl;
        std::cout << "  Changing Text: " << (changingText_->isDirty() ? "DIRTY" : "clean") << std::endl;
        std::cout << "  Button: " << (button_->isDirty() ? "DIRTY" : "clean") << std::endl;
    }
    
    void simulateRender() {
        int rendered = 0;
        
        // Simulate WidgetManager::renderAll() behavior
        if (staticText_->isDirty()) {
            rendered++;
            staticText_->markClean();
        }
        
        if (changingText_->isDirty()) {
            rendered++;
            changingText_->markClean();
        }
        
        if (button_->isDirty()) {
            rendered++;
            button_->markClean();
        }
        
        dirtyCount_ += rendered;
        
        if (frameCount_ % 10 == 0) {
            float efficiency = (3.0f * frameCount_ - dirtyCount_) / (3.0f * frameCount_) * 100.0f;
            std::cout << "Render efficiency: " << efficiency << "% (saved " 
                      << (3 * frameCount_ - dirtyCount_) << " out of " 
                      << (3 * frameCount_) << " render calls)" << std::endl;
        }
    }
    
    void testWidgetChanges() {
        std::cout << "\n=== Testing Widget State Changes ===" << std::endl;
        
        // Test text changes
        std::cout << "Before setText - changingText dirty: " << changingText_->isDirty() << std::endl;
        changingText_->setText("New Text");
        std::cout << "After setText - changingText dirty: " << changingText_->isDirty() << std::endl;
        
        // Test same text (should NOT mark dirty)
        changingText_->markClean();
        std::cout << "After markClean - changingText dirty: " << changingText_->isDirty() << std::endl;
        changingText_->setText("New Text");  // Same text
        std::cout << "After setText (same text) - changingText dirty: " << changingText_->isDirty() << std::endl;
        
        // Test position changes
        std::cout << "Before setPosition - staticText dirty: " << staticText_->isDirty() << std::endl;
        staticText_->setPosition(150, 150);
        std::cout << "After setPosition - staticText dirty: " << staticText_->isDirty() << std::endl;
        
        // Test same position (should NOT mark dirty)
        staticText_->markClean();
        std::cout << "After markClean - staticText dirty: " << staticText_->isDirty() << std::endl;
        staticText_->setPosition(150, 150);  // Same position
        std::cout << "After setPosition (same pos) - staticText dirty: " << staticText_->isDirty() << std::endl;
        
        // Test color changes
        std::cout << "Before setColor - changingText dirty: " << changingText_->isDirty() << std::endl;
        changingText_->setColor(Colors::Red);
        std::cout << "After setColor - changingText dirty: " << changingText_->isDirty() << std::endl;
    }
    
    void run() {
        initialize();
        testWidgetChanges();
        
        std::cout << "\n=== Running Animation Test ===" << std::endl;
        
        // Run animation test for 100 frames
        for (int i = 0; i < 100; i++) {
            update();
        }
        
        std::cout << "\n=== Test Complete ===" << std::endl;
        std::cout << "Total frames: " << frameCount_ << std::endl;
        std::cout << "Total render calls: " << dirtyCount_ << std::endl;
        std::cout << "Potential render calls: " << (3 * frameCount_) << std::endl;
        std::cout << "Efficiency: " << ((3.0f * frameCount_ - dirtyCount_) / (3.0f * frameCount_) * 100.0f) << "%" << std::endl;
    }
};

int main() {
    try {
        DirtyCheckingTest test;
        test.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
