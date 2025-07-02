#pragma once

#include "widget.hpp"
#include "../../core/signal.hpp"
#include "../../font/font.hpp"
#include <string>
#include <functional>
#include <memory>

namespace Fern {
    struct TextInputConfig {
        int x;
        int y;
        int width;
        int height;
        uint32_t backgroundColor;
        uint32_t borderColor;
        uint32_t focusBorderColor;
        uint32_t textColor;
        uint32_t cursorColor;
        std::string placeholder;
        int textSize;
        FontType fontType;
        int borderWidth;
        int padding;
        size_t maxLength;
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
        const std::string& getPlaceholder() const { return config_.placeholder; }
        
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
    };
    
    // Helper function
    std::shared_ptr<TextInputWidget> TextInput(const TextInputConfig& config, bool addToManager = true);
    
    // Default config helper
    TextInputConfig DefaultTextInputConfig();
}
