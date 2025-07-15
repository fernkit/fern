#pragma once
#include <string>
#include "widget.hpp"          
#include "../../core/input.hpp"
#include "../../core/types.hpp"
#include "../../font/font.hpp"

namespace Fern {
    
    // Modern configuration classes for TextWidget
    class TextStyle {
    public:
        TextStyle() 
            : color_(0xFFFFFFFF)              // White text
            , backgroundColor_(0x00000000)    // Transparent background
            , fontSize_(2)                    // Good default size
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
            , hasBackground_(false)
            , padding_(0)
            , alignment_(0)                   // 0=left, 1=center, 2=right
            , shadow_(false)
            , shadowColor_(0xFF000000)        // Black shadow
            , shadowOffset_(1)
        {}
        
        // Fluent interface for easy configuration
        TextStyle& color(uint32_t c) { color_ = c; return *this; }
        TextStyle& backgroundColor(uint32_t c) { backgroundColor_ = c; hasBackground_ = true; return *this; }
        TextStyle& fontSize(int size) { fontSize_ = size; return *this; }
        TextStyle& padding(int p) { padding_ = p; return *this; }
        TextStyle& alignment(int a) { alignment_ = a; return *this; }  // 0=left, 1=center, 2=right
        TextStyle& shadow(bool enabled, uint32_t shadowColor = 0xFF000000, int offset = 1) {
            shadow_ = enabled;
            shadowColor_ = shadowColor;
            shadowOffset_ = offset;
            return *this;
        }
        TextStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        TextStyle& useTTFFont(const std::string& fontName = "") { 
            fontType_ = FontType::TTF; 
            ttfFontName_ = fontName;
            if (fontSize_ < 16) fontSize_ = 16; // TTF needs larger sizes
            return *this; 
        }
        
        // Getters
        uint32_t getColor() const { return color_; }
        uint32_t getBackgroundColor() const { return backgroundColor_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        bool hasBackground() const { return hasBackground_; }
        int getPadding() const { return padding_; }
        int getAlignment() const { return alignment_; }
        bool hasShadow() const { return shadow_; }
        uint32_t getShadowColor() const { return shadowColor_; }
        int getShadowOffset() const { return shadowOffset_; }
        
    private:
        uint32_t color_;
        uint32_t backgroundColor_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
        bool hasBackground_;
        int padding_;
        int alignment_;
        bool shadow_;
        uint32_t shadowColor_;
        int shadowOffset_;
    };
    
    class TextConfig {
    public:
        TextConfig(int x, int y, const std::string& text)
            : x_(x), y_(y), text_(text)
        {}
        
        // Fluent interface
        TextConfig& style(const TextStyle& s) { style_ = s; return *this; }
        TextConfig& text(const std::string& t) { text_ = t; return *this; }
        TextConfig& position(int x, int y) { x_ = x; y_ = y; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        const std::string& getText() const { return text_; }
        const TextStyle& getStyle() const { return style_; }
        
        // Position setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        
    private:
        int x_, y_;
        std::string text_;
        TextStyle style_;
    };
    
 
    class TextWidget : public Widget {
    public:
        // Modern constructor with configuration
        TextWidget(const TextConfig& config);
        
        // Legacy constructor for backward compatibility
        TextWidget(Point position, const std::string& text, int size, uint32_t color, 
                  FontType fontType = FontType::Bitmap);
        
        void render() override;
        bool handleInput(const InputState& input) override { return false; }
        
        void setText(const std::string& text);
        void setSize(int size);
        void setColor(uint32_t color);
        void setFontType(FontType type) { fontType_ = type; }
        void setConfig(const TextConfig& config);
        void renderTTF();
        void renderBitmap();
        
        const std::string& getText() const { return text_; }
        int getSize() const { return size_; }
        uint32_t getColor() const { return color_; }
        FontType getFontType() const { return fontType_; }
        const TextConfig& getConfig() const { return config_; }
        
    private:
        std::string text_;
        int size_;
        uint32_t color_;
        FontType fontType_;
        TextConfig config_;
        
        void updateDimensions();
        void renderBackground();
        void renderShadow();
        void renderMainText();
    };
    
    // Modern factory function with configuration
    std::shared_ptr<TextWidget> Text(const TextConfig& config, bool addToManager = true);
    
    // Legacy factory function for backward compatibility
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                    int size, uint32_t color, bool addToManager = true,
                                    FontType fontType = FontType::Bitmap);
    
    // Preset configurations
    namespace TextPresets {
        TextConfig Title(int x, int y, const std::string& text);
        TextConfig Subtitle(int x, int y, const std::string& text);
        TextConfig Body(int x, int y, const std::string& text);
        TextConfig Caption(int x, int y, const std::string& text);
        TextConfig Button(int x, int y, const std::string& text);
        TextConfig Error(int x, int y, const std::string& text);
        TextConfig Success(int x, int y, const std::string& text);
    }
}