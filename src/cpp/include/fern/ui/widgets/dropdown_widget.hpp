#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Fern {
    // Class-based configuration for dropdown
    class DropdownStyle {
    public:
        DropdownStyle()
            : backgroundColor_(0xFFFFFFFF)     // White background
            , borderColor_(0xFF666666)        // Gray border
            , selectedColor_(0xFF007BFF)      // Blue selection
            , textColor_(0xFF000000)          // Black text
            , hoverColor_(0xFFE9ECEF)         // Light gray hover
            , dropdownBackgroundColor_(0xFFFFFFFF) // White dropdown background
            , borderWidth_(1)
            , padding_(8)
            , maxVisibleItems_(5)             // Max items before scrolling
            , fontSize_(2)
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
        {}
        
        // Fluent interface
        DropdownStyle& backgroundColor(uint32_t color) { backgroundColor_ = color; return *this; }
        DropdownStyle& borderColor(uint32_t color) { borderColor_ = color; return *this; }
        DropdownStyle& selectedColor(uint32_t color) { selectedColor_ = color; return *this; }
        DropdownStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        DropdownStyle& hoverColor(uint32_t color) { hoverColor_ = color; return *this; }
        DropdownStyle& dropdownBackgroundColor(uint32_t color) { dropdownBackgroundColor_ = color; return *this; }
        DropdownStyle& borderWidth(int width) { borderWidth_ = width; return *this; }
        DropdownStyle& padding(int pad) { padding_ = pad; return *this; }
        DropdownStyle& maxVisibleItems(int count) { maxVisibleItems_ = count; return *this; }
        DropdownStyle& fontSize(int size) { fontSize_ = size; return *this; }
        DropdownStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        DropdownStyle& useTTFFont(const std::string& fontName = "") { 
            fontType_ = FontType::TTF; 
            ttfFontName_ = fontName;
            if (fontSize_ < 16) fontSize_ = 16;
            return *this; 
        }
        
        // Getters
        uint32_t getBackgroundColor() const { return backgroundColor_; }
        uint32_t getBorderColor() const { return borderColor_; }
        uint32_t getSelectedColor() const { return selectedColor_; }
        uint32_t getTextColor() const { return textColor_; }
        uint32_t getHoverColor() const { return hoverColor_; }
        uint32_t getDropdownBackgroundColor() const { return dropdownBackgroundColor_; }
        int getBorderWidth() const { return borderWidth_; }
        int getPadding() const { return padding_; }
        int getMaxVisibleItems() const { return maxVisibleItems_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        
    private:
        uint32_t backgroundColor_;
        uint32_t borderColor_;
        uint32_t selectedColor_;
        uint32_t textColor_;
        uint32_t hoverColor_;
        uint32_t dropdownBackgroundColor_;
        int borderWidth_;
        int padding_;
        int maxVisibleItems_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
    };
    
    struct DropdownItem {
        std::string text;
        std::string value;
        
        DropdownItem(const std::string& text, const std::string& value = "")
            : text(text), value(value.empty() ? text : value) {}
    };
    
    class DropdownConfig {
    public:
        DropdownConfig(int x, int y, int width = 200, int height = 35)
            : x_(x), y_(y), width_(width), height_(height)
            , placeholder_("Select an option...")
            , selectedIndex_(-1)
        {}
        
        // Fluent interface
        DropdownConfig& placeholder(const std::string& text) { placeholder_ = text; return *this; }
        DropdownConfig& items(const std::vector<DropdownItem>& items) { items_ = items; return *this; }
        DropdownConfig& addItem(const std::string& text, const std::string& value = "") { 
            items_.emplace_back(text, value); 
            return *this; 
        }
        DropdownConfig& selectedIndex(int index) { selectedIndex_ = index; return *this; }
        DropdownConfig& style(const DropdownStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        const std::string& getPlaceholder() const { return placeholder_; }
        const std::vector<DropdownItem>& getItems() const { return items_; }
        int getSelectedIndex() const { return selectedIndex_; }
        const DropdownStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        std::string placeholder_;
        std::vector<DropdownItem> items_;
        int selectedIndex_;
        DropdownStyle style_;
    };
    
    class DropdownWidget : public Widget {
    public:
        DropdownWidget(const DropdownConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Widget interface
        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Dropdown specific methods
        void setSelectedIndex(int index);
        int getSelectedIndex() const { return selectedIndex_; }
        const DropdownItem* getSelectedItem() const;
        void addItem(const std::string& text, const std::string& value = "");
        void clearItems();
        void setItems(const std::vector<DropdownItem>& items);
        const std::vector<DropdownItem>& getItems() const { return config_.getItems(); }
        
        // State management
        void open();
        void close();
        bool isOpen() const { return isOpen_; }
        
        // Signals
        Signal<int> onSelectionChanged;                    // Emitted when selection changes (index)
        Signal<const DropdownItem&> onItemSelected;       // Emitted when item is selected
        Signal<bool> onOpenStateChanged;                   // Emitted when dropdown opens/closes
        
    private:
        DropdownConfig config_;
        int selectedIndex_;
        bool isOpen_ = false;
        bool isHovered_ = false;
        int hoveredItemIndex_ = -1;
        
        void renderMainButton();
        void renderDropdown();
        void renderArrow();
        void renderText(const std::string& text, int x, int y, uint32_t color);
        bool isPointInMainButton(int x, int y) const;
        bool isPointInDropdown(int x, int y) const;
        int getItemIndexAtPoint(int x, int y) const;
        int calculateTextWidth(const std::string& text) const;
        int getDropdownHeight() const;
        int getItemHeight() const;
    };
    
    // Factory function
    std::shared_ptr<DropdownWidget> Dropdown(const DropdownConfig& config, bool addToManager = false);
    
    // Preset configurations
    namespace DropdownPresets {
        DropdownConfig Default(int x, int y, int width = 200, int height = 35);
        DropdownConfig Modern(int x, int y, int width = 250, int height = 40);
        DropdownConfig Compact(int x, int y, int width = 150, int height = 25);
    }
}
