#include <fern/fern.hpp>
#include <emscripten.h>

using namespace Fern;

void setupUI() {
    EM_ASM({
        console.log("🌿 Baseline Alignment Test: Starting...");
    });
    
    // Load TTF font
    bool fontLoaded = TTF::load("roboto", "fonts/RobotoMono-VariableFont_wght.ttf");
    if (fontLoaded) {
        EM_ASM({
            console.log("🌿 ✅ TTF Font loaded successfully!");
        });
    } else {
        EM_ASM({
            console.log("🌿 ❌ TTF Font failed to load");
        });
    }
    
    // Draw baseline reference lines
    int baselineY1 = 100;
    int baselineY2 = 200;
    int baselineY3 = 300;
    
    // Draw horizontal reference lines for baselines
    addWidget(Line(Point(30, baselineY1), Point(750, baselineY1), 1, Colors::Red));
    addWidget(Line(Point(30, baselineY2), Point(750, baselineY2), 1, Colors::Red));
    addWidget(Line(Point(30, baselineY3), Point(750, baselineY3), 1, Colors::Red));
    
    // Title
    addWidget(Text(Point(50, 30), "Baseline Alignment Test", 3, Colors::DarkBlue, false));
    addWidget(Text(Point(50, 60), "Red lines = baselines. Text should sit ON the lines.", 2, Colors::Gray, false));
    
    // Test bitmap font alignment (should already work correctly)
    addWidget(Text(Point(50, 80), "Bitmap (size 2):", 1, Colors::Black, false));
    addWidget(Text(Point(200, baselineY1), "Bitmap: ABCDabcd123gyp", 2, Colors::Black, false));
    
    // Test TTF font alignment (should now work correctly with baseline fix)
    if (fontLoaded) {
        addWidget(Text(Point(50, 180), "TTF (24pt):", 1, Colors::Black, false));
        addWidget(Text(Point(200, baselineY2), "TTF Font: ABCDabcd123gyp", 24, Colors::Black, true, FontType::TTF));
        
        addWidget(Text(Point(50, 280), "TTF (32pt):", 1, Colors::Black, false));
        addWidget(Text(Point(200, baselineY3), "TTF Large: ABCDabcd123gyp", 32, Colors::Black, true, FontType::TTF));
    }
    
    // Test text input with baseline alignment
    addWidget(Text(Point(50, 350), "Text Input Test:", 2, Colors::DarkGreen, false));
    
    // Bitmap input (baseline reference)
    addWidget(Text(Point(50, 380), "Bitmap input:", 1, Colors::Black, false));
    addWidget(TextInput(TextInputConfig(200, 380, 300, 30)
        .placeholder("Bitmap: type ABCgyp...")
        .style(TextInputStyle()
            .backgroundColor(Colors::White)
            .borderColor(Colors::Gray)
            .focusBorderColor(Colors::Blue)
            .textColor(Colors::Black)
            .fontSize(2)
            .useBitmapFont())));
    
    // TTF input (should now align to baseline correctly)
    if (fontLoaded) {
        addWidget(Text(Point(50, 430), "TTF input:", 1, Colors::Black, false));
        addWidget(TextInput(TextInputConfig(200, 430, 400, 45)
            .placeholder("TTF: type ABCgyp...")
            .style(TextInputStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .focusBorderColor(Colors::Blue)
                .textColor(Colors::Black)
                .fontSize(24)
                .useTTFFont("roboto"))));
    }
    
    // Instructions
    addWidget(Text(Point(50, 500), "Instructions:", 2, Colors::Purple, false));
    addWidget(Text(Point(50, 525), "• Text should sit ON the red baseline lines", 1, Colors::Black, false));
    addWidget(Text(Point(50, 545), "• Descenders (g, y, p) should go below the line", 1, Colors::Black, false));
    addWidget(Text(Point(50, 565), "• Capital letters should touch or nearly touch the line", 1, Colors::Black, false));
    addWidget(Text(Point(50, 585), "• Type in inputs to test baseline alignment", 1, Colors::Black, false));
    
    EM_ASM({
        console.log("🌿 Baseline Alignment Test: UI setup complete");
    });
}

void draw() {
    Draw::fill(0xFFF8F8F8); // Very light background
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}

/*
This example tests the baseline alignment fix for TTF fonts.

🔧 WHAT WAS FIXED:
- TTF fonts were rendering with center alignment
- Now they use proper baseline alignment like real typography

✅ EXPECTED BEHAVIOR:
- Text should sit ON the red baseline lines
- Letters like 'g', 'y', 'p' should extend below the baseline
- Capital letters should align properly with the baseline
- Text inputs should position text correctly

🏗️ TECHNICAL DETAILS:
- Fixed bearingY calculation: now uses glyph.header.yMax * scale
- Y coordinate passed to renderText is treated as baseline position
- Proper typography positioning for professional appearance

Build with: fern --cpp --embed-file fonts baseline_test.cpp
*/
