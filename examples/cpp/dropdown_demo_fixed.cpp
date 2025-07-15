#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

std::shared_ptr<DropdownWidget> languageDropdown;
std::shared_ptr<DropdownWidget> countryDropdown;
std::shared_ptr<DropdownWidget> colorDropdown;
std::shared_ptr<DropdownWidget> fontSizeDropdown;

void setupEventHandlers();

void setupDropdowns() {
    // Language selection dropdown
    std::vector<DropdownItem> languages = {
        DropdownItem("English"),
        DropdownItem("Spanish"),
        DropdownItem("French"),
        DropdownItem("German"),
        DropdownItem("Italian"),
        DropdownItem("Portuguese"),
        DropdownItem("Russian"),
        DropdownItem("Chinese"),
        DropdownItem("Japanese"),
        DropdownItem("Korean")
    };
    
    languageDropdown = Dropdown(
        DropdownConfig(50, 100, 200, 35)
            .placeholder("Select Language")
            .items(languages)
            .selectedIndex(0)  // Default to English
            .style(DropdownStyle()
                .backgroundColor(Colors::White)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::LightBlue)
                .hoverColor(Colors::LightGray)
                .textColor(Colors::Black)
                .fontSize(2)
                .borderWidth(1)
                .useBitmapFont()),
        true
    );

    // Country selection dropdown
    std::vector<DropdownItem> countries = {
        DropdownItem("United States"),
        DropdownItem("Canada"),
        DropdownItem("Mexico"),
        DropdownItem("Brazil"),
        DropdownItem("Argentina"),
        DropdownItem("United Kingdom"),
        DropdownItem("France"),
        DropdownItem("Germany"),
        DropdownItem("Spain"),
        DropdownItem("Italy"),
        DropdownItem("China"),
        DropdownItem("Japan"),
        DropdownItem("South Korea"),
        DropdownItem("Australia"),
        DropdownItem("India")
    };
    
    countryDropdown = Dropdown(
        DropdownConfig(300, 100, 250, 35)
            .placeholder("Select Country")
            .items(countries)
            .style(DropdownStyle()
                .backgroundColor(0xFFE8F4FD)  // Light blue background
                .borderColor(Colors::Blue)
                .selectedColor(Colors::Blue)
                .hoverColor(0xFFD1E7DD)       // Light green hover
                .textColor(Colors::DarkBlue)
                .fontSize(2)
                .borderWidth(2)
                .maxVisibleItems(6)
                .useBitmapFont()),
        true
    );

    // Color theme dropdown
    std::vector<DropdownItem> colors = {
        DropdownItem("Default"),
        DropdownItem("Red Theme"),
        DropdownItem("Green Theme"),
        DropdownItem("Blue Theme"),
        DropdownItem("Purple Theme"),
        DropdownItem("Orange Theme"),
        DropdownItem("Dark Theme")
    };
    
    colorDropdown = Dropdown(
        DropdownConfig(50, 200, 200, 30)
            .placeholder("Select Theme")
            .items(colors)
            .selectedIndex(0)  // Default theme
            .style(DropdownStyle()
                .backgroundColor(Colors::Black)
                .borderColor(Colors::White)
                .selectedColor(Colors::DarkGray)
                .hoverColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(2)
                .borderWidth(1)
                .maxVisibleItems(4)
                .useBitmapFont()),
        true
    );

    // Font size dropdown (demonstrating different styling)
    std::vector<DropdownItem> fontSizes = {
        DropdownItem("Small (1x)"),
        DropdownItem("Medium (2x)"),
        DropdownItem("Large (3x)"),
        DropdownItem("Extra Large (4x)")
    };
    
    fontSizeDropdown = Dropdown(
        DropdownConfig(300, 200, 220, 30)
            .placeholder("Select Font Size")
            .items(fontSizes)
            .selectedIndex(1)  // Default to medium
            .style(DropdownStyle()
                .backgroundColor(0xFFF8F9FA)  // Very light gray
                .borderColor(0xFF6C757D)      // Medium gray
                .selectedColor(0xFF007BFF)    // Bootstrap blue
                .hoverColor(0xFFE9ECEF)       // Light hover
                .textColor(0xFF212529)        // Dark gray text
                .fontSize(2)
                .borderWidth(1)
                .maxVisibleItems(4)
                .useBitmapFont()),
        true
    );

    setupEventHandlers();
}

void setupEventHandlers() {
    // Language dropdown events
    languageDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = languageDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Language selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
        }
    });

    languageDropdown->onOpenStateChanged.connect([](bool isOpen) {
        std::cout << "Language dropdown " << (isOpen ? "opened" : "closed") << std::endl;
    });

    // Country dropdown events
    countryDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = countryDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Country selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
        }
    });

    // Color theme dropdown events
    colorDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = colorDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Color theme selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
            
            // You could change the overall UI color scheme here
            if (selectedItem->text == "Red Theme") {
                std::cout << "Switching to red color scheme..." << std::endl;
            } else if (selectedItem->text == "Dark Theme") {
                std::cout << "Switching to dark color scheme..." << std::endl;
            }
        }
    });

    // Font size dropdown events
    fontSizeDropdown->onSelectionChanged.connect([](int index) {
        auto selectedItem = fontSizeDropdown->getSelectedItem();
        if (selectedItem) {
            std::cout << "Font size selected: " << selectedItem->text << " (index: " << index << ")" << std::endl;
            
            // You could change the UI font size here
            int scale = index + 1;  // Convert index to scale (1-4)
            std::cout << "Text scale would be: " << scale << std::endl;
        }
    });
}

void draw() {
    // Clear screen
    Draw::fill(0xFF2C3E50); // Dark blue-gray background

    // Title
    Text(Point(50, 30), "DROPDOWN DEMO", 3, Colors::White, true);

    // Section labels
    Text(Point(50, 70), "Language & Country:", 2, Colors::Yellow, true);
    Text(Point(50, 170), "Theme & Font Size:", 2, Colors::Yellow, true);

    // Current selections display
    Text(Point(50, 280), "Current Selections:", 2, Colors::LightBlue, true);
    
    // Get current selections and display them
    auto languageItem = languageDropdown->getSelectedItem();
    auto countryItem = countryDropdown->getSelectedItem();
    auto colorItem = colorDropdown->getSelectedItem();
    auto fontItem = fontSizeDropdown->getSelectedItem();
    
    char buffer[256];
    sprintf(buffer, "Language: %s", languageItem ? languageItem->text.c_str() : "None");
    Text(Point(50, 310), buffer, 1, Colors::White, true);
    
    sprintf(buffer, "Country: %s", countryItem ? countryItem->text.c_str() : "None");
    Text(Point(50, 330), buffer, 1, Colors::White, true);
    
    sprintf(buffer, "Theme: %s", colorItem ? colorItem->text.c_str() : "None");
    Text(Point(50, 350), buffer, 1, Colors::White, true);
    
    sprintf(buffer, "Font: %s", fontItem ? fontItem->text.c_str() : "None");
    Text(Point(50, 370), buffer, 1, Colors::White, true);

    // Instructions
    Text(Point(50, 420), "Click dropdowns to see options", 1, Colors::LightGray, true);
    Text(Point(50, 440), "Notice different styling for each dropdown", 1, Colors::LightGray, true);
    Text(Point(50, 460), "Some dropdowns have default selections", 1, Colors::LightGray, true);

    // Dropdown state info
    bool langOpen = languageDropdown->isOpen();
    bool countryOpen = countryDropdown->isOpen();
    
    if (langOpen || countryOpen) {
        const char* openDropdown = langOpen ? "Language" : "Country";
        sprintf(buffer, "%s dropdown is currently open", openDropdown);
        Text(Point(300, 420), buffer, 1, Colors::Yellow, true);
    }
}

int main() {
    Fern::initialize();
    setupDropdowns();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
