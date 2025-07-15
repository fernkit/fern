#include <fern/fern.hpp>
#include <iostream>
#include <cmath>

using namespace Fern;

// Global variables for animation
static int animationFrame = 0;
static std::vector<std::shared_ptr<CircleWidget>> animatedCircles;

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic circles with different sizes and colors
    auto smallCircle = Circle(15, Point(100, 100), Colors::Red);
    auto mediumCircle = Circle(30, Point(200, 100), Colors::Green);  
    auto largeCircle = Circle(45, Point(320, 100), Colors::Blue);
    
    // Example 2: Interactive circle with hover and click
    auto interactiveCircle = Circle(40, Point(150, 200), Colors::Purple);
    
    // Add hover effect
    interactiveCircle->onHover.connect([interactiveCircle](bool isHovered) {
        if (isHovered) {
            interactiveCircle->setColor(Colors::Magenta);  // Highlight on hover
        } else {
            interactiveCircle->setColor(Colors::Purple);   // Normal color
        }
    });
    
    // Add click effect
    interactiveCircle->onClick.connect([interactiveCircle]() {
        std::cout << "Interactive circle clicked!" << std::endl;
        // Toggle between purple and cyan
        static bool isPurple = true;
        if (isPurple) {
            interactiveCircle->setColor(Colors::Cyan);
        } else {
            interactiveCircle->setColor(Colors::Purple);
        }
        isPurple = !isPurple;
    });
    
    // Example 3: Status indicators
    auto onlineStatus = Circle(12, Point(50, 300), Colors::Green);
    auto offlineStatus = Circle(12, Point(100, 300), Colors::Red);
    auto pendingStatus = Circle(12, Point(150, 300), Colors::Yellow);
    
    // Example 4: Progress dots
    std::vector<std::shared_ptr<CircleWidget>> progressDots;
    for (int i = 0; i < 5; ++i) {
        auto dot = Circle(10, Point(250 + i * 30, 300), Colors::Gray);
        progressDots.push_back(dot);
    }
    
    // Set first 3 dots as completed
    for (int i = 0; i < 3; ++i) {
        progressDots[i]->setColor(Colors::Green);
    }
    
    // Example 5: Color palette
    std::vector<uint32_t> paletteColors = {
        Colors::Red, Colors::Green, Colors::Blue,
        Colors::Yellow, Colors::Magenta, Colors::Cyan,
        Colors::Orange, Colors::White, Colors::Gray
    };
    
    for (int i = 0; i < paletteColors.size(); ++i) {
        int x = 50 + (i % 3) * 60;
        int y = 400 + (i / 3) * 60;
        
        auto colorCircle = Circle(20, Point(x, y), paletteColors[i]);
        
        // Add click handler to select color
        colorCircle->onClick.connect([paletteColors, i]() {
            std::cout << "Selected color: 0x" << std::hex << paletteColors[i] << std::endl;
        });
        
        // Add hover effect
        colorCircle->onHover.connect([colorCircle](bool isHovered) {
            if (isHovered) {
                colorCircle->setRadius(25);  // Slightly larger
            } else {
                colorCircle->setRadius(20);  // Normal size
            }
        });
    }
    
    // Example 6: Animated circles for loading effect
    for (int i = 0; i < 8; ++i) {
        double angle = (i * 2.0 * M_PI) / 8.0;
        int x = 400 + (int)(40 * cos(angle));
        int y = 450 + (int)(40 * sin(angle));
        
        auto animatedCircle = Circle(8, Point(x, y), Colors::Gray);
        animatedCircles.push_back(animatedCircle);
    }
    
    // Example 7: Navigation dots
    std::vector<std::shared_ptr<CircleWidget>> navDots;
    for (int i = 0; i < 5; ++i) {
        auto dot = Circle(8, Point(500 + i * 25, 300), Colors::Gray);
        navDots.push_back(dot);
        
        // Set first dot as active
        if (i == 0) {
            dot->setColor(Colors::Blue);
        }
        
        // Add navigation functionality
        dot->onClick.connect([navDots, i]() {
            // Reset all dots to inactive
            for (auto& navDot : navDots) {
                navDot->setColor(Colors::Gray);
            }
            // Set clicked dot as active
            navDots[i]->setColor(Colors::Blue);
            std::cout << "Navigated to page " << (i + 1) << std::endl;
        });
    }
    
    // Example 8: Button-like circles
    auto playButton = Circle(25, Point(600, 150), Colors::Green);
    auto pauseButton = Circle(25, Point(650, 150), Colors::Yellow);
    auto stopButton = Circle(25, Point(700, 150), Colors::Red);
    
    playButton->onClick.connect([]() {
        std::cout << "Play button clicked!" << std::endl;
    });
    
    pauseButton->onClick.connect([]() {
        std::cout << "Pause button clicked!" << std::endl;
    });
    
    stopButton->onClick.connect([]() {
        std::cout << "Stop button clicked!" << std::endl;
    });
    
    // Add hover effects to buttons
    auto addButtonHover = [](std::shared_ptr<CircleWidget> button, uint32_t normalColor, uint32_t hoverColor) {
        button->onHover.connect([button, normalColor, hoverColor](bool isHovered) {
            if (isHovered) {
                button->setColor(hoverColor);
            } else {
                button->setColor(normalColor);
            }
        });
    };
    
    addButtonHover(playButton, Colors::Green, Colors::LightGreen);
    addButtonHover(pauseButton, Colors::Yellow, Colors::Amber);
    addButtonHover(stopButton, Colors::Red, Colors::LightRed);
    
    // Add title
    auto title = Text(Point(0, 0), "Circle Widget Examples", 3, Colors::White);
    auto centerTitle = std::make_shared<CenterWidget>(0, 0, screenWidth, 60);
    centerTitle->add(title);
    addWidget(centerTitle);
    
    // Add instructions
    auto instructions = Text(Point(0, 0), "Hover and click the circles to interact!", 2, Colors::Gray);
    auto centerInstructions = std::make_shared<CenterWidget>(0, 40, screenWidth, 60);
    centerInstructions->add(instructions);
    addWidget(centerInstructions);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
    
    // Animate the loading circles
    animationFrame++;
    for (int i = 0; i < animatedCircles.size(); ++i) {
        // Calculate opacity based on animation frame
        int opacity = 255 - ((animationFrame - i * 8) % 64) * 4;
        if (opacity < 64) opacity = 64;
        
        // Create fading effect (simplified - just change to different grays)
        uint32_t grayLevel = opacity >> 2;  // Convert to 0-63 range
        uint32_t color = (grayLevel << 16) | (grayLevel << 8) | grayLevel;
        animatedCircles[i]->setColor(color);
    }
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
