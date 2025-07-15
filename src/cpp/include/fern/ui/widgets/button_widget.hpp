#pragma once

#include "widget.hpp"
#include <string>
#include <functional>
#include <memory>
#include "../../core/signal.hpp"

namespace Fern {
    
    // Modern class-based configuration for ButtonWidget
    class ButtonStyle {
    public:
        ButtonStyle()
            : normalColor_(0xFF3366CC)     // Blue
            , hoverColor_(0xFF4477DD)      // Lighter blue
            , pressColor_(0xFF2255BB)      // Darker blue
            , textColor_(0xFFFFFFFF)       // White text
            , textScale_(2)                // Good default size
            , borderRadius_(4)             // Rounded corners
            , borderWidth_(0)              // No border by default
            , borderColor_(0xFF666666)     // Gray border
        {}
        
        // Fluent interface for easy configuration
        ButtonStyle& normalColor(uint32_t color) { normalColor_ = color; return *this; }
        ButtonStyle& hoverColor(uint32_t color) { hoverColor_ = color; return *this; }
        ButtonStyle& pressColor(uint32_t color) { pressColor_ = color; return *this; }
        ButtonStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        ButtonStyle& textScale(int scale) { textScale_ = scale; return *this; }
        ButtonStyle& borderRadius(int radius) { borderRadius_ = radius; return *this; }
        ButtonStyle& border(int width, uint32_t color) { 
            borderWidth_ = width; 
            borderColor_ = color; 
            return *this; 
        }
        
        // Getters
        uint32_t getNormalColor() const { return normalColor_; }
        uint32_t getHoverColor() const { return hoverColor_; }
        uint32_t getPressColor() const { return pressColor_; }
        uint32_t getTextColor() const { return textColor_; }
        int getTextScale() const { return textScale_; }
        int getBorderRadius() const { return borderRadius_; }
        int getBorderWidth() const { return borderWidth_; }
        uint32_t getBorderColor() const { return borderColor_; }
        
    private:
        uint32_t normalColor_;
        uint32_t hoverColor_;
        uint32_t pressColor_;
        uint32_t textColor_;
        int textScale_;
        int borderRadius_;
        int borderWidth_;
        uint32_t borderColor_;
    };
    
    class ButtonConfig {
    public:
        ButtonConfig(int x, int y, int width, int height, const std::string& label)
            : x_(x), y_(y), width_(width), height_(height), label_(label)
        {}
        
        // Fluent interface
        ButtonConfig& style(const ButtonStyle& s) { style_ = s; return *this; }
        ButtonConfig& label(const std::string& text) { label_ = text; return *this; }
        ButtonConfig& position(int x, int y) { x_ = x; y_ = y; return *this; }
        ButtonConfig& size(int width, int height) { width_ = width; height_ = height; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        const std::string& getLabel() const { return label_; }
        const ButtonStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        std::string label_;
        ButtonStyle style_;
    };

    class ButtonWidget : public Widget {
    public:
        ButtonWidget(const ButtonConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;

        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Configuration updates
        void setConfig(const ButtonConfig& config);
        void setLabel(const std::string& label);
        const ButtonConfig& getConfig() const { return config_; }
    
        Signal<> onClick;       
        Signal<bool> onHover;   
        Signal<bool> onPress;   
        
    private:
        ButtonConfig config_;
        bool isHovered_ = false;
        bool isPressed_ = false;
        
        void renderBackground();
        void renderBorder();
        void renderText();
    };
    
    // Modern factory function
    std::shared_ptr<ButtonWidget> Button(const ButtonConfig& config, bool addToManager = true);
    
    // Preset configurations
    namespace ButtonPresets {
        ButtonConfig Primary(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Secondary(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Success(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Danger(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Warning(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Info(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Light(int x, int y, int width, int height, const std::string& label);
        ButtonConfig Dark(int x, int y, int width, int height, const std::string& label);
    }
}