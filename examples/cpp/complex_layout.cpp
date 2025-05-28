#include <fern/fern.hpp>
#include <string>

using namespace Fern;

// Helper for creating control buttons
std::shared_ptr<Widget> createControlButton(uint32_t color, const std::string& label, float scale = 1.5) {
    return Container(
        Colors::Transparent,
        0, 0, 80, 50,
        false,
        Center(Text(Point(0, 0), label, scale, color, false), false)
    );
}

void setupUI() {
    WidgetManager::getInstance().clear();
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    
    addWidget(
        Container(
            Colors::Black,
            0, 0, width, height,
            false,
            Padding(
                Column({                   
                    SizedBox(0, 30, false),
                    
                    // Album art
                    Center(
                        Container(
                            Colors::DarkBlue, // Album color
                            0, 0, 280, 280,
                            false,
                            Center(Text(Point(0, 0), "MUSIC", 6, Colors::SkyBlue, false), false)
                        ),
                        false
                    ),
                    
                    SizedBox(0, 25, false),
                    
                    // Song info
                    Column({
                        Center(Text(Point(0, 0), "COSMIC WAVES", 2.5, Colors::White, false), false),
                        SizedBox(0, 8, false),
                        Center(Text(Point(0, 0), "STELLER ORCHESTRA", 1.2, Colors::LightGray, false), false)
                    }, false),
                    
                    SizedBox(0, 25, false),
                    
                    // Progress bar
                    Column({
                        Container(
                            Colors::DarkGray,
                            0, 0, 0, 4,
                            false,
                            Row({
                                Container(Colors::SkyBlue, 0, 0, width * 0.35, 4, false)
                            }, false)
                        ),
                        
                        SizedBox(0, 8, false),
                        
                        Row({
                            Text(Point(0, 0), "2:14", 1, Colors::Gray, false),
                            SizedBox(0, 0, false),
                            Text(Point(0, 0), "5:30", 1, Colors::Gray, false)
                        }, false, MainAxisAlignment::SpaceBetween)
                    }, false),
                    
                    SizedBox(0, 25, false),
                    
                    // Control buttons
                    Center(
                        Row({
                            createControlButton(Colors::LightGray, "PREV"),
                            SizedBox(25, 0, false),
                            createControlButton(Colors::White, "II", 1.8),
                            SizedBox(25, 0, false),
                            createControlButton(Colors::LightGray, "NEXT")
                        }, false),
                        false
                    ),
                    
                    SizedBox(0, 25, false),
                    
                    // Volume and extras
                    Row({
                        createControlButton(Colors::LightGray, "UP", 1.2),
                        
                        Container(
                            Colors::DarkGray,
                            0, 0, 100, 4,
                            false,
                            Container(Colors::White, 0, 0, 65, 4, false)
                        ),
                        
                        SizedBox(0, 0, false), // Spacer
                        
                        createControlButton(Colors::LightGray, "REFRESH", 1.2),
                        createControlButton(Colors::LightGray, "LIKE", 1.2)
                    }, false, MainAxisAlignment::SpaceBetween),
                    
                    SizedBox(0, 0, false), // Expandable spacer
                    
                    // Currently playing bar
                    Container(
                        Colors::Charcoal,
                        0, 0, 0, 60,
                        false,
                        Padding(
                            Row({
                                Container(
                                    Colors::DarkBlue,
                                    0, 0, 40, 40,
                                    false
                                ),
                                
                                SizedBox(15, 0, false),
                                
                                Column({
                                    Text(Point(0, 0), "NEXT: LUNAR ECLIPSE", 1.2, Colors::White, false),
                                    SizedBox(0, 4, false),
                                    Text(Point(0, 0), "STELLAR ORCHESTRA", 1, Colors::Gray, false)
                                }, false),
                                
                                SizedBox(0, 0, false), // Expandable spacer
                                
                                createControlButton(Colors::White, "NEXT", 1)
                            }, false),
                            10,
                            false
                        )
                    )
                }, false),
                20,
                false
            )
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