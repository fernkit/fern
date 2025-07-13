#include <fern/fern.hpp>
#include <memory>
#include <vector>
#include <cmath>

using namespace Fern;

// Global widgets for interaction
std::shared_ptr<SliderWidget> volumeSlider;
std::shared_ptr<ProgressBarWidget> loadingProgress;
std::shared_ptr<CircularIndicatorWidget> cpuIndicator;
std::shared_ptr<ProgressBarWidget> memoryProgress;
std::shared_ptr<RadioButtonGroup> themeGroup;
std::shared_ptr<DropdownWidget> languageDropdown;
std::shared_ptr<SliderWidget> brightnessSlider;

void setupDashboard() {
    WidgetManager::getInstance().clear();
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create widgets with proper positioning
    
    // Volume slider (horizontal)
    volumeSlider = Slider(
        SliderConfig(50, 80, 200, 30)
            .range(0.0f, 100.0f)
            .initialValue(75.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thumbColor(Colors::White)
                .showValue(true)
                .textColor(Colors::White)
                .textScale(1)),
        true
    );
    
    // Loading progress bar
    loadingProgress = ProgressBar(
        ProgressBarConfig(50, 130, 300, 20)
            .range(0.0f, 1.0f)
            .value(0.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .borderColor(Colors::Black)
                .borderWidth(1)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(1)
                .useBitmapFont()),
        true
    );
    
    // CPU usage circular indicator
    cpuIndicator = CircularIndicator(
        CircularIndicatorConfig(450, 80, 80)
            .range(0.0f, 100.0f)
            .value(45.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Orange)
                .thickness(6)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(1)),
        true
    );
    
    // Memory usage progress bar (vertical)
    memoryProgress = ProgressBar(
        ProgressBarConfig(580, 80, 20, 120)
            .range(0.0f, 100.0f)
            .value(60.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Red)
                .borderColor(Colors::Black)
                .borderWidth(1)
                .showPercentage(false)),
        true
    );
    
    // Theme selection radio buttons
    themeGroup = std::make_shared<RadioButtonGroup>();
    
    auto darkTheme = RadioButton(
        RadioButtonConfig(50, 200, "Dark", "theme")
            .selected(true)
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(8)
                .spacing(5)
                .useBitmapFont()),
        true
    );
    darkTheme->setGroup(themeGroup);
    
    auto lightTheme = RadioButton(
        RadioButtonConfig(120, 200, "Light", "theme")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(8)
                .spacing(5)
                .useBitmapFont()),
        true
    );
    lightTheme->setGroup(themeGroup);
    
    auto autoTheme = RadioButton(
        RadioButtonConfig(200, 200, "Auto", "theme")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(8)
                .spacing(5)
                .useBitmapFont()),
        true
    );
    autoTheme->setGroup(themeGroup);
    
    // Language dropdown
    std::vector<DropdownItem> languages = {
        DropdownItem("English"),
        DropdownItem("Español"),
        DropdownItem("Français"),
        DropdownItem("Deutsch"),
        DropdownItem("日本語")
    };
    
    languageDropdown = Dropdown(
        DropdownConfig(350, 200, 150, 30)
            .placeholder("Select Language")
            .items(languages)
            .selectedIndex(0)
            .style(DropdownStyle()
                .backgroundColor(Colors::DarkGray)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::LightBlue)
                .hoverColor(Colors::LightGray)
                .textColor(Colors::White)
                .fontSize(1)
                .borderWidth(1)
                .maxVisibleItems(4)
                .useBitmapFont()),
        true
    );
    
    // Brightness slider (vertical)
    brightnessSlider = Slider(
        SliderConfig(650, 80, 30, 120)
            .range(0.0f, 100.0f)
            .initialValue(80.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Yellow)
                .thumbColor(Colors::White)
                .showValue(false)
                .textColor(Colors::White)
                .textScale(1)),
        true
    );
    
    // Add event handlers
    volumeSlider->onValueChanged.connect([](float value) {
        printf("Volume changed to: %.1f%%\n", value);
    });
    
    loadingProgress->onValueChanged.connect([](float value) {
        printf("Loading progress: %.1f%%\n", value * 100);
    });
    
    themeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selectedButton) {
        if (selectedButton) {
            printf("Theme changed to: %s\n", selectedButton->getText().c_str());
        }
    });
    
    languageDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = languageDropdown->getSelectedItem();
        if (selectedItem) {
            printf("Language changed to: %s (index: %d)\n", selectedItem->text.c_str(), index);
        }
    });
    
    brightnessSlider->onValueChanged.connect([](float value) {
        printf("Brightness changed to: %.1f%%\n", value);
    });
}

void updateAnimations() {
    static float time = 0.0f;
    time += 0.016f; // Assuming 60 FPS
    
    // Animate loading progress
    if (loadingProgress) {
        float progress = (sin(time * 0.5f) + 1.0f) * 0.5f; // 0 to 1
        loadingProgress->setValue(progress);
    }
    
    // Animate CPU usage
    if (cpuIndicator) {
        float cpu = 45.0f + sin(time * 0.8f) * 15.0f; // 30-60%
        cpuIndicator->setValue(cpu);
    }
    
    // Slowly animate memory usage
    if (memoryProgress) {
        float memory = 60.0f + sin(time * 0.3f) * 10.0f; // 50-70%
        memoryProgress->setValue(memory);
    }
}

void draw() {
    // Clear the screen with a dark background
    Draw::fill(0xFF1E1E1E);

    // Draw title
    Text(Point(50, 20), "CONTROL DASHBOARD", 3, Colors::White, true);

    // Draw section labels
    Text(Point(50, 60), "AUDIO CONTROLS:", 1.5, Colors::Cyan, true);
    Text(Point(50, 85), "Volume:", 1.2, Colors::LightGray, true);
    Text(Point(50, 135), "Loading:", 1.2, Colors::LightGray, true);

    Text(Point(450, 60), "SYSTEM MONITORING:", 1.5, Colors::Orange, true);
    Text(Point(430, 170), "CPU Usage", 1.2, Colors::LightGray, true);
    Text(Point(560, 210), "Memory", 1.2, Colors::LightGray, true);
    Text(Point(630, 210), "Brightness", 1.2, Colors::LightGray, true);

    Text(Point(50, 180), "PREFERENCES:", 1.5, Colors::Purple, true);
    Text(Point(50, 205), "Theme:", 1.2, Colors::LightGray, true);
    Text(Point(350, 180), "Language:", 1.2, Colors::LightGray, true);

    // Update animations
    updateAnimations();

    // Instructions
    Text(Point(50, 270), "Interact with the controls to see real-time updates", 1.2, Colors::Yellow, true);
    Text(Point(50, 290), "Watch the animated progress indicators", 1.2, Colors::Yellow, true);
}

int main() {
    Fern::initialize();
    setupDashboard();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
