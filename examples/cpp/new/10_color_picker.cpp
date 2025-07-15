#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> messageText;

void setupUI() {
    messageText = Text(Point(0, 0), "Click a color!", 2, Colors::White);
    
    auto redBtn = Button(
        ButtonConfig(0, 0, 60, 60, "")
            .style(ButtonStyle().normalColor(Colors::Red))
    );
    
    auto greenBtn = Button(
        ButtonConfig(0, 0, 60, 60, "")
            .style(ButtonStyle().normalColor(Colors::Green))
    );
    
    auto blueBtn = Button(
        ButtonConfig(0, 0, 60, 60, "")
            .style(ButtonStyle().normalColor(Colors::Blue))
    );
    
    auto yellowBtn = Button(
        ButtonConfig(0, 0, 60, 60, "")
            .style(ButtonStyle().normalColor(Colors::Yellow))
    );
    
    redBtn->onClick.connect([]() {
        messageText->setText("Red selected!");
        std::cout << "Red selected!" << std::endl;
    });
    
    greenBtn->onClick.connect([]() {
        messageText->setText("Green selected!");
        std::cout << "Green selected!" << std::endl;
    });
    
    blueBtn->onClick.connect([]() {
        messageText->setText("Blue selected!");
        std::cout << "Blue selected!" << std::endl;
    });
    
    yellowBtn->onClick.connect([]() {
        messageText->setText("Yellow selected!");
        std::cout << "Yellow selected!" << std::endl;
    });
    
    addWidget(
        Center(
            Column({
                Text(Point(0, 0), "Color Picker", 3, Colors::White),
                SizedBox(0, 20),
                messageText,
                SizedBox(0, 30),
                Row({
                    redBtn,
                    SizedBox(10, 0),
                    greenBtn,
                    SizedBox(10, 0),
                    blueBtn,
                    SizedBox(10, 0),
                    yellowBtn
                })
            })
        )
    );
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
