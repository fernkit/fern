#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

static std::shared_ptr<TextInputWidget> textInput;
static std::shared_ptr<TextWidget> outputText;
static std::shared_ptr<TextWidget> instructionText;

void setupUI() {
    // Title
    Text(Point(50, 30), "Text Input Demo", 3, Colors::White);
    
    // Instructions
    instructionText = Text(Point(50, 80), "Click on the text input below and start typing. Press Enter to submit.", 1, Colors::LightGray);
    
    // Create text input with simple, working configuration
    TextInputConfig config = DefaultTextInputConfig();
    config.x = 50;
    config.y = 130;
    config.width = 400;
    config.height = 35;
    config.backgroundColor = Colors::White;
    config.borderColor = Colors::Gray;
    config.focusBorderColor = Colors::Blue;
    config.textColor = Colors::Black;
    config.cursorColor = Colors::Black;
    config.placeholder = "Type something here...";
    config.textSize = 2;
    config.fontType = FontType::Bitmap;  // Start with bitmap to ensure it works
    config.borderWidth = 2;
    config.padding = 8;
    config.maxLength = 100;
    
    textInput = TextInput(config);
    
    // Output text
    outputText = Text(Point(50, 200), "Your text will appear here when you press Enter", 2, Colors::Yellow);
    
    // Connect signals
    textInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "Text changed: " << text << std::endl;
    });
    
    textInput->onEnterPressed.connect([](const std::string& text) {
        std::cout << "Enter pressed with text: " << text << std::endl;
        outputText->setText("You typed: \"" + text + "\"");
    });
    
    textInput->onFocusChanged.connect([](bool focused) {
        std::cout << "Focus changed: " << (focused ? "focused" : "unfocused") << std::endl;
    });
    
    // Create a clear button
    ButtonConfig clearButtonConfig = {
        .x = 470, .y = 130, .width = 80, .height = 35,
        .normalColor = Colors::Red,
        .hoverColor = Colors::LightRed,
        .pressColor = Colors::DarkRed,
        .label = "Clear",
        .textScale = 1,
        .textColor = Colors::White
    };
    
    auto clearButton = Button(clearButtonConfig);
    clearButton->onClick.connect([]() {
        textInput->clear();
        outputText->setText("Text cleared!");
        std::cout << "Text input cleared" << std::endl;
    });
    
    // Create a focus button
    ButtonConfig focusButtonConfig = {
        .x = 50, .y = 270, .width = 120, .height = 30,
        .normalColor = Colors::Blue,
        .hoverColor = Colors::LightBlue,
        .pressColor = Colors::DarkBlue,
        .label = "Focus Input",
        .textScale = 1,
        .textColor = Colors::White
    };
    
    auto focusButton = Button(focusButtonConfig);
    focusButton->onClick.connect([]() {
        textInput->setFocus(true);
        std::cout << "Text input focused programmatically" << std::endl;
    });
    
    // Status display
    Text(Point(50, 320), "Status: Click input field to focus, type text, press Enter to submit", 1, Colors::LightGray);
}

void draw() {
    Draw::fill(Colors::Gray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
