#include <fern/fern.hpp>
#include <iostream>
#include <string>

using namespace Fern;

// Modern color palette - carefully chosen for visual appeal
namespace ModernColors {
    // Primary colors
    const uint32_t BackgroundDark = 0xFF1A1A1A;      // Dark charcoal background
    const uint32_t SurfaceElevated = 0xFF2A2A2A;     // Elevated surface
    const uint32_t AccentBlue = 0xFF3B82F6;          // Modern blue accent
    const uint32_t AccentGreen = 0xFF10B981;         // Success green
    const uint32_t AccentRed = 0xFFEF4444;           // Alert red
    
    // Text colors
    const uint32_t TextPrimary = 0xFFFFFFFF;         // Pure white
    const uint32_t TextSecondary = 0xFFE5E7EB;       // Light gray
    const uint32_t TextMuted = 0xFF9CA3AF;           // Muted gray
    
    // Interactive states
    const uint32_t ButtonHover = 0xFF4F46E5;         // Indigo hover
    const uint32_t ButtonPress = 0xFF3730A3;         // Darker indigo press
    const uint32_t SurfaceHover = 0xFF374151;        // Surface hover state
}

// Global state
static int counter = 0;
static std::shared_ptr<TextWidget> counterDisplay;
static std::shared_ptr<TextWidget> statusText;

// Helper function to create modern button style
ButtonStyle createModernButtonStyle(uint32_t normalColor, uint32_t hoverColor, uint32_t pressColor) {
    ButtonStyle style;
    style.normalColor(normalColor)
         .hoverColor(hoverColor)
         .pressColor(pressColor)
         .textColor(ModernColors::TextPrimary)
         .textScale(2)
         .borderRadius(12);
    return style;
}

// Helper function to update counter display with modern styling
void updateCounterDisplay() {
    // Update the main counter text
    counterDisplay->setText(std::to_string(counter));
    
    // Update status text based on counter value
    std::string status;
    uint32_t statusColor;
    
    if (counter == 0) {
        status = "Start counting!";
        statusColor = ModernColors::TextMuted;
    } else if (counter > 0 && counter <= 10) {
        status = "Getting started";
        statusColor = ModernColors::AccentBlue;
    } else if (counter > 10 && counter <= 50) {
        status = "Making progress!";
        statusColor = ModernColors::AccentGreen;
    } else if (counter > 50) {
        status = "You're on fire!";
        statusColor = ModernColors::AccentGreen;
    } else {
        status = "Going backwards";
        statusColor = ModernColors::AccentRed;
    }
    
    statusText->setText(status);
    statusText->setColor(statusColor);
}

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create title text
    TextStyle titleStyle;
    titleStyle.color(ModernColors::TextPrimary)
              .fontSize(4)
              .useBitmapFont()
              .alignment(1); // Center alignment
    
    auto titleText = Text(TextConfig(0, 0, "MODERN COUNTER").style(titleStyle));
    
    // Create subtitle
    TextStyle subtitleStyle;
    subtitleStyle.color(ModernColors::TextSecondary)
                 .fontSize(2)
                 .useBitmapFont()
                 .alignment(1);
    
    auto subtitleText = Text(TextConfig(0, 0, "A beautiful counting experience").style(subtitleStyle));
    
    // Create counter display with large, prominent styling
    TextStyle counterStyle;
    counterStyle.color(ModernColors::AccentBlue)
                .fontSize(8)
                .useBitmapFont()
                .alignment(1)
                .backgroundColor(ModernColors::SurfaceElevated)
                .padding(20)
                .shadow(true, 0xFF000000, 4);
    
    counterDisplay = Text(TextConfig(0, 0, "0").style(counterStyle));
    
    // Create status text
    TextStyle statusStyle;
    statusStyle.color(ModernColors::TextMuted)
               .fontSize(2)
               .useBitmapFont()
               .alignment(1);
    
    statusText = Text(TextConfig(0, 0, "Start counting!").style(statusStyle));
    
    // Create increment button (+)
    auto incrementStyle = createModernButtonStyle(
        ModernColors::AccentGreen,
        0xFF059669,  // Darker green on hover
        0xFF047857   // Even darker green on press
    );
    
    auto incrementButton = Button(ButtonConfig(0, 0, 120, 55, "+ ADD")
        .style(incrementStyle));
    
    incrementButton->onClick.connect([]() {
        counter++;
        updateCounterDisplay();
        std::cout << "Counter incremented to: " << counter << std::endl;
    });
    
    // Create decrement button (-)
    auto decrementStyle = createModernButtonStyle(
        ModernColors::AccentRed,
        0xFFDC2626,  // Darker red on hover
        0xFFB91C1C   // Even darker red on press
    );
    
    auto decrementButton = Button(ButtonConfig(0, 0, 120, 55, "- SUB")
        .style(decrementStyle));
    
    decrementButton->onClick.connect([]() {
        counter--;
        updateCounterDisplay();
        std::cout << "Counter decremented to: " << counter << std::endl;
    });
    
    // Create reset button
    auto resetStyle = createModernButtonStyle(
        ModernColors::SurfaceHover,
        0xFF4B5563,  // Lighter gray on hover
        0xFF374151   // Medium gray on press
    );
    
    auto resetButton = Button(ButtonConfig(0, 0, 100, 45, "RESET")
        .style(resetStyle));
    
    resetButton->onClick.connect([]() {
        counter = 0;
        updateCounterDisplay();
        std::cout << "Counter reset to 0" << std::endl;
    });
    
    // Create button row layout
    std::vector<std::shared_ptr<Widget>> buttonRow = {
        decrementButton,
        SizedBox(20, 0),  // Horizontal spacing
        incrementButton
    };
    
    // Create main layout
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        SizedBox(0, 40),      // Top spacing
        titleText,
        SizedBox(0, 10),      // Small spacing
        subtitleText,
        SizedBox(0, 60),      // Large spacing before counter
        counterDisplay,
        SizedBox(0, 50),      // Spacing after counter
        statusText,
        SizedBox(0, 50),      // Spacing before buttons
        Row(buttonRow, false, MainAxisAlignment::Center, CrossAxisAlignment::Center),
        SizedBox(0, 30),      // Spacing before reset
        resetButton,
        SizedBox(0, 40)       // Bottom spacing
    };
    
    // Create centered container
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    
    addWidget(centerWidget);
}

void draw() {
    // Beautiful gradient background
    Draw::fill(ModernColors::BackgroundDark);
    
    // Optional: Add subtle texture or pattern
    // You could add some geometric patterns here for extra visual appeal
}

int main() {
    Fern::initialize(900, 700);  // Slightly larger window for better presentation
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}