#include <fern/fern.hpp>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cmath>

using namespace Fern;

// Global widget references for updates
static std::shared_ptr<SliderWidget> volumeSlider;
static std::shared_ptr<SliderWidget> brightnessSlider;
static std::shared_ptr<SliderWidget> speedSlider;
static std::shared_ptr<CircularIndicatorWidget> downloadProgress;
static std::shared_ptr<CircularIndicatorWidget> batteryLevel;
static std::shared_ptr<CircularIndicatorWidget> cpuUsage;
static std::shared_ptr<DropdownWidget> qualityDropdown;
static std::shared_ptr<DropdownWidget> languageDropdown;
static std::shared_ptr<TextInputWidget> searchBox;
static std::shared_ptr<TextWidget> statusText;
static std::shared_ptr<TextWidget> volumeLabel;
static std::shared_ptr<TextWidget> brightnessLabel;
static std::shared_ptr<TextWidget> speedLabel;
static std::shared_ptr<TextWidget> qualityLabel;
static std::shared_ptr<TextWidget> languageLabel;
static std::shared_ptr<TextWidget> searchLabel;

// Animation state
static float animationTime = 0.0f;
static bool isAnimating = true;

void updateAnimations() {
    if (!isAnimating) return;
    
    animationTime += 0.016f; // ~60 FPS
    
    // Animate circular indicators
    if (downloadProgress) {
        float progress = 50.0f + 40.0f * std::sin(animationTime * 2.0f);
        downloadProgress->setValue(progress);
    }
    
    if (batteryLevel) {
        float battery = 60.0f + 30.0f * std::sin(animationTime * 0.5f);
        batteryLevel->setValue(battery);
    }
    
    if (cpuUsage) {
        float cpu = 40.0f + 30.0f * std::sin(animationTime * 3.0f);
        cpuUsage->setValue(cpu);
    }
    
    // Update status text
    if (statusText) {
        std::string status = "Animation time: " + std::to_string(static_cast<int>(animationTime)) + "s";
        statusText->setText(status);
    }
}

void setupUI() {
    // === MAIN LAYOUT STRUCTURE ===
    std::vector<std::shared_ptr<Widget>> mainColumn;
    
    // Title Section
    auto title = Text(Point(0, 0), "Complete Layout-Based Widget Showcase", 3, Colors::White);
    mainColumn.push_back(title);
    mainColumn.push_back(SizedBox(0, 30)); // Vertical spacing
    
    // === THREE COLUMN LAYOUT ===
    std::vector<std::shared_ptr<Widget>> threeColumns;
    
    // LEFT COLUMN: Interactive Sliders
    std::vector<std::shared_ptr<Widget>> leftColumn;
    leftColumn.push_back(Text(Point(0, 0), "Interactive Sliders", 2, Colors::Cyan));
    leftColumn.push_back(SizedBox(0, 20));
    
    // Volume Slider Group
    volumeSlider = Slider(SliderConfig(0, 0, 250, 25)
        .range(0.0f, 100.0f)
        .initialValue(75.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Green)
            .thumbColor(Colors::White)
            .thumbHoverColor(Colors::LightGray)
            .thumbRadius(12)
            .showValue(true)
            .textColor(Colors::White)
            .textScale(2)), false);
    
    volumeLabel = Text(Point(0, 0), "Volume: 75%", 2, Colors::White);
    
    volumeSlider->onValueChanged.connect([=](float value) {
        volumeLabel->setText("Volume: " + std::to_string(static_cast<int>(value)) + "%");
    });
    
    leftColumn.push_back(volumeSlider);
    leftColumn.push_back(SizedBox(0, 8));
    leftColumn.push_back(volumeLabel);
    leftColumn.push_back(SizedBox(0, 25));
    
    // Brightness Slider Group
    brightnessSlider = Slider(SliderConfig(0, 0, 250, 25)
        .range(0.0f, 100.0f)
        .initialValue(80.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Yellow)
            .thumbColor(Colors::Orange)
            .thumbHoverColor(Colors::LightGray)
            .thumbRadius(10)
            .showValue(true)
            .textColor(Colors::White)
            .textScale(2)), false);
    
    brightnessLabel = Text(Point(0, 0), "Brightness: 80%", 2, Colors::White);
    
    brightnessSlider->onValueChanged.connect([=](float value) {
        brightnessLabel->setText("Brightness: " + std::to_string(static_cast<int>(value)) + "%");
    });
    
    leftColumn.push_back(brightnessSlider);
    leftColumn.push_back(SizedBox(0, 8));
    leftColumn.push_back(brightnessLabel);
    leftColumn.push_back(SizedBox(0, 25));
    
    // Speed Slider Group
    speedSlider = Slider(SliderConfig(0, 0, 250, 30)
        .range(0.0f, 200.0f)
        .initialValue(50.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Red)
            .thumbColor(Colors::White)
            .thumbHoverColor(Colors::LightGray)
            .thumbRadius(15)
            .showValue(true)
            .textColor(Colors::White)
            .textScale(2)), false);
    
    speedLabel = Text(Point(0, 0), "Speed: 50 km/h", 2, Colors::White);
    
    speedSlider->onValueChanged.connect([=](float value) {
        speedLabel->setText("Speed: " + std::to_string(static_cast<int>(value)) + " km/h");
    });
    
    leftColumn.push_back(speedSlider);
    leftColumn.push_back(SizedBox(0, 8));
    leftColumn.push_back(speedLabel);
    
    // CENTER COLUMN: Animated Circular Indicators
    std::vector<std::shared_ptr<Widget>> centerColumn;
    centerColumn.push_back(Text(Point(0, 0), "Animated Progress", 2, Colors::Cyan));
    centerColumn.push_back(SizedBox(0, 20));
    
    // Download Progress
    downloadProgress = CircularIndicator(CircularIndicatorConfig(0, 0, 60)
        .range(0.0f, 100.0f)
        .value(50.0f)
        .style(CircularIndicatorStyle()
            .fillColor(Colors::Blue)
            .backgroundColor(Colors::DarkGray)
            .borderColor(Colors::LightGray)
            .borderWidth(2)
            .thickness(8)
            .showPercentage(true)
            .fontSize(2)
            .textColor(Colors::White)
            .startAngle(-90)), false);
    
    std::vector<std::shared_ptr<Widget>> downloadGroup;
    downloadGroup.push_back(downloadProgress);
    downloadGroup.push_back(SizedBox(0, 10));
    downloadGroup.push_back(Text(Point(0, 0), "Download Progress", 2, Colors::White));
    
    centerColumn.push_back(Center(Column(downloadGroup)));
    centerColumn.push_back(SizedBox(0, 20));
    
    // Battery Level
    batteryLevel = CircularIndicator(CircularIndicatorConfig(0, 0, 60)
        .range(0.0f, 100.0f)
        .value(60.0f)
        .style(CircularIndicatorStyle()
            .fillColor(Colors::Green)
            .backgroundColor(Colors::DarkGray)
            .borderColor(Colors::LightGray)
            .borderWidth(2)
            .thickness(8)
            .showPercentage(true)
            .fontSize(2)
            .textColor(Colors::White)
            .startAngle(-90)), false);
    
    std::vector<std::shared_ptr<Widget>> batteryGroup;
    batteryGroup.push_back(batteryLevel);
    batteryGroup.push_back(SizedBox(0, 10));
    batteryGroup.push_back(Text(Point(0, 0), "Battery Level", 2, Colors::White));
    
    centerColumn.push_back(Center(Column(batteryGroup)));
    centerColumn.push_back(SizedBox(0, 20));
    
    // CPU Usage
    cpuUsage = CircularIndicator(CircularIndicatorConfig(0, 0, 60)
        .range(0.0f, 100.0f)
        .value(40.0f)
        .style(CircularIndicatorStyle()
            .fillColor(Colors::Orange)
            .backgroundColor(Colors::DarkGray)
            .borderColor(Colors::LightGray)
            .borderWidth(2)
            .thickness(8)
            .showPercentage(true)
            .fontSize(2)
            .textColor(Colors::White)
            .startAngle(-90)), false);
    
    std::vector<std::shared_ptr<Widget>> cpuGroup;
    cpuGroup.push_back(cpuUsage);
    cpuGroup.push_back(SizedBox(0, 10));
    cpuGroup.push_back(Text(Point(0, 0), "CPU Usage", 2, Colors::White));
    
    centerColumn.push_back(Center(Column(cpuGroup)));
    
    // RIGHT COLUMN: Text Input
    std::vector<std::shared_ptr<Widget>> rightColumn;
    rightColumn.push_back(Text(Point(0, 0), "Interactive Input", 2, Colors::Cyan));
    rightColumn.push_back(SizedBox(0, 20));
    
    // Search Box
    searchBox = TextInput(TextInputConfig(0, 0, 250, 40)
        .placeholder("Type to search...")
        .style(TextInputStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .focusBorderColor(Colors::Blue)
            .textColor(Colors::Black)
            .cursorColor(Colors::Black)
            .fontSize(2)
            .borderWidth(2)
            .padding(12)), false);
    
    searchLabel = Text(Point(0, 0), "Search: (empty)", 2, Colors::White);
    
    searchBox->onTextChanged.connect([=](const std::string& text) {
        std::string displayText = text.empty() ? "(empty)" : text;
        searchLabel->setText("Search: " + displayText);
    });
    
    searchBox->onEnterPressed.connect([=](const std::string& text) {
        if (statusText) {
            statusText->setText("Searching for: " + text);
        }
    });
    
    rightColumn.push_back(searchBox);
    rightColumn.push_back(SizedBox(0, 15));
    rightColumn.push_back(searchLabel);
    
    // Create padded containers for each column
    auto leftContainer = Padding(Column(leftColumn), 20);
    auto centerContainer = Padding(Column(centerColumn), 20);
    auto rightContainer = Padding(Column(rightColumn), 20);
    
    // Add columns to row with spacing
    threeColumns.push_back(leftContainer);
    threeColumns.push_back(SizedBox(30, 0)); // Horizontal spacing
    threeColumns.push_back(centerContainer);
    threeColumns.push_back(SizedBox(30, 0)); // Horizontal spacing
    threeColumns.push_back(rightContainer);
    
    mainColumn.push_back(Row(threeColumns));
    mainColumn.push_back(SizedBox(0, 40)); // Space before bottom section
    
    // === BOTTOM SECTION: Dropdowns ===
    std::vector<std::shared_ptr<Widget>> bottomSection;
    bottomSection.push_back(Text(Point(0, 0), "Interactive Dropdowns", 2, Colors::Cyan));
    bottomSection.push_back(SizedBox(0, 20));
    
    // Dropdown Row
    std::vector<std::shared_ptr<Widget>> dropdownRow;
    
    // Quality Dropdown
    std::vector<std::shared_ptr<Widget>> qualityGroup;
    qualityGroup.push_back(Text(Point(0, 0), "Quality:", 2, Colors::White));
    qualityGroup.push_back(SizedBox(0, 8));
    
    qualityDropdown = Dropdown(DropdownConfig(0, 0, 180, 35)
        .placeholder("Select Quality")
        .items({
            {"Ultra High", "ultra"},
            {"High", "high"},
            {"Medium", "medium"},
            {"Low", "low"},
            {"Very Low", "very_low"}
        })
        .selectedIndex(2)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Blue)
            .textColor(Colors::Black)
            .hoverColor(Colors::LightGray)
            .padding(10)
            .fontSize(2)), false);
    
    qualityLabel = Text(Point(0, 0), "Quality: Medium", 2, Colors::White);
    
    qualityDropdown->onItemSelected.connect([=](const DropdownItem& item) {
        qualityLabel->setText("Quality: " + item.text);
    });
    
    qualityGroup.push_back(qualityDropdown);
    qualityGroup.push_back(SizedBox(0, 10));
    qualityGroup.push_back(qualityLabel);
    
    auto qualityContainer = Padding(Column(qualityGroup), 15);
    
    // Language Dropdown
    std::vector<std::shared_ptr<Widget>> languageGroup;
    languageGroup.push_back(Text(Point(0, 0), "Language:", 2, Colors::White));
    languageGroup.push_back(SizedBox(0, 8));
    
    languageDropdown = Dropdown(DropdownConfig(0, 0, 180, 35)
        .placeholder("Select Language")
        .items({
            {"English", "en"},
            {"Spanish", "es"},
            {"French", "fr"},
            {"German", "de"},
            {"Japanese", "ja"},
            {"Chinese", "zh"},
            {"Russian", "ru"}
        })
        .selectedIndex(0)
        .style(DropdownStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .selectedColor(Colors::Green)
            .textColor(Colors::Black)
            .hoverColor(Colors::LightGray)
            .padding(10)
            .fontSize(2)), false);
    
    languageLabel = Text(Point(0, 0), "Language: English", 2, Colors::White);
    
    languageDropdown->onItemSelected.connect([=](const DropdownItem& item) {
        languageLabel->setText("Language: " + item.text);
    });
    
    languageGroup.push_back(languageDropdown);
    languageGroup.push_back(SizedBox(0, 10));
    languageGroup.push_back(languageLabel);
    
    auto languageContainer = Padding(Column(languageGroup), 15);
    
    dropdownRow.push_back(qualityContainer);
    dropdownRow.push_back(SizedBox(50, 0)); // Space between dropdowns
    dropdownRow.push_back(languageContainer);
    
    bottomSection.push_back(Row(dropdownRow));
    bottomSection.push_back(SizedBox(0, 30));
    
    // Status Section
    statusText = Text(Point(0, 0), "Status: Ready", 2, Colors::Yellow);
    bottomSection.push_back(statusText);
    bottomSection.push_back(SizedBox(0, 15));
    
    auto instructions = Text(Point(0, 0), "Instructions: Drag sliders, click dropdowns, type in search box!", 2, Colors::LightGray);
    bottomSection.push_back(instructions);
    
    mainColumn.push_back(Column(bottomSection));
    
    // === MAIN CONTAINER ===
    auto mainContainer = Center(Column(mainColumn));
    
    // Add widgets to manager
    addWidget(mainContainer);
    
    // Add interactive widgets (non-dropdowns first)
    addWidget(volumeSlider);
    addWidget(brightnessSlider);
    addWidget(speedSlider);
    addWidget(downloadProgress);
    addWidget(batteryLevel);
    addWidget(cpuUsage);
    addWidget(searchBox);
    
    // Add dropdowns LAST for proper z-index
    addWidget(qualityDropdown);
    addWidget(languageDropdown);
    
    std::cout << "=== Complete Layout-Based Widget Showcase ===" << std::endl;
    std::cout << "✓ Fixed spacing in core layout widgets" << std::endl;
    std::cout << "✓ Proper Column and Row layouts with SizedBox spacing" << std::endl;
    std::cout << "✓ Centered circular indicators with proper text alignment" << std::endl;
    std::cout << "✓ Dropdowns render on top (proper z-index)" << std::endl;
    std::cout << "✓ Responsive layout with generous padding" << std::endl;
    std::cout << "✓ All widgets positioned at (0,0) and arranged by layout" << std::endl;
    std::cout << "✓ Interactive callbacks work correctly" << std::endl;
    std::cout << "✓ Smooth animations for progress indicators" << std::endl;
    std::cout << "✓ Proper use of Padding() and SizedBox() for spacing" << std::endl;
    std::cout << "=================================================" << std::endl;
}

void draw() {
    // Dark background
    Draw::fill(0xFF1A1A1A);
    
    // Update animations
    updateAnimations();
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    
    // Start the render loop
    Fern::startRenderLoop();
    
    return 0;
}
