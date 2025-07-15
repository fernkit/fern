#include <fern/fern.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic slider with value display
    auto basicSlider = Fern::Slider(Fern::SliderConfig(100, 100, 200, 20)
        .range(0.0f, 100.0f)
        .initialValue(50.0f), true);
    
    basicSlider->onValueChanged.connect([](float value) {
        std::cout << "Basic slider: " << std::fixed << std::setprecision(1) 
                  << value << "%" << std::endl;
    });
    
    // Example 2: Volume control slider
    auto volumeSlider = Fern::Slider(Fern::SliderPresets::Volume(100, 150, 250, 25), true);
    
    volumeSlider->onValueChanged.connect([](float volume) {
        std::cout << "Volume: " << volume << "%" << std::endl;
    });
    
    volumeSlider->onDragging.connect([](bool isDragging) {
        if (isDragging) {
            std::cout << "Adjusting volume..." << std::endl;
        }
    });
    
    // Example 3: Color picker RGB sliders
    auto redSlider = Fern::Slider(Fern::SliderConfig(100, 200, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(Fern::SliderStyle()
            .trackColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Red)
            .thumbColor(Fern::Colors::White)
            .showValue(true)
            .textColor(Fern::Colors::White)), true);
    
    auto greenSlider = Fern::Slider(Fern::SliderConfig(100, 230, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(Fern::SliderStyle()
            .trackColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Green)
            .thumbColor(Fern::Colors::White)
            .showValue(true)
            .textColor(Fern::Colors::White)), true);
    
    auto blueSlider = Fern::Slider(Fern::SliderConfig(100, 260, 200, 20)
        .range(0.0f, 255.0f)
        .initialValue(128.0f)
        .style(Fern::SliderStyle()
            .trackColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Blue)
            .thumbColor(Fern::Colors::White)
            .showValue(true)
            .textColor(Fern::Colors::White)), true);
    
    // Color update function
    auto updateColor = [=]() {
        int r = static_cast<int>(redSlider->getValue());
        int g = static_cast<int>(greenSlider->getValue());
        int b = static_cast<int>(blueSlider->getValue());
        std::cout << "Color: RGB(" << r << ", " << g << ", " << b << ")" << std::endl;
    };
    
    redSlider->onValueChanged.connect([=](float) { updateColor(); });
    greenSlider->onValueChanged.connect([=](float) { updateColor(); });
    blueSlider->onValueChanged.connect([=](float) { updateColor(); });
    
    // Example 4: Precision slider with custom range
    auto precisionSlider = Fern::Slider(Fern::SliderConfig(400, 100, 250, 25)
        .range(-10.0f, 10.0f)
        .initialValue(0.0f)
        .style(Fern::SliderStyle()
            .trackColor(0xFF2C3E50)     // Dark blue-gray
            .fillColor(0xFF3498DB)      // Bright blue
            .thumbColor(0xFFECF0F1)     // Light gray
            .thumbHoverColor(0xFFBDC3C7) // Darker gray on hover
            .thumbRadius(12)            // Larger thumb for precision
            .showValue(true)
            .textColor(Fern::Colors::White)
            .textScale(2)), true);
    
    precisionSlider->onValueChanged.connect([](float value) {
        std::cout << "Precision: " << std::fixed << std::setprecision(2) 
                  << value << std::endl;
    });
    
    // Example 5: Mini equalizer with multiple sliders
    std::vector<std::shared_ptr<Fern::SliderWidget>> eqSliders;
    std::vector<std::string> frequencies = {"60Hz", "170Hz", "310Hz", "600Hz", "1kHz", "3kHz", "6kHz", "12kHz"};
    
    for (int i = 0; i < frequencies.size(); ++i) {
        auto slider = Fern::Slider(Fern::SliderConfig(400 + i * 35, 200, 25, 120)
            .range(-12.0f, 12.0f)    // dB range
            .initialValue(0.0f)
            .style(Fern::SliderStyle()
                .trackColor(Fern::Colors::DarkGray)
                .fillColor(Fern::Colors::Cyan)
                .thumbColor(Fern::Colors::White)
                .thumbRadius(8)
                .showValue(true)
                .textColor(Fern::Colors::White)
                .textScale(1)), true);
        
        slider->onValueChanged.connect([i, frequencies](float value) {
            std::cout << frequencies[i] << ": " << std::fixed << std::setprecision(1) 
                      << value << "dB" << std::endl;
        });
        
        eqSliders.push_back(slider);
    }
    
    // Example 6: Settings panel with labeled sliders
    auto brightnessSlider = Fern::Slider(Fern::SliderPresets::Brightness(100, 350, 250, 25), true);
    
    brightnessSlider->onValueChanged.connect([](float brightness) {
        std::cout << "Brightness: " << brightness << "%" << std::endl;
    });
    
    // Example 7: Temperature control slider
    auto tempSlider = Fern::Slider(Fern::SliderConfig(400, 350, 200, 25)
        .range(-20.0f, 40.0f)        // Celsius range
        .initialValue(20.0f)
        .style(Fern::SliderStyle()
            .trackColor(Fern::Colors::LightGray)
            .fillColor(Fern::Colors::Orange)
            .thumbColor(Fern::Colors::White)
            .thumbRadius(10)
            .showValue(true)
            .textColor(Fern::Colors::White)
            .textScale(2)), true);
    
    tempSlider->onValueChanged.connect([](float temp) {
        std::cout << "Temperature: " << std::fixed << std::setprecision(1) 
                  << temp << "°C" << std::endl;
    });
    
    // Example 8: Speed control with custom styling
    auto speedSlider = Fern::Slider(Fern::SliderConfig(100, 420, 300, 30)
        .range(0.0f, 200.0f)         // Speed in km/h
        .initialValue(50.0f)
        .style(Fern::SliderStyle()
            .trackColor(0xFF34495E)     // Dark slate
            .fillColor(0xFFE74C3C)      // Red for speed
            .thumbColor(0xFFF39C12)     // Orange thumb
            .thumbHoverColor(0xFFE67E22) // Darker orange on hover
            .thumbRadius(15)            // Large thumb
            .showValue(true)
            .textColor(Fern::Colors::White)
            .textScale(2)), true);
    
    speedSlider->onValueChanged.connect([](float speed) {
        std::cout << "Speed: " << std::fixed << std::setprecision(0) 
                  << speed << " km/h" << std::endl;
    });
    
    // Add drag feedback for speed control
    speedSlider->onDragging.connect([](bool isDragging) {
        if (isDragging) {
            std::cout << "Adjusting speed..." << std::endl;
        } else {
            std::cout << "Speed set!" << std::endl;
        }
    });
    
    std::cout << "Slider Widget Demo" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "1. Basic slider - drag to see value changes" << std::endl;
    std::cout << "2. Volume control - audio-themed slider" << std::endl;
    std::cout << "3. Color picker - RGB sliders working together" << std::endl;
    std::cout << "4. Precision slider - custom range with larger thumb" << std::endl;
    std::cout << "5. Mini equalizer - multiple vertical sliders" << std::endl;
    std::cout << "6. Brightness control - settings panel style" << std::endl;
    std::cout << "7. Temperature control - real-world range" << std::endl;
    std::cout << "8. Speed control - custom styling and feedback" << std::endl;
}

int main() {
    Fern::initialize();
    setupUI();

    Fern::startRenderLoop();
    return 0;
}
