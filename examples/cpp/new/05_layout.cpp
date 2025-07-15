#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    addWidget(
        Container(
            Colors::Black,
            0, 0, width, height,
            Column({
                Container(
                    Colors::DarkBlue,
                    0, 0, width, 80,
                    Center(Text(Point(0, 0), "HEADER", 2, Colors::White))
                ),
                
                Expanded(
                    Row({
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Red,
                                    0, 0, 0, 0,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White))
                                ), 10, false
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(Container(
                                Colors::Green,
                                0, 0, 0, 0,
                                Center(Text(Point(0, 0), "2", 3, Colors::White))
                            ), 10),
                            2
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Blue,
                                    0, 0, 0, 0,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White))
                            ), 10
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
                                    Center(Text(Point(0, 0), "1", 3, Colors::White))
                            ), 10
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Green,
                                    0, 0, 0, 0,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White))
                            ), 10
                            ),
                            1
                        ),
                        
                        Expanded(
                            Padding(
                                Container(
                                    Colors::Blue,
                                    0, 0, 0, 0,
                                    Center(Text(Point(0, 0), "1", 3, Colors::White))
                            ), 10
                            ),
                            1
                        )
                    }, false)
                ),

                
                // Footer (fixed height)
                Container(
                    Colors::DarkGray,
                    0, 0, 0, 80,
                    Center(Text(Point(0, 0), "FOOTER", 2, Colors::White))
                )
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