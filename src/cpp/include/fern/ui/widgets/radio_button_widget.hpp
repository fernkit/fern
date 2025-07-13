#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Fern {
    // Class-based configuration for radio button
    class RadioButtonStyle {
    public:
        RadioButtonStyle()
            : backgroundColor_(0xFFFFFFFF)     // White background
            , borderColor_(0xFF666666)        // Gray border
            , selectedColor_(0xFF007BFF)      // Blue when selected
            , textColor_(0xFF000000)          // Black text
            , hoverColor_(0xFFE9ECEF)         // Light gray hover
            , borderWidth_(2)
            , radius_(8)                      // Radio button radius
            , spacing_(8)                     // Space between button and text
            , fontSize_(2)
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
        {}
        
        // Fluent interface
        RadioButtonStyle& backgroundColor(uint32_t color) { backgroundColor_ = color; return *this; }
        RadioButtonStyle& borderColor(uint32_t color) { borderColor_ = color; return *this; }
        RadioButtonStyle& selectedColor(uint32_t color) { selectedColor_ = color; return *this; }
        RadioButtonStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        RadioButtonStyle& hoverColor(uint32_t color) { hoverColor_ = color; return *this; }
        RadioButtonStyle& borderWidth(int width) { borderWidth_ = width; return *this; }
        RadioButtonStyle& radius(int r) { radius_ = r; return *this; }
        RadioButtonStyle& spacing(int space) { spacing_ = space; return *this; }
        RadioButtonStyle& fontSize(int size) { fontSize_ = size; return *this; }
        RadioButtonStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        RadioButtonStyle& useTTFFont(const std::string& fontName = "") { 
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
        int getBorderWidth() const { return borderWidth_; }
        int getRadius() const { return radius_; }
        int getSpacing() const { return spacing_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        
    private:
        uint32_t backgroundColor_;
        uint32_t borderColor_;
        uint32_t selectedColor_;
        uint32_t textColor_;
        uint32_t hoverColor_;
        int borderWidth_;
        int radius_;
        int spacing_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
    };
    
    class RadioButtonConfig {
    public:
        RadioButtonConfig(int x, int y, const std::string& text, const std::string& groupName)
            : x_(x), y_(y), text_(text), groupName_(groupName), selected_(false)
        {}
        
        // Fluent interface
        RadioButtonConfig& text(const std::string& t) { text_ = t; return *this; }
        RadioButtonConfig& selected(bool sel) { selected_ = sel; return *this; }
        RadioButtonConfig& style(const RadioButtonStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        const std::string& getText() const { return text_; }
        const std::string& getGroupName() const { return groupName_; }
        bool isSelected() const { return selected_; }
        const RadioButtonStyle& getStyle() const { return style_; }
        
        // Position setter
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        
    private:
        int x_, y_;
        std::string text_;
        std::string groupName_;
        bool selected_;
        RadioButtonStyle style_;
    };
    
    // Forward declaration for group management
    class RadioButtonGroup;
    
    class RadioButtonWidget : public Widget {
    public:
        RadioButtonWidget(const RadioButtonConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Widget interface
        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Radio button specific methods
        void setSelected(bool selected);
        bool isSelected() const { return selected_; }
        void setText(const std::string& text);
        const std::string& getText() const { return config_.getText(); }
        const std::string& getGroupName() const { return config_.getGroupName(); }
        
        // Signals
        Signal<bool> onSelectionChanged;   // Emitted when selection changes
        Signal<> onSelected;               // Emitted when this button is selected
        
        // Group management
        void setGroup(std::shared_ptr<RadioButtonGroup> group) { group_ = group; }
        
    private:
        RadioButtonConfig config_;
        bool selected_;
        bool isHovered_ = false;
        std::shared_ptr<RadioButtonGroup> group_;
        
        void renderText();
        bool isPointInRadioButton(int x, int y) const;
        int calculateTextWidth() const;
    };
    
    // Radio button group for managing mutual exclusion
    class RadioButtonGroup {
    public:
        void addButton(std::shared_ptr<RadioButtonWidget> button);
        void selectButton(std::shared_ptr<RadioButtonWidget> button);
        std::shared_ptr<RadioButtonWidget> getSelected() const;
        const std::vector<std::shared_ptr<RadioButtonWidget>>& getButtons() const { return buttons_; }
        
        // Signals
        Signal<std::shared_ptr<RadioButtonWidget>> onSelectionChanged;
        
    private:
        std::vector<std::shared_ptr<RadioButtonWidget>> buttons_;
        std::shared_ptr<RadioButtonWidget> selected_;
    };
    
    // Factory functions
    std::shared_ptr<RadioButtonWidget> RadioButton(const RadioButtonConfig& config, bool addToManager = false);
    std::shared_ptr<RadioButtonGroup> RadioGroup();
    
    // Preset configurations
    namespace RadioButtonPresets {
        RadioButtonConfig Default(int x, int y, const std::string& text, const std::string& group);
        RadioButtonConfig Modern(int x, int y, const std::string& text, const std::string& group);
        RadioButtonConfig Compact(int x, int y, const std::string& text, const std::string& group);
    }
}
