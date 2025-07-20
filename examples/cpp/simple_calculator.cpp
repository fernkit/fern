#include <fern/fern.hpp>
#include <string>
#include <sstream>

using namespace Fern;

// Calculator color scheme
namespace CalcColors {
    const uint32_t Background = 0xFF2D3748;     // Dark gray
    const uint32_t Display = 0xFF1A202C;        // Darker gray
    const uint32_t NumberBtn = 0xFF4A5568;      // Medium gray
    const uint32_t OperatorBtn = 0xFFED8936;    // Orange
    const uint32_t EqualsBtn = 0xFF38A169;      // Green
    const uint32_t ClearBtn = 0xFFE53E3E;       // Red
    const uint32_t TextWhite = 0xFFFFFFFF;      // White text
}

// Global state
static double currentValue = 0;
static double storedValue = 0;
static char operation = '\0';
static bool newNumber = true;
static std::shared_ptr<TextWidget> displayText;
static std::shared_ptr<CenterWidget> centerWidget;

void updateDisplay() {
    std::ostringstream oss;
    if (currentValue == (int)currentValue) {
        oss << (int)currentValue;
    } else {
        oss << currentValue;
    }
    displayText->setText(oss.str());
}

void inputNumber(int digit) {
    if (newNumber) {
        currentValue = digit;
        newNumber = false;
    } else {
        currentValue = currentValue * 10 + digit;
    }
    updateDisplay();
}

void inputOperation(char op) {
    if (operation != '\0' && !newNumber) {
        // Perform pending operation
        switch (operation) {
            case '+': currentValue = storedValue + currentValue; break;
            case '-': currentValue = storedValue - currentValue; break;
            case '*': currentValue = storedValue * currentValue; break;
            case '/': 
                if (currentValue != 0) {
                    currentValue = storedValue / currentValue;
                }
                break;
        }
        updateDisplay();
    }
    
    storedValue = currentValue;
    operation = op;
    newNumber = true;
}

void calculate() {
    if (operation != '\0') {
        inputOperation('\0');
        operation = '\0';
    }
}

void clear() {
    currentValue = 0;
    storedValue = 0;
    operation = '\0';
    newNumber = true;
    updateDisplay();
}

std::shared_ptr<ButtonWidget> createNumberButton(int number) {
    ButtonStyle numberStyle;
    numberStyle.normalColor(CalcColors::NumberBtn)
               .hoverColor(0xFF5A6578)
               .pressColor(0xFF3A4558)
               .textColor(CalcColors::TextWhite)
               .textScale(3)
               .borderRadius(6);
    
    auto button = Button(ButtonConfig(0, 0, 60, 50, std::to_string(number)).style(numberStyle));
    button->onClick.connect([number]() {
        inputNumber(number);
    });
    return button;
}

std::shared_ptr<ButtonWidget> createOperatorButton(char op, const std::string& label) {
    ButtonStyle opStyle;
    opStyle.normalColor(CalcColors::OperatorBtn)
          .hoverColor(0xFFFA9F47)
          .pressColor(0xFFE57525)
          .textColor(CalcColors::TextWhite)
          .textScale(3)
          .borderRadius(6);
    
    auto button = Button(ButtonConfig(0, 0, 60, 50, label).style(opStyle));
    button->onClick.connect([op]() {
        inputOperation(op);
    });
    return button;
}

void setupUI() {
    // Display
    displayText = Text(Point(0, 0), "0", 4, CalcColors::TextWhite);
    
    // Number buttons (0-9)
    auto btn7 = createNumberButton(7);
    auto btn8 = createNumberButton(8);
    auto btn9 = createNumberButton(9);
    
    auto btn4 = createNumberButton(4);
    auto btn5 = createNumberButton(5);
    auto btn6 = createNumberButton(6);
    
    auto btn1 = createNumberButton(1);
    auto btn2 = createNumberButton(2);
    auto btn3 = createNumberButton(3);
    
    auto btn0 = createNumberButton(0);
    
    // Operator buttons
    auto btnAdd = createOperatorButton('+', "+");
    auto btnSub = createOperatorButton('-', "-");
    auto btnMul = createOperatorButton('*', "*");  // Use * instead of ×
    auto btnDiv = createOperatorButton('/', "/");  // Use / instead of ÷
    
    // Equals button
    ButtonStyle equalsStyle;
    equalsStyle.normalColor(CalcColors::EqualsBtn)
              .hoverColor(0xFF48BB78)
              .pressColor(0xFF2F855A)
              .textColor(CalcColors::TextWhite)
              .textScale(3)
              .borderRadius(6);
    
    auto btnEquals = Button(ButtonConfig(0, 0, 60, 50, "=").style(equalsStyle));
    btnEquals->onClick.connect([]() {
        calculate();
    });
    
    // Clear button
    ButtonStyle clearStyle;
    clearStyle.normalColor(CalcColors::ClearBtn)
             .hoverColor(0xFFED5F5F)
             .pressColor(0xFFD53F3F)
             .textColor(CalcColors::TextWhite)
             .textScale(2)
             .borderRadius(6);
    
    auto btnClear = Button(ButtonConfig(0, 0, 60, 50, "C").style(clearStyle));
    btnClear->onClick.connect([]() {
        clear();
    });
    
    // Layout - create proper grid layout using rows and columns
    std::vector<std::shared_ptr<Widget>> topRow = {btnDiv, SizedBox(10, 0), btnMul, SizedBox(10, 0), btnSub, SizedBox(10, 0), btnAdd};
    std::vector<std::shared_ptr<Widget>> row1 = {btn7, SizedBox(10, 0), btn8, SizedBox(10, 0), btn9};
    std::vector<std::shared_ptr<Widget>> row2 = {btn4, SizedBox(10, 0), btn5, SizedBox(10, 0), btn6};
    std::vector<std::shared_ptr<Widget>> row3 = {btn1, SizedBox(10, 0), btn2, SizedBox(10, 0), btn3};
    std::vector<std::shared_ptr<Widget>> bottomRow = {btnClear, SizedBox(10, 0), btn0, SizedBox(10, 0), btnEquals};
    
    std::vector<std::shared_ptr<Widget>> calcLayout = {
        displayText,
        SizedBox(0, 20),  // Reduced spacing
        Row(topRow),
        SizedBox(0, 10),  // Reduced spacing
        Row(row1),
        SizedBox(0, 10),  // Reduced spacing
        Row(row2),
        SizedBox(0, 10),  // Reduced spacing
        Row(row3),
        SizedBox(0, 10),  // Reduced spacing
        Row(bottomRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(calcLayout));
    addWidget(centerWidget);
}

void onWindowResize(int newWidth, int newHeight) {
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(CalcColors::Background);
}

int main() {
    Fern::initialize(400, 500);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::setWindowResizeCallback(onWindowResize);
    Fern::startRenderLoop();
    return 0;
}
