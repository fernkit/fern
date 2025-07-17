#include <fern/fern.hpp>

using namespace Fern;

static std::shared_ptr<TextWidget> modeText;
static std::shared_ptr<ButtonWidget> toggleButton;
static std::shared_ptr<CenterWidget> centerWidget;
static bool isOn = false;

void setupUI() {
    modeText = Text(Point(0, 0), "Mode: OFF", 3, Colors::Red);
    
    // Create toggle button with proper styling
    ButtonStyle offStyle;
    offStyle.normalColor(Colors::Gray)
           .hoverColor(Colors::LightGray)
           .pressColor(Colors::DarkGray)
           .textColor(Colors::White)
           .textScale(2)
           .borderRadius(8);  // Rounded corners
    
    toggleButton = Button(ButtonConfig(0, 0, 120, 50, "Turn ON").style(offStyle));
    
    toggleButton->onClick.connect([]() {
        isOn = !isOn;
        
        if (isOn) {
            modeText->setText("Mode: ON");
            modeText->setColor(Colors::Green);
            
            ButtonStyle onStyle;
            onStyle.normalColor(Colors::Green)
                  .hoverColor(Colors::LightGreen)
                  .pressColor(Colors::DarkGreen)
                  .textColor(Colors::White)
                  .textScale(2)
                  .borderRadius(8);
            
            // Keep the same position when updating config
            toggleButton->setConfig(ButtonConfig(toggleButton->getX(), toggleButton->getY(), 
                                               120, 50, "Turn OFF").style(onStyle));
        } else {
            modeText->setText("Mode: OFF");
            modeText->setColor(Colors::Red);
            
            ButtonStyle offStyle;
            offStyle.normalColor(Colors::Gray)
                   .hoverColor(Colors::LightGray)
                   .pressColor(Colors::DarkGray)
                   .textColor(Colors::White)
                   .textScale(2)
                   .borderRadius(8);
            
            // Keep the same position when updating config
            toggleButton->setConfig(ButtonConfig(toggleButton->getX(), toggleButton->getY(), 
                                               120, 50, "Turn ON").style(offStyle));
        }
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Toggle Switch Example", 3, Colors::White),
        SizedBox(0, 30),  // Proper spacing
        modeText,
        SizedBox(0, 40),  // More spacing before button
        toggleButton
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void onWindowResize(int newWidth, int newHeight) {
    // Update center widget to new dimensions
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(Colors::DarkBlue);
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
