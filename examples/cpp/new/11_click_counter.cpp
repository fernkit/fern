#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static int clickCount = 0;
static std::shared_ptr<TextWidget> counterDisplay;

void setupUI() {
    // Counter display
    counterDisplay = Text(Point(0, 0), "Clicks: 0", 3, Colors::White);
    
    // Click button
    auto button = Button(
        ButtonConfig(0, 0, 150, 50, "Click Me!")
            .style(ButtonStyle()
                .normalColor(Colors::Blue)
                .hoverColor(Colors::LightBlue)
                .pressColor(Colors::DarkBlue)
                .textColor(Colors::White)
                .textScale(2)
            )
    );
    button->onClick.connect([]() {
        clickCount++;
        counterDisplay->setText("Clicks: " + std::to_string(clickCount));
        std::cout << "Button clicked " << clickCount << " times!" << std::endl;
    });
    
    // Reset button
    auto resetButton = Button(
        ButtonConfig(0, 0, 100, 40, "Reset")
            .style(ButtonStyle()
                .normalColor(Colors::Red)
                .hoverColor(Colors::LightRed)
                .pressColor(Colors::DarkRed)
                .textColor(Colors::White)
                .textScale(2)
            )
    );
    resetButton->onClick.connect([]() {
        clickCount = 0;
        counterDisplay->setText("Clicks: 0");
        std::cout << "Counter reset!" << std::endl;
    });
    
    // Layout
    std::vector<std::shared_ptr<Widget>> children = {
        counterDisplay,
        SizedBox(0, 30),
        button,
        SizedBox(0, 20),
        resetButton
    };
    
    addWidget(Center(Column(children)));
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
