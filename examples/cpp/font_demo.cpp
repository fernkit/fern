#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

void setupUI() {
    // Clear any previous widgets
    WidgetManager::getInstance().clear();
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Try to load RobotoMono TTF font
    bool ttfLoaded = TTF::load("roboto", "../../fonts/RobotoMono-VariableFont_wght.ttf");
    
    if (ttfLoaded) {
        TTF::setDefault("roboto");
        std::cout << "✅ RobotoMono TTF font loaded successfully!" << std::endl;
    } else {
        std::cout << "❌ TTF font loading failed, using bitmap font only" << std::endl;
    }
    
    addWidget(
        Container(
            Colors::White,
            0, 0, width, height,
            
            Column({
                // Header section
                Container(
                    Colors::DarkBlue,
                    0, 0, 0, 80,
                    Center(Text(Point(0, 0), "FERN FONT COMPARISON DEMO", 3, Colors::White, false, FontType::Bitmap))
                ),
                                
                // Font type labels
                Row({
                    Container(
                        Colors::LightGray,
                        0, 0, width/2 - 10, 40,
                        Center(Text(Point(0, 0), "BITMAP FONT", 2, Colors::Black, false, FontType::Bitmap))
                    ),
                    Container(
                        Colors::LightBlue,
                        0, 0, width/2 - 10, 40,
                        Center(Text(Point(0, 0), ttfLoaded ? "TTF FONT (ROBOTO)" : "TTF NOT LOADED", 2, Colors::Black, false, FontType::Bitmap))
                    )
                }, false),
                
                
                // Size comparison section
                Text(Point(20, 0), "SIZE COMPARISON:", 2, Colors::DarkGray, false, FontType::Bitmap),
                
                // Large text comparison
                Row({
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 80,
                        Column({
                            Text(Point(10, 10), "LARGE 36", 4, Colors::Black, false, FontType::Bitmap),
                            Text(Point(10, 50), "MEDIUM 24", 3, Colors::DarkBlue, false, FontType::Bitmap)
                        }, false)
                    ),
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 80,
                        Column({
                            Text(Point(10, 10), "LARGE 36", 4, Colors::Black, false, FontType::TTF),
                            Text(Point(10, 50), "MEDIUM 24", 3, Colors::DarkBlue, false, FontType::TTF)
                        }, false)
                    )
                }, false),
                
                
                // Alphabet comparison
                Text(Point(20, 0), "ALPHABET COMPARISON:", 2, Colors::DarkGray, false, FontType::Bitmap),
                
                Row({
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 120,
                        Column({
                            Text(Point(10, 10), "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, Colors::Black, false, FontType::Bitmap),
                            Text(Point(10, 40), "0123456789", 2, Colors::Red, false, FontType::Bitmap),
                            Text(Point(10, 70), "SPECIAL: !@#$%^&*()", 2, Colors::Green, false, FontType::Bitmap)
                        }, false)
                    ),
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 120,
                        Column({
                            Text(Point(10, 10), "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, Colors::Black, false, FontType::TTF),
                            Text(Point(10, 40), "0123456789", 2, Colors::Red, false, FontType::TTF),
                            Text(Point(10, 70), "SPECIAL: !@#$%^&*()", 2, Colors::Green, false, FontType::TTF)
                        }, false)
                    )
                }, false),
                
                
                // Sample text comparison
                Text(Point(20, 0), "SAMPLE TEXT:", 2, Colors::DarkGray, false, FontType::Bitmap),
                
                Row({
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 100,
                        Column({
                            Text(Point(10, 10), "THE QUICK BROWN", 2, Colors::Black, false, FontType::Bitmap),
                            Text(Point(10, 40), "FOX JUMPS OVER", 2, Colors::Black, false, FontType::Bitmap),
                            Text(Point(10, 70), "THE LAZY DOG", 2, Colors::Black, false, FontType::Bitmap)
                        }, false)
                    ),
                    Container(
                        Colors::White,
                        0, 0, width/2 - 10, 100,
                        Column({
                            Text(Point(10, 10), "THE QUICK BROWN", 2, Colors::Black, false, FontType::TTF),
                            Text(Point(10, 40), "FOX JUMPS OVER", 2, Colors::Black, false, FontType::TTF),
                            Text(Point(10, 70), "THE LAZY DOG", 2, Colors::Black, false, FontType::TTF)
                        }, false)
                    )
                }, false),
                
                
                // Footer
                Container(
                    Colors::DarkGray,
                    0, 0, 0, 60,
                    Center(Text(Point(0, 0), ttfLoaded ? "TTF RENDERING ACTIVE" : "BITMAP ONLY MODE", 2, Colors::White, false, FontType::Bitmap))
                )
                
            }, false)
        )
    );
}

void draw() {
    Draw::fill(Colors::LightGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
