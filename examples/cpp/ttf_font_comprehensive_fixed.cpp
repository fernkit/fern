#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    std::vector<std::shared_ptr<Widget>> children;
    
    // STEP 1: Load TTF fonts explicitly at the top using Font::loadTTFFont()
    std::cout << "Loading TTF fonts..." << std::endl;
    
    // Try to load common system fonts
    std::vector<std::pair<std::string, std::string>> fontOptions = {
        {"arial", "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"},
        {"dejavu", "/usr/share/fonts/TTF/DejaVuSans.ttf"},
        {"ubuntu", "/usr/share/fonts/truetype/ubuntu/Ubuntu-Regular.ttf"},
        {"arial", "/System/Library/Fonts/Arial.ttf"},                   // macOS
        {"arial", "C:\\Windows\\Fonts\\arial.ttf"},                    // Windows
        {"custom", "assets/fonts/roboto.ttf"}                          // Custom font
    };
    
    std::string loadedFontName = "";
    for (const auto& fontOption : fontOptions) {
        if (Font::loadTTFFont(fontOption.first, fontOption.second)) {
            loadedFontName = fontOption.first;
            std::cout << "Successfully loaded font: " << fontOption.first << " from " << fontOption.second << std::endl;
            break;
        }
    }
    
    // STEP 2: Set the default TTF font if one was loaded
    if (!loadedFontName.empty()) {
        Font::setDefaultTTFFont(loadedFontName);
        std::cout << "Set default TTF font to: " << loadedFontName << std::endl;
    }
    
    // STEP 3: Create UI widgets using the loaded TTF fonts
    
    // Title
    children.push_back(Text(Point(0, 0), "TTF Font Loading Example", 4, Colors::White));
    children.push_back(SizedBox(0, 30));
    
    // Show font loading status
    if (!loadedFontName.empty()) {
        std::string statusText = "✓ TTF Font loaded: " + loadedFontName;
        children.push_back(Text(Point(0, 0), statusText, 2, Colors::Green));
    } else {
        children.push_back(Text(Point(0, 0), "✗ No TTF fonts available - using bitmap fallback", 2, Colors::Red));
    }
    children.push_back(SizedBox(0, 20));
    
    // Example 1: Using the loaded TTF font with TextStyle
    if (!loadedFontName.empty()) {
        children.push_back(Text(Point(0, 0), "TTF Font Examples:", 3, Colors::Yellow));
        children.push_back(SizedBox(0, 15));
        
        // Basic TTF text
        TextStyle basicStyle;
        basicStyle.useTTFFont(loadedFontName)  // Use the loaded font name
                 .fontSize(20)
                 .color(Colors::LightBlue);
        
        auto basicText = Text(TextConfig(0, 0, "Basic TTF text - 20pt").style(basicStyle));
        children.push_back(basicText);
        children.push_back(SizedBox(0, 15));
        
        // TTF with different sizes
        std::vector<int> sizes = {16, 24, 32};
        std::vector<uint32_t> colors = {Colors::White, Colors::Cyan, Colors::Orange};
        
        for (size_t i = 0; i < sizes.size(); i++) {
            TextStyle sizeStyle;
            sizeStyle.useTTFFont(loadedFontName)
                    .fontSize(sizes[i])
                    .color(colors[i]);
            
            std::string text = "TTF " + std::to_string(sizes[i]) + "pt";
            auto sizeText = Text(TextConfig(0, 0, text).style(sizeStyle));
            children.push_back(sizeText);
            children.push_back(SizedBox(0, 10));
        }
        
        children.push_back(SizedBox(0, 20));
        
        // TTF with effects
        TextStyle shadowStyle;
        shadowStyle.useTTFFont(loadedFontName)
                  .fontSize(24)
                  .color(Colors::Yellow)
                  .shadow(true, Colors::Black, 2);
        
        auto shadowText = Text(TextConfig(0, 0, "TTF with Shadow").style(shadowStyle));
        children.push_back(shadowText);
        children.push_back(SizedBox(0, 15));
        
        // TTF with background
        TextStyle bgStyle;
        bgStyle.useTTFFont(loadedFontName)
              .fontSize(18)
              .color(Colors::White)
              .backgroundColor(Colors::DarkBlue)
              .padding(10);
        
        auto bgText = Text(TextConfig(0, 0, "TTF with Background").style(bgStyle));
        children.push_back(bgText);
        children.push_back(SizedBox(0, 20));
    }
    
    // Example 2: Fallback to bitmap fonts
    children.push_back(Text(Point(0, 0), "Bitmap Font Fallback:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    
    // Regular bitmap text
    children.push_back(Text(Point(0, 0), "Bitmap font text (always available)", 2, Colors::White));
    children.push_back(SizedBox(0, 15));
    
    // Mixed usage example
    if (!loadedFontName.empty()) {
        children.push_back(Text(Point(0, 0), "Mixed Usage Example:", 3, Colors::Yellow));
        children.push_back(SizedBox(0, 15));
        
        // TTF title
        TextStyle titleStyle;
        titleStyle.useTTFFont(loadedFontName)
                 .fontSize(28)
                 .color(Colors::Green);
        
        auto titleText = Text(TextConfig(0, 0, "TTF Title").style(titleStyle));
        children.push_back(titleText);
        children.push_back(SizedBox(0, 10));
        
        // Bitmap subtitle
        children.push_back(Text(Point(0, 0), "Bitmap subtitle", 2, Colors::Gray));
        children.push_back(SizedBox(0, 20));
    }
    
    // Instructions
    children.push_back(Text(Point(0, 0), "How to use TTF fonts:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    children.push_back(Text(Point(0, 0), "1. Font::loadTTFFont(\"name\", \"/path/to/font.ttf\")", 1, Colors::White));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "2. Font::setDefaultTTFFont(\"name\")", 1, Colors::White));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "3. TextStyle().useTTFFont(\"name\").fontSize(20)", 1, Colors::White));
    children.push_back(SizedBox(0, 20));
    
    // Font installation tips
    children.push_back(Text(Point(0, 0), "Install fonts:", 2, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    children.push_back(Text(Point(0, 0), "Ubuntu: sudo apt install fonts-liberation", 1, Colors::Gray));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "Arch: sudo pacman -S ttf-liberation", 1, Colors::Gray));
    children.push_back(SizedBox(0, 8));
    children.push_back(Text(Point(0, 0), "Or copy .ttf to ~/.local/share/fonts/", 1, Colors::Gray));
    
    // Create the main layout
    auto mainColumn = Column(children);
    
    // Center everything
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(mainColumn);
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    std::cout << "🌿 Starting TTF Font Demo..." << std::endl;
    
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
