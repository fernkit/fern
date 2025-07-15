#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic progress bar
    auto basicProgress = Fern::ProgressBar(Fern::ProgressBarConfig(50, 50, 300, 20)
        .value(45.0f)
        .range(0.0f, 100.0f), true);
    
    basicProgress->onValueChanged.connect([](float value) {
        std::cout << "Basic progress: " << value << "%" << std::endl;
    });
    
    // Example 2: Download progress with preset
    auto downloadProgress = Fern::ProgressBar(Fern::ProgressBarPresets::Download(50, 90, 400, 25), true);
    downloadProgress->setValue(25.0f);
    
    downloadProgress->onValueChanged.connect([](float progress) {
        std::cout << "Download: " << progress << "% complete" << std::endl;
    });
    
    downloadProgress->onComplete.connect([]() {
        std::cout << "Download completed!" << std::endl;
    });
    
    // Example 3: Health bar
    auto healthBar = Fern::ProgressBar(Fern::ProgressBarPresets::Health(50, 130, 200, 15), true);
    healthBar->setValue(80.0f);
    
    healthBar->onValueChanged.connect([](float health) {
        if (health < 25.0f) {
            std::cout << "Health critical: " << health << "%" << std::endl;
        } else if (health < 50.0f) {
            std::cout << "Health low: " << health << "%" << std::endl;
        } else {
            std::cout << "Health: " << health << "%" << std::endl;
        }
    });
    
    // Example 4: Loading progress
    auto loadingProgress = Fern::ProgressBar(Fern::ProgressBarPresets::Loading(50, 170, 250, 20), true);
    loadingProgress->setValue(60.0f);
    
    loadingProgress->onValueChanged.connect([](float progress) {
        std::cout << "Loading: " << progress << "%" << std::endl;
    });
    
    // Example 5: Multi-step process progress
    auto processProgress = Fern::ProgressBar(Fern::ProgressBarConfig(50, 210, 350, 25)
        .value(0.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::Gray)
            .fillColor(Fern::Colors::Purple)
            .borderColor(Fern::Colors::DarkGray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(2)), true);
    
    std::vector<std::string> processSteps = {
        "Initializing...",
        "Loading data...",
        "Processing...",
        "Saving results...",
        "Complete!"
    };
    
    int currentStep = 0;
    processProgress->onValueChanged.connect([&](float progress) {
        int stepIndex = static_cast<int>(progress / 25.0f);
        if (stepIndex < processSteps.size() && stepIndex != currentStep) {
            currentStep = stepIndex;
            std::cout << processSteps[currentStep] << std::endl;
        }
    });
    
    processProgress->onComplete.connect([&]() {
        std::cout << processSteps.back() << std::endl;
    });
    
    // Example 6: System resource monitoring
    auto cpuBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 260, 150, 12)
        .value(45.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Blue)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)), true);
    
    auto memoryBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 280, 150, 12)
        .value(67.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Orange)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)), true);
    
    auto diskBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 300, 150, 12)
        .value(32.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::DarkGray)
            .fillColor(Fern::Colors::Green)
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)), true);
    
    cpuBar->onValueChanged.connect([](float cpu) {
        std::cout << "CPU: " << cpu << "%" << std::endl;
    });
    
    memoryBar->onValueChanged.connect([](float memory) {
        std::cout << "Memory: " << memory << "%" << std::endl;
    });
    
    diskBar->onValueChanged.connect([](float disk) {
        std::cout << "Disk: " << disk << "%" << std::endl;
    });
    
    // Example 7: Battery level indicator
    auto batteryBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 340, 120, 18)
        .value(65.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::Black)
            .fillColor(Fern::Colors::Green)
            .borderColor(Fern::Colors::White)
            .borderWidth(2)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)), true);
    
    batteryBar->onValueChanged.connect([](float level) {
        if (level < 20.0f) {
            std::cout << "Battery low: " << level << "%" << std::endl;
        } else if (level < 50.0f) {
            std::cout << "Battery medium: " << level << "%" << std::endl;
        } else {
            std::cout << "Battery: " << level << "%" << std::endl;
        }
    });
    
    // Example 8: Custom styled progress bar
    auto customBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 380, 300, 30)
        .value(40.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(0xFF2C3E50)   // Dark blue-gray
            .fillColor(0xFF3498DB)         // Bright blue
            .borderColor(0xFF34495E)       // Darker border
            .borderWidth(3)
            .showPercentage(true)
            .textColor(0xFFFFFFFF)         // White text
            .fontSize(3)                   // Large text
            .useBitmapFont()), true);
    
    customBar->onValueChanged.connect([](float value) {
        std::cout << "Custom progress: " << value << "%" << std::endl;
    });
    
    // Example 9: File transfer progress
    auto transferBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 430, 350, 22)
        .value(78.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::LightGray)
            .fillColor(Fern::Colors::Cyan)
            .borderColor(Fern::Colors::DarkGray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::Black)
            .fontSize(2)), true);
    
    transferBar->onValueChanged.connect([](float progress) {
        std::cout << "File transfer: " << progress << "%" << std::endl;
    });
    
    transferBar->onComplete.connect([]() {
        std::cout << "File transfer completed!" << std::endl;
    });
    
    // Example 10: Experience/Level progress
    auto xpBar = Fern::ProgressBar(Fern::ProgressBarConfig(50, 470, 280, 16)
        .value(85.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(0xFF4A4A4A)   // Dark gray
            .fillColor(0xFFFFD700)         // Gold
            .borderColor(0xFF8B4513)       // Saddle brown
            .borderWidth(2)
            .showPercentage(true)
            .textColor(Fern::Colors::White)
            .fontSize(1)), true);
    
    xpBar->onValueChanged.connect([](float xp) {
        std::cout << "Experience: " << xp << "%" << std::endl;
    });
    
    xpBar->onComplete.connect([]() {
        std::cout << "Level up!" << std::endl;
    });
    
    // Example 11: Skill progress bars
    std::vector<std::string> skills = {"Strength", "Agility", "Intelligence", "Wisdom"};
    std::vector<float> skillValues = {72.0f, 58.0f, 91.0f, 45.0f};
    std::vector<uint32_t> skillColors = {
        Fern::Colors::Red,      // Strength
        Fern::Colors::Green,    // Agility
        Fern::Colors::Blue,     // Intelligence
        Fern::Colors::Purple    // Wisdom
    };
    
    for (int i = 0; i < skills.size(); ++i) {
        auto skillBar = Fern::ProgressBar(Fern::ProgressBarConfig(450, 50 + i * 30, 200, 15)
            .value(skillValues[i])
            .range(0.0f, 100.0f)
            .style(Fern::ProgressBarStyle()
                .backgroundColor(Fern::Colors::DarkGray)
                .fillColor(skillColors[i])
                .borderColor(Fern::Colors::Gray)
                .borderWidth(1)
                .showPercentage(true)
                .textColor(Fern::Colors::White)
                .fontSize(1)), true);
        
        skillBar->onValueChanged.connect([skills, i](float value) {
            std::cout << skills[i] << ": " << value << "%" << std::endl;
        });
    }
    
    // Example 12: Installation progress
    auto installBar = Fern::ProgressBar(Fern::ProgressBarConfig(450, 200, 250, 20)
        .value(33.0f)
        .range(0.0f, 100.0f)
        .style(Fern::ProgressBarStyle()
            .backgroundColor(Fern::Colors::White)
            .fillColor(0xFF28A745)         // Bootstrap green
            .borderColor(Fern::Colors::Gray)
            .borderWidth(1)
            .showPercentage(true)
            .textColor(Fern::Colors::Black)
            .fontSize(2)), true);
    
    installBar->onValueChanged.connect([](float progress) {
        std::cout << "Installation: " << progress << "%" << std::endl;
    });
    
    installBar->onComplete.connect([]() {
        std::cout << "Installation completed successfully!" << std::endl;
    });
    
    std::cout << "Progress Bar Widget Demo" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "Demonstrating various progress bar styles and use cases:" << std::endl;
    std::cout << "1. Basic progress bar" << std::endl;
    std::cout << "2. Download progress" << std::endl;
    std::cout << "3. Health bar" << std::endl;
    std::cout << "4. Loading progress" << std::endl;
    std::cout << "5. Multi-step process" << std::endl;
    std::cout << "6. System resource monitoring (CPU, Memory, Disk)" << std::endl;
    std::cout << "7. Battery level indicator" << std::endl;
    std::cout << "8. Custom styled progress bar" << std::endl;
    std::cout << "9. File transfer progress" << std::endl;
    std::cout << "10. Experience/Level progress" << std::endl;
    std::cout << "11. Skill progress bars" << std::endl;
    std::cout << "12. Installation progress" << std::endl;
}



int main() {
    Fern::initialize();
    setupUI();

    Fern::startRenderLoop();
    return 0;
}