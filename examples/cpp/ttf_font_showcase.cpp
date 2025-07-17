#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // TTF Font examples
    
    // System font example
    TextStyle systemStyle;
    systemStyle.useTTFFont("Arial")  // Try to use system Arial font
              .fontSize(24)
              .color(Colors::White);
    
    auto systemText = Text(TextConfig(0, 0, "System Font (Arial, 24pt)").style(systemStyle));
    
    // Different TTF font sizes
    std::vector<std::shared_ptr<Widget>> sizeExamples;
    std::vector<int> sizes = {12, 16, 20, 24, 32};
    std::vector<uint32_t> colors = {Colors::White, Colors::Yellow, Colors::Cyan, Colors::Green, Colors::Orange};
    
    for (size_t i = 0; i < sizes.size(); i++) {
        TextStyle sizeStyle;
        sizeStyle.useTTFFont("Arial")
                .fontSize(sizes[i])
                .color(colors[i]);
        
        std::string text = "TTF " + std::to_string(sizes[i]) + "pt";
        auto sizeText = Text(TextConfig(0, 0, text).style(sizeStyle));
        sizeExamples.push_back(sizeText);
        sizeExamples.push_back(SizedBox(0, 10));
    }
    
    // TTF with effects
    TextStyle shadowStyle;
    shadowStyle.useTTFFont("Arial")
              .fontSize(20)
              .color(Colors::Yellow)
              .shadow(true, Colors::Black, 2);
    
    auto shadowText = Text(TextConfig(0, 0, "TTF with Shadow Effect").style(shadowStyle));
    
    TextStyle backgroundStyle;
    backgroundStyle.useTTFFont("Arial")
                  .fontSize(18)
                  .color(Colors::White)
                  .backgroundColor(Colors::DarkBlue)
                  .padding(10);
    
    auto backgroundText = Text(TextConfig(0, 0, "TTF with Background").style(backgroundStyle));
    
    // Fallback to bitmap font example
    TextStyle fallbackStyle;
    fallbackStyle.useTTFFont("NonExistentFont")  // This will fallback to bitmap
                .fontSize(3)  // Bitmap scale
                .color(Colors::Red);
    
    auto fallbackText = Text(TextConfig(0, 0, "Fallback to Bitmap (if TTF unavailable)").style(fallbackStyle));
    
    // Font loading from file example (commented - requires font file)
    /*
    TextStyle fileStyle;
    fileStyle.useTTFFont("custom_font.ttf")  // Load from file
            .fontSize(22)
            .color(Colors::Magenta);
    
    auto fileText = Text(TextConfig(0, 0, "Custom Font from File").style(fileStyle));
    */
    
    // Dynamic font switching example
    static std::shared_ptr<TextWidget> dynamicText;
    static bool useTTF = true;
    
    auto updateFont = []() {
        if (useTTF) {
            TextStyle ttfStyle;
            ttfStyle.useTTFFont("Arial")
                   .fontSize(20)
                   .color(Colors::Green);
            dynamicText->setConfig(TextConfig(0, 0, "Using TTF Font").style(ttfStyle));
        } else {
            TextStyle bitmapStyle;
            bitmapStyle.useBitmapFont()
                      .fontSize(3)
                      .color(Colors::Blue);
            dynamicText->setConfig(TextConfig(0, 0, "Using Bitmap Font").style(bitmapStyle));
        }
        useTTF = !useTTF;
    };
    
    dynamicText = Text(TextConfig(0, 0, "Using TTF Font").style(
        TextStyle().useTTFFont("Arial").fontSize(20).color(Colors::Green)));
    
    ButtonStyle switchStyle;
    switchStyle.normalColor(Colors::Purple)
              .hoverColor(Colors::Magenta)
              .pressColor(Colors::DarkMagenta)
              .textColor(Colors::White)
              .textScale(2);
    
    auto switchButton = Button(ButtonConfig(0, 0, 150, 40, "Switch Font").style(switchStyle));
    switchButton->onClick.connect(updateFont);
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "TTF Font Showcase", 4, Colors::White),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "System TTF Font:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        systemText,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Different TTF Sizes:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        Column(sizeExamples),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "TTF with Effects:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        shadowText,
        SizedBox(0, 15),
        backgroundText,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Dynamic Font Switching:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        dynamicText,
        SizedBox(0, 15),
        switchButton,
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Fallback Example:", 2, Colors::LightBlue),
        SizedBox(0, 15),
        fallbackText
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
