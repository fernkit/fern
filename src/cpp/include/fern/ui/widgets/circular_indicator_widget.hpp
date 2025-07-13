#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <memory>
#include <string>

namespace Fern {
    // Class-based configuration for circular indicator
    class CircularIndicatorStyle {
    public:
        CircularIndicatorStyle()
            : backgroundColor_(0xFF404040)      // Dark gray background
            , fillColor_(0xFF00AA00)           // Green fill
            , borderColor_(0xFF808080)         // Gray border
            , textColor_(0xFFFFFFFF)           // White text
            , borderWidth_(2)
            , thickness_(8)                    // Arc thickness
            , showPercentage_(true)
            , fontSize_(2)
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
            , clockwise_(true)
            , startAngle_(0.0f)               // Start from top (0 degrees)
        {}
        
        // Fluent interface
        CircularIndicatorStyle& backgroundColor(uint32_t color) { backgroundColor_ = color; return *this; }
        CircularIndicatorStyle& fillColor(uint32_t color) { fillColor_ = color; return *this; }
        CircularIndicatorStyle& borderColor(uint32_t color) { borderColor_ = color; return *this; }
        CircularIndicatorStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        CircularIndicatorStyle& borderWidth(int width) { borderWidth_ = width; return *this; }
        CircularIndicatorStyle& thickness(int thick) { thickness_ = thick; return *this; }
        CircularIndicatorStyle& showPercentage(bool show) { showPercentage_ = show; return *this; }
        CircularIndicatorStyle& fontSize(int size) { fontSize_ = size; return *this; }
        CircularIndicatorStyle& clockwise(bool cw) { clockwise_ = cw; return *this; }
        CircularIndicatorStyle& startAngle(float angle) { startAngle_ = angle; return *this; }
        CircularIndicatorStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        CircularIndicatorStyle& useTTFFont(const std::string& fontName = "") { 
            fontType_ = FontType::TTF; 
            ttfFontName_ = fontName;
            if (fontSize_ < 16) fontSize_ = 16;
            return *this; 
        }
        
        // Getters
        uint32_t getBackgroundColor() const { return backgroundColor_; }
        uint32_t getFillColor() const { return fillColor_; }
        uint32_t getBorderColor() const { return borderColor_; }
        uint32_t getTextColor() const { return textColor_; }
        int getBorderWidth() const { return borderWidth_; }
        int getThickness() const { return thickness_; }
        bool getShowPercentage() const { return showPercentage_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        bool isClockwise() const { return clockwise_; }
        float getStartAngle() const { return startAngle_; }
        
    private:
        uint32_t backgroundColor_;
        uint32_t fillColor_;
        uint32_t borderColor_;
        uint32_t textColor_;
        int borderWidth_;
        int thickness_;
        bool showPercentage_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
        bool clockwise_;
        float startAngle_;
    };
    
    class CircularIndicatorConfig {
    public:
        CircularIndicatorConfig(int x, int y, int radius = 50)
            : x_(x), y_(y), radius_(radius)
            , minValue_(0.0f), maxValue_(100.0f), currentValue_(0.0f)
        {}
        
        // Fluent interface
        CircularIndicatorConfig& range(float minValue, float maxValue) { 
            minValue_ = minValue; 
            maxValue_ = maxValue; 
            return *this; 
        }
        CircularIndicatorConfig& value(float value) { currentValue_ = value; return *this; }
        CircularIndicatorConfig& radius(int r) { radius_ = r; return *this; }
        CircularIndicatorConfig& style(const CircularIndicatorStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getRadius() const { return radius_; }
        float getMinValue() const { return minValue_; }
        float getMaxValue() const { return maxValue_; }
        float getCurrentValue() const { return currentValue_; }
        const CircularIndicatorStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setRadius(int radius) { radius_ = radius; }
        
    private:
        int x_, y_, radius_;
        float minValue_, maxValue_, currentValue_;
        CircularIndicatorStyle style_;
    };
    
    class CircularIndicatorWidget : public Widget {
    public:
        CircularIndicatorWidget(const CircularIndicatorConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Widget interface
        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Circular indicator specific methods
        void setValue(float value);
        float getValue() const { return currentValue_; }
        void setRange(float minValue, float maxValue);
        float getPercentage() const;
        
        // Signals
        Signal<float> onValueChanged;   // Emitted when value changes
        Signal<> onComplete;            // Emitted when reaches 100%
        
    private:
        CircularIndicatorConfig config_;
        float currentValue_;
        
        void clampValue();
        void renderArc(int centerX, int centerY, int radius, float startAngle, float endAngle, uint32_t color);
        void renderText();
    };
    
    // Factory function
    std::shared_ptr<CircularIndicatorWidget> CircularIndicator(const CircularIndicatorConfig& config, bool addToManager = false);
    
    // Preset configurations
    namespace CircularIndicatorPresets {
        CircularIndicatorConfig Default(int x, int y, int radius = 50);
        CircularIndicatorConfig Loading(int x, int y, int radius = 40);
        CircularIndicatorConfig Health(int x, int y, int radius = 30);
        CircularIndicatorConfig Battery(int x, int y, int radius = 25);
    }
}
