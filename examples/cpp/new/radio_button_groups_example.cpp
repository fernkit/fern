#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace Fern;

// Radio button group management class
class RadioButtonGroup {
private:
    std::vector<std::shared_ptr<RadioButtonWidget>> buttons_;
    std::string name_;
    int selectedIndex_;
    
public:
    RadioButtonGroup(const std::string& name) : name_(name), selectedIndex_(-1) {}
    
    void addButton(std::shared_ptr<RadioButtonWidget> button) {
        buttons_.push_back(button);
        int index = buttons_.size() - 1;
        
        // Connect to button's selection event
        button->onSelectionChanged.connect([this, index](bool selected) {
            if (selected) {
                selectButton(index);
            }
        });
    }
    
    void selectButton(int index) {
        if (index >= 0 && index < buttons_.size()) {
            // Deselect all other buttons
            for (int i = 0; i < buttons_.size(); i++) {
                if (i != index) {
                    buttons_[i]->setSelected(false);
                }
            }
            
            // Select the clicked button
            buttons_[index]->setSelected(true);
            selectedIndex_ = index;
            
            std::cout << "Group '" << name_ << "': Button " << index << " selected" << std::endl;
        }
    }
    
    int getSelectedIndex() const { return selectedIndex_; }
    
    std::shared_ptr<RadioButtonWidget> getSelectedButton() const {
        if (selectedIndex_ >= 0 && selectedIndex_ < buttons_.size()) {
            return buttons_[selectedIndex_];
        }
        return nullptr;
    }
    
    void setSelectedIndex(int index) {
        selectButton(index);
    }
};

// Global radio button groups
static std::unique_ptr<RadioButtonGroup> sizeGroup;
static std::unique_ptr<RadioButtonGroup> colorGroup;
static std::unique_ptr<RadioButtonGroup> qualityGroup;
static std::unique_ptr<RadioButtonGroup> difficultyGroup;

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
    
    sizeGroup = std::make_unique<RadioButtonGroup>("Size");
    
    // Create size radio buttons
    std::vector<std::string> sizes = {"Small", "Medium", "Large", "X-Large"};
    for (int i = 0; i < sizes.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, 150, 25)
            .text(sizes[i])
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .padding(8)), false);
        
        sizeGroup->addButton(button);
        leftColumnChildren.push_back(button);
        leftColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    sizeGroup->setSelectedIndex(1); // Medium
    
    sizeStatus = Text(Point(0, 0), "Selected: Medium", 2, Colors::Yellow);
    leftColumnChildren.push_back(sizeStatus);
    leftColumnChildren.push_back(SizedBox(0, 30));
    
    // Color Selection Group
    auto colorTitle = Text(Point(0, 0), "Color Selection", 2, Colors::Cyan);
    leftColumnChildren.push_back(colorTitle);
    leftColumnChildren.push_back(SizedBox(0, 15));
    
    colorGroup = std::make_unique<RadioButtonGroup>("Color");
    
    // Create color radio buttons with different colors
    std::vector<std::pair<std::string, uint32_t>> colors = {
        {"Red", Colors::Red},
        {"Green", Colors::Green},
        {"Blue", Colors::Blue},
        {"Yellow", Colors::Yellow},
        {"Purple", Colors::Purple}
    };
    
    for (int i = 0; i < colors.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, 150, 25)
            .text(colors[i].first)
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(colors[i].second)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .padding(8)), false);
        
        colorGroup->addButton(button);
        leftColumnChildren.push_back(button);
        leftColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    colorGroup->setSelectedIndex(0); // Red
    
    colorStatus = Text(Point(0, 0), "Selected: Red", 2, Colors::Yellow);
    leftColumnChildren.push_back(colorStatus);
    
    // Create left column container
    auto leftColumn = std::make_shared<PaddingWidget>(0, 0, 300, 600, 30, 30, 30, 30);
    leftColumn->add(Column(leftColumnChildren));
    
    // === RIGHT COLUMN ===
    std::vector<std::shared_ptr<Widget>> rightColumnChildren;
    
    // Quality Selection Group
    auto qualityTitle = Text(Point(0, 0), "Quality Settings", 2, Colors::Cyan);
    rightColumnChildren.push_back(qualityTitle);
    rightColumnChildren.push_back(SizedBox(0, 15));
    
    qualityGroup = std::make_unique<RadioButtonGroup>("Quality");
    
    // Create quality radio buttons
    std::vector<std::string> qualities = {"Low", "Medium", "High", "Ultra", "Maximum"};
    for (int i = 0; i < qualities.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, 150, 25)
            .text(qualities[i])
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Green)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .padding(8)), false);
        
        qualityGroup->addButton(button);
        rightColumnChildren.push_back(button);
        rightColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    qualityGroup->setSelectedIndex(2); // High
    
    qualityStatus = Text(Point(0, 0), "Selected: High", 2, Colors::Yellow);
    rightColumnChildren.push_back(qualityStatus);
    rightColumnChildren.push_back(SizedBox(0, 30));
    
    // Difficulty Selection Group
    auto difficultyTitle = Text(Point(0, 0), "Difficulty Level", 2, Colors::Cyan);
    rightColumnChildren.push_back(difficultyTitle);
    rightColumnChildren.push_back(SizedBox(0, 15));
    
    difficultyGroup = std::make_unique<RadioButtonGroup>("Difficulty");
    
    // Create difficulty radio buttons
    std::vector<std::string> difficulties = {"Easy", "Normal", "Hard", "Expert", "Nightmare"};
    for (int i = 0; i < difficulties.size(); i++) {
        auto button = RadioButton(RadioButtonConfig(0, 0, 150, 25)
            .text(difficulties[i])
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Orange)
                .textColor(Colors::Black)
                .borderColor(Colors::Gray)
                .hoverColor(Colors::LightGray)
                .fontSize(2)
                .padding(8)), false);
        
        difficultyGroup->addButton(button);
        rightColumnChildren.push_back(button);
        rightColumnChildren.push_back(SizedBox(0, 8));
    }
    
    // Set default selection
    difficultyGroup->setSelectedIndex(1); // Normal
    
    difficultyStatus = Text(Point(0, 0), "Selected: Normal", 2, Colors::Yellow);
    rightColumnChildren.push_back(difficultyStatus);
    
    // Create right column container
    auto rightColumn = std::make_shared<PaddingWidget>(0, 0, 300, 600, 30, 30, 30, 30);
    rightColumn->add(Column(rightColumnChildren));
    
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
    auto mainContainer = std::make_shared<CenterWidget>(0, 0, width, height);
    mainContainer->add(Column(mainColumnChildren));
    
    // Add main container to widget manager
    addWidget(mainContainer);
    
    // Add all radio buttons to widget manager
    for (auto& button : sizeGroup->buttons_) {
        addWidget(button);
    }
    for (auto& button : colorGroup->buttons_) {
        addWidget(button);
    }
    for (auto& button : qualityGroup->buttons_) {
        addWidget(button);
    }
    for (auto& button : difficultyGroup->buttons_) {
        addWidget(button);
    }
    
    // Set up status update callbacks
    // We need to access the private buttons_ member, so let's modify the approach
    
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
        auto selectedButton = sizeGroup->getSelectedButton();
        if (selectedButton) {
            sizeStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (colorGroup && colorStatus) {
        auto selectedButton = colorGroup->getSelectedButton();
        if (selectedButton) {
            colorStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (qualityGroup && qualityStatus) {
        auto selectedButton = qualityGroup->getSelectedButton();
        if (selectedButton) {
            qualityStatus->setText("Selected: " + selectedButton->getText());
        }
    }
    
    if (difficultyGroup && difficultyStatus) {
        auto selectedButton = difficultyGroup->getSelectedButton();
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
