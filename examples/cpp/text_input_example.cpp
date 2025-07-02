#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

static std::shared_ptr<TextInputWidget> textInput;
static std::shared_ptr<TextWidget> outputText;
static std::shared_ptr<TextWidget> instructionText;

void setupUI() {
    // Title
    Text(Point(50, 30), "TEXT INPUT DEMO", 3, Colors::White);
    
    // Instructions
    instructionText = Text(Point(50, 80), "CLICK ON THE DEMO AND START TYPING. PRESS ENTER TO SUBMIT", 1, Colors::LightGray);
    
    // Create text input with custom configuration
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
    config.placeholder = "TYPE SOMETHING HERE";
    config.textSize = 2;
    config.fontType = FontType::Bitmap;
    config.borderWidth = 2;
    config.padding = 8;
    config.maxLength = 100;
    
    textInput = TextInput(config);
    
    // Output text to show what was typed
    outputText = Text(Point(50, 200), "YOUR TEXT WILL APPEAR HERE WHEN YOU ENTER", 2, Colors::Yellow);
    
    // Connect signals
    textInput->onTextChanged.connect([](const std::string& text) {
        std::cout << "Text changed: " << text << std::endl;
    });
    
    textInput->onEnterPressed.connect([](const std::string& text) {
        std::cout << "Enter pressed with text: " << text << std::endl;
        outputText->setText("YOU TYPED: \"" + text + "\"");
    });
    
    textInput->onFocusChanged.connect([](bool focused) {
        std::cout << "FOCUS CHANGED: " << (focused ? "focused" : "unfocused") << std::endl;
    });
    
    // Create a clear button
    ButtonConfig clearButtonConfig = {
        .x = 470, .y = 130, .width = 80, .height = 35,
        .normalColor = Colors::Red,
        .hoverColor = Colors::LightRed,
        .pressColor = Colors::DarkRed,
        .label = "CLEAR",
        .textScale = 1,
        .textColor = Colors::White
    };
    
    auto clearButton = Button(clearButtonConfig);
    clearButton->onClick.connect([]() {
        textInput->clear();
        outputText->setText("TEXT CLEARED!");
        std::cout << "Text input cleared" << std::endl;
    });
    
    // Create a focus button for testing
    ButtonConfig focusButtonConfig = {
        .x = 50, .y = 270, .width = 120, .height = 30,
        .normalColor = Colors::Blue,
        .hoverColor = Colors::LightBlue,
        .pressColor = Colors::DarkBlue,
        .label = "FOCUS INPUT",
        .textScale = 1,
        .textColor = Colors::White
    };
    
    auto focusButton = Button(focusButtonConfig);
    focusButton->onClick.connect([]() {
        textInput->setFocus(true);
        std::cout << "Text input focused programmatically" << std::endl;
    });
    
    // Status display
    Text(Point(50, 320), "STATUS: CLICK INPUT FIELD TO FOCUS, TYPE TEXT, PRESS ENTER TO SUBMIT", 1, Colors::LightGray);
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
