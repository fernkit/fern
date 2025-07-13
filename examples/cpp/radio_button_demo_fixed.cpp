#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

std::shared_ptr<RadioButtonGroup> themeGroup;
std::shared_ptr<RadioButtonWidget> lightTheme;
std::shared_ptr<RadioButtonWidget> darkTheme;
std::shared_ptr<RadioButtonWidget> autoTheme;

std::shared_ptr<RadioButtonGroup> qualityGroup;
std::shared_ptr<RadioButtonWidget> lowQuality;
std::shared_ptr<RadioButtonWidget> mediumQuality;
std::shared_ptr<RadioButtonWidget> highQuality;
std::shared_ptr<RadioButtonWidget> ultraQuality;

std::shared_ptr<RadioButtonGroup> sizeGroup;
std::shared_ptr<RadioButtonWidget> smallSize;
std::shared_ptr<RadioButtonWidget> mediumSize;
std::shared_ptr<RadioButtonWidget> largeSize;

void setupEventHandlers();

// Background color based on selected theme
uint32_t backgroundColor = 0xFF34495E;

void setupRadioButtons() {
    // Theme selection group
    themeGroup = std::make_shared<RadioButtonGroup>();

    lightTheme = RadioButton(
        RadioButtonConfig(50, 100, "Light Theme", "themeGroup")
            .selected(false)
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    lightTheme->setGroup(themeGroup);

    darkTheme = RadioButton(
        RadioButtonConfig(50, 140, "Dark Theme", "themeGroup")
            .selected(true)  // Default selection
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    darkTheme->setGroup(themeGroup);

    autoTheme = RadioButton(
        RadioButtonConfig(50, 180, "Auto Theme", "themeGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    autoTheme->setGroup(themeGroup);

    // Quality selection group
    qualityGroup = std::make_shared<RadioButtonGroup>();

    lowQuality = RadioButton(
        RadioButtonConfig(300, 100, "Low Quality", "qualityGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    lowQuality->setGroup(qualityGroup);

    mediumQuality = RadioButton(
        RadioButtonConfig(300, 140, "Medium Quality", "qualityGroup")
            .selected(true)  // Default selection
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    mediumQuality->setGroup(qualityGroup);

    highQuality = RadioButton(
        RadioButtonConfig(300, 180, "High Quality", "qualityGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    highQuality->setGroup(qualityGroup);

    ultraQuality = RadioButton(
        RadioButtonConfig(300, 220, "Ultra Quality", "qualityGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(12)
                .useBitmapFont()),
        true
    );
    ultraQuality->setGroup(qualityGroup);

    // Size selection group
    sizeGroup = std::make_shared<RadioButtonGroup>();

    smallSize = RadioButton(
        RadioButtonConfig(550, 100, "Small", "sizeGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Red)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(6)  // Smaller circle
                .spacing(10)
                .useBitmapFont()),
        true
    );
    smallSize->setGroup(sizeGroup);

    mediumSize = RadioButton(
        RadioButtonConfig(550, 140, "Medium", "sizeGroup")
            .selected(true)  // Default selection
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Red)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)  // Medium circle
                .spacing(12)
                .useBitmapFont()),
        true
    );
    mediumSize->setGroup(sizeGroup);

    largeSize = RadioButton(
        RadioButtonConfig(550, 180, "Large", "sizeGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Red)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(10)  // Larger circle
                .spacing(14)
                .useBitmapFont()),
        true
    );
    largeSize->setGroup(sizeGroup);

    setupEventHandlers();
}

void setupEventHandlers() {
    // Theme selection events
    themeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selectedButton) {
        if (selectedButton) {
            std::cout << "Theme changed to: " << selectedButton->getText() << std::endl;
            
            // Change background color based on theme
            std::string label = selectedButton->getText();
            if (label == "Light Theme") {
                backgroundColor = 0xFFECF0F1; // Light gray
            } else if (label == "Dark Theme") {
                backgroundColor = 0xFF2C3E50; // Dark blue-gray
            } else if (label == "Auto Theme") {
                backgroundColor = 0xFF9B59B6; // Purple
            }
        }
    });

    // Quality selection events
    qualityGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selectedButton) {
        if (selectedButton) {
            std::cout << "Quality changed to: " << selectedButton->getText() << std::endl;
        }
    });

    // Size selection events
    sizeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selectedButton) {
        if (selectedButton) {
            std::cout << "Size changed to: " << selectedButton->getText() << std::endl;
        }
    });

    // Individual radio button events
    lightTheme->onSelectionChanged.connect([](bool selected) {
        if (selected) {
            std::cout << "Light theme activated!" << std::endl;
        }
    });

    darkTheme->onSelectionChanged.connect([](bool selected) {
        if (selected) {
            std::cout << "Dark theme activated!" << std::endl;
        }
    });
}

void draw() {
    // Clear screen with theme-based background
    Draw::fill(backgroundColor);

    // Determine text color based on background
    uint32_t textColor = (backgroundColor == 0xFFECF0F1) ? Colors::Black : Colors::White;

    // Title
    Text(Point(50, 30), "RADIO BUTTON DEMO", 3, textColor, true);

    // Section headers
    Text(Point(50, 70), "Theme Selection:", 2, Colors::Blue, true);
    Text(Point(300, 70), "Quality Settings:", 2, Colors::Green, true);
    Text(Point(550, 70), "Size Options:", 2, Colors::Red, true);

    // Current selections display
    auto selectedTheme = themeGroup->getSelected();
    auto selectedQuality = qualityGroup->getSelected();
    auto selectedSize = sizeGroup->getSelected();

    Text(Point(50, 280), "Current Selections:", 2, Colors::Yellow, true);
    
    char buffer[128];
    sprintf(buffer, "Theme: %s", selectedTheme ? selectedTheme->getText().c_str() : "None");
    Text(Point(50, 310), buffer, 1, textColor, true);
    
    sprintf(buffer, "Quality: %s", selectedQuality ? selectedQuality->getText().c_str() : "None");
    Text(Point(50, 330), buffer, 1, textColor, true);
    
    sprintf(buffer, "Size: %s", selectedSize ? selectedSize->getText().c_str() : "None");
    Text(Point(50, 350), buffer, 1, textColor, true);

    // Instructions
    Text(Point(50, 400), "Click radio buttons to change selections", 1, Colors::LightGray, true);
    Text(Point(50, 420), "Notice how the background changes with theme selection", 1, Colors::LightGray, true);
}

int main() {
    Fern::initialize();
    setupRadioButtons();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
