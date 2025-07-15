#include <fern/fern.hpp>
#include <iostream>
#include <cmath>

using namespace Fern;

// Global variables for animation
static int animationFrame = 0;
static std::vector<std::shared_ptr<LineWidget>> animatedLines;

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic lines with different thicknesses
    auto thinLine = Line(Point(50, 100), Point(250, 100), 1, Colors::White);
    auto mediumLine = Line(Point(50, 120), Point(250, 120), 3, Colors::Green);
    auto thickLine = Line(Point(50, 140), Point(250, 140), 6, Colors::Blue);
    auto extraThickLine = Line(Point(50, 160), Point(250, 160), 10, Colors::Red);
    
    // Example 2: Different line orientations
    auto horizontalLine = Line(Point(300, 100), Point(500, 100), 2, Colors::Yellow);
    auto verticalLine = Line(Point(400, 80), Point(400, 180), 2, Colors::Yellow);
    auto diagonalLine1 = Line(Point(300, 80), Point(500, 180), 2, Colors::Yellow);
    auto diagonalLine2 = Line(Point(500, 80), Point(300, 180), 2, Colors::Yellow);
    
    // Example 3: Create a rectangle using lines
    int rectX = 50, rectY = 200, rectWidth = 200, rectHeight = 100;
    auto rectTop = Line(Point(rectX, rectY), Point(rectX + rectWidth, rectY), 3, Colors::Cyan);
    auto rectRight = Line(Point(rectX + rectWidth, rectY), Point(rectX + rectWidth, rectY + rectHeight), 3, Colors::Cyan);
    auto rectBottom = Line(Point(rectX + rectWidth, rectY + rectHeight), Point(rectX, rectY + rectHeight), 3, Colors::Cyan);
    auto rectLeft = Line(Point(rectX, rectY + rectHeight), Point(rectX, rectY), 3, Colors::Cyan);
    
    // Example 4: Create a triangle using lines
    Point triP1(350, 200);
    Point triP2(450, 200);
    Point triP3(400, 260);
    auto triSide1 = Line(triP1, triP2, 3, Colors::Magenta);
    auto triSide2 = Line(triP2, triP3, 3, Colors::Magenta);
    auto triSide3 = Line(triP3, triP1, 3, Colors::Magenta);
    
    // Example 5: Grid pattern
    int gridStartX = 50, gridStartY = 320;
    int gridSize = 40, gridRows = 4, gridCols = 5;
    
    // Vertical grid lines
    for (int col = 0; col <= gridCols; ++col) {
        auto gridLine = Line(
            Point(gridStartX + col * gridSize, gridStartY), 
            Point(gridStartX + col * gridSize, gridStartY + gridRows * gridSize), 
            1, 
            Colors::Gray
        );
    }
    
    // Horizontal grid lines
    for (int row = 0; row <= gridRows; ++row) {
        auto gridLine = Line(
            Point(gridStartX, gridStartY + row * gridSize), 
            Point(gridStartX + gridCols * gridSize, gridStartY + row * gridSize), 
            1, 
            Colors::Gray
        );
    }
    
    // Example 6: Star pattern
    Point starCenter(400, 380);
    int starRadius = 50;
    
    // Create 8 lines radiating from center
    for (int i = 0; i < 8; ++i) {
        double angle = (i * 2.0 * M_PI) / 8.0;
        Point starEnd(
            starCenter.x + (int)(starRadius * cos(angle)),
            starCenter.y + (int)(starRadius * sin(angle))
        );
        auto starLine = Line(starCenter, starEnd, 2, Colors::Orange);
    }
    
    // Example 7: Progress bar using lines
    int progressBarX = 300, progressBarY = 320;
    int progressBarWidth = 200, progressBarHeight = 20;
    
    // Background line
    auto progressBg = Line(
        Point(progressBarX, progressBarY), 
        Point(progressBarX + progressBarWidth, progressBarY), 
        progressBarHeight, 
        Colors::DarkGray
    );
    
    // Progress line (60% progress)
    int progressWidth = (progressBarWidth * 60) / 100;
    auto progressFill = Line(
        Point(progressBarX, progressBarY), 
        Point(progressBarX + progressWidth, progressBarY), 
        progressBarHeight, 
        Colors::Green
    );
    
    // Example 8: Connection lines between circles
    auto circle1 = Circle(15, Point(120, 480), Colors::Blue);
    auto circle2 = Circle(15, Point(220, 520), Colors::Red);
    auto circle3 = Circle(15, Point(320, 480), Colors::Green);
    
    // Connect circles with lines
    auto connection1 = Line(Point(120, 480), Point(220, 520), 2, Colors::Yellow);
    auto connection2 = Line(Point(220, 520), Point(320, 480), 2, Colors::Yellow);
    auto connection3 = Line(Point(320, 480), Point(120, 480), 2, Colors::Yellow);
    
    // Example 9: Simple chart/graph
    std::vector<int> dataPoints = {50, 80, 120, 90, 150, 110, 180};
    int chartStartX = 500, chartStartY = 450;
    
    // Create lines connecting data points
    for (int i = 0; i < dataPoints.size() - 1; ++i) {
        Point start(chartStartX + i * 30, chartStartY - dataPoints[i]);
        Point end(chartStartX + (i + 1) * 30, chartStartY - dataPoints[i + 1]);
        
        auto chartLine = Line(start, end, 2, Colors::Cyan);
        
        // Add data point circles
        auto startPoint = Circle(3, start, Colors::White);
        if (i == dataPoints.size() - 2) {  // Last point
            auto endPoint = Circle(3, end, Colors::White);
        }
    }
    
    // Chart axes
    auto xAxis = Line(Point(chartStartX - 20, chartStartY), Point(chartStartX + 200, chartStartY), 2, Colors::White);
    auto yAxis = Line(Point(chartStartX, chartStartY + 20), Point(chartStartX, chartStartY - 200), 2, Colors::White);
    
    // Example 10: Animated lines for wave effect
    for (int i = 0; i < 15; ++i) {
        auto waveLine = Line(
            Point(50 + i * 20, 550), 
            Point(50 + i * 20, 550), 
            3, 
            Colors::Purple
        );
        animatedLines.push_back(waveLine);
    }
    
    // Example 11: Arrow
    Point arrowStart(600, 200);
    Point arrowEnd(720, 200);
    
    // Main arrow line
    auto arrowLine = Line(arrowStart, arrowEnd, 4, Colors::Red);
    
    // Arrowhead lines
    int arrowSize = 20;
    auto arrowHead1 = Line(
        arrowEnd, 
        Point(arrowEnd.x - arrowSize, arrowEnd.y - arrowSize/2), 
        3, 
        Colors::Red
    );
    auto arrowHead2 = Line(
        arrowEnd, 
        Point(arrowEnd.x - arrowSize, arrowEnd.y + arrowSize/2), 
        3, 
        Colors::Red
    );
    
    // Example 12: Crosshair
    Point crosshairCenter(650, 350);
    int crosshairSize = 25;
    
    auto crosshairH = Line(
        Point(crosshairCenter.x - crosshairSize, crosshairCenter.y), 
        Point(crosshairCenter.x + crosshairSize, crosshairCenter.y), 
        2, 
        Colors::Red
    );
    auto crosshairV = Line(
        Point(crosshairCenter.x, crosshairCenter.y - crosshairSize), 
        Point(crosshairCenter.x, crosshairCenter.y + crosshairSize), 
        2, 
        Colors::Red
    );
    
    // Add title
    auto title = Text(Point(0, 0), "Line Widget Examples", 3, Colors::White);
    auto centerTitle = std::make_shared<CenterWidget>(0, 0, screenWidth, 60);
    centerTitle->add(title);
    addWidget(centerTitle);
    
    // Add instructions
    auto instructions = Text(Point(0, 0), "Lines, shapes, grids, charts, and animations", 2, Colors::Gray);
    auto centerInstructions = std::make_shared<CenterWidget>(0, 40, screenWidth, 60);
    centerInstructions->add(instructions);
    addWidget(centerInstructions);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
    
    // Animate the wave lines
    animationFrame++;
    for (int i = 0; i < animatedLines.size(); ++i) {
        double wave = sin((animationFrame + i * 5) * 0.1) * 30;
        Point start(50 + i * 20, 550);
        Point end(50 + i * 20, 550 + (int)wave);
        
        animatedLines[i]->setStart(start);
        animatedLines[i]->setEnd(end);
        
        // Change color based on wave height
        uint32_t intensity = (uint32_t)(128 + 127 * sin((animationFrame + i * 5) * 0.1));
        uint32_t color = (intensity << 16) | (intensity << 8) | 255;  // Blue to purple gradient
        animatedLines[i]->setColor(color);
    }
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
