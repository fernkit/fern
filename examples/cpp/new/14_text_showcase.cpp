#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Title
    auto title = Text(Point(0, 0), "Text Styles", 4, Colors::White);
    
    // Different text examples
    auto smallText = Text(Point(0, 0), "Small Text (size 1)", 1, Colors::LightGray);
    auto normalText = Text(Point(0, 0), "Normal Text (size 2)", 2, Colors::White);
    auto largeText = Text(Point(0, 0), "Large Text (size 3)", 3, Colors::Yellow);
    auto extraLargeText = Text(Point(0, 0), "Extra Large (size 4)", 4, Colors::Orange);
    
    // Colored text examples
    auto redText = Text(Point(0, 0), "Red Text", 2, Colors::Red);
    auto greenText = Text(Point(0, 0), "Green Text", 2, Colors::Green);
    auto blueText = Text(Point(0, 0), "Blue Text", 2, Colors::Blue);
    auto cyanText = Text(Point(0, 0), "Cyan Text", 2, Colors::Cyan);
    auto magentaText = Text(Point(0, 0), "Magenta Text", 2, Colors::Magenta);
    
    // Layout
    std::vector<std::shared_ptr<Widget>> sizeSection = {
        Text(Point(0, 0), "Size Examples:", 2, Colors::LightBlue),
        SizedBox(0, 30),
        smallText,
        SizedBox(0, 30),
        normalText,
        SizedBox(0, 30),
        largeText,
        SizedBox(0, 30),
        extraLargeText
    };
    
    std::vector<std::shared_ptr<Widget>> colorSection = {
        Text(Point(0, 0), "Color Examples:", 2, Colors::LightBlue),
        SizedBox(0, 35),
        redText,
        SizedBox(0, 35),
        greenText,
        SizedBox(0, 35),
        blueText,
        SizedBox(0, 35),
        cyanText,
        SizedBox(0, 35),
        magentaText
    };
    
    std::vector<std::shared_ptr<Widget>> mainColumn = {
        title,
        SizedBox(0, 30),
        Column(sizeSection),
        SizedBox(0, 40),
        Column(colorSection)
    };
    
    addWidget(Center(Column(mainColumn)));
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
