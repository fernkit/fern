#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> statusText;

void setupUI() {
    statusText = Text(Point(0, 0), "Ready", 2, Colors::Green);
    
    auto btn1 = Button(
        ButtonPresets::Primary(0, 0, 80, 35, "Option 1")
    );
    
    auto btn2 = Button(
        ButtonPresets::Info(0, 0, 80, 35, "Option 2")
    );
    
    auto btn3 = Button(
        ButtonPresets::Warning(0, 0, 80, 35, "Option 3")
    );
    
    btn1->onClick.connect([]() {
        statusText->setText("Option 1 selected");
        std::cout << "Option 1 selected" << std::endl;
    });
    
    btn2->onClick.connect([]() {
        statusText->setText("Option 2 selected");
        std::cout << "Option 2 selected" << std::endl;
    });
    
    btn3->onClick.connect([]() {
        statusText->setText("Option 3 selected");
        std::cout << "Option 3 selected" << std::endl;
    });
    
    addWidget(
        Center(
            Column({
                Text(Point(0, 0), "Choose an option:", 3, Colors::White),
                SizedBox(0, 20),
                statusText,
                SizedBox(0, 30),
                Row({
                    btn1,
                    SizedBox(10, 0),
                    btn2,
                    SizedBox(10, 0),
                    btn3
                })
            })
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
