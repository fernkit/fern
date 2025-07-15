#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic circular indicator
    auto basicIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(100, 100, 40)
        .value(75.0f)
        .range(0.0f, 100.0f), true);
    
    basicIndicator->onValueChanged.connect([](float value) {
        std::cout << "Basic indicator: " << value << "%" << std::endl;
    });
    
    // Example 2: Loading indicator with preset
    auto loadingIndicator = Fern::CircularIndicator(Fern::CircularIndicatorPresets::Loading(200, 100, 35), true);
    loadingIndicator->setValue(60.0f);
    
    loadingIndicator->onValueChanged.connect([](float progress) {
        std::cout << "Loading: " << progress << "%" << std::endl;
    });
    
    loadingIndicator->onComplete.connect([]() {
        std::cout << "Loading completed!" << std::endl;
    });
    
    // Example 3: Health indicator
    auto healthIndicator = Fern::CircularIndicator(Fern::CircularIndicatorPresets::Health(300, 100, 30), true);
    healthIndicator->setValue(80.0f);
    
    healthIndicator->onValueChanged.connect([](float health) {
        if (health < 25.0f) {
            std::cout << "Health critical: " << health << "%" << std::endl;
        } else if (health < 50.0f) {
            std::cout << "Health low: " << health << "%" << std::endl;
        } else {
            std::cout << "Health: " << health << "%" << std::endl;
        }
    });
    
    // Example 4: Battery indicator
    auto batteryIndicator = Fern::CircularIndicator(Fern::CircularIndicatorPresets::Battery(400, 100, 25), true);
    batteryIndicator->setValue(65.0f);
    
    batteryIndicator->onValueChanged.connect([](float level) {
        if (level < 20.0f) {
            std::cout << "Battery low: " << level << "%" << std::endl;
        } else if (level < 50.0f) {
            std::cout << "Battery medium: " << level << "%" << std::endl;
        } else {
            std::cout << "Battery: " << level << "%" << std::endl;
        }
    });
    
    // Example 5: Custom styled indicator
    auto customIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(100, 200, 50)
        .value(45.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(0xFF2C3E50)   // Dark blue-gray
            .fillColor(0xFF3498DB)         // Bright blue
            .borderColor(0xFF34495E)       // Darker border
            .borderWidth(3)
            .thickness(10)                 // Thick arc
            .showPercentage(true)
            .textColor(0xFFFFFFFF)         // White text
            .fontSize(2)                   // Large text
            .clockwise(true)
            .startAngle(-90.0f)            // Start from top
            .useBitmapFont()), true);
    
    customIndicator->onValueChanged.connect([](float value) {
        std::cout << "Custom indicator: " << value << "%" << std::endl;
    });
    
    // Example 6: System resource monitoring
    auto cpuIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(220, 200, 30)
        .value(45.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Blue)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    auto memoryIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(280, 200, 30)
        .value(67.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Orange)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    auto diskIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(340, 200, 30)
        .value(32.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Green)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    auto networkIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(400, 200, 30)
        .value(78.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Purple)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    cpuIndicator->onValueChanged.connect([](float cpu) {
        std::cout << "CPU: " << cpu << "%" << std::endl;
    });
    
    memoryIndicator->onValueChanged.connect([](float memory) {
        std::cout << "Memory: " << memory << "%" << std::endl;
    });
    
    diskIndicator->onValueChanged.connect([](float disk) {
        std::cout << "Disk: " << disk << "%" << std::endl;
    });
    
    networkIndicator->onValueChanged.connect([](float network) {
        std::cout << "Network: " << network << "%" << std::endl;
    });
    
    // Example 7: Download progress indicator
    auto downloadIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(100, 300, 40)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::LightGray)
            .fillColor(Fern::Colors::Cyan)
            .borderColor(Fern::Colors::DarkGray)
            .borderWidth(2)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::Black)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    downloadIndicator->onValueChanged.connect([](float progress) {
        std::cout << "Download: " << progress << "%" << std::endl;
    });
    
    downloadIndicator->onComplete.connect([]() {
        std::cout << "Download completed!" << std::endl;
    });
    
    // Example 8: Experience/Level indicator
    auto xpIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(200, 300, 45)
        .value(75.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(0xFF4A4A4A)   // Dark gray
            .fillColor(0xFFFFD700)         // Gold
            .borderColor(0xFF8B4513)       // Brown border
            .borderWidth(2)
            .thickness(8)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    xpIndicator->onValueChanged.connect([](float xp) {
        std::cout << "Experience: " << xp << "%" << std::endl;
    });
    
    xpIndicator->onComplete.connect([]() {
        std::cout << "Level up!" << std::endl;
    });
    
    // Example 9: Counter-clockwise indicator
    auto counterClockwiseIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(310, 300, 35)
        .value(60.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Red)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(false)           // Counter-clockwise
            .startAngle(-90.0f)), true);
    
    counterClockwiseIndicator->onValueChanged.connect([](float value) {
        std::cout << "Counter-clockwise: " << value << "%" << std::endl;
    });
    
    // Example 10: Temperature indicator
    auto tempIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(400, 300, 35)
        .value(72.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Orange)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(6)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    tempIndicator->onValueChanged.connect([](float temp) {
        std::cout << "Temperature: " << temp << "°C" << std::endl;
    });
    
    // Example 11: Skill indicators
    std::vector<std::string> skills = {"Strength", "Agility", "Intelligence", "Wisdom"};
    std::vector<float> skillValues = {72.0f, 58.0f, 91.0f, 45.0f};
    std::vector<uint32_t> skillColors = {
        Fern::Colors::Red,      // Strength
        Fern::Colors::Green,    // Agility
        Fern::Colors::Blue,     // Intelligence
        Fern::Colors::Purple    // Wisdom
    };
    
    for (int i = 0; i < skills.size(); ++i) {
        auto skillIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(100 + i * 80, 400, 25)
            .value(skillValues[i])
            .range(0.0f, 100.0f)
            .style(Fern::CircularIndicatorStyle()
                .backgroundColor(Fern::Colors::DarkGray)
                .fillColor(skillColors[i])
                .borderColor(Fern::Colors::Gray)
                .borderWidth(1)
                .thickness(4)
                .showPercentage(true)
                .textColor(Fern::Colors::White)
                .fontSize(1)
                .clockwise(true)
                .startAngle(-90.0f)), true);
        
        skillIndicator->onValueChanged.connect([skills, i](float value) {
            std::cout << skills[i] << ": " << value << "%" << std::endl;
        });
    }
    
    // Example 12: Large prominent indicator
    auto largeIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(500, 150, 60)
        .value(85.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Green)
            .borderColor(Fern::Colors::Black)
            .borderWidth(3)
            .thickness(12)                 // Thick arc
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(3)                   // Large text
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    largeIndicator->onValueChanged.connect([](float value) {
        std::cout << "Large indicator: " << value << "%" << std::endl;
    });
    
    // Example 13: Status indicators with different colors
    auto successIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(480, 300, 25)
        .value(100.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Green)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(4)
            .showPercentage(false)         // Status, not percentage
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    auto warningIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(520, 300, 25)
        .value(60.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Yellow)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(4)
            .showPercentage(false)         // Status, not percentage
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    auto errorIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(560, 300, 25)
        .value(25.0f)
        .range(0.0f, 100.0f)
        .style(Fern::CircularIndicatorStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Red)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .thickness(4)
            .showPercentage(false)         // Status, not percentage
            .clockwise(true)
            .startAngle(-90.0f)), true);
    
    successIndicator->onValueChanged.connect([](float value) {
        std::cout << "Success status: " << value << "%" << std::endl;
    });
    
    warningIndicator->onValueChanged.connect([](float value) {
        std::cout << "Warning status: " << value << "%" << std::endl;
    });
    
    errorIndicator->onValueChanged.connect([](float value) {
        std::cout << "Error status: " << value << "%" << std::endl;
    });
    
    // Example 14: Compact indicators
    for (int i = 0; i < 6; ++i) {
        auto compactIndicator = Fern::CircularIndicator(Fern::CircularIndicatorConfig(480 + i * 30, 400, 15)
            .value(20.0f + i * 15.0f)
            .range(0.0f, 100.0f)
            .style(Fern::CircularIndicatorStyle()
                .backgroundColor(Fern::Colors::DarkGray)
                .fillColor(Fern::Colors::Blue)
                .borderColor(Fern::Colors::Gray)
                .borderWidth(1)
                .thickness(3)
                .showPercentage(false)     // Too small for text
                .clockwise(true)
                .startAngle(-90.0f)), true);
        
        compactIndicator->onValueChanged.connect([i](float value) {
            std::cout << "Compact " << i+1 << ": " << value << "%" << std::endl;
        });
    }
    
    std::cout << "Circular Indicator Widget Demo" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Demonstrating various circular indicator styles and use cases:" << std::endl;
    std::cout << "1. Basic circular indicator" << std::endl;
    std::cout << "2. Loading indicator (preset)" << std::endl;
    std::cout << "3. Health indicator (preset)" << std::endl;
    std::cout << "4. Battery indicator (preset)" << std::endl;
    std::cout << "5. Custom styled indicator" << std::endl;
    std::cout << "6. System resource monitoring (CPU, Memory, Disk, Network)" << std::endl;
    std::cout << "7. Download progress indicator" << std::endl;
    std::cout << "8. Experience/Level indicator" << std::endl;
    std::cout << "9. Counter-clockwise indicator" << std::endl;
    std::cout << "10. Temperature indicator" << std::endl;
    std::cout << "11. Skill indicators" << std::endl;
    std::cout << "12. Large prominent indicator" << std::endl;
    std::cout << "13. Status indicators (success, warning, error)" << std::endl;
    std::cout << "14. Compact indicators" << std::endl;
}

int main() {
    Fern::init(700, 500, "Circular Indicator Widget Example");
    setupUI();
    Fern::run();
    return 0;
}
