#include <fern/fern.hpp>
#include <iostream>
#include <memory>
#include <cmath>

using namespace Fern;

std::shared_ptr<CircularIndicatorWidget> loadingSpinner;
std::shared_ptr<CircularIndicatorWidget> downloadProgress;
std::shared_ptr<CircularIndicatorWidget> cpuUsage;
std::shared_ptr<CircularIndicatorWidget> memoryUsage;

float downloadValue = 0.0f;
float cpuValue = 0.0f;
float memoryValue = 0.0f;

void setupIndicators() {
    // Loading spinner (always spinning)
    loadingSpinner = CircularIndicator(
        CircularIndicatorConfig(150, 120, 60)
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::LightGray)
                .fillColor(Colors::Blue)
                .thickness(8)),
        true
    );

    // Download progress circle
    downloadProgress = CircularIndicator(
        CircularIndicatorConfig(350, 120, 60)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .thickness(10)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(2)),
        true
    );

    // CPU usage indicator
    cpuUsage = CircularIndicator(
        CircularIndicatorConfig(150, 300, 50)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
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

    // Memory usage indicator
    memoryUsage = CircularIndicator(
        CircularIndicatorConfig(350, 300, 50)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::LightGray)
                .fillColor(Colors::Orange)
                .thickness(6)
                .showPercentage(true)
                .textColor(Colors::Black)
                .fontSize(1)),
        true
    );

    // Setup event handlers
    downloadProgress->onValueChanged.connect([](float progress) {
        if (progress >= 1.0f) {
            std::cout << "Download completed!" << std::endl;
        }
    });

    cpuUsage->onValueChanged.connect([](float progress) {
        if (progress > 0.8f) {
            std::cout << "High CPU usage detected!" << std::endl;
        }
    });
}

void updateIndicators() {
    static int frameCount = 0;
    frameCount++;

    // Update download progress (linear)
    downloadValue += 0.006f;
    if (downloadValue > 1.0f) {
        downloadValue = 0.0f;
    }
    downloadProgress->setValue(downloadValue);

    // Update CPU usage (sine wave simulation)
    cpuValue = (std::sin(frameCount * 0.03f) + 1.0f) * 0.5f; // 0.0 to 1.0
    cpuUsage->setValue(cpuValue);

    // Update memory usage (slower sine wave)
    memoryValue = (std::sin(frameCount * 0.015f) * 0.3f + 0.6f); // 0.3 to 0.9
    if (memoryValue < 0.0f) memoryValue = 0.0f;
    if (memoryValue > 1.0f) memoryValue = 1.0f;
    memoryUsage->setValue(memoryValue);
}

void draw() {
    // Clear screen
    Draw::fill(0xFF2C3E50); // Dark blue background

    // Title
    Text(Point(50, 30), "CIRCULAR INDICATOR DEMO", 3, Colors::White, true);

    // Section labels
    Text(Point(100, 200), "Loading Spinner", 2, Colors::LightBlue, true);
    Text(Point(280, 200), "Download Progress", 2, Colors::Green, true);
    
    Text(Point(120, 380), "CPU Usage", 2, Colors::Red, true);
    Text(Point(300, 380), "Memory Usage", 2, Colors::Orange, true);

    // Current values as text
    char buffer[64];
    sprintf(buffer, "Download: %.1f%%", downloadValue * 100);
    Text(Point(50, 420), buffer, 1, Colors::Green, true);
    
    sprintf(buffer, "CPU: %.1f%%", cpuValue * 100);
    Text(Point(200, 420), buffer, 1, Colors::Red, true);
    
    sprintf(buffer, "Memory: %.1f%%", memoryValue * 100);
    Text(Point(320, 420), buffer, 1, Colors::Orange, true);

    // Instructions
    Text(Point(50, 460), "Watch the different circular indicator styles and animations", 1, Colors::Yellow, true);

    // Update animations
    updateIndicators();
}

int main() {
    Fern::initialize();
    setupIndicators();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
