#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    

    // Simple layout without complex nesting
    
    // Title
    addWidget(Text(Point(50, 30), "Fixed Text Input Demo", 3, Colors::DarkBlue, false));
    addWidget(Text(Point(50, 70), "Test: ABC123abc (should display correctly now!)", 2, Colors::Gray, false));
    
    // Text input 1: Simple style with good contrast
    addWidget(Text(Point(50, 120), "Basic Input:", 2, Colors::Black, false));
    addWidget(TextInput(TextInputConfig(50, 150, 300, 35)
        .placeholder("Type here...")
        .style(TextInputStyle()
            .backgroundColor(0xFFFFFF)      // White background
            .borderColor(0x888888)          // Gray border
            .focusBorderColor(0x0066CC)     // Blue when focused
            .textColor(0x000000)            // BLACK text (good contrast!)
            .cursorColor(0xFF0000)          // RED cursor (visible!)
            .fontSize(2)                    // Good size for bitmap
            .borderWidth(2)
            .padding(8)
            .useBitmapFont())));
    
    // Text input 2: Different style
    addWidget(Text(Point(50, 220), "Styled Input:", 2, Colors::Black, false));
    addWidget(TextInput(TextInputConfig(50, 250, 350, 40)
        .placeholder("Enter text here...")
        .maxLength(30)
        .style(TextInputStyle()
            .backgroundColor(0xF0F8FF)      // Very light blue background
            .borderColor(0x4169E1)          // Royal blue border
            .focusBorderColor(0xFF4500)     // Orange when focused
            .textColor(0x191970)            // Dark blue text (good contrast!)
            .cursorColor(0xFF1493)          // Pink cursor (very visible!)
            .fontSize(2)
            .borderWidth(3)
            .padding(10)
            .useBitmapFont())));
    
    // Instructions
    addWidget(Text(Point(50, 320), "Instructions:", 2, Colors::DarkGreen, false));
    addWidget(Text(Point(50, 350), "• Click in a box to focus it", 1, Colors::Black, false));
    addWidget(Text(Point(50, 370), "• Type letters and numbers", 1, Colors::Black, false));
    addWidget(Text(Point(50, 390), "• Use arrow keys and backspace", 1, Colors::Black, false));
    addWidget(Text(Point(50, 410), "• Notice the text is NOW VISIBLE!", 1, Colors::Red, false));
}

void draw() {
    Draw::fill(0xFFF5F5F5); // Light gray background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
