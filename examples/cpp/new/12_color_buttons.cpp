#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> statusText;

void setupUI() {
    // Status display
    statusText = Text(Point(0, 0), "Select a color:", 2, Colors::White);
    
    // Color buttons
    auto redButton = Button(
        ButtonConfig(0, 0, 80, 80, "")
            .style(ButtonStyle()
                .normalColor(Colors::Red)
                .hoverColor(Colors::LightRed)
                .pressColor(Colors::DarkRed)
                .textColor(Colors::White)
                .textScale(1)
            )
    );
    redButton->onClick.connect([]() {
        statusText->setText("Red selected!");
        std::cout << "Red color selected!" << std::endl;
    });
    
    auto greenButton = Button(
        ButtonConfig(0, 0, 80, 80, "")
            .style(ButtonStyle()
                .normalColor(Colors::Green)
                .hoverColor(Colors::LightGreen)
                .pressColor(Colors::DarkGreen)
                .textColor(Colors::White)
                .textScale(1)
            )
    );
    greenButton->onClick.connect([]() {
        statusText->setText("Green selected!");
        std::cout << "Green color selected!" << std::endl;
    });
    
    auto blueButton = Button(
        ButtonConfig(0, 0, 80, 80, "")
            .style(ButtonStyle()
                .normalColor(Colors::Blue)
                .hoverColor(Colors::LightBlue)
                .pressColor(Colors::DarkBlue)
                .textColor(Colors::White)
                .textScale(1)
            )
    );
    blueButton->onClick.connect([]() {
        statusText->setText("Blue selected!");
        std::cout << "Blue color selected!" << std::endl;
    });
    
    auto yellowButton = Button(
        ButtonConfig(0, 0, 80, 80, "")
            .style(ButtonStyle()
                .normalColor(Colors::Yellow)
                .hoverColor(Colors::Amber)
                .pressColor(Colors::DarkGray)
                .textColor(Colors::Black)
                .textScale(1)
            )
    );
    yellowButton->onClick.connect([]() {
        statusText->setText("Yellow selected!");
        std::cout << "Yellow color selected!" << std::endl;
    });
    
    // Layout
    std::vector<std::shared_ptr<Widget>> colorRow = {
        redButton,
        SizedBox(15, 0),
        greenButton,
        SizedBox(15, 0),
        blueButton,
        SizedBox(15, 0),
        yellowButton
    };
    
    std::vector<std::shared_ptr<Widget>> mainColumn = {
        Text(Point(0, 0), "Color Picker", 3, Colors::White),
        SizedBox(0, 30),
        statusText,
        SizedBox(0, 40),
        Row(colorRow)
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
