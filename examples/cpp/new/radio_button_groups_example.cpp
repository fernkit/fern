#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace Fern;

// Global radio button groups
static std::shared_ptr<RadioButtonGroup> sizeGroup;
static std::shared_ptr<RadioButtonGroup> colorGroup;
static std::shared_ptr<RadioButtonGroup> qualityGroup;
static std::shared_ptr<RadioButtonGroup> difficultyGroup;

// Status text widgets
static std::shared_ptr<TextWidget> sizeStatus;
static std::shared_ptr<TextWidget> colorStatus;
static std::shared_ptr<TextWidget> qualityStatus;
static std::shared_ptr<TextWidget> difficultyStatus;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create main column layout
    std::vector<std::shared_ptr<Widget>> mainColumnChildren;
    
    // Title
    auto title = Text(Point(0, 0), "Radio Button Groups Example", 3, Colors::White);
    mainColumnChildren.push_back(title);
    mainColumnChildren.push_back(SizedBox(0, 40));
    
    // Create two-column layout for radio button groups
    std::vector<std::shared_ptr<Widget>> twoColumnChildren;
    
    // === LEFT COLUMN ===
    std::vector<std::shared_ptr<Widget>> leftColumnChildren;
    
    // Size Selection Group
    auto sizeTitle = Text(Point(0, 0), "Size Selection", 2, Colors::Cyan);
    leftColumnChildren.push_back(sizeTitle);
    leftColumnChildren.push_back(SizedBox(0, 15));
    
    sizeGroup = RadioGroup();
    
    // Create size radio buttons
    std::vector<std::string> sizes = {"Small", "Medium", "Large", "X-Large"};
    for (int i = 0; i < sizes.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, sizes[i], "size")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .spacing(8)), false);
        
        sizeGroup->addButton(button);
        leftColumnChildren.push_back(button);
        leftColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    if (!sizeGroup->getButtons().empty()) {
        sizeGroup->selectButton(sizeGroup->getButtons()[1]); // Medium
    }
    
    sizeStatus = Text(Point(0, 0), "Selected: Medium", 2, Colors::Yellow);
    leftColumnChildren.push_back(sizeStatus);
    leftColumnChildren.push_back(SizedBox(0, 30));
    
    // Color Selection Group
    auto colorTitle = Text(Point(0, 0), "Color Selection", 2, Colors::Cyan);
    leftColumnChildren.push_back(colorTitle);
    leftColumnChildren.push_back(SizedBox(0, 15));
    
    colorGroup = RadioGroup();
    
    // Create color radio buttons with different colors
    std::vector<std::pair<std::string, uint32_t>> colors = {
        {"Red", Colors::Red},
        {"Green", Colors::Green},
        {"Blue", Colors::Blue},
        {"Yellow", Colors::Yellow},
        {"Purple", Colors::Purple}
    };
    
    for (int i = 0; i < colors.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, colors[i].first, "color")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(colors[i].second)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .spacing(8)), false);
        
        colorGroup->addButton(button);
        leftColumnChildren.push_back(button);
        leftColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    if (!colorGroup->getButtons().empty()) {
        colorGroup->selectButton(colorGroup->getButtons()[0]); // Red
    }
    
    colorStatus = Text(Point(0, 0), "Selected: Red", 2, Colors::Yellow);
    leftColumnChildren.push_back(colorStatus);
    
    // Create left column container
    auto leftColumn = Padding(Column(leftColumnChildren), 30);
    
    // === RIGHT COLUMN ===
    std::vector<std::shared_ptr<Widget>> rightColumnChildren;
    
    // Quality Selection Group
    auto qualityTitle = Text(Point(0, 0), "Quality Settings", 2, Colors::Cyan);
    rightColumnChildren.push_back(qualityTitle);
    rightColumnChildren.push_back(SizedBox(0, 15));
    
    qualityGroup = RadioGroup();
    
    // Create quality radio buttons
    std::vector<std::string> qualities = {"Low", "Medium", "High", "Ultra", "Maximum"};
    for (int i = 0; i < qualities.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, qualities[i], "quality")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .spacing(8)), false);
        
        qualityGroup->addButton(button);
        rightColumnChildren.push_back(button);
        rightColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    if (!qualityGroup->getButtons().empty()) {
        qualityGroup->selectButton(qualityGroup->getButtons()[2]); // High
    }
    
    qualityStatus = Text(Point(0, 0), "Selected: High", 2, Colors::Yellow);
    rightColumnChildren.push_back(qualityStatus);
    rightColumnChildren.push_back(SizedBox(0, 30));
    
    // Difficulty Selection Group
    auto difficultyTitle = Text(Point(0, 0), "Difficulty Level", 2, Colors::Cyan);
    rightColumnChildren.push_back(difficultyTitle);
    rightColumnChildren.push_back(SizedBox(0, 15));
    
    difficultyGroup = RadioGroup();
    
    // Create difficulty radio buttons
    std::vector<std::string> difficulties = {"Easy", "Normal", "Hard", "Expert", "Nightmare"};
    for (int i = 0; i < difficulties.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, difficulties[i], "difficulty")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Orange)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .spacing(8)), false);
        
        difficultyGroup->addButton(button);
        rightColumnChildren.push_back(button);
        rightColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    if (!difficultyGroup->getButtons().empty()) {
        difficultyGroup->selectButton(difficultyGroup->getButtons()[1]); // Normal
    }
    
    difficultyStatus = Text(Point(0, 0), "Selected: Normal", 2, Colors::Yellow);
    rightColumnChildren.push_back(difficultyStatus);
    
    // Create right column container
    auto rightColumn = Padding(Column(rightColumnChildren), 30);
    
    // Add columns to two-column row
    twoColumnChildren.push_back(leftColumn);
    twoColumnChildren.push_back(SizedBox(60, 0)); // Space between columns
    twoColumnChildren.push_back(rightColumn);
    
    // Create two-column row
    auto twoColumnRow = Row(twoColumnChildren);
    mainColumnChildren.push_back(twoColumnRow);
    
    mainColumnChildren.push_back(SizedBox(0, 30));
    
    // Instructions
    auto instructions = Text(Point(0, 0), "Click radio buttons to select. Only one per group can be selected.", 2, Colors::LightGray);
    mainColumnChildren.push_back(instructions);
    
    // Create main container
    auto mainContainer = Center(Column(mainColumnChildren));
    
    // Add main container to widget manager
    addWidget(mainContainer);
    
    // Add all radio buttons to widget manager
    for (auto& button : sizeGroup->getButtons()) {
        addWidget(button);
    }
    for (auto& button : colorGroup->getButtons()) {
        addWidget(button);
    }
    for (auto& button : qualityGroup->getButtons()) {
        addWidget(button);
    }
    for (auto& button : difficultyGroup->getButtons()) {
        addWidget(button);
    }
    
    std::cout << "=== Radio Button Groups Example ===" << std::endl;
    std::cout << "1. Multiple radio button groups with mutual exclusion" << std::endl;
    std::cout << "2. Each group allows only one selection at a time" << std::endl;
    std::cout << "3. Clicking a button deselects others in the same group" << std::endl;
    std::cout << "4. Different styling for each group" << std::endl;
    std::cout << "5. Status indicators show current selections" << std::endl;
    std::cout << "=================================" << std::endl;
}

void draw() {
    // Dark background
    Draw::fill(0xFF1A1A1A);
    
    // Update status texts based on current selections
    if (sizeGroup && sizeStatus) {
        auto selectedButton = sizeGroup->getSelected();
        if (selectedButton) {
            sizeStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (colorGroup && colorStatus) {
        auto selectedButton = colorGroup->getSelected();
        if (selectedButton) {
            colorStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (qualityGroup && qualityStatus) {
        auto selectedButton = qualityGroup->getSelected();
        if (selectedButton) {
            qualityStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (difficultyGroup && difficultyStatus) {
        auto selectedButton = difficultyGroup->getSelected();
        if (selectedButton) {
            difficultyStatus->setText("Selected: " + selectedButton->getText());
        }
    }
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    
    // Start the render loop
    Fern::startRenderLoop();
    
    return 0;
}
