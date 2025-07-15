#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

void draw() {
    Draw::fill(Colors::DarkBlue);
}

static int clickCount = 0;

int main() {
    std::cout << "Testing layout button counter demo..." << std::endl;
    
    Fern::initialize();
    
    // Get actual screen dimensions
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create text widget for counter
    auto counterText = Text(Point(0,0), "Count: 0", 3, Colors::White, false, Fern::FontType::Bitmap);
    
    // Create button
    auto incrementButton = Button(ButtonConfig{0, 0, 200, 60, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Click Me!", 2, Colors::White});
    
    // Connect button event to update counter
    incrementButton->onClick.connect([counterText](){
        clickCount++;
        counterText->setText("Count: " + std::to_string(clickCount));
        std::cout << "Button clicked! Count: " << clickCount << std::endl;
    });
    
    // Create simple vertical layout
    auto mainColumn = Column({
        counterText,
        SizedBox(0, 30, false),  // Spacing
        incrementButton
    }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
    
    // Add to widget manager in a container
    addWidget(
        Container(
            Colors::Transparent,
            0, 0, width, height,
            Center(mainColumn, false)
        )
    );
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}