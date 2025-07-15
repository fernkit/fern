#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create containers with different colors
    auto redContainer = Container(
        Colors::Red,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Red\nContainer", 2, Colors::White))
    );
    
    auto greenContainer = Container(
        Colors::Green,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Green\nContainer", 2, Colors::White))
    );
    
    auto blueContainer = Container(
        Colors::Blue,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Blue\nContainer", 2, Colors::White))
    );
    
    auto yellowContainer = Container(
        Colors::Yellow,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Yellow\nContainer", 2, Colors::Black))
    );
    
    auto purpleContainer = Container(
        Colors::Purple,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Purple\nContainer", 2, Colors::White))
    );
    
    auto orangeContainer = Container(
        Colors::Orange,
        0, 0, 0, 0,
        Center(Text(Point(0, 0), "Orange\nContainer", 2, Colors::White))
    );
    
    // Create a grid layout
    std::vector<std::shared_ptr<Widget>> topRow = {
        Expanded(redContainer, 1),
        Expanded(greenContainer, 1),
        Expanded(blueContainer, 1)
    };
    
    std::vector<std::shared_ptr<Widget>> bottomRow = {
        Expanded(yellowContainer, 1),
        Expanded(purpleContainer, 1),
        Expanded(orangeContainer, 1)
    };
    
    std::vector<std::shared_ptr<Widget>> mainColumn = {
        Container(
            Colors::DarkGray,
            0, 0, 0, 60,
            Center(Text(Point(0, 0), "Container Grid", 3, Colors::White))
        ),
        Expanded(Row(topRow), 1),
        Expanded(Row(bottomRow), 1),
        Container(
            Colors::DarkGray,
            0, 0, 0, 40,
            Center(Text(Point(0, 0), "Footer", 2, Colors::White))
        )
    };
    
    addWidget(
        Container(
            Colors::Black,
            0, 0, width, height,
            Column(mainColumn)
        )
    );
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
