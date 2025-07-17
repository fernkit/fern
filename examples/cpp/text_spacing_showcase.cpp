#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Text layout spacing examples
    
    // INCORRECT: Text overlapping without spacing
    std::vector<std::shared_ptr<Widget>> badExample = {
        Text(Point(0, 0), "Title (overlapping)", 4, Colors::Red),
        Text(Point(0, 0), "Subtitle (no spacing)", 2, Colors::Red),
        Text(Point(0, 0), "Body text runs into subtitle", 2, Colors::Red)
    };
    
    // CORRECT: Proper spacing between text elements
    std::vector<std::shared_ptr<Widget>> goodExample = {
        Text(Point(0, 0), "Title (properly spaced)", 4, Colors::Green),
        SizedBox(0, 32),  // Height for size 4 text: 4 * 8 = 32 pixels
        Text(Point(0, 0), "Subtitle (with spacing)", 2, Colors::Green),
        SizedBox(0, 16),  // Height for size 2 text: 2 * 8 = 16 pixels
        Text(Point(0, 0), "Body text with proper spacing", 2, Colors::Green)
    };
    
    // Text height calculation examples
    std::vector<std::shared_ptr<Widget>> heightDemo = {
        Text(Point(0, 0), "Size 1 (8px height)", 1, Colors::White),
        SizedBox(0, 8),   // 1 * 8 = 8 pixels
        Text(Point(0, 0), "Size 2 (16px height)", 2, Colors::White),
        SizedBox(0, 16),  // 2 * 8 = 16 pixels
        Text(Point(0, 0), "Size 3 (24px height)", 3, Colors::White),
        SizedBox(0, 24),  // 3 * 8 = 24 pixels
        Text(Point(0, 0), "Size 4 (32px height)", 4, Colors::White)
    };
    
    // Spacing formula examples
    auto formulaText = Text(Point(0, 0), "Spacing Formula: text_height = size * 8 pixels", 2, Colors::Yellow);
    auto tipText = Text(Point(0, 0), "Tip: Use SizedBox(0, text_height) for proper spacing", 2, Colors::Cyan);
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Text Layout & Spacing Guide", 3, Colors::White),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "❌ INCORRECT - No spacing (overlapping):", 2, Colors::Red),
        SizedBox(0, 20),
        Column(badExample),
        SizedBox(0, 40),
        
        Text(Point(0, 0), "✅ CORRECT - Proper spacing:", 2, Colors::Green),
        SizedBox(0, 20),
        Column(goodExample),
        SizedBox(0, 40),
        
        Text(Point(0, 0), "Text Height Calculation Demo:", 2, Colors::LightBlue),
        SizedBox(0, 20),
        Column(heightDemo),
        SizedBox(0, 40),
        
        Text(Point(0, 0), "Spacing Guidelines:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        formulaText,
        SizedBox(0, 10),
        tipText,
        SizedBox(0, 20),
        Text(Point(0, 0), "For related content: use 50-75% of text height", 1, Colors::LightGray),
        SizedBox(0, 8),
        Text(Point(0, 0), "For sections: use 150-200% of text height", 1, Colors::LightGray)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
