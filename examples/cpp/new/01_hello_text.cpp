#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Simple centered text
    addWidget(
        Center(
            Text(Point(0, 0), "Hello, Fern!", 3, Colors::White)
        )
    );
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
