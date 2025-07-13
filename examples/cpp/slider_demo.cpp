#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

static std::shared_ptr<TextWidget> volumeText;
static std::shared_ptr<TextWidget> brightnessText;
static std::shared_ptr<SliderWidget> volumeSlider;
static std::shared_ptr<SliderWidget> brightnessSlider;

void setupUI() {
    // Title
    Text(Point(50, 50), "SLIDER DEMO", 3, Colors::White, true);
    
    // Volume slider
    volumeText = Text(Point(50, 120), "Volume: 50", 2, Colors::White, true);
    
    auto volumeConfig = SliderConfig(50, 150, 300, 25)
        .range(0.0f, 100.0f)
        .initialValue(50.0f)
        .style(SliderStyle()
            .trackColor(Colors::Gray)
            .fillColor(Colors::Green)
            .thumbColor(Colors::White)
            .thumbHoverColor(Colors::LightGray)
            .thumbRadius(12)
            .showValue(false)); // We'll show custom text
    
    volumeSlider = Slider(volumeConfig, true);
    volumeSlider->onValueChanged.connect([](float value) {
        volumeText->setText("Volume: " + std::to_string(static_cast<int>(value)));
        std::cout << "Volume changed to: " << value << std::endl;
    });
    
    // Brightness slider
    brightnessText = Text(Point(50, 220), "Brightness: 75", 2, Colors::White, true);
    
    auto brightnessConfig = SliderConfig(50, 250, 300, 25)
        .range(0.0f, 100.0f)
        .initialValue(75.0f)
        .style(SliderStyle()
            .trackColor(Colors::DarkGray)
            .fillColor(Colors::Yellow)
            .thumbColor(Colors::Orange)
            .thumbHoverColor(Colors::Amber)
            .thumbRadius(10)
            .showValue(false));
    
    brightnessSlider = Slider(brightnessConfig, true);
    brightnessSlider->onValueChanged.connect([](float value) {
        brightnessText->setText("Brightness: " + std::to_string(static_cast<int>(value)));
        std::cout << "Brightness changed to: " << value << std::endl;
    });
    
    // Dragging status indicators
    volumeSlider->onDragging.connect([](bool dragging) {
        std::cout << "Volume slider " << (dragging ? "started" : "stopped") << " dragging" << std::endl;
    });
    
    brightnessSlider->onDragging.connect([](bool dragging) {
        std::cout << "Brightness slider " << (dragging ? "started" : "stopped") << " dragging" << std::endl;
    });
    
    // Instructions
    Text(Point(50, 320), "Click and drag the sliders or click on the track", 1, Colors::LightGray, true);
    Text(Point(50, 340), "to change values. Watch the console for events!", 1, Colors::LightGray, true);
    
    // Reset button
    auto resetConfig = ButtonConfig{
        .x = 50,
        .y = 380,
        .width = 120,
        .height = 35,
        .normalColor = Colors::Danger,
        .hoverColor = Colors::LightRed,
        .pressColor = Colors::DarkRed,
        .label = "RESET",
        .textScale = 1,
        .textColor = Colors::White
    };
    
    auto resetButton = Button(resetConfig, true);
    resetButton->onClick.connect([]() {
        volumeSlider->setValue(50.0f);
        brightnessSlider->setValue(75.0f);
        std::cout << "Sliders reset to default values!" << std::endl;
    });
}

void draw() {
    // Clear background with a gradient-like effect
    Draw::fill(Colors::DarkBlue);
    
    // Draw some decorative elements
    Draw::rect(20, 20, getWidth() - 40, getHeight() - 40, Colors::Black);
}

int main() {
    std::cout << "Starting Fern Slider Demo..." << std::endl;
    
    Fern::initialize(450, 500);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
