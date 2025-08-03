#include <fern/fern.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace Fern;

// Color picker interface
namespace PickerColors {
    const uint32_t Background = 0xFF1F2937;     // Dark blue-gray
    const uint32_t Panel = 0xFF374151;          // Medium gray
    const uint32_t Text = 0xFFFFFFFF;           // White
    const uint32_t TextSecondary = 0xFFD1D5DB;  // Light gray
}

// Global state
static int redValue = 128;
static int greenValue = 128;
static int blueValue = 128;
static std::shared_ptr<TextWidget> colorHexText;
static std::shared_ptr<CircleWidget> colorPreview;
static std::shared_ptr<TextWidget> redLabel;
static std::shared_ptr<TextWidget> greenLabel;
static std::shared_ptr<TextWidget> blueLabel;
static std::shared_ptr<CenterWidget> centerWidget;

uint32_t getCurrentColor() {
    return 0xFF000000 | (redValue << 16) | (greenValue << 8) | blueValue;
}

std::string toHex(uint32_t color) {
    std::ostringstream oss;
    oss << "#" << std::hex << std::uppercase << std::setfill('0') << std::setw(6) 
        << (color & 0xFFFFFF);
    return oss.str();
}

void updateColorDisplay() {
    uint32_t currentColor = getCurrentColor();
    colorPreview->setColor(currentColor);
    colorHexText->setText(toHex(currentColor));
    
    redLabel->setText("Red: " + std::to_string(redValue));
    greenLabel->setText("Green: " + std::to_string(greenValue));
    blueLabel->setText("Blue: " + std::to_string(blueValue));
}

std::shared_ptr<ButtonWidget> createColorButton(const std::string& label, 
                                               uint32_t color, 
                                               std::function<void()> onClick) {
    ButtonStyle colorStyle;
    colorStyle.normalColor(color)
             .hoverColor(color | 0x33000000)  // Add slight transparency for hover
             .pressColor(color & 0xDDFFFFFF)  // Darken for press
             .textColor(0xFFFFFFFF)
             .textScale(2)
             .borderRadius(6);
    
    auto button = Button(ButtonConfig(0, 0, 80, 35, label).style(colorStyle));
    button->autoSizeToContent(10);
    button->onClick.connect(onClick);
    return button;
}

void setupUI() {
    // Title
    auto titleText = Text(Point(0, 0), "Color Picker", 4, PickerColors::Text);
    
    // Color preview circle
    colorPreview = Circle(60, Point(0,0), getCurrentColor());
    
    // Hex display
    colorHexText = Text(Point(0, 0), toHex(getCurrentColor()), 3, PickerColors::Text);
    
    // RGB labels
    redLabel = Text(Point(0, 0), "Red: " + std::to_string(redValue), 2, PickerColors::TextSecondary);
    greenLabel = Text(Point(0, 0), "Green: " + std::to_string(greenValue), 2, PickerColors::TextSecondary);
    blueLabel = Text(Point(0, 0), "Blue: " + std::to_string(blueValue), 2, PickerColors::TextSecondary);
    
    // Red controls
    auto redUp = createColorButton("+", 0xFFE53E3E, []() {
        redValue = std::min(255, redValue + 10);
        updateColorDisplay();
    });
    auto redDown = createColorButton("-", 0xFFE53E3E, []() {
        redValue = std::max(0, redValue - 10);
        updateColorDisplay();
    });
    
    // Green controls
    auto greenUp = createColorButton("+", 0xFF38A169, []() {
        greenValue = std::min(255, greenValue + 10);
        updateColorDisplay();
    });
    auto greenDown = createColorButton("-", 0xFF38A169, []() {
        greenValue = std::max(0, greenValue - 10);
        updateColorDisplay();
    });
    
    // Blue controls
    auto blueUp = createColorButton("+", 0xFF3182CE, []() {
        blueValue = std::min(255, blueValue + 10);
        updateColorDisplay();
    });
    auto blueDown = createColorButton("-", 0xFF3182CE, []() {
        blueValue = std::max(0, blueValue - 10);
        updateColorDisplay();
    });
    
    // Preset color buttons
    auto presetRed = createColorButton("Red", 0xFFE53E3E, []() {
        redValue = 229; greenValue = 62; blueValue = 62;
        updateColorDisplay();
    });
    auto presetGreen = createColorButton("Green", 0xFF38A169, []() {
        redValue = 56; greenValue = 161; blueValue = 105;
        updateColorDisplay();
    });
    auto presetBlue = createColorButton("Blue", 0xFF3182CE, []() {
        redValue = 49; greenValue = 130; blueValue = 206;
        updateColorDisplay();
    });
    auto presetPurple = createColorButton("Purple", 0xFF805AD5, []() {
        redValue = 128; greenValue = 90; blueValue = 213;
        updateColorDisplay();
    });
    
    // Layout using proper Row and Column widgets
    std::vector<std::shared_ptr<Widget>> redRow = {redLabel, SizedBox(20, 0), redDown, SizedBox(10, 0), redUp};
    std::vector<std::shared_ptr<Widget>> greenRow = {greenLabel, SizedBox(20, 0), greenDown, SizedBox(10, 0), greenUp};
    std::vector<std::shared_ptr<Widget>> blueRow = {blueLabel, SizedBox(20, 0), blueDown, SizedBox(10, 0), blueUp};
    std::vector<std::shared_ptr<Widget>> presetRow = {presetRed, SizedBox(10, 0), presetGreen, SizedBox(10, 0), presetBlue, SizedBox(10, 0), presetPurple};
    
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        titleText,
        SizedBox(0, 30),
        colorPreview,
        SizedBox(0, 20),
        colorHexText,
        SizedBox(0, 30),
        Row(redRow),
        SizedBox(0, 15),
        Row(greenRow),
        SizedBox(0, 15),
        Row(blueRow),
        SizedBox(0, 30),
        Text(Point(0, 0), "Presets:", 2, PickerColors::TextSecondary),
        SizedBox(0, 10),
        Row(presetRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    addWidget(centerWidget);
    
    // Initial display update
    updateColorDisplay();
}

void onWindowResize(int newWidth, int newHeight) {
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(PickerColors::Background);
}

int main() {
    Fern::initialize(600, 700);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::setWindowResizeCallback(onWindowResize);
    Fern::startRenderLoop();
    return 0;
}
