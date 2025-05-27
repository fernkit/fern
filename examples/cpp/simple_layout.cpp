#include <fern/fern.hpp>
#include <fern/ui/layout.hpp>
#include <memory>
#include <string>
#include <iostream>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();

    ButtonConfig config = {
            .width = width/2,
            .height = 40,
            .normalColor = Colors::Green,
            .hoverColor = Colors::LightGreen,
            .pressColor = Colors::DarkGreen,
            .label = "CLICK ME",
            .textScale = 2,
            .textColor = Colors::White,
            .onClick = []() {
                std::cout << "Button clicked!" << std::endl;
            }
    };
    
    auto mainLayout = Column({
        Text(Point(0, 0), "COLUMN LAYOUT EXAMPLE", 3, Colors::White, false),
        Text(Point(0, 0), "WIDGETS ARE STACKED VERTICALLY", 2, Colors::LightGray, false),
        Circle(height/8, Point(0, 0), Colors::Blue, false),
        Button(config, false),
        
        Text(Point(0, 0), "SCREEN SIZE: " + 
             std::to_string(width) + " X " + std::to_string(height), 
             2, Colors::Yellow, false)
    });
}

void draw() {
    Draw::fill(Colors::DarkGray);
    
    static int lastWidth = 0;
    static int lastHeight = 0;
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    if (width != lastWidth || height != lastHeight) {
        WidgetManager::getInstance().clear();
        setupUI();
        
        lastWidth = width;
        lastHeight = height;
    }
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}