#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> displayText;

void setupUI() {
    // Title
    auto title = Text(Point(0, 0), "Text Input Example", 3, Colors::White);
    
    // Display area
    displayText = Text(Point(0, 0), "Type something in the input below...", 2, Colors::Gray);
    
    // Text input style
    TextInputStyle style;
    style.backgroundColor(Colors::White)
         .borderColor(Colors::Gray)
         .focusBorderColor(Colors::Blue)
         .textColor(Colors::Black)
         .padding(10)
         .fontSize(2);
    
    // Create text input
    auto textInputConfig = TextInputPresets::Default(0,0);
    auto textInput = TextInput(textInputConfig);
    
    // Handle text changes
    textInput->onTextChanged.connect([](const std::string& text) {
        displayText->setText("You typed: " + text);
        std::cout << "Text changed to: " << text << std::endl;
    });
    
    // Handle enter key
    textInput->onEnterPressed.connect([](const std::string& text) {
        displayText->setText("Submitted: " + text);
        std::cout << "Enter pressed with text: " << text << std::endl;
    });
    
    // Layout
    std::vector<std::shared_ptr<Widget>> children = {
        title,
        SizedBox(0, 30),
        displayText,
        SizedBox(0, 40),
        textInput
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
