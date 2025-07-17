#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> dimensionText;
static std::shared_ptr<ButtonWidget> testButton;
static int resizeCount = 0;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    std::cout << "Setting up UI with initial size: " << width << "x" << height << std::endl;
    
    // Dimension display
    std::string dimensionStr = "Canvas: " + std::to_string(width) + "x" + std::to_string(height);
    dimensionText = Text(Point(0, 0), dimensionStr, 2, Colors::White);
    
    // Responsive button (50% of window width)
    int buttonWidth = width * 0.5;
    ButtonStyle testStyle;
    testStyle.normalColor(Colors::Blue)
           .hoverColor(Colors::LightBlue)
           .pressColor(Colors::DarkBlue)
           .textColor(Colors::White)
           .textScale(2)
           .borderRadius(8);
    
    testButton = Button(ButtonConfig(0, 0, buttonWidth, 50, "Responsive Button").style(testStyle));
    testButton->onClick.connect([]() {
        std::cout << "Button clicked! Current size: " << Fern::getWidth() << "x" << Fern::getHeight() << std::endl;
    });
    
    // Simple layout
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Simple Resize Test", 4, Colors::White),
        SizedBox(0, 40),
        
        dimensionText,
        SizedBox(0, 30),
        
        testButton,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Instructions:", 2, Colors::Yellow),
        SizedBox(0, 20),
        Text(Point(0, 0), "• Resize the window - everything should refresh cleanly", 1, Colors::LightGray),
        SizedBox(0, 10),
        Text(Point(0, 0), "• No trails or artifacts should remain", 1, Colors::LightGray),
        SizedBox(0, 10),
        Text(Point(0, 0), "• Button width adapts to 50% of window width", 1, Colors::LightGray),
        SizedBox(0, 10),
        Text(Point(0, 0), "• Background should always be solid blue", 1, Colors::LightGray)
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void onWindowResize(int newWidth, int newHeight) {
    resizeCount++;
    std::cout << "=== RESIZE EVENT #" << resizeCount << " ===" << std::endl;
    std::cout << "New dimensions: " << newWidth << "x" << newHeight << std::endl;
    
    // Update dimension text
    if (dimensionText) {
        std::string dimensionStr = "Canvas: " + std::to_string(newWidth) + "x" + std::to_string(newHeight);
        dimensionText->setText(dimensionStr);
        std::cout << "Updated dimension text" << std::endl;
    }
    
    // Update button width to 50% of new width
    if (testButton) {
        int newButtonWidth = newWidth * 0.5;
        
        ButtonStyle testStyle;
        testStyle.normalColor(Colors::Blue)
               .hoverColor(Colors::LightBlue)
               .pressColor(Colors::DarkBlue)
               .textColor(Colors::White)
               .textScale(2)
               .borderRadius(8);
        
        ButtonConfig newConfig(0, 0, newButtonWidth, 50, "Responsive Button");
        newConfig.style(testStyle);
        testButton->setConfig(newConfig);
        
        std::cout << "Updated button width to: " << newButtonWidth << "px" << std::endl;
    }
    
    std::cout << "Resize handling complete." << std::endl;
}

void draw() {
    // IMPORTANT: Always fill the entire canvas with background color
    // This ensures no trails or artifacts remain after resize
    Draw::fill(Colors::DarkBlue);
}

int main() {
    std::cout << "=== Simple Resize Test ===" << std::endl;
    
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    
    // Enable resize callback
    Fern::setWindowResizeCallback(onWindowResize);
    
    std::cout << "Starting render loop..." << std::endl;
    std::cout << "Try resizing the window - it should refresh cleanly!" << std::endl;
    
    Fern::startRenderLoop();
    return 0;
}
