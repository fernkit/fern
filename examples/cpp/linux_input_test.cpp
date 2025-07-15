#include "fern/fern.hpp"
#include <iostream>

using namespace Fern;

static std::shared_ptr<TextWidget> statusText;
static std::shared_ptr<TextWidget> inputText;
static std::shared_ptr<ButtonWidget> testButton;
static int mouseX = 0, mouseY = 0;
static bool mouseDown = false;

void setupUI() {
    std::cout << "=== Linux Input Test ===" << std::endl;
    
    // Status display
    statusText = Text(Point(50, 50), "Move mouse and click to test input", 2, Colors::White);
    inputText = Text(Point(50, 100), "Mouse: (0, 0) - Not pressed", 2, Colors::Green);
    
    // Test button
    ButtonConfig config = {
        .x = 50, .y = 200, .width = 200, .height = 50,
        .normalColor = Colors::Blue,
        .hoverColor = Colors::LightBlue,
        .pressColor = Colors::DarkBlue,
        .label = "Test Button",
        .textScale = 2,
        .textColor = Colors::White
    };
    testButton = Button(config);
    
    // Set up button events
    testButton->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
        statusText->setText("Button clicked at mouse (" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")");
    });
    
    testButton->onHover.connect([](bool hovered) {
        std::cout << "Button " << (hovered ? "hovered" : "unhovered") << std::endl;
        if (hovered) {
            statusText->setText("Button is being hovered");
        } else {
            statusText->setText("Button hover ended");
        }
    });
    
    testButton->onPress.connect([](bool pressed) {
        std::cout << "Button " << (pressed ? "pressed" : "released") << std::endl;
    });
}

void draw() {
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Get current input state
    const auto& input = Input::getState();
    mouseX = input.mouseX;
    mouseY = input.mouseY;
    mouseDown = input.mouseDown;
    
    // Update input display
    std::string mouseInfo = "Mouse: (" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";
    if (mouseDown) {
        mouseInfo += " - PRESSED";
    } else {
        mouseInfo += " - Released";
    }
    inputText->setText(mouseInfo);
    
    // Draw instructions
    DrawText::drawText("Linux Input Test", 50, 20, 2, Colors::White);
    DrawText::drawText("Move mouse, click, and interact with button", 50, 300, 1, Colors::LightGray);
    DrawText::drawText("Check console for detailed input events", 50, 320, 1, Colors::LightGray);
    
    // Draw mouse cursor position
    Draw::circle(mouseX, mouseY, 5, mouseDown ? Colors::Red : Colors::Yellow);
}

int main() {
    std::cout << "=== Testing Linux Input Fixes ===" << std::endl;
    
    // Initialize Fern
    Fern::initialize();
    
    // Set up UI
    setupUI();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start render loop
    Fern::startRenderLoop();
    
    return 0;
}
