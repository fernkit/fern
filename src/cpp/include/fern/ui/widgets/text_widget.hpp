#pragma once
#include <string>
#include "widget.hpp"          
#include "../../core/input.hpp"
#include "../../core/types.hpp"
#include "../../font/font.hpp"

namespace Fern {
    
    /**
     * @brief Style configuration for TextWidget
     * 
     * TextStyle provides a fluent interface to configure the appearance
     * of text including color, font type/size, background, alignment, and shadow effects.
     * Supports both bitmap and TTF fonts with appropriate size recommendations.
     * 
     * @example Basic text styling:
     * @code
     * TextStyle style;
     * style.color(Colors::White)
     *      .fontSize(16)
     *      .useTTFFont("arial")
     *      .alignment(1); // Center
     * @endcode
     * 
     * @example Text with background and shadow:
     * @code
     * TextStyle style;
     * style.color(Colors::Black)
     *      .backgroundColor(Colors::Yellow)
     *      .shadow(true, Colors::Gray, 2)
     *      .padding(5);
     * @endcode
     */
    class TextStyle {
    public:
        /**
         * @brief Construct a new Text Style with default values
         * 
         * Default style includes white text, transparent background, medium font size,
         * bitmap font type, and left alignment.
         */
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
        
        /**
         * @brief Set text color
         * @param c Color in ARGB format
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& color(uint32_t c) { color_ = c; return *this; }
        
        /**
         * @brief Set background color (enables background rendering)
         * @param c Background color in ARGB format
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& backgroundColor(uint32_t c) { backgroundColor_ = c; hasBackground_ = true; return *this; }
        
        /**
         * @brief Set font size
         * @param size Font size (1-5 for bitmap, 16+ recommended for TTF)
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& fontSize(int size) { fontSize_ = size; return *this; }
        
        /**
         * @brief Set padding around text
         * @param p Padding in pixels
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& padding(int p) { padding_ = p; return *this; }
        
        /**
         * @brief Set text alignment
         * @param a Alignment: 0=left, 1=center, 2=right
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& alignment(int a) { alignment_ = a; return *this; }
        
        /**
         * @brief Configure text shadow
         * @param enabled Whether to show shadow
         * @param shadowColor Shadow color in ARGB format
         * @param offset Shadow offset in pixels
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& shadow(bool enabled, uint32_t shadowColor = 0xFF000000, int offset = 1) {
            shadow_ = enabled;
            shadowColor_ = shadowColor;
            shadowOffset_ = offset;
            return *this;
        }
        
        /**
         * @brief Use bitmap font rendering (small, pixelated style)
         * @return TextStyle& Reference for method chaining
         */
        TextStyle& useBitmapFont() { fontType_ = FontType::Bitmap; return *this; }
        
        /**
         * @brief Use TTF font rendering (smooth, scalable)
         * @param fontName TTF font name (empty for default)
         * @return TextStyle& Reference for method chaining
         * @note Automatically adjusts font size to 16+ if too small for TTF
         */
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
    
    /**
     * @brief Configuration container for TextWidget positioning and content
     * 
     * TextConfig combines position, text content, and styling into a single
     * configuration object for easy text widget creation.
     * 
     * @example Basic text configuration:
     * @code
     * auto config = TextConfig(100, 50, "Hello World")
     *     .style(TextStyle().color(Colors::Red).fontSize(3));
     * @endcode
     */
    class TextConfig {
    public:
        /**
         * @brief Construct a new Text Config
         * @param x X position in pixels
         * @param y Y position in pixels  
         * @param text Text content to display
         */
        TextConfig(int x, int y, const std::string& text)
            : x_(x), y_(y), text_(text)
        {}
        
        /**
         * @brief Set text style configuration
         * @param s TextStyle configuration object
         * @return TextConfig& Reference for method chaining
         */
        TextConfig& style(const TextStyle& s) { style_ = s; return *this; }
        
        /**
         * @brief Update text content
         * @param t New text content
         * @return TextConfig& Reference for method chaining
         */
        TextConfig& text(const std::string& t) { text_ = t; return *this; }
        
        /**
         * @brief Update position
         * @param x New X position
         * @param y New Y position
         * @return TextConfig& Reference for method chaining
         */
        TextConfig& position(int x, int y) { x_ = x; y_ = y; return *this; }
        
        // Getters
        int getX() const { return x_; }
        int getY() const { return y_; }
        const std::string& getText() const { return text_; }
        const TextStyle& getStyle() const { return style_; }
        
        /**
         * @brief Update position (non-fluent setter)
         * @param x New X position
         * @param y New Y position
         */
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        
    private:
        int x_, y_;
        std::string text_;
        TextStyle style_;
    };
    
 
 
    /**
     * @brief Widget for displaying text with various styling options
     * 
     * TextWidget provides flexible text rendering capabilities including:
     * - Support for both bitmap and TTF fonts
     * - Configurable colors, alignment, and sizing  
     * - Background colors and shadow effects
     * - Modern configuration-based API and legacy constructor support
     * 
     * @example Modern usage with configuration:
     * @code
     * auto config = TextConfig(100, 50, "Hello World")
     *     .style(TextStyle().color(Colors::White).useTTFFont("arial").fontSize(20));
     * auto text = Text(config);
     * @endcode
     * 
     * @example Legacy usage:
     * @code
     * auto text = Text({100, 50}, "Hello World", 2, Colors::White);
     * @endcode
     */
    class TextWidget : public Widget {
    public:
        /**
         * @brief Construct TextWidget with modern configuration
         * @param config TextConfig object containing position, text, and style
         */
        TextWidget(const TextConfig& config);
        
        /**
         * @brief Legacy constructor for backward compatibility
         * @param position Text position
         * @param text Text content
         * @param size Font size
         * @param color Text color in ARGB format
         * @param fontType Font type (Bitmap or TTF)
         */
        TextWidget(Point position, const std::string& text, int size, uint32_t color, 
                  FontType fontType = FontType::Bitmap);
        
        /**
         * @brief Render the text widget to the canvas
         */
        void render() override;
        
        /**
         * @brief Handle input events (text widgets don't handle input)
         * @param input Input state
         * @return false (text widgets are not interactive)
         */
        bool handleInput(const InputState& input) override { return false; }
        
        // Content and style setters
        void setText(const std::string& text);
        void setSize(int size);
        void setColor(uint32_t color);
        void setFontType(FontType type) { fontType_ = type; }
        void setConfig(const TextConfig& config);
        
        // Rendering methods for different font types
        void renderTTF();
        void renderBitmap();
        
        // Getters
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
    
    /**
     * @brief Factory function for creating TextWidget with modern configuration
     * @param config TextConfig object containing position, text, and style
     * @param addToManager Whether to automatically add to widget manager
     * @return std::shared_ptr<TextWidget> Shared pointer to created widget
     */
    std::shared_ptr<TextWidget> Text(const TextConfig& config, bool addToManager = true);
    
    /**
     * @brief Legacy factory function for backward compatibility
     * @param position Text position
     * @param text Text content
     * @param size Font size
     * @param color Text color in ARGB format
     * @param addToManager Whether to automatically add to widget manager
     * @param fontType Font type (Bitmap or TTF)
     * @return std::shared_ptr<TextWidget> Shared pointer to created widget
     */
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                    int size, uint32_t color, bool addToManager = true,
                                    FontType fontType = FontType::Bitmap);
    
    /**
     * @brief Predefined text style configurations for common use cases
     * 
     * TextPresets provides convenient preset configurations for various
     * text styles like titles, subtitles, body text, captions, etc.
     * 
     * @example Using presets:
     * @code
     * auto title = Text(TextPresets::Title(100, 50, "Main Title"));
     * auto subtitle = Text(TextPresets::Subtitle(100, 80, "Subtitle"));
     * auto body = Text(TextPresets::Body(100, 110, "Body text content"));
     * @endcode
     */
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