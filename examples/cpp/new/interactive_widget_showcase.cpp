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

// Animation state
static float animationTime = 0.0f;
static bool isAnimating = true;

void updateAnimations() {
    if (!isAnimating) return;
    
    animationTime += 0.016f; // ~60 FPS
    
    // Animate circular indicators
    if (downloadProgress) {
        float progress = 50.0f + 40.0f * sin(animationTime * 2.0f);
        downloadProgress->setValue(progress);
    }
    
    if (batteryLevel) {
        float battery = 60.0f + 30.0f * sin(animationTime * 0.5f);
        batteryLevel->setValue(battery);
    }
    
    if (cpuUsage) {
        float cpu = 40.0f + 30.0f * sin(animationTime * 3.0f);
        cpuUsage->setValue(cpu);
    }
    
    // Update status text
    if (statusText) {
        std::string status = "Animation time: " + std::to_string(static_cast<int>(animationTime)) + "s";
        statusText->setText(status);
    }
}

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Title
    auto title = Text(Point(screenWidth / 2 - 150, 20), "Interactive Widget Showcase", 3, Colors::White);
    
    // Column 1: Sliders
    auto sliderTitle = Text(Point(50, 80), "Interactive Sliders", 2, Colors::Cyan);
    
    // Volume slider with live feedback
    volumeSlider = Slider(SliderConfig(50, 110, 200, 25)
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
            .textScale(2)), true);
    
    auto volumeLabel = Text(Point(50, 150), "Volume: 75%", 2, Colors::White);
    
    volumeSlider->onValueChanged.connect([=](float value) {
        volumeLabel->setText("Volume: " + std::to_string(static_cast<int>(value)) + "%");
        std::cout << "Volume changed to: " << value << "%" << std::endl;
    });
    
    // Brightness slider
    brightnessSlider = Slider(SliderConfig(50, 180, 200, 25)
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
            .textScale(2)), true);
    
    auto brightnessLabel = Text(Point(50, 220), "Brightness: 80%", 2, Colors::White);
    
    brightnessSlider->onValueChanged.connect([=](float value) {
        brightnessLabel->setText("Brightness: " + std::to_string(static_cast<int>(value)) + "%");
        std::cout << "Brightness changed to: " << value << "%" << std::endl;
    });
    
    // Speed slider
    speedSlider = Slider(SliderConfig(50, 250, 200, 30)
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
            .textScale(2)), true);
    
    auto speedLabel = Text(Point(50, 290), "Speed: 50 km/h", 2, Colors::White);
    
    speedSlider->onValueChanged.connect([=](float value) {
        speedLabel->setText("Speed: " + std::to_string(static_cast<int>(value)) + " km/h");
        std::cout << "Speed changed to: " << value << " km/h" << std::endl;
    });
    
    // Column 2: Animated Circular Indicators
    auto indicatorTitle = Text(Point(350, 80), "Animated Progress Indicators", 2, Colors::Cyan);
    
    // Download progress (animated)
    downloadProgress = CircularIndicator(CircularIndicatorConfig(350, 110, 40)
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
            .startAngle(-90)), true);
    
    auto downloadLabel = Text(Point(320, 200), "Download Progress", 2, Colors::White);
    
    // Battery level (animated)
    batteryLevel = CircularIndicator(CircularIndicatorConfig(450, 110, 40)
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
            .startAngle(-90)), true);
    
    auto batteryLabel = Text(Point(430, 200), "Battery Level", 2, Colors::White);
    
    // CPU usage (animated)
    cpuUsage = CircularIndicator(CircularIndicatorConfig(550, 110, 40)
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
            .startAngle(-90)), true);
    
    auto cpuLabel = Text(Point(530, 200), "CPU Usage", 2, Colors::White);
    
    // Column 3: Dropdowns
    auto dropdownTitle = Text(Point(50, 330), "Interactive Dropdowns", 2, Colors::Cyan);
    
    // Quality dropdown
    qualityDropdown = Dropdown(DropdownConfig(50, 360, 150, 30)
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
            .padding(8)
            .fontSize(2)), true);
    
    auto qualityLabel = Text(Point(50, 400), "Quality: Medium", 2, Colors::White);
    
    qualityDropdown->onItemSelected.connect([=](const DropdownItem& item) {
        qualityLabel->setText("Quality: " + item.text);
        std::cout << "Quality selected: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    // Language dropdown
    languageDropdown = Dropdown(DropdownConfig(220, 360, 150, 30)
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
            .padding(8)
            .fontSize(2)), true);
    
    auto languageLabel = Text(Point(220, 400), "Language: English", 2, Colors::White);
    
    languageDropdown->onItemSelected.connect([=](const DropdownItem& item) {
        languageLabel->setText("Language: " + item.text);
        std::cout << "Language selected: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    // Text input with live feedback
    auto inputTitle = Text(Point(400, 330), "Interactive Text Input", 2, Colors::Cyan);
    
    searchBox = TextInput(TextInputConfig(400, 360, 250, 35)
        .placeholder("Type to search...")
        .style(TextInputStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .focusBorderColor(Colors::Blue)
            .textColor(Colors::Black)
            .cursorColor(Colors::Black)
            .fontSize(2)
            .borderWidth(2)
            .padding(10)), true);
    
    auto searchLabel = Text(Point(400, 400), "Search: (empty)", 2, Colors::White);
    
    searchBox->onTextChanged.connect([=](const std::string& text) {
        std::string displayText = text.empty() ? "(empty)" : text;
        searchLabel->setText("Search: " + displayText);
        std::cout << "Search text: " << text << std::endl;
    });
    
    searchBox->onEnterPressed.connect([=](const std::string& text) {
        std::cout << "Search submitted: " << text << std::endl;
        statusText->setText("Searching for: " + text);
    });
    
    // Status and control area
    statusText = Text(Point(50, 450), "Status: Ready", 2, Colors::Yellow);
    
    // Animation control
    auto animationLabel = Text(Point(50, 480), "Animation: ON (Click to toggle)", 2, Colors::Cyan);
    
    // Control instructions
    auto instructions = Text(Point(50, 520), "Instructions: Drag sliders, click dropdowns, type in text box, watch animations!", 2, Colors::LightGray);
    
    std::cout << "=== Interactive Widget Showcase ===" << std::endl;
    std::cout << "1. Drag the sliders to see real-time value updates" << std::endl;
    std::cout << "2. Watch the circular indicators animate automatically" << std::endl;
    std::cout << "3. Click dropdowns to select different options" << std::endl;
    std::cout << "4. Type in the search box to see live text updates" << std::endl;
    std::cout << "5. All widgets provide console feedback!" << std::endl;
    std::cout << "====================================" << std::endl;
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
