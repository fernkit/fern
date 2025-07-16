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
    style.backgroundColor(Colors::Amber)
         .borderColor(Colors::Gray)
         .focusBorderColor(Colors::Blue)  // Changed from Amber to Blue for better visibility
         .textColor(Colors::Black)
         .cursorColor(Colors::Black)      // Added cursor color
         .padding(10)
         .fontSize(2)
         .borderWidth(2);                 // Added border width for better visibility
    
    // Create text input with custom style
    auto textInput = TextInput(TextInputConfig(0, 0, 300, 40)
        .placeholder("Type something here...")
        .style(style));
    
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
    Draw::fill(Colors::LightGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
