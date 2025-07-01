#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

static uint32_t pixels[600 * 800];
static int clickCount = 0;
static std::shared_ptr<TextWidget> counterText;

void setupUI() {
    Text(Point(50, 50), "BUTTON DEMO", 3, Colors::White);
    counterText = Text(Point(50, 400), "COUNT: 0", 2, Colors::White);
    
    ButtonConfig config = {
        .x = 300, .y = 250, .width = 200, .height = 50,
        .normalColor = Colors::Green,
        .hoverColor = Colors::LightGreen,
        .pressColor = Colors::DarkGreen,
        .label = "CLICK ME",
        .textScale = 2,
        .textColor = Colors::White
    };
    
    auto button = Button(config);
    button->onClick.connect([]() {
        clickCount++;
        counterText->setText("COUNT: " + std::to_string(clickCount));
        std::cout << "Clicked! Count: " << clickCount << std::endl;
    });
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}