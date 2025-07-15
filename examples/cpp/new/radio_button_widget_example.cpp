#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic radio button group
    auto option1 = Fern::RadioButton(Fern::RadioButtonConfig(50, 50, "Option 1", "basic")
        .selected(true), true);
    auto option2 = Fern::RadioButton(Fern::RadioButtonConfig(50, 80, "Option 2", "basic")
        .selected(false), true);
    auto option3 = Fern::RadioButton(Fern::RadioButtonConfig(50, 110, "Option 3", "basic")
        .selected(false), true);
    
    option1->onSelected.connect([]() {
        std::cout << "Basic: Option 1 selected" << std::endl;
    });
    
    option2->onSelected.connect([]() {
        std::cout << "Basic: Option 2 selected" << std::endl;
    });
    
    option3->onSelected.connect([]() {
        std::cout << "Basic: Option 3 selected" << std::endl;
    });
    
    // Example 2: Graphics quality settings
    auto lowQuality = Fern::RadioButton(Fern::RadioButtonConfig(200, 50, "Low Quality", "graphics")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto mediumQuality = Fern::RadioButton(Fern::RadioButtonConfig(200, 80, "Medium Quality", "graphics")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto highQuality = Fern::RadioButton(Fern::RadioButtonConfig(200, 110, "High Quality", "graphics")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    lowQuality->onSelected.connect([]() {
        std::cout << "Graphics: Low Quality selected" << std::endl;
    });
    
    mediumQuality->onSelected.connect([]() {
        std::cout << "Graphics: Medium Quality selected" << std::endl;
    });
    
    highQuality->onSelected.connect([]() {
        std::cout << "Graphics: High Quality selected" << std::endl;
    });
    
    // Example 3: Theme selection
    auto lightTheme = Fern::RadioButton(Fern::RadioButtonConfig(400, 50, "Light Theme", "theme")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Orange)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .borderWidth(2)
            .radius(10)
            .spacing(12)), true);
    
    auto darkTheme = Fern::RadioButton(Fern::RadioButtonConfig(400, 80, "Dark Theme", "theme")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Orange)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .borderWidth(2)
            .radius(10)
            .spacing(12)), true);
    
    lightTheme->onSelected.connect([]() {
        std::cout << "Theme: Light Theme selected" << std::endl;
    });
    
    darkTheme->onSelected.connect([]() {
        std::cout << "Theme: Dark Theme selected" << std::endl;
    });
    
    // Example 4: Language selection
    auto english = Fern::RadioButton(Fern::RadioButtonConfig(50, 170, "English", "language")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto spanish = Fern::RadioButton(Fern::RadioButtonConfig(50, 200, "Español", "language")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto french = Fern::RadioButton(Fern::RadioButtonConfig(50, 230, "Français", "language")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    english->onSelected.connect([]() {
        std::cout << "Language: English selected" << std::endl;
    });
    
    spanish->onSelected.connect([]() {
        std::cout << "Language: Spanish selected" << std::endl;
    });
    
    french->onSelected.connect([]() {
        std::cout << "Language: French selected" << std::endl;
    });
    
    // Example 5: Game difficulty
    auto easy = Fern::RadioButton(Fern::RadioButtonConfig(200, 170, "Easy", "difficulty")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto normal = Fern::RadioButton(Fern::RadioButtonConfig(200, 200, "Normal", "difficulty")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Yellow)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto hard = Fern::RadioButton(Fern::RadioButtonConfig(200, 230, "Hard", "difficulty")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Red)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    easy->onSelected.connect([]() {
        std::cout << "Difficulty: Easy selected" << std::endl;
    });
    
    normal->onSelected.connect([]() {
        std::cout << "Difficulty: Normal selected" << std::endl;
    });
    
    hard->onSelected.connect([]() {
        std::cout << "Difficulty: Hard selected" << std::endl;
    });
    
    // Example 6: Survey satisfaction rating
    auto verySatisfied = Fern::RadioButton(Fern::RadioButtonConfig(400, 170, "Very Satisfied", "satisfaction")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto satisfied = Fern::RadioButton(Fern::RadioButtonConfig(400, 200, "Satisfied", "satisfaction")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto neutral = Fern::RadioButton(Fern::RadioButtonConfig(400, 230, "Neutral", "satisfaction")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Yellow)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto dissatisfied = Fern::RadioButton(Fern::RadioButtonConfig(400, 260, "Dissatisfied", "satisfaction")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Red)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    verySatisfied->onSelected.connect([]() {
        std::cout << "Survey: Very Satisfied selected" << std::endl;
    });
    
    satisfied->onSelected.connect([]() {
        std::cout << "Survey: Satisfied selected" << std::endl;
    });
    
    neutral->onSelected.connect([]() {
        std::cout << "Survey: Neutral selected" << std::endl;
    });
    
    dissatisfied->onSelected.connect([]() {
        std::cout << "Survey: Dissatisfied selected" << std::endl;
    });
    
    // Example 7: Payment method selection
    auto creditCard = Fern::RadioButton(Fern::RadioButtonConfig(50, 320, "Credit Card", "payment")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto paypal = Fern::RadioButton(Fern::RadioButtonConfig(50, 350, "PayPal", "payment")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto bankTransfer = Fern::RadioButton(Fern::RadioButtonConfig(50, 380, "Bank Transfer", "payment")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    creditCard->onSelected.connect([]() {
        std::cout << "Payment: Credit Card selected" << std::endl;
    });
    
    paypal->onSelected.connect([]() {
        std::cout << "Payment: PayPal selected" << std::endl;
    });
    
    bankTransfer->onSelected.connect([]() {
        std::cout << "Payment: Bank Transfer selected" << std::endl;
    });
    
    // Example 8: File format selection
    auto pngFormat = Fern::RadioButton(Fern::RadioButtonConfig(200, 320, "PNG", "format")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto jpgFormat = Fern::RadioButton(Fern::RadioButtonConfig(200, 350, "JPG", "format")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    auto gifFormat = Fern::RadioButton(Fern::RadioButtonConfig(200, 380, "GIF", "format")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .radius(8)
            .spacing(10)), true);
    
    pngFormat->onSelected.connect([]() {
        std::cout << "Format: PNG selected" << std::endl;
    });
    
    jpgFormat->onSelected.connect([]() {
        std::cout << "Format: JPG selected" << std::endl;
    });
    
    gifFormat->onSelected.connect([]() {
        std::cout << "Format: GIF selected" << std::endl;
    });
    
    // Example 9: Custom styled radio buttons
    auto customOption1 = Fern::RadioButton(Fern::RadioButtonConfig(400, 320, "Custom Option 1", "custom")
        .selected(true)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(0xFFF8F9FA)   // Light gray background
            .borderColor(0xFF6C757D)       // Gray border
            .selectedColor(0xFF007BFF)     // Blue selection
            .textColor(0xFF212529)         // Dark text
            .hoverColor(0xFFE9ECEF)        // Light hover
            .borderWidth(2)
            .radius(12)                    // Larger radius
            .spacing(15)                   // More spacing
            .fontSize(2)                   // Larger text
            .useBitmapFont()), true);
    
    auto customOption2 = Fern::RadioButton(Fern::RadioButtonConfig(400, 360, "Custom Option 2", "custom")
        .selected(false)
        .style(Fern::RadioButtonStyle()
            .backgroundColor(0xFFF8F9FA)   // Light gray background
            .borderColor(0xFF6C757D)       // Gray border
            .selectedColor(0xFF007BFF)     // Blue selection
            .textColor(0xFF212529)         // Dark text
            .hoverColor(0xFFE9ECEF)        // Light hover
            .borderWidth(2)
            .radius(12)                    // Larger radius
            .spacing(15)                   // More spacing
            .fontSize(2)                   // Larger text
            .useBitmapFont()), true);
    
    customOption1->onSelected.connect([]() {
        std::cout << "Custom: Option 1 selected" << std::endl;
    });
    
    customOption2->onSelected.connect([]() {
        std::cout << "Custom: Option 2 selected" << std::endl;
    });
    
    // Example 10: Preset styles
    auto modernRadio1 = Fern::RadioButton(Fern::RadioButtonPresets::Modern(50, 450, "Modern Style 1", "modern"), true);
    auto modernRadio2 = Fern::RadioButton(Fern::RadioButtonPresets::Modern(50, 480, "Modern Style 2", "modern"), true);
    
    auto compactRadio1 = Fern::RadioButton(Fern::RadioButtonPresets::Compact(250, 450, "Compact 1", "compact"), true);
    auto compactRadio2 = Fern::RadioButton(Fern::RadioButtonPresets::Compact(250, 480, "Compact 2", "compact"), true);
    
    modernRadio1->onSelected.connect([]() {
        std::cout << "Modern: Style 1 selected" << std::endl;
    });
    
    modernRadio2->onSelected.connect([]() {
        std::cout << "Modern: Style 2 selected" << std::endl;
    });
    
    compactRadio1->onSelected.connect([]() {
        std::cout << "Compact: 1 selected" << std::endl;
    });
    
    compactRadio2->onSelected.connect([]() {
        std::cout << "Compact: 2 selected" << std::endl;
    });
    
    std::cout << "Radio Button Widget Demo" << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << "Click on radio buttons to see selection changes:" << std::endl;
    std::cout << "1. Basic Options (group: basic)" << std::endl;
    std::cout << "2. Graphics Quality (group: graphics)" << std::endl;
    std::cout << "3. Theme Selection (group: theme)" << std::endl;
    std::cout << "4. Language Selection (group: language)" << std::endl;
    std::cout << "5. Game Difficulty (group: difficulty)" << std::endl;
    std::cout << "6. Survey Satisfaction (group: satisfaction)" << std::endl;
    std::cout << "7. Payment Method (group: payment)" << std::endl;
    std::cout << "8. File Format (group: format)" << std::endl;
    std::cout << "9. Custom Styled (group: custom)" << std::endl;
    std::cout << "10. Preset Styles (groups: modern, compact)" << std::endl;
}

    
int main() {
    Fern::initialize();
    setupUI();

    Fern::startRenderLoop();
    return 0;
}