#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

std::shared_ptr<ProgressBarWidget> fileProgress;
std::shared_ptr<ProgressBarWidget> networkProgress;
std::shared_ptr<ProgressBarWidget> installProgress;

float fileProgressValue = 0.0f;
float networkProgressValue = 0.0f;
float installProgressValue = 0.0f;

void setupProgressBars() {
    // File download progress bar
    fileProgress = ProgressBar(
        ProgressBarConfig(50, 100, 400, 30)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
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

    // Network upload progress bar
    networkProgress = ProgressBar(
        ProgressBarConfig(50, 180, 400, 25)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
            .value(0.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .borderColor(Colors::White)
                .borderWidth(1)
                .showPercentage(true)
                .textColor(Colors::Yellow)
                .fontSize(2)
                .useBitmapFont()),
        true
    );

    // Installation progress bar
    installProgress = ProgressBar(
        ProgressBarConfig(50, 260, 400, 20)
            .range(0.0f, 1.0f)  // Set range to 0.0-1.0
            .value(0.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::Gray)
                .fillColor(Colors::Orange)
                .borderColor(Colors::DarkGray)
                .borderWidth(1)
                .showPercentage(false)  // No percentage text
                .useBitmapFont()),
        true
    );

    // Setup event handlers
    fileProgress->onValueChanged.connect([](float progress) {
        if (progress >= 1.0f) {
            std::cout << "File download completed!" << std::endl;
        }
    });

    networkProgress->onValueChanged.connect([](float progress) {
        if (progress >= 1.0f) {
            std::cout << "Network upload completed!" << std::endl;
        }
    });

    installProgress->onComplete.connect([]() {
        std::cout << "Installation finished!" << std::endl;
    });
}

void updateProgress() {
    static int frameCount = 0;
    frameCount++;

    // Simulate different progress speeds
    fileProgressValue += 0.008f;  // Fast download
    if (fileProgressValue > 1.0f) {
        fileProgressValue = 0.0f;
    }

    networkProgressValue += 0.004f;  // Medium upload
    if (networkProgressValue > 1.0f) {
        networkProgressValue = 0.0f;
    }

    installProgressValue += 0.002f;  // Slow installation
    if (installProgressValue > 1.0f) {
        installProgressValue = 0.0f;
    }

    // Update progress bars
    fileProgress->setValue(fileProgressValue);
    networkProgress->setValue(networkProgressValue);
    installProgress->setValue(installProgressValue);
}

void draw() {
    // Clear screen
    Draw::fill(0xFF34495E); // Dark blue-gray

    // Title
    Text(Point(50, 30), "PROGRESS BAR DEMO", 3, Colors::White, true);

    // Labels
    Text(Point(50, 70), "File Download (with percentage):", 2, Colors::LightGray, true);
    Text(Point(50, 150), "Network Upload (custom colors):", 2, Colors::LightGray, true);
    Text(Point(50, 230), "Software Installation (no percentage):", 2, Colors::LightGray, true);

    // Progress values as text
    char buffer[64];
    sprintf(buffer, "Download: %.1f%%", fileProgressValue * 100);
    Text(Point(50, 310), buffer, 1, Colors::Green, true);
    
    sprintf(buffer, "Upload: %.1f%%", networkProgressValue * 100);
    Text(Point(200, 310), buffer, 1, Colors::Blue, true);
    
    sprintf(buffer, "Install: %.1f%%", installProgressValue * 100);
    Text(Point(350, 310), buffer, 1, Colors::Orange, true);

    // Instructions
    Text(Point(50, 350), "Watch the animated progress bars with different styles", 1, Colors::Yellow, true);

    // Update animations
    updateProgress();
}

int main() {
    Fern::initialize();
    setupProgressBars();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
