#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    std::vector<std::shared_ptr<Widget>> children;
    
    // Title
    children.push_back(Text(Point(0, 0), "Layout & Spacing Examples", 4, Colors::White));
    children.push_back(SizedBox(0, 30));
    
    // Problem demonstration
    children.push_back(Text(Point(0, 0), "PROBLEM: Text Overlapping Without Spacing", 3, Colors::Red));
    children.push_back(SizedBox(0, 20));
    
    // Bad example - overlapping text
    children.push_back(Text(Point(0, 0), "--- Overlapping Text Demo ---", 2, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    std::vector<std::shared_ptr<Widget>> badExample = {
        Text(Point(0, 0), "Title (Size 4)", 4, Colors::White),
        Text(Point(0, 0), "Subtitle (Size 2)", 2, Colors::Amber),
        // No spacing - these will overlap!
        Button(ButtonConfig(0, 0, 150, 40, "Action").style(ButtonStyle()))
    };
    
    children.push_back(Text(Point(0, 0), "Without SizedBox spacing:", 2, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    children.push_back(Column(badExample));
    children.push_back(SizedBox(0, 40));
    
    // Solution demonstration
    children.push_back(Text(Point(0, 0), "SOLUTION: Proper Text Height & Spacing", 3, Colors::Green));
    children.push_back(SizedBox(0, 20));
    
    children.push_back(Text(Point(0, 0), "--- Properly Spaced Demo ---", 2, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    std::vector<std::shared_ptr<Widget>> goodExample = {
        Text(Point(0, 0), "Title (Size 4)", 4, Colors::White),
        SizedBox(0, 25),  // Spacing proportional to text size
        Text(Point(0, 0), "Subtitle (Size 2)", 2, Colors::Amber),
        SizedBox(0, 20),  // Spacing before button
        Button(ButtonConfig(0, 0, 150, 40, "Action").style(ButtonStyle()))
    };
    
    children.push_back(Text(Point(0, 0), "With proper SizedBox spacing:", 2, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    children.push_back(Column(goodExample));
    children.push_back(SizedBox(0, 40));
    
    // Text height calculation guide
    children.push_back(Text(Point(0, 0), "Text Height Calculation Guide:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    children.push_back(Text(Point(0, 0), "Recommended spacing formulas:", 2, Colors::Cyan));
    children.push_back(SizedBox(0, 15));
    
    children.push_back(Text(Point(0, 0), "• Between different size texts: (larger_size + smaller_size) * 4", 1, Colors::White));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "• Between same size texts: text_size * 8", 1, Colors::White));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "• Before/after buttons: text_size * 10", 1, Colors::White));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "• Section separators: text_size * 15", 1, Colors::White));
    children.push_back(SizedBox(0, 25));
    
    // Different text sizes with calculated spacing
    children.push_back(Text(Point(0, 0), "Text Size Examples with Calculated Spacing:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    // Size 1 text
    children.push_back(Text(Point(0, 0), "Size 1 Text (8px bitmap)", 1, Colors::LightGray));
    children.push_back(SizedBox(0, 8));  // 1 * 8
    children.push_back(Text(Point(0, 0), "Another Size 1 Text", 1, Colors::LightGray));
    children.push_back(SizedBox(0, 20));  // Section separator
    
    // Size 2 text
    children.push_back(Text(Point(0, 0), "Size 2 Text (16px bitmap)", 2, Colors::White));
    children.push_back(SizedBox(0, 16));  // 2 * 8
    children.push_back(Text(Point(0, 0), "Another Size 2 Text", 2, Colors::White));
    children.push_back(SizedBox(0, 20));  // Section separator
    
    // Size 3 text
    children.push_back(Text(Point(0, 0), "Size 3 Text (24px bitmap)", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 24));  // 3 * 8
    children.push_back(Text(Point(0, 0), "Another Size 3 Text", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));  // Section separator
    
    // Size 4 text
    children.push_back(Text(Point(0, 0), "Size 4 Text (32px bitmap)", 4, Colors::Orange));
    children.push_back(SizedBox(0, 32));  // 4 * 8
    children.push_back(Text(Point(0, 0), "Another Size 4 Text", 4, Colors::Orange));
    children.push_back(SizedBox(0, 40));  // Larger section separator
    
    // Mixed sizes with proper spacing
    children.push_back(Text(Point(0, 0), "Mixed Sizes with Proper Spacing:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    children.push_back(Text(Point(0, 0), "Large Header", 4, Colors::White));
    children.push_back(SizedBox(0, 28));  // (4 + 2) * 4 = 24, rounded up to 28
    children.push_back(Text(Point(0, 0), "Medium Subtitle", 2, Colors::Amber));
    children.push_back(SizedBox(0, 16));  // (2 + 1) * 4 = 12, rounded up to 16
    children.push_back(Text(Point(0, 0), "Small description text goes here", 1, Colors::LightGray));
    children.push_back(SizedBox(0, 30));  // Before button: larger spacing
    
    // Button with proper spacing
    ButtonStyle actionStyle;
    actionStyle.normalColor(Colors::Blue)
              .hoverColor(Colors::LightBlue)
              .pressColor(Colors::DarkBlue)
              .textColor(Colors::White)
              .textScale(2);
    
    children.push_back(Button(ButtonConfig(0, 0, 150, 40, "Action Button").style(actionStyle)));
    children.push_back(SizedBox(0, 40));
    
    // Responsive spacing example
    children.push_back(Text(Point(0, 0), "Responsive Layout Example:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Scale spacing based on screen size
    int baseSpacing = std::max(10, width / 80);  // Minimum 10px, scales with width
    
    children.push_back(Text(Point(0, 0), "Responsive Title", 4, Colors::White));
    children.push_back(SizedBox(0, baseSpacing * 3));
    children.push_back(Text(Point(0, 0), "Responsive Subtitle", 2, Colors::Amber));
    children.push_back(SizedBox(0, baseSpacing * 2));
    
    // Responsive button width
    int buttonWidth = width * 0.4;  // Max 250px or 40% of screen
    children.push_back(Button(ButtonConfig(0, 0, buttonWidth, 45, "Responsive Button").style(actionStyle)));
    children.push_back(SizedBox(0, 30));
    
    // Best practices
    children.push_back(Text(Point(0, 0), "Layout Best Practices:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    std::vector<std::string> tips = {
        "1. Always use SizedBox for spacing between elements",
        "2. Calculate spacing based on text size: size * 8px minimum",
        "3. Use larger spacing before/after buttons and sections",
        "4. Consider screen size for responsive spacing",
        "5. Group related elements with smaller spacing",
        "6. Separate unrelated elements with larger spacing",
        "7. Test on different screen sizes",
        "8. Use consistent spacing throughout your app"
    };
    
    for (const auto& tip : tips) {
        children.push_back(Text(Point(0, 0), tip, 1, Colors::Cyan));
        children.push_back(SizedBox(0, 10));
    }
    
    // Center everything
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    std::cout << "🌿 Starting Layout & Spacing Demo..." << std::endl;
    
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
