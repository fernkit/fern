#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {

    
    // Load TTF font
    bool fontLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
        TTF::setDefault("roboto");
        
    
    // Very simple layout with TTF fonts everywhere
    
    // Title with TTF font - larger size for TTF
    addWidget(Text(Point(50, 30), "TTF COLOR TEST - Text Input", 48, Colors::Red, true, FontType::TTF)); 
    addWidget(Text(Point(50, 90), "All text uses TTF fonts - should be crisp and clear", 24, Colors::Black, true, FontType::TTF)); 
    
    // Simple text input with TTF
    addWidget(Text(Point(50, 140), "Test TTF input below", 28, Colors::Blue, true, FontType::TTF)); 
    
    // TTF text input with high contrast
    addWidget(TextInput(TextInputConfig(50, 180, 500, 60)
        .placeholder("TTF placeholder text...")
        .style(TextInputStyle()
            .backgroundColor(Colors::White)         // PURE WHITE background
            .borderColor(Colors::Black)             // PURE BLACK border
            .focusBorderColor(Colors::Red)          // PURE RED when focused
            .textColor(Colors::Black)               // PURE BLACK text
            .cursorColor(Colors::Red)               // PURE RED cursor
            .fontSize(24)                           // Good size for TTF
            .borderWidth(3)                         // Thick border
            .padding(12)                            // Good padding for TTF
            .useTTFFont("roboto"))));               // Use TTF font
    
    // Another TTF input with different colors
    addWidget(Text(Point(50, 270), "Second TTF input", 28, Colors::Green, true, FontType::TTF)); 
    
    addWidget(TextInput(TextInputConfig(50, 310, 500, 60)
        .placeholder("Another TTF placeholder...")
        .style(TextInputStyle()
            .backgroundColor(Colors::LightBlue)     // Light blue background 
            .borderColor(Colors::DarkBlue)          // Dark blue border
            .focusBorderColor(Colors::Orange)       // Orange when focused
            .textColor(Colors::DarkBlue)            // Dark blue text
            .cursorColor(Colors::Red)               // Red cursor
            .fontSize(24)                           // TTF size
            .borderWidth(3)
            .padding(12)
            .useTTFFont("roboto"))));
    
    
    // TTF Instructions
    addWidget(Text(Point(50, 530), "TTF Instructions:", 32, Colors::Magenta, true, FontType::TTF)); 
    addWidget(Text(Point(50, 570), "• Click in boxes to focus them", 20, Colors::Black, true, FontType::TTF));
    addWidget(Text(Point(50, 600), "• Type ABC123abc to test character mapping", 20, Colors::Black, true, FontType::TTF));
    addWidget(Text(Point(50, 630), "• TTF text should be crisp and scalable", 20, Colors::Black, true, FontType::TTF));
    addWidget(Text(Point(50, 660), "• Notice smooth fonts vs pixelated bitmap", 20, Colors::DarkGreen, true, FontType::TTF));
   
}

void draw() {
    Draw::fill(0xFFF5F5F5); // Very light gray background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}


