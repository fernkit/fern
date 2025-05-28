#include <fern/fern.hpp>
using namespace Fern;

void setupUI() {
    // Clear any previous widgets
    WidgetManager::getInstance().clear();
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    addWidget(
        Container(
            0xFF1E1E1E,        
            0, 0, width, height,
            
            Column({
             Column({
                Text(Point(0, 0), "END TEXT", 2, Colors::White, false),
                Container(Colors::Red, 0, 0, 100, 40),
                Container(Colors::Green, 0, 0, 100, 40),
                Container(Colors::Blue, 0, 0, 100, 40)
            }, false, MainAxisAlignment::Center, CrossAxisAlignment::End),

            Row({
                Text(Point(0, 0), "SPACED TEXT", 2, Colors::White, false),
                Container(Colors::Red, 0, 0, 100, 40),
                Container(Colors::Green, 0, 0, 100, 40),
                Container(Colors::Blue, 0, 0, 100, 40)
            }, false, MainAxisAlignment::SpaceBetween)

            }, false)
        )
    );
}

void draw() {
    Draw::fill(0xFF121212);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}