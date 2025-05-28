#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    addWidget(
        Container(
            Colors::Black,
            0, 0, width, height,
            false,
            Column({
                Container(
                    Colors::DarkBlue,
                    0, 0, 0, 80,
                    false,
                    Center(Text(Point(0, 0), "HEADER", 2, Colors::White, false), false)
                ),
                
                Expanded(
                    Row({
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Red,
                                    0, 0, 0, 0,
                                    false,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White, false), false)
                                ), 10, false
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(Container(
                                Colors::Green,
                                0, 0, 0, 0,
                                false,
                                Center(Text(Point(0, 0), "2", 3, Colors::White, false), false)
                            ), 10, false),
                            2
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Blue,
                                    0, 0, 0, 0,
                                    false,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White, false), false)
                            ), 10, false
                            ),
                            1
                        )
                    }, false)
                ),
                Expanded(
                    Column({
                         Expanded(
                            Padding(
                                Container(
                                    Colors::Red,
                                    0, 0, 0, 0,
                                    false,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White, false), false)
                            ), 10, false
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Green,
                                    0, 0, 0, 0,
                                    false,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White, false), false)
                            ), 10, false
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Blue,
                                    0, 0, 0, 0,
                                    false,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White, false), false)
                            ), 10, false
                            ),
                            1
                        )
                    }, false)
                ),

                
                // Footer (fixed height)
                Container(
                    Colors::DarkGray,
                    0, 0, 0, 80,
                    false,
                    Center(Text(Point(0, 0), "FOOTER", 2, Colors::White, false), false)
                )
            }, false)
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