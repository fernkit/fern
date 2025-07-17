#pragma once

#include "widget.hpp"
#include "../../core/responsive_widget.hpp"
#include <string>
#include <functional>
#include <memory>
#include "../../core/signal.hpp"

namespace Fern {
    
    /**
     * @brief Style configuration for ButtonWidget
     * 
     * ButtonStyle provides a fluent interface to configure the appearance
     * of buttons including colors, text styling, borders, and rounded corners.
     * 
     * @example Basic style usage:
     * @code
     * ButtonStyle style;
     * style.normalColor(Colors::Blue)
     *      .hoverColor(Colors::LightBlue)
     *      .textColor(Colors::White)
     *      .borderRadius(8);
     * @endcode
     */
    class ButtonStyle {
    public:
        /**
         * @brief Construct a new Button Style with default values
         * 
         * Default style includes blue theme with white text and rounded corners.
         */
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
        
        /**
         * @brief Set the normal (default) button color
         * 
         * @param color Color in ARGB format
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& normalColor(uint32_t color) { normalColor_ = color; return *this; }
        
        /**
         * @brief Set the hover button color
         * 
         * @param color Color in ARGB format
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& hoverColor(uint32_t color) { hoverColor_ = color; return *this; }
        
        /**
         * @brief Set the pressed button color
         * 
         * @param color Color in ARGB format
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& pressColor(uint32_t color) { pressColor_ = color; return *this; }
        
        /**
         * @brief Set the text color
         * 
         * @param color Color in ARGB format
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& textColor(uint32_t color) { textColor_ = color; return *this; }
        
        /**
         * @brief Set the text scale factor
         * 
         * @param scale Scale multiplier (1 = small, 2 = normal, 3 = large, etc.)
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& textScale(int scale) { textScale_ = scale; return *this; }
        
        /**
         * @brief Set the border radius for rounded corners
         * 
         * @param radius Radius in pixels (0 = square corners)
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& borderRadius(int radius) { borderRadius_ = radius; return *this; }
        
        /**
         * @brief Set border width and color
         * 
         * @param width Border width in pixels
         * @param color Border color in ARGB format
         * @return ButtonStyle& Reference for method chaining
         */
        ButtonStyle& border(int width, uint32_t color) { 
            borderWidth_ = width; 
            borderColor_ = color; 
            return *this; 
        }
        
        // Getters
        /**
         * @brief Get the normal button color
         * @return uint32_t Color in ARGB format
         */
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
    
    /**
     * @brief Configuration class for ButtonWidget
     * 
     * ButtonConfig provides a fluent interface for configuring button properties
     * including position, size, label text, and styling. It supports method chaining
     * for convenient configuration.
     * 
     * @example Configuration usage:
     * @code
     * ButtonConfig config(100, 50, 120, 40, "Click Me");
     * config.style(ButtonStyle().normalColor(Colors::Green))
     *       .position(150, 75)
     *       .size(140, 45);
     * @endcode
     */
    class ButtonConfig {
    public:
        /**
         * @brief Construct a new Button Config
         * 
         * @param x X position of the button
         * @param y Y position of the button
         * @param width Width of the button in pixels
         * @param height Height of the button in pixels
         * @param label Text label displayed on the button
         */
        ButtonConfig(int x, int y, int width, int height, const std::string& label)
            : x_(x), y_(y), width_(width), height_(height), label_(label)
        {}
        
        /**
         * @brief Set the button style (fluent interface)
         * 
         * @param s ButtonStyle configuration
         * @return ButtonConfig& Reference for method chaining
         */
        ButtonConfig& style(const ButtonStyle& s) { style_ = s; return *this; }
        
        /**
         * @brief Set the button label text (fluent interface)
         * 
         * @param text New label text
         * @return ButtonConfig& Reference for method chaining
         */
        ButtonConfig& label(const std::string& text) { label_ = text; return *this; }
        
        /**
         * @brief Set the button position (fluent interface)
         * 
         * @param x New X position
         * @param y New Y position
         * @return ButtonConfig& Reference for method chaining
         */
        ButtonConfig& position(int x, int y) { x_ = x; y_ = y; return *this; }
        
        /**
         * @brief Set the button size (fluent interface)
         * 
         * @param width New width in pixels
         * @param height New height in pixels
         * @return ButtonConfig& Reference for method chaining
         */
        ButtonConfig& size(int width, int height) { width_ = width; height_ = height; return *this; }
        
        /**
         * @brief Get the X position
         * 
         * @return X position in pixels
         */
        int getX() const { return x_; }
        
        /**
         * @brief Get the Y position
         * 
         * @return Y position in pixels
         */
        int getY() const { return y_; }
        
        /**
         * @brief Get the button width
         * 
         * @return Width in pixels
         */
        int getWidth() const { return width_; }
        
        /**
         * @brief Get the button height
         * 
         * @return Height in pixels
         */
        int getHeight() const { return height_; }
        
        /**
         * @brief Get the button label text
         * 
         * @return Current label text
         */
        const std::string& getLabel() const { return label_; }
        
        /**
         * @brief Get the button style configuration
         * 
         * @return Current ButtonStyle configuration
         */
        const ButtonStyle& getStyle() const { return style_; }
        
        /**
         * @brief Set the button position
         * 
         * @param x New X position
         * @param y New Y position
         */
        void setPosition(int x, int y) { x_ = x; y_ = y; }
        
        /**
         * @brief Set the button size
         * 
         * @param width New width in pixels
         * @param height New height in pixels
         */
        void setSize(int width, int height) { width_ = width; height_ = height; }
        
    private:
        int x_, y_, width_, height_;
        std::string label_;
        ButtonStyle style_;
    };

    /**
     * @brief Interactive button widget with customizable styling
     * 
     * ButtonWidget is a clickable UI element that supports hover effects, custom styling,
     * and event handling through signals. It can be configured with text, colors, borders,
     * and auto-sizing capabilities.
     * 
     * @example Basic button usage:
     * @code
     * ButtonConfig config(100, 50, 120, 40, "Click Me");
     * auto button = Button(config);
     * button->onClick.connect([]() {
     *     std::cout << "Button clicked!" << std::endl;
     * });
     * @endcode
     * 
     * @example Styled button with presets:
     * @code
     * auto primaryBtn = Button(ButtonPresets::Primary(10, 10, 100, 35, "Save"));
     * auto dangerBtn = Button(ButtonPresets::Danger(120, 10, 100, 35, "Delete"));
     * @endcode
     */
    class ButtonWidget : public Widget {
    public:
        /**
         * @brief Construct a new Button Widget
         * 
         * @param config ButtonConfig containing position, size, label, and style
         */
        ButtonWidget(const ButtonConfig& config);
        
        /**
         * @brief Render the button to the canvas
         */
        void render() override;
        
        /**
         * @brief Handle input events (hover, click, press)
         * 
         * @param input Current input state
         * @return true if input was handled, false otherwise
         */
        bool handleInput(const InputState& input) override;

        /**
         * @brief Get the button width
         * 
         * @return Width in pixels
         */
        int getWidth() const override;
        
        /**
         * @brief Get the button height
         * 
         * @return Height in pixels
         */
        int getHeight() const override;
        
        /**
         * @brief Set the button position
         * 
         * @param x New X position
         * @param y New Y position
         */
        void setPosition(int x, int y) override;
        
        /**
         * @brief Get the X position
         * 
         * @return X position in pixels
         */
        int getX() const override;
        
        /**
         * @brief Get the Y position
         * 
         * @return Y position in pixels
         */
        int getY() const override;
        
        /**
         * @brief Resize the button
         * 
         * @param width New width in pixels
         * @param height New height in pixels
         */
        void resize(int width, int height) override;
        
        /**
         * @brief Update the button configuration
         * 
         * @param config New ButtonConfig
         */
        void setConfig(const ButtonConfig& config);
        
        /**
         * @brief Set the button label text
         * 
         * @param label New label text
         */
        void setLabel(const std::string& label);
        
        /**
         * @brief Get the current button configuration
         * 
         * @return Current ButtonConfig
         */
        const ButtonConfig& getConfig() const { return config_; }
    
        /**
         * @brief Signal emitted when button is clicked
         * 
         * Connect to handle click events:
         * @code
         * button->onClick.connect();
         * @endcode
         */
        Signal<> onClick;       
        
        /**
         * @brief Signal emitted when hover state changes
         * 
         * @param isHovered true when mouse enters, false when leaves
         */
        Signal<bool> onHover;   
        
        /**
         * @brief Signal emitted when press state changes
         * 
         * @param isPressed true when mouse button down, false when released
         */
        Signal<bool> onPress;   
        
        /**
         * @brief Automatically resize button to fit text content
         * 
         * @param padding Extra padding around text in pixels (default: 16)
         */
        void autoSizeToContent(int padding = 16);
        
        /**
         * @brief Calculate the width needed for given text
         * 
         * @param text Text to measure
         * @param textScale Text scale factor
         * @return Required width in pixels
         */
        static int calculateTextWidth(const std::string& text, int textScale);
        
        /**
         * @brief Calculate the height needed for given text scale
         * 
         * @param textScale Text scale factor
         * @return Required height in pixels
         */
        static int calculateTextHeight(int textScale);
        
    private:
        ButtonConfig config_;     ///< Button configuration (position, size, label, style)
        bool isHovered_ = false;  ///< Current hover state
        bool isPressed_ = false;  ///< Current press state
        
        /**
         * @brief Render the button background
         */
        void renderBackground();
        
        /**
         * @brief Render the button border
         */
        void renderBorder();
        
        /**
         * @brief Render the button text
         */
        void renderText();
    };
    
    /**
     * @brief Factory function to create a button widget
     * 
     * @param config ButtonConfig containing all button properties
     * @param addToManager Whether to automatically add to widget manager (default: true)
     * @return std::shared_ptr<ButtonWidget> Shared pointer to the created button
     * 
     * @example
     * @code
     * auto button = Button(ButtonConfig(50, 50, 100, 30, "Submit"));
     * @endcode
     */
    std::shared_ptr<ButtonWidget> Button(const ButtonConfig& config, bool addToManager = true);
    
    /**
     * @brief Pre-configured button styles for common use cases
     * 
     * ButtonPresets provides factory functions for creating commonly-used
     * button configurations with appropriate colors and styling.
     */
    namespace ButtonPresets {
        /**
         * @brief Create a primary button configuration (blue theme)
         * 
         * @param x X position
         * @param y Y position  
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Primary button configuration
         */
        ButtonConfig Primary(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a secondary button configuration (gray theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Secondary button configuration
         */
        ButtonConfig Secondary(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a success button configuration (green theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Success button configuration
         */
        ButtonConfig Success(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a danger button configuration (red theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Danger button configuration
         */
        ButtonConfig Danger(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a warning button configuration (orange theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Warning button configuration
         */
        ButtonConfig Warning(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create an info button configuration (light blue theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Info button configuration
         */
        ButtonConfig Info(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a light button configuration (light gray theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Light button configuration
         */
        ButtonConfig Light(int x, int y, int width, int height, const std::string& label);
        
        /**
         * @brief Create a dark button configuration (dark gray theme)
         * 
         * @param x X position
         * @param y Y position
         * @param width Button width
         * @param height Button height
         * @param label Button text
         * @return ButtonConfig Dark button configuration
         */
        ButtonConfig Dark(int x, int y, int width, int height, const std::string& label);
    }
}