#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    addWidget(
        Center(
            Column({
                Text(Point(0, 0), "Small Text", 1, Colors::White),
                SizedBox(0, 20),
                Text(Point(0, 0), "Medium Text", 2, Colors::Yellow),
                SizedBox(0, 20),
                Text(Point(0, 0), "Large Text", 3, Colors::Green),
                SizedBox(0, 20),
                Text(Point(0, 0), "Extra Large", 4, Colors::Red)
            })
        )
    );
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
