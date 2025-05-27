#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

static uint32_t pixels[600 * 800];
static std::shared_ptr<CircleWidget> mainCircle;
static std::shared_ptr<LineWidget> mainLine;
static std::shared_ptr<TextWidget> statusText;

void setupUI() {
    Text(Point(50, 50), "INTERACTIVE SHAPES DEMO", 3, Colors::White);
    statusText = Text(Point(50, 500), "INTERACT WITH SHAPES", 2, Colors::LightGray);
    mainCircle = Circle(50, Point(200, 300), Colors::Blue);
    mainCircle->onHover.connect([](bool isHovered) {
        if (isHovered) {
            mainCircle->setColor(Colors::LightBlue);
            statusText->setText("MOUSE OVER CIRCLE");
        } else {
            mainCircle->setColor(Colors::Blue);
            statusText->setText("INTERACT WITH SHAPES");
        }
    });
    
    mainCircle->onClick.connect([]() {
        statusText->setText("CIRCLE CLICKED");
        mainCircle->setRadius(mainCircle->getRadius() + 10);
        if (mainCircle->getRadius() > 100) {
            mainCircle->setRadius(50);
        }
    });
    
    mainLine = Line(Point(400, 250), Point(600, 350), 5, Colors::Green);
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize(pixels, 800, 600);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}

