#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Circle hover event demo
    auto circle1 = Circle(40, Point(150, 150), Colors::Red);
    
    // Change color on hover with proper event handling
    circle1->onHover.connect([circle1](bool isHovered) {
        if (isHovered) {
            circle1->setColor(Colors::Yellow);  // Highlight color
        } else {
            circle1->setColor(Colors::Red);     // Normal color
        }
    });
    
    // Handle clicks
    circle1->onClick.connect([circle1]() {
        // Toggle between red and blue on click
        static bool isRed = true;
        if (isRed) {
            circle1->setColor(Colors::Blue);
        } else {
            circle1->setColor(Colors::Red);
        }
        isRed = !isRed;
    });
    
    // Multiple circles for testing
    auto circle2 = Circle(30, Point(300, 150), Colors::Green);
    circle2->onHover.connect([circle2](bool isHovered) {
        if (isHovered) {
            circle2->setColor(Colors::LightGreen);
        } else {
            circle2->setColor(Colors::Green);
        }
    });
    
    auto circle3 = Circle(50, Point(450, 150), Colors::Blue);
    circle3->onHover.connect([circle3](bool isHovered) {
        if (isHovered) {
            circle3->setColor(Colors::Cyan);
        } else {
            circle3->setColor(Colors::Blue);
        }
    });
    
    // Status text to show hover state
    static std::shared_ptr<TextWidget> statusText;
    statusText = Text(Point(0, 0), "Hover over circles to test events", 2, Colors::White);
    
    // Update status based on hover
    circle1->onHover.connect([](bool isHovered) {
        if (isHovered) {
            statusText->setText("Red circle is hovered!");
            statusText->setColor(Colors::Yellow);
        } else {
            statusText->setText("Hover over circles to test events");
            statusText->setColor(Colors::White);
        }
    });
    
    circle2->onHover.connect([](bool isHovered) {
        if (isHovered) {
            statusText->setText("Green circle is hovered!");
            statusText->setColor(Colors::LightGreen);
        }
    });
    
    circle3->onHover.connect([](bool isHovered) {
        if (isHovered) {
            statusText->setText("Blue circle is hovered!");
            statusText->setColor(Colors::Cyan);
        }
    });
    
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Circle Hover Event Demo", 3, Colors::White),
        SizedBox(0, 20),
        Text(Point(0, 0), "Fixed hover detection with proper center calculation", 2, Colors::LightGray),
        SizedBox(0, 30),
        statusText,
        SizedBox(0, 40),
        Text(Point(0, 0), "Try hovering and clicking the circles below:", 2, Colors::LightBlue),
        SizedBox(0, 20),
        // Note: Circles are positioned absolutely, so they won't follow column layout
        // This is just for visual reference
        Text(Point(0, 0), "Red (40px) - Yellow on hover, toggles Red/Blue on click", 1, Colors::LightGray),
        SizedBox(0, 10),
        Text(Point(0, 0), "Green (30px) - Light green on hover", 1, Colors::LightGray),
        SizedBox(0, 10),
        Text(Point(0, 0), "Blue (50px) - Cyan on hover", 1, Colors::LightGray)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(children));
    addWidget(centerWidget);
    
    // Add circles directly (they position themselves absolutely)
    addWidget(circle1);
    addWidget(circle2);
    addWidget(circle3);
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
