#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

// Button preset styles
namespace ButtonPresets {
    ButtonStyle Primary() {
        ButtonStyle style;
        return style.normalColor(Colors::Blue)
                   .hoverColor(Colors::LightBlue)
                   .pressColor(Colors::DarkBlue)
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8);
    }
    
    ButtonStyle Secondary() {
        ButtonStyle style;
        return style.normalColor(Colors::Gray)
                   .hoverColor(Colors::LightGray)
                   .pressColor(Colors::DarkGray)
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(6);
    }
    
    ButtonStyle Success() {
        ButtonStyle style;
        return style.normalColor(Colors::Green)
                   .hoverColor(Colors::LightGreen)
                   .pressColor(Colors::DarkGreen)
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8);
    }
    
    ButtonStyle Warning() {
        ButtonStyle style;
        return style.normalColor(Colors::Orange)
                   .hoverColor(0xFFFF8C00)  // DarkOrange
                   .pressColor(0xFFFF4500)  // OrangeRed
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8);
    }
    
    ButtonStyle Danger() {
        ButtonStyle style;
        return style.normalColor(Colors::Red)
                   .hoverColor(0xFFFF6B6B)  // Light red
                   .pressColor(Colors::DarkRed)
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8);
    }
    
    ButtonStyle Outline() {
        ButtonStyle style;
        return style.normalColor(Colors::Transparent)
                   .hoverColor(0x20FFFFFF)  // Semi-transparent white
                   .pressColor(0x40FFFFFF)  // More opaque white
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8)
                   .borderColor(Colors::White)
                   .borderWidth(2);
    }
    
    ButtonStyle Rounded() {
        ButtonStyle style;
        return style.normalColor(Colors::Purple)
                   .hoverColor(0xFFDA70D6)  // Orchid
                   .pressColor(0xFF8B008B)  // DarkMagenta
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(25);  // Very rounded
    }
    
    ButtonStyle Flat() {
        ButtonStyle style;
        return style.normalColor(Colors::Transparent)
                   .hoverColor(0x20000000)  // Semi-transparent black
                   .pressColor(0x40000000)  // More opaque black
                   .textColor(Colors::Black)
                   .textScale(2)
                   .borderRadius(0);  // No rounding
    }
}

// Global variables for dynamic examples
static std::shared_ptr<TextWidget> statusText;
static std::shared_ptr<ButtonWidget> toggleButton;
static std::shared_ptr<ButtonWidget> autoSizeButton;
static bool isToggled = false;

void setupUI() {
    std::vector<std::shared_ptr<Widget>> children;
    
    // Title
    children.push_back(Text(Point(0, 0), "Button Examples & Presets", 4, Colors::White));
    children.push_back(SizedBox(0, 30));
    
    // Button Presets Section
    children.push_back(Text(Point(0, 0), "Button Presets:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    // Row of preset buttons
    std::vector<std::shared_ptr<Widget>> presetRow1 = {
        Button(ButtonConfig(0, 0, 120, 40, "Primary").style(ButtonPresets::Primary())),
        SizedBox(10, 0),  // Horizontal spacing
        Button(ButtonConfig(0, 0, 120, 40, "Secondary").style(ButtonPresets::Secondary())),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Success").style(ButtonPresets::Success()))
    };
    children.push_back(Row(presetRow1));
    children.push_back(SizedBox(0, 15));
    
    std::vector<std::shared_ptr<Widget>> presetRow2 = {
        Button(ButtonConfig(0, 0, 120, 40, "Warning").style(ButtonPresets::Warning())),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Danger").style(ButtonPresets::Danger())),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Outline").style(ButtonPresets::Outline()))
    };
    children.push_back(Row(presetRow2));
    children.push_back(SizedBox(0, 15));
    
    std::vector<std::shared_ptr<Widget>> presetRow3 = {
        Button(ButtonConfig(0, 0, 120, 40, "Rounded").style(ButtonPresets::Rounded())),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Flat").style(ButtonPresets::Flat()))
    };
    children.push_back(Row(presetRow3));
    children.push_back(SizedBox(0, 40));
    
    // Auto-sizing button example
    children.push_back(Text(Point(0, 0), "Auto-sizing Button:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    // Button that auto-sizes based on content
    ButtonStyle autoStyle = ButtonPresets::Primary();
    autoSizeButton = Button(ButtonConfig(0, 0, -1, 40, "Click to change text").style(autoStyle));  // -1 for auto-width
    autoSizeButton->onClick.connect([]() {
        static int clickCount = 0;
        clickCount++;
        
        std::vector<std::string> texts = {
            "Short",
            "Medium length text",
            "This is a very long button text that should auto-resize",
            "X"
        };
        
        std::string newText = texts[clickCount % texts.size()];
        
        // Create new config with auto-width
        ButtonConfig newConfig(0, 0, -1, 40, newText);
        newConfig.style(ButtonPresets::Primary());
        autoSizeButton->setConfig(newConfig);
    });
    
    children.push_back(autoSizeButton);
    children.push_back(SizedBox(0, 30));
    
    // Toggle button with fixed positioning
    children.push_back(Text(Point(0, 0), "Toggle Switch (Fixed Position):", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    statusText = Text(Point(0, 0), "Status: OFF", 2, Colors::Red);
    children.push_back(statusText);
    children.push_back(SizedBox(0, 15));
    
    // Fixed position toggle button
    ButtonStyle offStyle = ButtonPresets::Secondary();
    toggleButton = Button(ButtonConfig(0, 0, 140, 45, "Turn ON").style(offStyle));
    
    // Store original position to maintain it during toggles
    static int originalX = 0;
    static int originalY = 0;
    static bool positionStored = false;
    
    toggleButton->onClick.connect([&]() {
        // Store original position on first click
        if (!positionStored) {
            originalX = toggleButton->getX();
            originalY = toggleButton->getY();
            positionStored = true;
        }
        
        isToggled = !isToggled;
        
        if (isToggled) {
            statusText->setText("Status: ON");
            statusText->setColor(Colors::Green);
            
            // Create new config but preserve position
            ButtonConfig newConfig(originalX, originalY, 140, 45, "Turn OFF");
            newConfig.style(ButtonPresets::Success());
            toggleButton->setConfig(newConfig);
        } else {
            statusText->setText("Status: OFF");
            statusText->setColor(Colors::Red);
            
            // Create new config but preserve position
            ButtonConfig newConfig(originalX, originalY, 140, 45, "Turn ON");
            newConfig.style(ButtonPresets::Secondary());
            toggleButton->setConfig(newConfig);
        }
    });
    
    children.push_back(toggleButton);
    children.push_back(SizedBox(0, 40));
    
    // Border radius examples
    children.push_back(Text(Point(0, 0), "Border Radius Examples:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    std::vector<std::shared_ptr<Widget>> radiusRow = {
        Button(ButtonConfig(0, 0, 100, 40, "0px").style(
            ButtonStyle().normalColor(Colors::Blue).textColor(Colors::White).borderRadius(0)
        )),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 100, 40, "5px").style(
            ButtonStyle().normalColor(Colors::Blue).textColor(Colors::White).borderRadius(5)
        )),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 100, 40, "15px").style(
            ButtonStyle().normalColor(Colors::Blue).textColor(Colors::White).borderRadius(15)
        )),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 100, 40, "25px").style(
            ButtonStyle().normalColor(Colors::Blue).textColor(Colors::White).borderRadius(25)
        ))
    };
    children.push_back(Row(radiusRow));
    children.push_back(SizedBox(0, 40));
    
    // Responsive button example
    children.push_back(Text(Point(0, 0), "Responsive Button:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    // Button that takes percentage of screen width
    int width = Fern::getWidth();
    int responsiveWidth = width * 0.6;  // 60% of screen width
    
    auto responsiveButton = Button(ButtonConfig(0, 0, responsiveWidth, 50, "60% Screen Width").style(ButtonPresets::Primary()));
    children.push_back(responsiveButton);
    children.push_back(SizedBox(0, 30));
    
    // Button states demonstration
    children.push_back(Text(Point(0, 0), "Interactive States:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    auto interactiveButton = Button(ButtonConfig(0, 0, 180, 50, "Hover & Click Me").style(ButtonPresets::Primary()));
    interactiveButton->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    children.push_back(interactiveButton);
    children.push_back(SizedBox(0, 20));
    
    // Instructions
    children.push_back(Text(Point(0, 0), "Try hovering and clicking the buttons above!", 1, Colors::Cyan));
    
    // Center everything with responsive layout
    int height = Fern::getHeight();
    auto scrollContainer = std::make_shared<ScrollWidget>(0, 0, width, height);
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    scrollContainer->add(centerWidget);
    addWidget(scrollContainer);
}

void draw() {
    Draw::fill(Colors::Black);
    
    // Handle window resize updates
    static int lastWidth = 0;
    static int lastHeight = 0;
    int currentWidth = Fern::getWidth();
    int currentHeight = Fern::getHeight();
    
    if (currentWidth != lastWidth || currentHeight != lastHeight) {
        lastWidth = currentWidth;
        lastHeight = currentHeight;
        
        // Update responsive button if it exists
        if (autoSizeButton) {
            int newResponsiveWidth = currentWidth * 0.6;
            // Update button width while preserving other properties
            // This would require framework support for live updates
        }
    }
}

int main() {
    std::cout << "🌿 Starting Button Presets Demo..." << std::endl;
    
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
