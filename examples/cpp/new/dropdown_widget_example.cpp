#include <fern/fern.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Example 1: Basic dropdown
    auto basicDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 50, 200, 35)
        .placeholder("Select an option...")
        .items({
            {"Option 1", "opt1"},
            {"Option 2", "opt2"},
            {"Option 3", "opt3"},
            {"Option 4", "opt4"}
        })
        .selectedIndex(0), true);
    
    basicDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Basic dropdown: " << item.text << " selected (value: " << item.value << ")" << std::endl;
    });
    
    // Example 2: Language selection
    auto languageDropdown = Fern::Dropdown(Fern::DropdownConfig(280, 50, 200, 35)
        .placeholder("Select language...")
        .items({
            {"English", "en"},
            {"Spanish", "es"},
            {"French", "fr"},
            {"German", "de"},
            {"Italian", "it"},
            {"Japanese", "ja"},
            {"Chinese", "zh"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .maxVisibleItems(4)), true);
    
    languageDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Language: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    // Example 3: Graphics quality settings
    auto qualityDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 110, 180, 30)
        .placeholder("Graphics Quality")
        .items({
            {"Low", "low"},
            {"Medium", "medium"},
            {"High", "high"},
            {"Ultra", "ultra"}
        })
        .selectedIndex(1)  // Default to Medium
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    qualityDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Graphics quality: " << item.text << std::endl;
    });
    
    // Example 4: Resolution selection
    auto resolutionDropdown = Fern::Dropdown(Fern::DropdownConfig(250, 110, 180, 30)
        .placeholder("Resolution")
        .items({
            {"1280x720", "720p"},
            {"1920x1080", "1080p"},
            {"2560x1440", "1440p"},
            {"3840x2160", "4k"}
        })
        .selectedIndex(1)  // Default to 1080p
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    resolutionDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Resolution: " << item.text << std::endl;
    });
    
    // Example 5: Audio output selection
    auto audioDropdown = Fern::Dropdown(Fern::DropdownConfig(450, 110, 180, 30)
        .placeholder("Audio Output")
        .items({
            {"Speakers", "speakers"},
            {"Headphones", "headphones"},
            {"USB Audio", "usb"},
            {"Bluetooth", "bluetooth"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    audioDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Audio output: " << item.text << std::endl;
    });
    
    // Example 6: File format selection
    auto formatDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 170, 150, 30)
        .placeholder("File Format")
        .items({
            {"PNG", "png"},
            {"JPG", "jpg"},
            {"GIF", "gif"},
            {"BMP", "bmp"},
            {"TIFF", "tiff"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    formatDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "File format: " << item.text << std::endl;
    });
    
    // Example 7: Font selection
    auto fontDropdown = Fern::Dropdown(Fern::DropdownConfig(220, 170, 180, 30)
        .placeholder("Font Family")
        .items({
            {"Arial", "arial"},
            {"Times New Roman", "times"},
            {"Courier New", "courier"},
            {"Helvetica", "helvetica"},
            {"Georgia", "georgia"},
            {"Verdana", "verdana"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .maxVisibleItems(4)), true);
    
    fontDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Font: " << item.text << std::endl;
    });
    
    // Example 8: Country selection
    auto countryDropdown = Fern::Dropdown(Fern::DropdownConfig(420, 170, 200, 30)
        .placeholder("Select country...")
        .items({
            {"United States", "us"},
            {"Canada", "ca"},
            {"United Kingdom", "uk"},
            {"France", "fr"},
            {"Germany", "de"},
            {"Japan", "jp"},
            {"Australia", "au"},
            {"Brazil", "br"},
            {"India", "in"},
            {"China", "cn"}
        })
        .selectedIndex(-1)  // No default selection
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Orange)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .maxVisibleItems(6)), true);
    
    countryDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Country: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    // Example 9: Product size selection
    auto sizeDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 230, 120, 30)
        .placeholder("Size")
        .items({
            {"Small", "s"},
            {"Medium", "m"},
            {"Large", "l"},
            {"Extra Large", "xl"},
            {"XXL", "xxl"}
        })
        .selectedIndex(1)  // Default to Medium
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Cyan)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    sizeDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Size: " << item.text << std::endl;
    });
    
    // Example 10: Color selection
    auto colorDropdown = Fern::Dropdown(Fern::DropdownConfig(190, 230, 120, 30)
        .placeholder("Color")
        .items({
            {"Red", "red"},
            {"Blue", "blue"},
            {"Green", "green"},
            {"Yellow", "yellow"},
            {"Black", "black"},
            {"White", "white"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Cyan)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    colorDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Color: " << item.text << std::endl;
    });
    
    // Example 11: Material selection
    auto materialDropdown = Fern::Dropdown(Fern::DropdownConfig(330, 230, 120, 30)
        .placeholder("Material")
        .items({
            {"Cotton", "cotton"},
            {"Polyester", "polyester"},
            {"Wool", "wool"},
            {"Silk", "silk"},
            {"Linen", "linen"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Cyan)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    materialDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Material: " << item.text << std::endl;
    });
    
    // Example 12: Priority selection
    auto priorityDropdown = Fern::Dropdown(Fern::DropdownConfig(470, 230, 120, 30)
        .placeholder("Priority")
        .items({
            {"Low", "low"},
            {"Normal", "normal"},
            {"High", "high"},
            {"Critical", "critical"}
        })
        .selectedIndex(1)  // Default to Normal
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Red)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    priorityDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Priority: " << item.text << std::endl;
    });
    
    // Example 13: Custom styled dropdown
    auto customDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 290, 250, 35)
        .placeholder("Custom Styled Dropdown")
        .items({
            {"Option Alpha", "alpha"},
            {"Option Beta", "beta"},
            {"Option Gamma", "gamma"},
            {"Option Delta", "delta"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(0xFFF8F9FA)       // Light gray background
            .borderColor(0xFF6C757D)           // Gray border
            .selectedColor(0xFF007BFF)         // Blue selection
            .textColor(0xFF212529)             // Dark text
            .hoverColor(0xFFE9ECEF)            // Light hover
            .dropdownBackgroundColor(0xFFFFFFFF) // White dropdown
            .borderWidth(2)
            .padding(10)
            .maxVisibleItems(3)
            .fontSize(2)
            .useBitmapFont()), true);
    
    customDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Custom dropdown: " << item.text << " selected" << std::endl;
    });
    
    // Example 14: Preset styles
    auto modernDropdown = Fern::Dropdown(Fern::DropdownPresets::Modern(320, 290, 200, 35), true);
    modernDropdown->setItems({
        {"Modern Option 1", "mod1"},
        {"Modern Option 2", "mod2"},
        {"Modern Option 3", "mod3"}
    });
    modernDropdown->setSelectedIndex(0);
    
    modernDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Modern dropdown: " << item.text << std::endl;
    });
    
    // Example 15: Compact dropdown
    auto compactDropdown = Fern::Dropdown(Fern::DropdownPresets::Compact(50, 350, 130, 25), true);
    compactDropdown->setItems({
        {"Compact 1", "c1"},
        {"Compact 2", "c2"},
        {"Compact 3", "c3"}
    });
    compactDropdown->setSelectedIndex(0);
    
    compactDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Compact dropdown: " << item.text << std::endl;
    });
    
    // Example 16: Time zone selection
    auto timezoneDropdown = Fern::Dropdown(Fern::DropdownConfig(200, 350, 220, 30)
        .placeholder("Select timezone...")
        .items({
            {"UTC-08:00 (PST)", "pst"},
            {"UTC-05:00 (EST)", "est"},
            {"UTC+00:00 (GMT)", "gmt"},
            {"UTC+01:00 (CET)", "cet"},
            {"UTC+09:00 (JST)", "jst"}
        })
        .selectedIndex(2)  // Default to GMT
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Blue)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .maxVisibleItems(4)), true);
    
    timezoneDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Timezone: " << item.text << std::endl;
    });
    
    // Example 17: Sort options
    auto sortDropdown = Fern::Dropdown(Fern::DropdownConfig(440, 350, 150, 30)
        .placeholder("Sort by...")
        .items({
            {"Name (A-Z)", "name_asc"},
            {"Name (Z-A)", "name_desc"},
            {"Date (Newest)", "date_desc"},
            {"Date (Oldest)", "date_asc"},
            {"Size (Largest)", "size_desc"},
            {"Size (Smallest)", "size_asc"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Purple)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)
            .maxVisibleItems(5)), true);
    
    sortDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Sort by: " << item.text << " (" << item.value << ")" << std::endl;
    });
    
    // Example 18: Dynamic dropdown that changes based on other selections
    auto categoryDropdown = Fern::Dropdown(Fern::DropdownConfig(50, 410, 150, 30)
        .placeholder("Category")
        .items({
            {"Electronics", "electronics"},
            {"Clothing", "clothing"},
            {"Books", "books"}
        })
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    auto subcategoryDropdown = Fern::Dropdown(Fern::DropdownConfig(220, 410, 150, 30)
        .placeholder("Subcategory")
        .items({
            {"Smartphones", "smartphones"},
            {"Laptops", "laptops"},
            {"Tablets", "tablets"}
        })  // Default to Electronics subcategories
        .selectedIndex(0)
        .style(Fern::DropdownStyle()
            .backgroundColor(Fern::Colors::White)
            .borderColor(Fern::Colors::Gray)
            .selectedColor(Fern::Colors::Green)
            .textColor(Fern::Colors::Black)
            .hoverColor(Fern::Colors::LightGray)), true);
    
    // Update subcategory based on category selection
    categoryDropdown->onItemSelected.connect([=](const Fern::DropdownItem& category) {
        std::cout << "Category: " << category.text << std::endl;
        
        if (category.value == "electronics") {
            subcategoryDropdown->setItems({
                {"Smartphones", "smartphones"},
                {"Laptops", "laptops"},
                {"Tablets", "tablets"}
            });
        } else if (category.value == "clothing") {
            subcategoryDropdown->setItems({
                {"Shirts", "shirts"},
                {"Pants", "pants"},
                {"Shoes", "shoes"}
            });
        } else if (category.value == "books") {
            subcategoryDropdown->setItems({
                {"Fiction", "fiction"},
                {"Non-fiction", "nonfiction"},
                {"Technical", "technical"}
            });
        }
        
        subcategoryDropdown->setSelectedIndex(0);
    });
    
    subcategoryDropdown->onItemSelected.connect([](const Fern::DropdownItem& item) {
        std::cout << "Subcategory: " << item.text << std::endl;
    });
    
    // Open/close state tracking
    categoryDropdown->onOpenStateChanged.connect([](bool isOpen) {
        std::cout << "Category dropdown " << (isOpen ? "opened" : "closed") << std::endl;
    });
    
    std::cout << "Dropdown Widget Demo" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << "Click on dropdowns to see selection changes:" << std::endl;
    std::cout << "1. Basic dropdown" << std::endl;
    std::cout << "2. Language selection" << std::endl;
    std::cout << "3. Graphics quality settings" << std::endl;
    std::cout << "4. Resolution selection" << std::endl;
    std::cout << "5. Audio output selection" << std::endl;
    std::cout << "6. File format selection" << std::endl;
    std::cout << "7. Font selection" << std::endl;
    std::cout << "8. Country selection" << std::endl;
    std::cout << "9-12. Product configuration (size, color, material, priority)" << std::endl;
    std::cout << "13. Custom styled dropdown" << std::endl;
    std::cout << "14. Modern preset" << std::endl;
    std::cout << "15. Compact preset" << std::endl;
    std::cout << "16. Time zone selection" << std::endl;
    std::cout << "17. Sort options" << std::endl;
    std::cout << "18. Dynamic category/subcategory dropdowns" << std::endl;
}

int main() {
    Fern::initialize();
    setupUI();

    Fern::startRenderLoop();
    return 0;
}