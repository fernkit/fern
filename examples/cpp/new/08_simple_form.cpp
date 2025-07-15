#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    auto submitBtn = Button(
        ButtonPresets::Success(0, 0, 100, 40, "Submit")
    );
    
    auto cancelBtn = Button(
        ButtonPresets::Secondary(0, 0, 100, 40, "Cancel")
    );
    
    submitBtn->onClick.connect([]() {
        std::cout << "Form submitted!" << std::endl;
    });
    
    cancelBtn->onClick.connect([]() {
        std::cout << "Form cancelled!" << std::endl;
    });
    
    addWidget(
        Center(
            Column({
                Text(Point(0, 0), "Simple Form", 3, Colors::White),
                SizedBox(0, 30),
                Text(Point(0, 0), "Name: [Input Field]", 2, Colors::Gray),
                SizedBox(0, 20),
                Text(Point(0, 0), "Email: [Input Field]", 2, Colors::Gray),
                SizedBox(0, 30),
                Row({
                    submitBtn,
                    SizedBox(20, 0),
                    cancelBtn
                })
            })
        )
    );
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
