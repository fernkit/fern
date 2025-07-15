#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

void draw() {
    Draw::fill(Colors::DarkBlue);
}

static int clickCount = 0;
static std::string currentMessage = "Hello, World!";

int main() {
    std::cout << "Testing layout button event handling with text updates..." << std::endl;
    
    Fern::initialize();
    
    // Get actual screen dimensions
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Create text widgets - positioned by layouts, so coordinates are (0,0)
    auto titleText = Text(Point(0,0), "Layout Button & Text Demo", 3, Colors::White, false, Fern::FontType::Bitmap);
    auto counterText = Text(Point(0,0), "Click count: 0", 2, Colors::Yellow, false, Fern::FontType::Bitmap);
    auto messageText = Text(Point(0,0), currentMessage, 2, Colors::LightGray, false, Fern::FontType::Bitmap);
    auto instructionText = Text(Point(0,0), "All elements are positioned by layouts!", 1, Colors::LightBlue, false, Fern::FontType::Bitmap);
    
    // Debug: Print text widget dimensions
    std::cout << "Title text dimensions: " << titleText->getWidth() << "x" << titleText->getHeight() << std::endl;
    std::cout << "Counter text dimensions: " << counterText->getWidth() << "x" << counterText->getHeight() << std::endl;
    std::cout << "Message text dimensions: " << messageText->getWidth() << "x" << messageText->getHeight() << std::endl;
    std::cout << "Instruction text dimensions: " << instructionText->getWidth() << "x" << instructionText->getHeight() << std::endl;
    
    // // Create buttons - positioned by layouts, so coordinates are (0,0)
    // auto incrementButton = Button(ButtonConfig{0, 0, 150, 40, Colors::Blue, Colors::LightBlue, Colors::DarkBlue, "Count +1", 1, Colors::White});
    // auto resetButton = Button(ButtonConfig{0, 0, 150, 40, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Reset", 1, Colors::White});
    // auto messageButton = Button(ButtonConfig{0, 0, 150, 40, Colors::Red, Colors::LightRed, Colors::DarkRed, "Change Msg", 1, Colors::White});

    // // Connect button events
    // incrementButton->onClick.connect([counterText](){
    //     clickCount++;
    //     counterText->setText("Click count: " + std::to_string(clickCount));
    //     std::cout << "Increment clicked! Count: " << clickCount << std::endl;
    // });

    // resetButton->onClick.connect([counterText](){
    //     clickCount = 0;
    //     counterText->setText("Click count: 0");
    //     std::cout << "Reset clicked!" << std::endl;
    // });
    
    // messageButton->onClick.connect([messageText](){
    //     static int msgIndex = 0;
    //     std::vector<std::string> messages = {
    //         "Hello, World!",
    //         "Layouts are working!",
    //         "Button events work too!",
    //         "No manual positioning needed!",
    //         "Everything is automatic!"
    //     };
    //     msgIndex = (msgIndex + 1) % messages.size();
    //     currentMessage = messages[msgIndex];
    //     messageText->setText(currentMessage);
    //     std::cout << "Message changed to: " << currentMessage << std::endl;
    // });
    
    // // Create button row layout
    // auto buttonRow = Row({incrementButton, resetButton, messageButton}, true, MainAxisAlignment::Center, CrossAxisAlignment::Center);
    
    // Create main column layout with proper spacing
    // auto mainColumn = Column({
    //     titleText,
    //     SizedBox(0, 20, false),
    //     counterText,
    //     SizedBox(0, 20, false),
    //     messageText,
    //     SizedBox(0, 20, false),
    //     instructionText
    // }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);

    auto mainColumn = Column({
        titleText,
        SizedBox(0, 20, false),
        counterText,
        SizedBox(0, 20, false),
        messageText,
        SizedBox(0, 20, false),
        instructionText
    }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
    
    // Add the entire layout to the widget manager wrapped in a full-screen container
    addWidget(
        Container(
            Colors::Transparent,
            0, 0, width, height,  // Use actual screen dimensions
            Center(mainColumn, false)
        )
    );
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
