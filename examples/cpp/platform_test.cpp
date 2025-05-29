#include <fern/fern.hpp>

using namespace Fern;

std::shared_ptr<ContainerWidget> animatedBox;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    // Black background
    addWidget(Container(
        Colors::Black,
        0, 0, width, height
    ));
    
    // Create a small colored box that we'll animate
    animatedBox = Container(
        Colors::Blue,
        width/2, height/2, 50, 50,
        Center(Text(Point(0, 0), "BOX", 1, Colors::White, false), false),
        true
    );
}

void draw() {
    if (!animatedBox) return;
    
    static int frame = 0;
    static int directionX = 1;
    static int directionY = 1;
    static int x = Fern::getWidth()/2;
    static int y = Fern::getHeight()/2;
    
    frame++;
    
    int speed = 1;
    x += directionX * speed;
    y += directionY * speed;
    
    if (x <= 0 || x >= Fern::getWidth() - 5) {
        directionX *= -1;
        x = std::max(0, std::min(x, Fern::getWidth() - 5));
    }
    
    if (y <= 0 || y >= Fern::getHeight() - 5) {
        directionY *= -1;
        y = std::max(0, std::min(y, Fern::getHeight() - 5));
    }
    
    animatedBox->setPosition(x, y);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}