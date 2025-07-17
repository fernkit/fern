#include <fern/fern.hpp>

using namespace Fern;

static std::shared_ptr<ButtonWidget> autoSizeButton;
static std::shared_ptr<ButtonWidget> responsiveButton;
static std::shared_ptr<CenterWidget> centerWidget;

void setupUI() {
    // Button presets examples
    auto primaryButton = Button(ButtonPresets::Primary(0, 0, 150, 40, "Primary"));
    auto secondaryButton = Button(ButtonPresets::Secondary(0, 0, 150, 40, "Secondary"));
    auto successButton = Button(ButtonPresets::Success(0, 0, 150, 40, "Success"));
    auto dangerButton = Button(ButtonPresets::Danger(0, 0, 150, 40, "Danger"));
    auto warningButton = Button(ButtonPresets::Warning(0, 0, 150, 40, "Warning"));
    
    // Auto-sizing button example
    ButtonStyle autoStyle;
    autoStyle.normalColor(Colors::Purple)
           .hoverColor(Colors::Magenta)
           .pressColor(Colors::DarkMagenta)
           .textColor(Colors::White)
           .textScale(2)
           .borderRadius(6);
    
    autoSizeButton = Button(ButtonConfig(0, 0, 200, 40, "Auto Size").style(autoStyle));
    autoSizeButton->onClick.connect([]() {
        static int counter = 0;
        counter++;
        
        std::vector<std::string> texts = {
            "Short",
            "Medium text",
            "This is a very long button text that demonstrates auto-sizing",
            "Auto Size"
        };
        
        std::string newText = texts[counter % texts.size()];
        
        // Create new config with same style but new text
        ButtonConfig newConfig(autoSizeButton->getX(), autoSizeButton->getY(), 200, 40, newText);
        newConfig.style(autoStyle);
        
        autoSizeButton->setConfig(newConfig);
        // Auto-size the button to fit the new text
        autoSizeButton->autoSizeToContent(20);  // 20 pixels padding
    });
    
    // Responsive button that adapts to window size
    ButtonStyle responsiveStyle;
    responsiveStyle.normalColor(Colors::Teal)
                  .hoverColor(Colors::Cyan)
                  .pressColor(Colors::DarkCyan)
                  .textColor(Colors::White)
                  .textScale(2)
                  .borderRadius(10);
    
    int width = Fern::getWidth();
    int buttonWidth = width * 0.3;  // 30% of screen width
    responsiveButton = Button(ButtonConfig(0, 0, buttonWidth, 50, "Responsive").style(responsiveStyle));
    
    // Border radius demonstration
    ButtonStyle radiusStyle;
    radiusStyle.normalColor(Colors::Orange)
              .hoverColor(Colors::Yellow)
              .pressColor(Colors::Red)
              .textColor(Colors::Black)
              .textScale(2)
              .borderRadius(20);  // Large border radius
    
    auto roundButton = Button(ButtonConfig(0, 0, 120, 60, "Round").style(radiusStyle));
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Button Features Showcase", 3, Colors::White),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Button Presets:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        Row({primaryButton, SizedBox(15, 0), secondaryButton, SizedBox(15, 0), successButton}),
        SizedBox(0, 15),
        Row({dangerButton, SizedBox(15, 0), warningButton}),
        SizedBox(0, 40),
        
        Text(Point(0, 0), "Auto-sizing (click to change text):", 2, Colors::LightBlue),
        SizedBox(0, 15),
        autoSizeButton,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Responsive width (30% of screen):", 2, Colors::LightBlue),
        SizedBox(0, 15),
        responsiveButton,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Border radius demonstration:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        roundButton
    };
    
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void onWindowResize(int newWidth, int newHeight) {
    // Update responsive button width
    if (responsiveButton) {
        int newButtonWidth = newWidth * 0.3;  // 30% of new width
        ButtonConfig newConfig(0, 0, newButtonWidth, 50, "Responsive");
        newConfig.style(ButtonStyle()
                      .normalColor(Colors::Teal)
                      .hoverColor(Colors::Cyan)
                      .pressColor(Colors::DarkCyan)
                      .textColor(Colors::White)
                      .textScale(2)
                      .borderRadius(10));
        responsiveButton->setConfig(newConfig);
    }
    
    // Update center widget dimensions
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    
    // Set up window resize callback
    Fern::setWindowResizeCallback(onWindowResize);
    
    Fern::startRenderLoop();
    return 0;
}
