#include <fern/fern.hpp>
#include <emscripten.h>

using namespace Fern;

void setupUI() {
    EM_ASM({
        console.log("🌿 Simple Color Test: Starting...");
    });
    
    // Very simple layout with VERY obvious colors
    
    // Title with obvious colors
    addWidget(Text(Point(50, 30), "COLOR TEST - Text Input", 3, Colors::Red, false)); 
    addWidget(Text(Point(50, 70), "The text should be BLACK on WHITE", 2, Colors::Black, false)); 
    
    // Simple text input with MAXIMUM contrast
    addWidget(Text(Point(50, 120), "Test input below:", 2, Colors::Blue, false)); 
    
    // Super simple text input with extreme contrast
    addWidget(TextInput(TextInputConfig(50, 150, 400, 50)
        .placeholder("GRAY placeholder text...")
        .style(TextInputStyle()
            .backgroundColor(Colors::White)         // PURE WHITE background
            .borderColor(Colors::Black)             // PURE BLACK border
            .focusBorderColor(Colors::Red)          // PURE RED when focused
            .textColor(Colors::Black)               // PURE BLACK text
            .cursorColor(Colors::Red)               // PURE RED cursor
            .fontSize(2)                            // Size 2 for bitmap
            .borderWidth(3)                         // Thick border
            .padding(10)                            // Good padding
            .useBitmapFont())));
    
    // Another one with different colors
    addWidget(Text(Point(50, 230), "Second test input:", 2, Colors::Green, false)); 
    
    addWidget(TextInput(TextInputConfig(50, 260, 400, 50)
        .placeholder("Another placeholder...")
        .style(TextInputStyle()
            .backgroundColor(Colors::Yellow)        // YELLOW background 
            .borderColor(Colors::Black)             // BLACK border
            .focusBorderColor(Colors::Blue)         // BLUE when focused
            .textColor(Colors::Black)               // BLACK text
            .cursorColor(Colors::Red)               // RED cursor
            .fontSize(2)
            .borderWidth(3)
            .padding(10)
            .useBitmapFont())));
    
    // Instructions
    addWidget(Text(Point(50, 350), "Instructions:", 2, Colors::Magenta, false)); 
    addWidget(Text(Point(50, 380), "• Click in boxes to focus", 2, Colors::Black, false));
    addWidget(Text(Point(50, 410), "• Type ABC123abc to test mapping", 2, Colors::Black, false));
    addWidget(Text(Point(50, 440), "• Text should be BLACK, placeholder GRAY", 2, Colors::Black, false));
    
    EM_ASM({
        console.log("🌿 Simple Color Test: UI setup complete");
    });
}

void draw() {
    Draw::fill(0xFFD3D3D3); // Light gray background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}