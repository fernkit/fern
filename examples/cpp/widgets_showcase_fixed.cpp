#include <fern/fern.hpp>
#include <iostream>
#include <memory>
#include <cmath>

using namespace Fern;

// Global widget variables
std::shared_ptr<ProgressBarWidget> downloadProgress;
std::shared_ptr<ProgressBarWidget> uploadProgress;
std::shared_ptr<CircularIndicatorWidget> loadingSpinner;
std::shared_ptr<CircularIndicatorWidget> cpuUsage;
std::shared_ptr<RadioButtonGroup> themeGroup;
std::shared_ptr<RadioButtonWidget> lightTheme;
std::shared_ptr<RadioButtonWidget> darkTheme;
std::shared_ptr<RadioButtonWidget> autoTheme;
std::shared_ptr<DropdownWidget> languageDropdown;
std::shared_ptr<DropdownWidget> qualityDropdown;

// Animation variables
float downloadProgressValue = 0.0f;
float uploadProgressValue = 0.0f;
float cpuUsageValue = 0.0f;
bool isUploading = false;

void setupEventHandlers();

void setupWidgets() {
    // Progress bars
    downloadProgress = ProgressBar(
        ProgressBarConfig(50, 50, 400, 25)
            .range(0.0f, 1.0f)  // Set correct range
            .value(0.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .borderColor(Colors::Black)
                .borderWidth(2)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(2)
                .useBitmapFont()),
        true
    );

    uploadProgress = ProgressBar(
        ProgressBarConfig(50, 100, 400, 25)
            .range(0.0f, 1.0f)  // Set correct range
            .value(0.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .borderColor(Colors::Black)
                .borderWidth(2)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(2)
                .useBitmapFont()),
        true
    );

    // Circular indicators
    loadingSpinner = CircularIndicator(
        CircularIndicatorConfig(550, 75, 50)
            .range(0.0f, 1.0f)  // Set correct range
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::LightGray)
                .fillColor(Colors::Orange)
                .thickness(6)),
        true
    );

    cpuUsage = CircularIndicator(
        CircularIndicatorConfig(650, 75, 50)
            .range(0.0f, 1.0f)  // Set correct range
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::LightGray)
                .fillColor(Colors::Red)
                .thickness(6)
                .showPercentage(true)
                .textColor(Colors::Black)
                .fontSize(1)),
        true
    );

    // Radio button group for theme selection
    themeGroup = std::make_shared<RadioButtonGroup>();

    lightTheme = RadioButton(
        RadioButtonConfig(50, 180, "Light Theme", "themeGroup")
            .selected(true)
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(10)
                .useBitmapFont()),
        true
    );
    lightTheme->setGroup(themeGroup);

    darkTheme = RadioButton(
        RadioButtonConfig(50, 210, "Dark Theme", "themeGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(10)
                .useBitmapFont()),
        true
    );
    darkTheme->setGroup(themeGroup);

    autoTheme = RadioButton(
        RadioButtonConfig(50, 240, "Auto Theme", "themeGroup")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::White)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .radius(8)
                .spacing(10)
                .useBitmapFont()),
        true
    );
    autoTheme->setGroup(themeGroup);

    // Dropdown menus
    std::vector<DropdownItem> languages = {
        DropdownItem("English"),
        DropdownItem("Spanish"),
        DropdownItem("French"),
        DropdownItem("German"),
        DropdownItem("Japanese"),
        DropdownItem("Chinese")
    };
    
    languageDropdown = Dropdown(
        DropdownConfig(300, 180, 200, 30)
            .placeholder("Select Language")
            .items(languages)
            .selectedIndex(0)
            .style(DropdownStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::LightBlue)
                .hoverColor(Colors::LightGray)
                .textColor(Colors::Black)
                .fontSize(2)
                .borderWidth(1)
                .maxVisibleItems(4)
                .useBitmapFont()),
        true
    );

    std::vector<DropdownItem> qualities = {
        DropdownItem("Low"),
        DropdownItem("Medium"),
        DropdownItem("High"),
        DropdownItem("Ultra")
    };
    
    qualityDropdown = Dropdown(
        DropdownConfig(300, 230, 200, 30)
            .placeholder("Select Quality")
            .items(qualities)
            .selectedIndex(2)
            .style(DropdownStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::LightBlue)
                .hoverColor(Colors::LightGray)
                .textColor(Colors::Black)
                .fontSize(2)
                .borderWidth(1)
                .maxVisibleItems(4)
                .useBitmapFont()),
        true
    );

    // Setup event handlers
    setupEventHandlers();
}

void setupEventHandlers() {
    // Progress bar events
    downloadProgress->onValueChanged.connect([](float progress) {
        std::cout << "Download progress: " << (progress * 100) << "%" << std::endl;
    });

    uploadProgress->onValueChanged.connect([](float progress) {
        std::cout << "Upload progress: " << (progress * 100) << "%" << std::endl;
    });

    // Radio button events
    themeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selectedButton) {
        if (selectedButton) {
            std::cout << "Theme changed to: " << selectedButton->getText() << std::endl;
        }
    });

    // Dropdown events
    languageDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = languageDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Language selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
        }
    });

    qualityDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = qualityDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Quality selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
        }
    });
}

void updateAnimations() {
    static int frameCount = 0;
    frameCount++;

    // Animate download progress
    downloadProgressValue += 0.005f;
    if (downloadProgressValue > 1.0f) {
        downloadProgressValue = 0.0f;
    }
    downloadProgress->setValue(downloadProgressValue);

    // Animate upload progress (starts when download is 50% complete)
    if (downloadProgressValue > 0.5f && !isUploading) {
        isUploading = true;
    }
    
    if (isUploading) {
        uploadProgressValue += 0.003f;
        if (uploadProgressValue > 1.0f) {
            uploadProgressValue = 1.0f;
            isUploading = false; // Reset for next cycle
        }
        uploadProgress->setValue(uploadProgressValue);
    }

    // Animate CPU usage with sine wave
    cpuUsageValue = (std::sin(frameCount * 0.02f) + 1.0f) * 0.5f; // 0.0 to 1.0
    cpuUsage->setValue(cpuUsageValue);
    
    // Update loading spinner by continuously updating its value
    float spinnerValue = (frameCount * 0.01f);
    while (spinnerValue > 1.0f) spinnerValue -= 1.0f;
    loadingSpinner->setValue(spinnerValue);
}

void draw() {
    // Clear the screen with a gradient-like background
    Draw::fill(0xFF2C3E50); // Dark blue-gray background

    // Draw title
    Text(Point(50, 20), "FERN WIDGETS SHOWCASE", 3, Colors::White, true);

    // Draw labels for sections
    Text(Point(50, 35), "PROGRESS BARS:", 2, Colors::Yellow, true);
    Text(Point(50, 55), "Download:", 1, Colors::White, true);
    Text(Point(50, 105), "Upload:", 1, Colors::White, true);

    Text(Point(550, 35), "CIRCULAR INDICATORS:", 2, Colors::Yellow, true);
    Text(Point(530, 140), "Loading", 1, Colors::White, true);
    Text(Point(620, 140), "CPU Usage", 1, Colors::White, true);

    Text(Point(50, 160), "RADIO BUTTONS:", 2, Colors::Yellow, true);
    Text(Point(300, 160), "DROPDOWNS:", 2, Colors::Yellow, true);

    // Update animations
    updateAnimations();

    // Instructions
    Text(Point(50, 280), "Click radio buttons to change theme", 1, Colors::LightGray, true);
    Text(Point(50, 300), "Use dropdowns to select options", 1, Colors::LightGray, true);
    Text(Point(50, 320), "Watch the animated progress indicators", 1, Colors::LightGray, true);
}

int main() {
    // Initialize Fern
    Fern::initialize();

    // Setup widgets
    setupWidgets();

    // Set draw callback
    Fern::setDrawCallback(draw);

    // Start the render loop
    Fern::startRenderLoop();

    return 0;
}
