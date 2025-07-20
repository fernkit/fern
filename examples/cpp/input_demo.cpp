#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

// Simple input demo colors
namespace InputColors {
    const uint32_t Background = 0xFF2C3E50;     // Dark blue background
    const uint32_t InputBg = 0xFFFFFFFF;        // White input background
    const uint32_t InputBorder = 0xFF3498DB;    // Blue border
    const uint32_t Text = 0xFF2C3E50;           // Dark text
    const uint32_t LabelText = 0xFFFFFFFF;      // White labels
}

static std::shared_ptr<TextInputWidget> nameInput;
static std::shared_ptr<TextInputWidget> emailInput;
static std::shared_ptr<TextWidget> displayText;
static std::shared_ptr<CenterWidget> centerWidget;

void updateDisplay() {
    std::string name = nameInput->getText();
    std::string email = emailInput->getText();
    
    std::string displayString = "Hello ";
    if (!name.empty()) {
        displayString += name;
    } else {
        displayString += "Guest";
    }
    
    if (!email.empty()) {
        displayString += " (" + email + ")";
    }
    
    displayString += "!";
    displayText->setText(displayString);
}

void setupUI() {
    // Title
    auto titleText = Text(Point(0, 0), "Simple Input Demo", 4, InputColors::LabelText);
    
    // Name input
    auto nameLabel = Text(Point(0, 0), "Name:", 2, InputColors::LabelText);
    
    TextInputStyle inputStyle;
    inputStyle.backgroundColor(InputColors::InputBg)
             .textColor(InputColors::Text)
             .borderColor(InputColors::InputBorder)
             .focusBorderColor(0xFF2980B9)
             .borderWidth(2)
             .fontSize(2)
             .padding(8);
    
    nameInput = TextInput(TextInputConfig(0, 0, 250, 35)
        .placeholder("Enter your name")
        .style(inputStyle));
    
    nameInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "Name changed to: " << text << std::endl;
        updateDisplay();
    });
    
    // Email input
    auto emailLabel = Text(Point(0, 0), "Email:", 2, InputColors::LabelText);
    
    emailInput = TextInput(TextInputConfig(0, 0, 250, 35)
        .placeholder("Enter your email")
        .style(inputStyle));
    
    emailInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "Email changed to: " << text << std::endl;
        updateDisplay();
    });
    
    // Display text
    displayText = Text(Point(0, 0), "Hello Guest!", 3, InputColors::LabelText);
    
    // Focus button
    ButtonStyle buttonStyle;
    buttonStyle.normalColor(InputColors::InputBorder)
              .hoverColor(0xFF2980B9)
              .pressColor(0xFF1F618D)
              .textColor(0xFFFFFFFF)
              .textScale(2)
              .borderRadius(6);
    
    auto focusNameBtn = Button(ButtonConfig(0, 0, 120, 35, "Focus Name").style(buttonStyle));
    focusNameBtn->onClick.connect([]() {
        nameInput->setFocus(true);
        std::cout << "Focused name input" << std::endl;
    });
    
    auto focusEmailBtn = Button(ButtonConfig(0, 0, 120, 35, "Focus Email").style(buttonStyle));
    focusEmailBtn->onClick.connect([]() {
        emailInput->setFocus(true);
        std::cout << "Focused email input" << std::endl;
    });
    
    auto clearBtn = Button(ButtonConfig(0, 0, 80, 35, "Clear").style(buttonStyle));
    clearBtn->onClick.connect([]() {
        nameInput->setText("");
        emailInput->setText("");
        updateDisplay();
    });
    
    // Layout
    std::vector<std::shared_ptr<Widget>> nameRow = {nameLabel, SizedBox(20, 0), nameInput};
    std::vector<std::shared_ptr<Widget>> emailRow = {emailLabel, SizedBox(20, 0), emailInput};
    std::vector<std::shared_ptr<Widget>> buttonRow = {focusNameBtn, SizedBox(10, 0), focusEmailBtn, SizedBox(10, 0), clearBtn};
    
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        titleText,
        SizedBox(0, 30),
        Row(nameRow),
        SizedBox(0, 20),
        Row(emailRow),
        SizedBox(0, 30),
        displayText,
        SizedBox(0, 30),
        Row(buttonRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    addWidget(centerWidget);
    
    // Set initial focus to name input
    nameInput->setFocus(true);
    updateDisplay();
}

void onWindowResize(int newWidth, int newHeight) {
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(InputColors::Background);
}

int main() {
    Fern::initialize(600, 400);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::setWindowResizeCallback(onWindowResize);
    Fern::startRenderLoop();
    return 0;
}
