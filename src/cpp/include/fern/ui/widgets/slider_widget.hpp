#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include <memory>
#include <functional>

namespace Fern {
    // Class-based configuration following the modern pattern
    class SliderStyle {
    public:
        // Constructor with sensible defaults
        SliderStyle()
            : trackColor_(0xFF606060)       // Gray track
            , fillColor_(0xFF0080FF)        // Blue fill
            , thumbColor_(0xFFFFFFFF)       // White thumb
            , thumbHoverColor_(0xFFE0E0E0)  // Light gray on hover
            , thumbRadius_(8)
            , showValue_(true)
            , textColor_(0xFFFFFFFF)        // White text
            , textScale_(1)
        {}
        
        // Fluent interface for easy configuration
        SliderStyle& trackColor(uint32_t color) { trackColor_ = color; return *this; }
        SliderStyle& fillColor(uint32_t color) { fillColor_ = color; return *this; }
        SliderStyle& thumbColor(uint32_t color) { thumbColor_ = color; return *this; }
        SliderStyle& thumbHoverColor(uint32_t color) { thumbHoverColor_ = color; return *this; }
        SliderStyle& thumbRadius(int radius) { thumbRadius_ = radius; return *this; }
        SliderStyle& showValue(bool show) { showValue_ = show; return *this; }
        SliderStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        SliderStyle& textScale(int scale) { textScale_ = scale; return *this; }
        
        // Getters
        uint32_t getTrackColor() const { return trackColor_; }
        uint32_t getFillColor() const { return fillColor_; }
        uint32_t getThumbColor() const { return thumbColor_; }
        uint32_t getThumbHoverColor() const { return thumbHoverColor_; }
        int getThumbRadius() const { return thumbRadius_; }
        bool getShowValue() const { return showValue_; }
        uint32_t getTextColor() const { return textColor_; }
        int getTextScale() const { return textScale_; }
        
    private:
        uint32_t trackColor_;
        uint32_t fillColor_;
        uint32_t thumbColor_;
        uint32_t thumbHoverColor_;
        int thumbRadius_;
        bool showValue_;
        uint32_t textColor_;
        int textScale_;
    };
    
    class SliderConfig {
    public:
        SliderConfig(int x, int y, int width = 200, int height = 20)
            : x_(x), y_(y), width_(width), height_(height)
            , minValue_(0.0f), maxValue_(100.0f), initialValue_(50.0f)
        {}
        
        // Fluent interface
        SliderConfig& range(float minValue, float maxValue) { 
            minValue_ = minValue; 
            maxValue_ = maxValue; 
            return *this; 
        }
        SliderConfig& initialValue(float value) { initialValue_ = value; return *this; }
        SliderConfig& style(const SliderStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        float getMinValue() const { return minValue_; }
        float getMaxValue() const { return maxValue_; }
        float getInitialValue() const { return initialValue_; }
        const SliderStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        float minValue_, maxValue_, initialValue_;
        SliderStyle style_;
    };
    
    class SliderWidget : public Widget {
    public:
        SliderWidget(const SliderConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Widget interface
        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Slider specific methods
        void setValue(float value);
        float getValue() const { return currentValue_; }
        void setRange(float minValue, float maxValue);
        
        // Signals
        Signal<float> onValueChanged;   // Emitted when value changes
        Signal<bool> onDragging;        // Emitted when drag state changes
        
    private:
        SliderConfig config_;
        float currentValue_;
        bool isDragging_ = false;
        bool isThumbHovered_ = false;
        
        // Helper methods
        float screenToValue(int screenX) const;
        int valueToScreen(float value) const;
        bool isPointInThumb(int x, int y) const;
        void clampValue();
    };
    
    // Factory function with class-based config
    std::shared_ptr<SliderWidget> Slider(const SliderConfig& config, bool addToManager = false);
    
    // Preset configurations
    namespace SliderPresets {
        SliderConfig Default(int x, int y, int width = 200, int height = 20);
        SliderConfig Volume(int x, int y, int width = 250, int height = 25);
        SliderConfig Brightness(int x, int y, int width = 250, int height = 25);
        SliderConfig ColorComponent(int x, int y, int width = 200, int height = 20);
    }
}
