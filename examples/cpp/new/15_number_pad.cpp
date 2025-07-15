#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> displayText;

void setupUI() {
    // Display
    displayText = Text(Point(0, 0), "0", 4, Colors::White);
    
    // Number pad buttons
    std::vector<std::shared_ptr<Widget>> buttonRows;
    
    // Row 1: 7, 8, 9
    std::vector<std::shared_ptr<Widget>> row1;
    for (int i = 7; i <= 9; i++) {
        ButtonStyle style;
        style.normalColor(Colors::LightGray)
            .hoverColor(Colors::White)
            .pressColor(Colors::Gray)
            .textScale(2)
            .textColor(Colors::Black);
        
        auto button = Button(ButtonConfig(0, 0, 60, 60, std::to_string(i)).style(style));
        
        button->onClick.connect([i]() {
            std::string current = displayText->getText();
            if (current == "0") {
                displayText->setText(std::to_string(i));
            } else {
                displayText->setText(current + std::to_string(i));
            }
        });
        
        row1.push_back(button);
        if (i < 9) row1.push_back(SizedBox(10, 0));
    }
    
    // Row 2: 4, 5, 6
    std::vector<std::shared_ptr<Widget>> row2;
    for (int i = 4; i <= 6; i++) {
        ButtonStyle style;
        style.normalColor(Colors::LightGray)
            .hoverColor(Colors::White)
            .pressColor(Colors::Gray)
            .textScale(2)
            .textColor(Colors::Black);
        
        auto button = Button(ButtonConfig(0, 0, 60, 60, std::to_string(i)).style(style));
        
        button->onClick.connect([i]() {
            std::string current = displayText->getText();
            if (current == "0") {
                displayText->setText(std::to_string(i));
            } else {
                displayText->setText(current + std::to_string(i));
            }
        });
        
        row2.push_back(button);
        if (i < 6) row2.push_back(SizedBox(10, 0));
    }
    
    // Row 3: 1, 2, 3
    std::vector<std::shared_ptr<Widget>> row3;
    for (int i = 1; i <= 3; i++) {
        ButtonStyle style;
        style.normalColor(Colors::LightGray)
            .hoverColor(Colors::White)
            .pressColor(Colors::Gray)
            .textScale(2)
            .textColor(Colors::Black);
        
        auto button = Button(ButtonConfig(0, 0, 60, 60, std::to_string(i)).style(style));
        
        button->onClick.connect([i]() {
            std::string current = displayText->getText();
            if (current == "0") {
                displayText->setText(std::to_string(i));
            } else {
                displayText->setText(current + std::to_string(i));
            }
        });
        
        row3.push_back(button);
        if (i < 3) row3.push_back(SizedBox(10, 0));
    }
    
    // Row 4: 0, Clear
    ButtonStyle zeroStyle;
    zeroStyle.normalColor(Colors::LightGray)
        .hoverColor(Colors::White)
        .pressColor(Colors::Gray)
        .textScale(2)
        .textColor(Colors::Black);
    
    auto zeroButton = Button(ButtonConfig(0, 0, 60, 60, "0").style(zeroStyle));
    zeroButton->onClick.connect([]() {
        std::string current = displayText->getText();
        if (current != "0") {
            displayText->setText(current + "0");
        }
    });
    
    ButtonStyle clearStyle;
    clearStyle.normalColor(Colors::Red)
        .hoverColor(Colors::LightRed)
        .pressColor(Colors::DarkRed)
        .textScale(2)
        .textColor(Colors::White);
    
    auto clearButton = Button(ButtonConfig(0, 0, 130, 60, "Clear").style(clearStyle));
    clearButton->onClick.connect([]() {
        displayText->setText("0");
        std::cout << "Display cleared!" << std::endl;
    });
    
    std::vector<std::shared_ptr<Widget>> row4 = {
        zeroButton,
        SizedBox(10, 0),
        clearButton
    };
    
    // Main layout
    std::vector<std::shared_ptr<Widget>> keypadColumn = {
        Row(row1),
        SizedBox(0, 10),
        Row(row2),
        SizedBox(0, 10),
        Row(row3),
        SizedBox(0, 10),
        Row(row4)
    };
    
    std::vector<std::shared_ptr<Widget>> mainColumn = {
        Text(Point(0, 0), "Number Pad", 3, Colors::White),
        SizedBox(0, 20),
        displayText,
        SizedBox(0, 30),
        Column(keypadColumn)
    };
    
    addWidget(Center(Column(mainColumn)));
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
