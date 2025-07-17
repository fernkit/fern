#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <string>
#include <functional>
#include <memory>

namespace Fern {
    
    /**
     * @brief Style configuration for TextInputWidget
     * 
     * TextInputStyle provides a fluent interface to configure the appearance
     * of text input fields including colors, borders, fonts, and layout.
     * Supports both bitmap and TTF fonts with focus state styling.
     * 
     * @example Basic input styling:
     * @code
     * TextInputStyle style;
     * style.backgroundColor(Colors::White)
     *      .borderColor(Colors::Gray)
     *      .focusBorderColor(Colors::Blue)
     *      .textColor(Colors::Black)
     *      .padding(8);
     * @endcode
     * 
     * @example TTF font input:
     * @code
     * TextInputStyle style;
     * style.useTTFFont("arial")
     *      .fontSize(18)
     *      .backgroundColor(Colors::LightGray);
     * @endcode
     */
    class TextInputStyle {
    public:
        /**
         * @brief Construct a new Text Input Style with default values
         * 
         * Default style includes white background, gray border, blue focus border,
         * black text, and medium font size suitable for bitmap fonts.
         */
        TextInputStyle() 
            : backgroundColor_(0xFFFFFF)    // White
            , borderColor_(0x888888)        // Gray  
            , focusBorderColor_(0x0066CC)   // Blue
            , textColor_(0x000000)          // Black
            , cursorColor_(0x000000)        // Black
            , borderWidth_(1)
            , padding_(4)
            , fontSize_(2)                  // Good size for bitmap font
            , fontType_(FontType::Bitmap)
            , ttfFontName_("")
        {}
        
        /**
         * @brief Set background color
         * @param color Background color in ARGB format
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& backgroundColor(uint32_t color) { backgroundColor_ = color; return *this; }
        
        /**
         * @brief Set border color for normal state
         * @param color Border color in ARGB format
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& borderColor(uint32_t color) { borderColor_ = color; return *this; }
        
        /**
         * @brief Set border color for focused state
         * @param color Focus border color in ARGB format
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& focusBorderColor(uint32_t color) { focusBorderColor_ = color; return *this; }
        
        /**
         * @brief Set text color
         * @param color Text color in ARGB format
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        
        /**
         * @brief Set cursor color
         * @param color Cursor color in ARGB format
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& cursorColor(uint32_t color) { cursorColor_ = color; return *this; }
        
        /**
         * @brief Set border width
         * @param width Border width in pixels
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& borderWidth(int width) { borderWidth_ = width; return *this; }
        
        /**
         * @brief Set padding around text
         * @param pad Padding in pixels
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& padding(int pad) { padding_ = pad; return *this; }
        
        /**
         * @brief Set font size
         * @param size Font size (1-5 for bitmap, 16+ recommended for TTF)
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& fontSize(int size) { fontSize_ = size; return *this; }
        
        /**
         * @brief Use bitmap font rendering
         * @return TextInputStyle& Reference for method chaining
         */
        TextInputStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        
        /**
         * @brief Use TTF font rendering
         * @param fontName TTF font name (empty for default)
         * @return TextInputStyle& Reference for method chaining
         * @note Automatically adjusts font size to 16+ if too small for TTF
         */
        TextInputStyle& useTTFFont(const std::string& fontName = "") { 
            fontType_ = FontType::TTF; 
            ttfFontName_ = fontName;
            if (fontSize_ < 16) fontSize_ = 16; // TTF needs larger sizes
            return *this; 
        }
        
        // Getters
        uint32_t getBackgroundColor() const { return backgroundColor_; }
        uint32_t getBorderColor() const { return borderColor_; }
        uint32_t getFocusBorderColor() const { return focusBorderColor_; }
        uint32_t getTextColor() const { return textColor_; }
        uint32_t getCursorColor() const { return cursorColor_; }
        int getBorderWidth() const { return borderWidth_; }
        int getPadding() const { return padding_; }
        int getFontSize() const { return fontSize_; }
        FontType getFontType() const { return fontType_; }
        const std::string& getTTFFontName() const { return ttfFontName_; }
        
    private:
        uint32_t backgroundColor_;
        uint32_t borderColor_;
        uint32_t focusBorderColor_;
        uint32_t textColor_;
        uint32_t cursorColor_;
        int borderWidth_;
        int padding_;
        int fontSize_;
        FontType fontType_;
        std::string ttfFontName_;
    };
    
    /**
     * @brief Configuration container for TextInputWidget positioning and setup
     * 
     * TextInputConfig combines position, dimensions, styling, and placeholder text
     * into a single configuration object for easy text input widget creation.
     * 
     * @example Basic text input configuration:
     * @code
     * auto config = TextInputConfig(100, 50, 200, 30, "Enter text...")
     *     .style(TextInputStyle().backgroundColor(Colors::White).borderColor(Colors::Gray));
     * @endcode
     */
    class TextInputConfig {
    public:
        TextInputConfig(int x, int y, int width, int height)
            : x_(x), y_(y), width_(width), height_(height)
            , placeholder_("")
            , maxLength_(256)
        {}
        
        // Fluent interface
        TextInputConfig& placeholder(const std::string& text) { placeholder_ = text; return *this; }
        TextInputConfig& maxLength(size_t length) { maxLength_ = length; return *this; }
        TextInputConfig& style(const TextInputStyle& s) { style_ = s; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        const std::string& getPlaceholder() const { return placeholder_; }
        size_t getMaxLength() const { return maxLength_; }
        const TextInputStyle& getStyle() const { return style_; }
        
        // Position/size setters
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        std::string placeholder_;
        size_t maxLength_;
        TextInputStyle style_;
    };
    
    class TextInputWidget : public Widget {
    public:
        TextInputWidget(const TextInputConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;

        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
        
        // Text manipulation
        void setText(const std::string& text);
        const std::string& getText() const { return text_; }
        void clear() { setText(""); }
        
        // Focus management
        void setFocus(bool focused);
        bool isFocused() const { return isFocused_; }
        
        // Placeholder
        void setPlaceholder(const std::string& placeholder);
        const std::string& getPlaceholder() const { return config_.getPlaceholder(); }
        
        // Signals
        Signal<const std::string&> onTextChanged;
        Signal<const std::string&> onEnterPressed;
        Signal<bool> onFocusChanged;
        
    private:
        TextInputConfig config_;
        std::string text_;
        size_t cursorPosition_;
        bool isFocused_;
        bool showCursor_;
        uint32_t cursorBlinkTimer_;
        static constexpr uint32_t CURSOR_BLINK_INTERVAL = 500; // milliseconds
        
        // Helper methods
        void handleKeyInput(const InputState& input);
        void handleTextInput(const InputState& input);
        void moveCursor(int direction);
        void insertText(const std::string& text);
        void deleteCharacter(bool forward = false);
        int getTextWidth(const std::string& text) const;
        int getCursorX() const;
        void updateCursorBlink();
        bool isPointInWidget(int x, int y) const;
        void renderText();
        void renderCursor();
        void renderBorder();
        void renderBackground();
        std::string clipTextToWidth(const std::string& text, int maxWidth) const;
        std::string getVisibleText(const std::string& text, int availableWidth) const;
        int getCharWidth(char c) const;
    };
    
    // Helper function with class-based config
    std::shared_ptr<TextInputWidget> TextInput(const TextInputConfig& config, bool addToManager = true);
    
    // Preset configurations
    namespace TextInputPresets {
        TextInputConfig Default(int x, int y, int width = 200, int height = 30);
        TextInputConfig Modern(int x, int y, int width = 250, int height = 40);
        TextInputConfig WithTTF(int x, int y, const std::string& fontName = "", int width = 300, int height = 45);
    }
}
