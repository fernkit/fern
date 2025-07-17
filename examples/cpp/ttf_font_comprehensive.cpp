#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Check if TTF fonts are available
    std::vector<std::shared_ptr<Widget>> children;
    
    // Title
    children.push_back(Text(Point(0, 0), "TTF Font Examples", 4, Colors::White));
    children.push_back(SizedBox(0, 30));
    
    // TTF Font Loading Examples
    children.push_back(Text(Point(0, 0), "Loading TTF Fonts:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    // Example 1: System fonts (common on most systems)
    try {
        // Load Arial font (common system font)
        auto arialFont = FontManager::loadTTFFont("Arial", 16);
        if (arialFont) {
            TextStyle arialStyle;
            arialStyle.useTTFFont("Arial")
                     .fontSize(16)
                     .color(Colors::LightBlue);
            
            auto arialText = Text(TextConfig(0, 0, "Arial Font - 16pt").style(arialStyle));
            children.push_back(arialText);
            children.push_back(SizedBox(0, 15));
        }
    } catch (const std::exception& e) {
        // Fallback to bitmap font with error message
        children.push_back(Text(Point(0, 0), "Arial font not available", 2, Colors::Red));
        children.push_back(SizedBox(0, 15));
    }
    
    // Example 2: DejaVu Sans (common on Linux)
    try {
        auto dejavuFont = FontManager::loadTTFFont("DejaVu Sans", 14);
        if (dejavuFont) {
            TextStyle dejavuStyle;
            dejavuStyle.useTTFFont("DejaVu Sans")
                      .fontSize(14)
                      .color(Colors::Green);
            
            auto dejavuText = Text(TextConfig(0, 0, "DejaVu Sans Font - 14pt").style(dejavuStyle));
            children.push_back(dejavuText);
            children.push_back(SizedBox(0, 15));
        }
    } catch (const std::exception& e) {
        children.push_back(Text(Point(0, 0), "DejaVu Sans font not available", 2, Colors::Red));
        children.push_back(SizedBox(0, 15));
    }
    
    // Example 3: Loading from file path
    children.push_back(Text(Point(0, 0), "Loading from File:", 2, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    
    // Try to load from common font paths
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/System/Library/Fonts/Arial.ttf",  // macOS
        "C:\\Windows\\Fonts\\arial.ttf"      // Windows
    };
    
    bool fontLoaded = false;
    for (const auto& fontPath : fontPaths) {
        try {
            auto fileFont = FontManager::loadTTFFont(fontPath, 18);
            if (fileFont) {
                TextStyle fileStyle;
                fileStyle.useTTFFont(fontPath)
                        .fontSize(18)
                        .color(Colors::Orange);
                
                auto fileText = Text(TextConfig(0, 0, "Font loaded from file - 18pt").style(fileStyle));
                children.push_back(fileText);
                fontLoaded = true;
                break;
            }
        } catch (const std::exception& e) {
            continue;
        }
    }
    
    if (!fontLoaded) {
        children.push_back(Text(Point(0, 0), "No TTF fonts found in system paths", 2, Colors::Red));
    }
    
    children.push_back(SizedBox(0, 30));
    
    // Different font sizes with TTF
    children.push_back(Text(Point(0, 0), "Font Size Variations:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    std::vector<int> sizes = {12, 16, 20, 24, 28};
    for (int size : sizes) {
        try {
            TextStyle sizeStyle;
            sizeStyle.useTTFFont("Arial")  // Fallback to system default if Arial not available
                    .fontSize(size)
                    .color(Colors::White);
            
            std::string text = "Font size " + std::to_string(size) + "pt";
            auto sizeText = Text(TextConfig(0, 0, text).style(sizeStyle));
            children.push_back(sizeText);
            children.push_back(SizedBox(0, 10));
        } catch (const std::exception& e) {
            // Fallback to bitmap font
            std::string text = "Font size " + std::to_string(size) + "pt (bitmap fallback)";
            children.push_back(Text(Point(0, 0), text, 2, Colors::Gray));
            children.push_back(SizedBox(0, 10));
        }
    }
    
    children.push_back(SizedBox(0, 30));
    
    // TTF Font with effects
    children.push_back(Text(Point(0, 0), "TTF with Effects:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    try {
        // Shadow effect
        TextStyle shadowStyle;
        shadowStyle.useTTFFont("Arial")
                  .fontSize(20)
                  .color(Colors::White)
                  .shadow(true, Colors::Black, 2);
        
        auto shadowText = Text(TextConfig(0, 0, "TTF with Shadow").style(shadowStyle));
        children.push_back(shadowText);
        children.push_back(SizedBox(0, 15));
        
        // Background effect
        TextStyle bgStyle;
        bgStyle.useTTFFont("Arial")
              .fontSize(18)
              .color(Colors::White)
              .backgroundColor(Colors::DarkBlue)
              .padding(10);
        
        auto bgText = Text(TextConfig(0, 0, "TTF with Background").style(bgStyle));
        children.push_back(bgText);
        children.push_back(SizedBox(0, 15));
        
    } catch (const std::exception& e) {
        children.push_back(Text(Point(0, 0), "TTF effects not available", 2, Colors::Red));
    }
    
    // Dynamic font loading example
    children.push_back(SizedBox(0, 30));
    children.push_back(Text(Point(0, 0), "Dynamic Font Loading:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 20));
    
    // Button to toggle font
    static bool usingTTF = false;
    static std::shared_ptr<TextWidget> dynamicText;
    
    // Create dynamic text
    dynamicText = Text(Point(0, 0), "Click button to toggle font type", 2, Colors::White);
    children.push_back(dynamicText);
    children.push_back(SizedBox(0, 15));
    
    // Toggle button
    ButtonStyle toggleStyle;
    toggleStyle.normalColor(Colors::Blue)
              .hoverColor(Colors::LightBlue)
              .pressColor(Colors::DarkBlue)
              .textColor(Colors::White)
              .textScale(2);
    
    auto toggleButton = Button(ButtonConfig(0, 0, 200, 40, "Toggle Font Type").style(toggleStyle));
    toggleButton->onClick.connect([dynamicText]() {
        static bool usingTTF = false;
        usingTTF = !usingTTF;
        
        if (usingTTF) {
            try {
                TextStyle ttfStyle;
                ttfStyle.useTTFFont("Arial")
                       .fontSize(16)
                       .color(Colors::Green);
                
                dynamicText->setConfig(TextConfig(0, 0, "Now using TTF font!").style(ttfStyle));
            } catch (const std::exception& e) {
                dynamicText->setText("TTF font not available, using bitmap");
                dynamicText->setColor(Colors::Red);
            }
        } else {
            dynamicText->setText("Now using bitmap font!");
            dynamicText->setColor(Colors::White);
            dynamicText->setSize(2);
        }
    });
    
    children.push_back(toggleButton);
    children.push_back(SizedBox(0, 30));
    
    // Font installation instructions
    children.push_back(Text(Point(0, 0), "Font Installation Tips:", 3, Colors::Yellow));
    children.push_back(SizedBox(0, 15));
    children.push_back(Text(Point(0, 0), "Ubuntu/Debian: sudo apt install fonts-liberation", 1, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    children.push_back(Text(Point(0, 0), "Arch Linux: sudo pacman -S ttf-liberation", 1, Colors::Cyan));
    children.push_back(SizedBox(0, 10));
    children.push_back(Text(Point(0, 0), "Or place .ttf files in: ~/.local/share/fonts/", 1, Colors::Cyan));
    
    // Center everything
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
    std::cout << "🌿 Starting TTF Font Demo..." << std::endl;
    
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
