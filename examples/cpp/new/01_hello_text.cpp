#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Test the original hello text with proper centering
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Text(Point(0, 0), "Hello, Fern!", 3, Colors::White));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
