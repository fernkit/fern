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
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create main column layout with proper spacing
    std::vector<std::shared_ptr<Widget>> mainColumnChildren;
    
    // Title section with generous spacing
    auto title = Text(Point(0, 0), "Interactive Widget Showcase - Layout Based", 3, Colors::White);
    mainColumnChildren.push_back(title);
    mainColumnChildren.push_back(SizedBox(0, 40)); // More space after title
    
    // Create three-column layout using Row with proper spacing
    std::vector<std::shared_ptr<Widget>> threeColumnChildren;
    
    // === LEFT COLUMN: Sliders ===
    std::vector<std::shared_ptr<Widget>> leftColumnChildren;
    
    // Slider section title
    auto sliderTitle = Text(Point(0, 0), "Interactive Sliders", 2, Colors::Cyan);
    leftColumnChildren.push_back(sliderTitle);
    leftColumnChildren.push_back(SizedBox(0, 25)); // More space after title
    
    // Volume slider with proper spacing
    volumeSlider = Slider(SliderConfig(0, 0, 200, 25)
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
        std::cout << "Volume changed to: " << value << "%" << std::endl;
    });
    
    leftColumnChildren.push_back(volumeSlider);
    leftColumnChildren.push_back(SizedBox(0, 15));
    leftColumnChildren.push_back(volumeLabel);
    leftColumnChildren.push_back(SizedBox(0, 30)); // More space between sliders
    
    // Brightness slider
    brightnessSlider = Slider(SliderConfig(0, 0, 200, 25)
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
        std::cout << "Brightness changed to: " << value << "%" << std::endl;
    });
    
    leftColumnChildren.push_back(brightnessSlider);
    leftColumnChildren.push_back(SizedBox(0, 15));
    leftColumnChildren.push_back(brightnessLabel);
    leftColumnChildren.push_back(SizedBox(0, 30));
    
    // Speed slider
    speedSlider = Slider(SliderConfig(0, 0, 200, 30)
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
        std::cout << "Speed changed to: " << value << " km/h" << std::endl;
    });
    
    leftColumnChildren.push_back(speedSlider);
    leftColumnChildren.push_back(SizedBox(0, 15));
    leftColumnChildren.push_back(speedLabel);
    
    // Create left column container with generous padding
    auto leftColumn = std::make_shared<PaddingWidget>(0, 0, 280, 450, 30, 30, 30, 30);
    leftColumn->add(Column(leftColumnChildren));
    
    // === CENTER COLUMN: Circular Indicators ===
    std::vector<std::shared_ptr<Widget>> centerColumnChildren;
    
    auto indicatorTitle = Text(Point(0, 0), "Animated Progress", 2, Colors::Cyan);
    centerColumnChildren.push_back(indicatorTitle);
    centerColumnChildren.push_back(SizedBox(0, 30)); // More space after title
    
    // Create vertical layout for circular indicators (easier to manage)
    std::vector<std::shared_ptr<Widget>> indicatorColumnChildren;
    
    // Download progress
    std::vector<std::shared_ptr<Widget>> downloadGroup;
    downloadProgress = CircularIndicator(CircularIndicatorConfig(0, 0, 50) // Larger radius
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
    
    auto downloadLabel = Text(Point(0, 0), "Download Progress", 2, Colors::White);
    downloadGroup.push_back(downloadProgress);
    downloadGroup.push_back(SizedBox(0, 15));
    downloadGroup.push_back(downloadLabel);
    
    auto downloadContainer = std::make_shared<CenterWidget>(0, 0, 200, 140);
    downloadContainer->add(Column(downloadGroup));
    indicatorColumnChildren.push_back(downloadContainer);
    
    indicatorColumnChildren.push_back(SizedBox(0, 25)); // Space between indicators
    
    // Battery level
    std::vector<std::shared_ptr<Widget>> batteryGroup;
    batteryLevel = CircularIndicator(CircularIndicatorConfig(0, 0, 50)
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
    
    auto batteryLabel = Text(Point(0, 0), "Battery Level", 2, Colors::White);
    batteryGroup.push_back(batteryLevel);
    batteryGroup.push_back(SizedBox(0, 15));
    batteryGroup.push_back(batteryLabel);
    
    auto batteryContainer = std::make_shared<CenterWidget>(0, 0, 200, 140);
    batteryContainer->add(Column(batteryGroup));
    indicatorColumnChildren.push_back(batteryContainer);
    
    indicatorColumnChildren.push_back(SizedBox(0, 25));
    
    // CPU usage
    std::vector<std::shared_ptr<Widget>> cpuGroup;
    cpuUsage = CircularIndicator(CircularIndicatorConfig(0, 0, 50)
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
    
    auto cpuLabel = Text(Point(0, 0), "CPU Usage", 2, Colors::White);
    cpuGroup.push_back(cpuUsage);
    cpuGroup.push_back(SizedBox(0, 15));
    cpuGroup.push_back(cpuLabel);
    
    auto cpuContainer = std::make_shared<CenterWidget>(0, 0, 200, 140);
    cpuContainer->add(Column(cpuGroup));
    indicatorColumnChildren.push_back(cpuContainer);
    
    // Add indicator column to center column
    centerColumnChildren.push_back(Column(indicatorColumnChildren));
    
    // Create center column container
    auto centerColumn = std::make_shared<PaddingWidget>(0, 0, 280, 450, 30, 30, 30, 30);
    centerColumn->add(Column(centerColumnChildren));
    
    // === RIGHT COLUMN: Text Input ===
    std::vector<std::shared_ptr<Widget>> rightColumnChildren;
    
    auto inputTitle = Text(Point(0, 0), "Interactive Input", 2, Colors::Cyan);
    rightColumnChildren.push_back(inputTitle);
    rightColumnChildren.push_back(SizedBox(0, 30));
    
    // Text input with generous spacing
    searchBox = TextInput(TextInputConfig(0, 0, 240, 40) // Larger input box
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
        std::cout << "Search text: " << text << std::endl;
    });
    
    searchBox->onEnterPressed.connect([=](const std::string& text) {
        std::cout << "Search submitted: " << text << std::endl;
        statusText->setText("Searching for: " + text);
    });
    
    rightColumnChildren.push_back(searchBox);
    rightColumnChildren.push_back(SizedBox(0, 20));
    rightColumnChildren.push_back(searchLabel);
    
    // Create right column container
    auto rightColumn = std::make_shared<PaddingWidget>(0, 0, 280, 450, 30, 30, 30, 30);
    rightColumn->add(Column(rightColumnChildren));
    
    // Add columns to three-column row with spacing
    threeColumnChildren.push_back(leftColumn);
    threeColumnChildren.push_back(SizedBox(40, 0)); // Space between columns
    threeColumnChildren.push_back(centerColumn);
    threeColumnChildren.push_back(SizedBox(40, 0)); // Space between columns
    threeColumnChildren.push_back(rightColumn);
    
    // Create three-column row
    auto threeColumnRow = Row(threeColumnChildren);
    mainColumnChildren.push_back(threeColumnRow);
    
    mainColumnChildren.push_back(SizedBox(0, 50)); // Large space before bottom section
    
    // === BOTTOM SECTION: Dropdowns and Status ===
    std::vector<std::shared_ptr<Widget>> bottomSectionChildren;
    
    auto dropdownTitle = Text(Point(0, 0), "Interactive Dropdowns", 2, Colors::Cyan);
    bottomSectionChildren.push_back(dropdownTitle);
    bottomSectionChildren.push_back(SizedBox(0, 25));
    
    // Dropdown row with proper spacing
    std::vector<std::shared_ptr<Widget>> dropdownRowChildren;
    
    // Quality dropdown group
    std::vector<std::shared_ptr<Widget>> qualityGroup;
    auto qualityText = Text(Point(0, 0), "Quality:", 2, Colors::White);
    qualityDropdown = Dropdown(DropdownConfig(0, 0, 180, 35) // Larger dropdown
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
        std::cout << "Quality selected: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    qualityGroup.push_back(qualityText);
    qualityGroup.push_back(SizedBox(0, 8));
    qualityGroup.push_back(qualityDropdown);
    qualityGroup.push_back(SizedBox(0, 15));
    qualityGroup.push_back(qualityLabel);
    
    auto qualityContainer = std::make_shared<PaddingWidget>(0, 0, 220, 140, 20, 20, 20, 20);
    qualityContainer->add(Column(qualityGroup));
    dropdownRowChildren.push_back(qualityContainer);
    
    dropdownRowChildren.push_back(SizedBox(80, 0)); // Large space between dropdowns
    
    // Language dropdown group
    std::vector<std::shared_ptr<Widget>> languageGroup;
    auto languageText = Text(Point(0, 0), "Language:", 2, Colors::White);
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
        std::cout << "Language selected: " + item.text << " (" << item.value << ")" << std::endl;
    });
    
    languageGroup.push_back(languageText);
    languageGroup.push_back(SizedBox(0, 8));
    languageGroup.push_back(languageDropdown);
    languageGroup.push_back(SizedBox(0, 15));
    languageGroup.push_back(languageLabel);
    
    auto languageContainer = std::make_shared<PaddingWidget>(0, 0, 220, 140, 20, 20, 20, 20);
    languageContainer->add(Column(languageGroup));
    dropdownRowChildren.push_back(languageContainer);
    
    bottomSectionChildren.push_back(Row(dropdownRowChildren));
    bottomSectionChildren.push_back(SizedBox(0, 30));
    
    // Status and instructions with spacing
    statusText = Text(Point(0, 0), "Status: Ready", 2, Colors::Yellow);
    bottomSectionChildren.push_back(statusText);
    bottomSectionChildren.push_back(SizedBox(0, 15));
    
    auto instructions = Text(Point(0, 0), "Instructions: Drag sliders, click dropdowns, type in search box!", 2, Colors::LightGray);
    bottomSectionChildren.push_back(instructions);
    
    // Add bottom section to main column
    mainColumnChildren.push_back(Column(bottomSectionChildren));
    
    // Create main container with centered layout
    auto mainContainer = std::make_shared<CenterWidget>(0, 0, width, height);
    mainContainer->add(Column(mainColumnChildren));
    
    // Add main container to widget manager
    addWidget(mainContainer);
    
    // Add all interactive widgets to widget manager (non-dropdowns first)
    addWidget(volumeSlider);
    addWidget(brightnessSlider);
    addWidget(speedSlider);
    addWidget(downloadProgress);
    addWidget(batteryLevel);
    addWidget(cpuUsage);
    addWidget(searchBox);
    
    // Add dropdowns LAST for proper z-index (they will render on top)
    addWidget(qualityDropdown);
    addWidget(languageDropdown);
    
    std::cout << "=== Layout-Based Interactive Widget Showcase ===" << std::endl;
    std::cout << "1. Uses Column and Row layouts with proper spacing" << std::endl;
    std::cout << "2. Proper z-index for dropdowns (they render on top)" << std::endl;
    std::cout << "3. Responsive layout with generous padding and spacing" << std::endl;
    std::cout << "4. Animated circular indicators with proper text centering" << std::endl;
    std::cout << "5. Interactive feedback for all widgets" << std::endl;
    std::cout << "===============================================" << std::endl;
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
