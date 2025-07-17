#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <memory>
#include <string>

namespace Fern {
    /**
     * @brief Style configuration for ProgressBarWidget
     * 
     * ProgressBarStyle provides a fluent interface to configure the appearance
     * of progress bars including colors, borders, text display, and fonts.
     * 
     * @example Basic progress bar styling:
     * @code
     * ProgressBarStyle style;
     * style.backgroundColor(Colors::Gray)
     *      .fillColor(Colors::Green)
     *      .showPercentage(true)
     *      .borderWidth(2);
     * @endcode
     * 
     * @example Custom color progress bar:
     * @code
     * ProgressBarStyle style;
     * style.fillColor(Colors::Blue)
     *      .textColor(Colors::White)
     *      .useTTFFont("arial")
     *      .fontSize(18);
     * @endcode
     */
    class ProgressBarStyle {
    public:
        ProgressBarStyle()
            : backgroundColor_(0xFF404040)      // Dark gray background
            , fillColor_(0xFF00AA00)           // Green fill
            , borderColor_(0xFF808080)         // Gray border
            , textColor_(0xFFFFFFFF)           // White text
            , borderWidth_(1)
            , showPercentage_(true)
            , fontSize_(2)
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
        {}
        
        // Fluent interface
        ProgressBarStyle& backgroundColor(uint32_t color) { backgroundColor_ = color; return *this; }
        ProgressBarStyle& fillColor(uint32_t color) { fillColor_ = color; return *this; }
        ProgressBarStyle& borderColor(uint32_t color) { borderColor_ = color; return *this; }
        ProgressBarStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        ProgressBarStyle& borderWidth(int width) { borderWidth_ = width; return *this; }
        ProgressBarStyle& showPercentage(bool show) { showPercentage_ = show; return *this; }
        ProgressBarStyle& fontSize(int size) { fontSize_ = size; return *this; }
        ProgressBarStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        ProgressBarStyle& useTTFFont(const std::string& fontName = "") { 
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
        bool getShowPercentage() const { return showPercentage_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        
    private:
        uint32_t backgroundColor_;
        uint32_t fillColor_;
        uint32_t borderColor_;
        uint32_t textColor_;
        int borderWidth_;
        bool showPercentage_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
    };
    
    class ProgressBarConfig {
    public:
        ProgressBarConfig(int x, int y, int width = 300, int height = 25)
            : x_(x), y_(y), width_(width), height_(height)
            , minValue_(0.0f), maxValue_(100.0f), currentValue_(0.0f)
        {}
        
        // Fluent interface
        ProgressBarConfig& range(float minValue, float maxValue) { 
            minValue_ = minValue; 
            maxValue_ = maxValue; 
            return *this; 
        }
        ProgressBarConfig& value(float value) { currentValue_ = value; return *this; }
        ProgressBarConfig& style(const ProgressBarStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        float getMinValue() const { return minValue_; }
        float getMaxValue() const { return maxValue_; }
        float getCurrentValue() const { return currentValue_; }
        const ProgressBarStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        float minValue_, maxValue_, currentValue_;
        ProgressBarStyle style_;
    };
    
    class ProgressBarWidget : public Widget {
    public:
        ProgressBarWidget(const ProgressBarConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Widget interface
        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Progress bar specific methods
        void setValue(float value);
        float getValue() const { return currentValue_; }
        void setRange(float minValue, float maxValue);
        float getPercentage() const;
        
        // Signals
        Signal<float> onValueChanged;   // Emitted when value changes
        Signal<> onComplete;            // Emitted when reaches 100%
        
    private:
        ProgressBarConfig config_;
        float currentValue_;
        
        void clampValue();
        void renderText();
    };
    
    // Factory function
    std::shared_ptr<ProgressBarWidget> ProgressBar(const ProgressBarConfig& config, bool addToManager = false);
    
    // Preset configurations
    namespace ProgressBarPresets {
        ProgressBarConfig Default(int x, int y, int width = 300, int height = 25);
        ProgressBarConfig Loading(int x, int y, int width = 250, int height = 20);
        ProgressBarConfig Health(int x, int y, int width = 200, int height = 15);
        ProgressBarConfig Download(int x, int y, int width = 400, int height = 30);
    }
}
