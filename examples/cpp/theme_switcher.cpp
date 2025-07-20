#include <fern/fern.hpp>

using namespace Fern;

// Modern color themes
namespace LightTheme {
    const uint32_t Background = 0xFFF8FAFC;    // Very light gray
    const uint32_t Surface = 0xFFFFFFFF;       // Pure white
    const uint32_t Primary = 0xFF3B82F6;       // Clean blue
    const uint32_t Text = 0xFF1E293B;          // Dark slate
    const uint32_t TextSecondary = 0xFF64748B; // Medium gray
}

namespace DarkTheme {
    const uint32_t Background = 0xFF0F172A;    // Dark slate
    const uint32_t Surface = 0xFF1E293B;       // Lighter slate
    const uint32_t Primary = 0xFF60A5FA;       // Light blue
    const uint32_t Text = 0xFFFFFFFF;          // Pure white
    const uint32_t TextSecondary = 0xFFE2E8F0; // Light gray
}

// Global state
static bool isDarkMode = false;
static std::shared_ptr<TextWidget> titleText;
static std::shared_ptr<TextWidget> descriptionText;
static std::shared_ptr<ButtonWidget> themeButton;
static std::shared_ptr<CenterWidget> centerWidget;

void updateTheme() {
    if (isDarkMode) {
        // Switch to dark theme
        titleText->setColor(DarkTheme::Text);
        descriptionText->setColor(DarkTheme::TextSecondary);
        
        ButtonStyle darkStyle;
        darkStyle.normalColor(DarkTheme::Primary)
                .hoverColor(0xFF3B82F6)
                .pressColor(0xFF2563EB)
                .textColor(0xFFFFFFFF)
                .textScale(2)
                .borderRadius(8);
        
        themeButton->setConfig(ButtonConfig(0, 0, 140, 45, "Light Mode").style(darkStyle));
    } else {
        // Switch to light theme
        titleText->setColor(LightTheme::Text);
        descriptionText->setColor(LightTheme::TextSecondary);
        
        ButtonStyle lightStyle;
        lightStyle.normalColor(LightTheme::Primary)
                 .hoverColor(0xFF2563EB)
                 .pressColor(0xFF1D4ED8)
                 .textColor(0xFFFFFFFF)
                 .textScale(2)
                 .borderRadius(8);
        
        themeButton->setConfig(ButtonConfig(0, 0, 140, 45, "Dark Mode").style(lightStyle));
    }
}

uint32_t getCurrentBackground() {
    return isDarkMode ? DarkTheme::Background : LightTheme::Background;
}

void setupUI() {
    titleText = Text(Point(0, 0), "Theme Switcher", 4, LightTheme::Text);
    descriptionText = Text(Point(0, 0), "Toggle between light and dark modes", 2, LightTheme::TextSecondary);
    
    ButtonStyle initialStyle;
    initialStyle.normalColor(LightTheme::Primary)
               .hoverColor(0xFF2563EB)
               .pressColor(0xFF1D4ED8)
               .textColor(0xFFFFFFFF)
               .textScale(2)
               .borderRadius(8);
    
    themeButton = Button(ButtonConfig(0, 0, 140, 45, "Dark Mode").style(initialStyle));
    
    themeButton->onClick.connect([]() {
        isDarkMode = !isDarkMode;
        updateTheme();
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        titleText,
        SizedBox(0, 15),
        descriptionText,
        SizedBox(0, 40),
        themeButton
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void onWindowResize(int newWidth, int newHeight) {
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(getCurrentBackground());
}

int main() {
    Fern::initialize(800, 600);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::setWindowResizeCallback(onWindowResize);
    Fern::startRenderLoop();
    return 0;
}
