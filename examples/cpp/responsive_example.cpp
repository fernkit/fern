#include <fern/fern.hpp>
#include <iostream>
#include <memory>
#include <string>

using namespace Fern;

static std::shared_ptr<CircleWidget> centerCircle;
static std::shared_ptr<LineWidget> horizontalLine;
static std::shared_ptr<TextWidget> statusText;
static std::shared_ptr<ButtonWidget> actionButton;

static int prevWidth = 0;
static int prevHeight = 0;

void setupUI();
void connectEventHandlers();

void setupUI()
{
    int width = Fern::getWidth();
    int height = Fern::getHeight();

    statusText = Text(Point(width / 2, static_cast<int>(height * 0.9)),
                      "WINDOW SIZE: " + std::to_string(width) + " x " + std::to_string(height),
                      2, Colors::LightGray);

    int circleRadius = std::min(width, height) / 10;
    centerCircle = Circle(circleRadius, Point(width / 2, height / 2), Colors::Blue);

    horizontalLine = Line(Point(static_cast<int>(width * 0.1), static_cast<int>(height * 0.7)),
                          Point(static_cast<int>(width * 0.9), static_cast<int>(height * 0.7)),
                          3, Colors::Green);

    int buttonWidth = width / 5;
    int buttonHeight = height / 15;

    ButtonConfig btnConfig = {
        .x = static_cast<int>(width / 2 - buttonWidth / 2),
        .y = static_cast<int>(height * 0.8 - buttonHeight / 2),
        .width = buttonWidth,
        .height = buttonHeight,
        .normalColor = Colors::Red,
        .hoverColor = Colors::LightRed,
        .pressColor = Colors::DarkRed,
        .label = "RESIZE ME",
        .textScale = 2,
        .textColor = Colors::White};

    actionButton = Button(btnConfig);
    int cornerSize = std::min(width, height) / 20;

    Circle(cornerSize, Point(cornerSize, cornerSize), Colors::Yellow);
    Circle(cornerSize, Point(width - cornerSize, cornerSize), Colors::Purple);
    Circle(cornerSize, Point(cornerSize, height - cornerSize), Colors::Cyan);
    Circle(cornerSize, Point(width - cornerSize, height - cornerSize), Colors::Orange);

    prevWidth = width;
    prevHeight = height;
}

void draw()
{
    Draw::fill(Colors::DarkGray);

    int width = Fern::getWidth();
    int height = Fern::getHeight();

    for (int y = 0; y < height; y++)
    {
        uint8_t intensity = 30 + (y * 50 / height);
        uint32_t color = (0xFF << 24) | (intensity << 16) | (intensity / 2 << 8) | intensity;

        Draw::line(0, y, width, y, 1, color);
    }

    int currentWidth = Fern::getWidth();
    int currentHeight = Fern::getHeight();

    if (currentWidth != prevWidth || currentHeight != prevHeight)
    {
        bool justResized = true;
        WidgetManager::getInstance().clear();
        setupUI();

        // IMPORTANT: Reconnect all event handlers
        connectEventHandlers();
        if (justResized)
        {
            statusText->setText("RESIZED: " + std::to_string(currentWidth) + " x " +
                                std::to_string(currentHeight));
        }
    }
}

void connectEventHandlers()
{
    actionButton->onClick.connect([]()
                                  {
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        
        centerCircle->setRadius(centerCircle->getRadius() + 10);
        if (centerCircle->getRadius() > std::min(width, height) / 5) {
            centerCircle->setRadius(std::min(width, height) / 20);
        }
        
        Point start = horizontalLine->getStart();
        Point end = horizontalLine->getEnd();
        horizontalLine->setStart(Point(end.x, end.y));
        horizontalLine->setEnd(Point(start.x, start.y));
        
        statusText->setText("BUTTON CLICKED! CIRCLE RADIUS: " + 
                         std::to_string(centerCircle->getRadius())); });
}

int main()
{
    Fern::initialize();
    setupUI();
    connectEventHandlers();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}