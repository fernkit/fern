#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    auto primaryBtn = Button(
        ButtonPresets::Primary(0, 0, 120, 40, "Primary")
    );
    
    auto successBtn = Button(
        ButtonPresets::Success(0, 0, 120, 40, "Success")
    );
    
    auto dangerBtn = Button(
        ButtonPresets::Danger(0, 0, 120, 40, "Danger")
    );
    
    auto warningBtn = Button(
        ButtonPresets::Warning(0, 0, 120, 40, "Warning")
    );
    
    primaryBtn->onClick.connect([]() {
        std::cout << "Primary button clicked!" << std::endl;
    });
    
    successBtn->onClick.connect([]() {
        std::cout << "Success button clicked!" << std::endl;
    });
    
    dangerBtn->onClick.connect([]() {
        std::cout << "Danger button clicked!" << std::endl;
    });
    
    warningBtn->onClick.connect([]() {
        std::cout << "Warning button clicked!" << std::endl;
    });
    
    addWidget(
        Center(
            Column({
                primaryBtn,
                SizedBox(0, 10),
                successBtn,
                SizedBox(0, 10),
                dangerBtn,
                SizedBox(0, 10),
                warningBtn
            })
        )
    );
}

void draw() {
    Draw::fill(Colors::LightGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
