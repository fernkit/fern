#include "../../../include/fern/ui/widgets/text_input_widget.hpp"
#include "../../../include/fern/core/input.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <cstring>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

extern Fern::Canvas* globalCanvas;

namespace Fern {
    TextInputWidget::TextInputWidget(const TextInputConfig& config)
        : config_(config), text_(""), cursorPosition_(0), isFocused_(false), 
          showCursor_(true), cursorBlinkTimer_(0) {
        setPosition(config.x, config.y);
        resize(config.width, config.height);
    }
    
    void TextInputWidget::render() {
        renderBackground();
        renderBorder();
        renderText();
        
        if (isFocused_) {
            updateCursorBlink();
            if (showCursor_) {
                renderCursor();
            }
        }
    }
    
    void TextInputWidget::renderBackground() {
        Draw::rect(x_, y_, config_.width, config_.height, config_.backgroundColor);
    }
    
    void TextInputWidget::renderBorder() {
        uint32_t borderColor = isFocused_ ? config_.focusBorderColor : config_.borderColor;
        
        if (config_.borderWidth > 0) {
            // Top border
            Draw::rect(x_, y_, config_.width, config_.borderWidth, borderColor);
            // Bottom border
            Draw::rect(x_, y_ + config_.height - config_.borderWidth, 
                      config_.width, config_.borderWidth, borderColor);
            // Left border
            Draw::rect(x_, y_, config_.borderWidth, config_.height, borderColor);
            // Right border
            Draw::rect(x_ + config_.width - config_.borderWidth, y_, 
                      config_.borderWidth, config_.height, borderColor);
        }
    }
    
    void TextInputWidget::renderText() {
        std::string displayText = text_;
        
        // Show placeholder if empty and not focused
        if (displayText.empty() && !isFocused_ && !config_.placeholder.empty()) {
            displayText = config_.placeholder;
        }
        
        if (!displayText.empty()) {
            int textX = x_ + config_.padding + config_.borderWidth;
            int textY = y_ + config_.padding + config_.borderWidth;
            
            // Simple text rendering without complex scrolling for now
            DrawText::drawText(displayText.c_str(), textX, textY, config_.textSize, config_.textColor);
        }
    }
    
    void TextInputWidget::renderCursor() {
        if (!isFocused_) return;
        
        int cursorX = getCursorX();
        int cursorY = y_ + config_.padding + config_.borderWidth;
        int cursorHeight = config_.textSize;
        
        // Draw a simple vertical line cursor with thickness of 1
        Draw::line(cursorX, cursorY, cursorX, cursorY + cursorHeight, 1, config_.cursorColor);
    }
    
    bool TextInputWidget::handleInput(const InputState& input) {
        bool wasHandled = false;
        
        // Check for mouse click to set focus
        if (input.mouseClicked) {
            bool clickedInside = isPointInWidget(input.mouseX, input.mouseY);
            setFocus(clickedInside);
            if (clickedInside) {
                // Set cursor position based on click location
                // For now, just set to end of text
                cursorPosition_ = text_.length();
                wasHandled = true;
            }
        }
        
        if (isFocused_) {
            handleKeyInput(input);
            handleTextInput(input);
            wasHandled = true;
        }
        
        return wasHandled;
    }
    
    void TextInputWidget::handleKeyInput(const InputState& input) {
        // Handle special keys that were just pressed
        if (input.isKeyJustPressed(KeyCode::ArrowLeft)) {
            moveCursor(-1);
        }
        if (input.isKeyJustPressed(KeyCode::ArrowRight)) {
            moveCursor(1);
        }
        if (input.isKeyJustPressed(KeyCode::Backspace)) {
            deleteCharacter(false);
#ifdef __EMSCRIPTEN__
            EM_ASM({
                console.log("Backspace pressed, text now: " + UTF8ToString($0));
            }, text_.c_str());
#endif
        }
        if (input.isKeyJustPressed(KeyCode::Delete)) {
            deleteCharacter(true);
        }
        if (input.isKeyJustPressed(KeyCode::Enter)) {
            onEnterPressed.emit(text_);
        }
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            setFocus(false);
        }
    }
    
    void TextInputWidget::handleTextInput(const InputState& input) {
        if (input.hasTextInput && !input.textInput.empty()) {
            // Debug output
#ifdef __EMSCRIPTEN__
            EM_ASM({
                console.log("TextInputWidget received text: " + UTF8ToString($0));
            }, input.textInput.c_str());
#endif
            insertText(input.textInput);
        }
    }
    
    void TextInputWidget::moveCursor(int direction) {
        if (direction < 0) {
            if (cursorPosition_ > 0) {
                cursorPosition_--;
            }
        } else if (direction > 0) {
            if (cursorPosition_ < text_.length()) {
                cursorPosition_++;
            }
        }
    }
    
    void TextInputWidget::insertText(const std::string& text) {
        if (text_.length() + text.length() <= config_.maxLength) {
            text_.insert(cursorPosition_, text);
            cursorPosition_ += text.length();
            onTextChanged.emit(text_);
        }
    }
    
    void TextInputWidget::deleteCharacter(bool forward) {
        if (forward) {
            if (cursorPosition_ < text_.length()) {
                text_.erase(cursorPosition_, 1);
                onTextChanged.emit(text_);
            }
        } else {
            if (cursorPosition_ > 0) {
                cursorPosition_--;
                text_.erase(cursorPosition_, 1);
                onTextChanged.emit(text_);
            }
        }
    }
    
    int TextInputWidget::getTextWidth(const std::string& text) const {
        if (config_.fontType == FontType::TTF && Font::hasTTFFont()) {
            return Font::getTextWidth(text, config_.textSize, FontType::TTF);
        } else {
            int charWidth = config_.textSize * 6 / 8;
            return text.length() * charWidth;
        }
    }
    
    int TextInputWidget::getCursorX() const {
        int baseX = x_ + config_.padding + config_.borderWidth;
        
        if (cursorPosition_ == 0) {
            return baseX;
        }
        
        std::string textToCursor = text_.substr(0, cursorPosition_);
        return baseX + getTextWidth(textToCursor);
    }
    
    void TextInputWidget::updateCursorBlink() {
        static uint32_t lastTime = 0;
        uint32_t currentTime = 0; // TODO: Get actual time from platform
        
        if (currentTime - lastTime > CURSOR_BLINK_INTERVAL) {
            showCursor_ = !showCursor_;
            lastTime = currentTime;
        }
    }
    
    bool TextInputWidget::isPointInWidget(int x, int y) const {
        return x >= x_ && x < x_ + config_.width &&
               y >= y_ && y < y_ + config_.height;
    }
    
    void TextInputWidget::setText(const std::string& text) {
        if (text.length() <= config_.maxLength) {
            text_ = text;
            cursorPosition_ = std::min(cursorPosition_, text_.length());
            onTextChanged.emit(text_);
        }
    }
    
    void TextInputWidget::setFocus(bool focused) {
        if (isFocused_ != focused) {
            isFocused_ = focused;
            showCursor_ = focused;
#ifdef __EMSCRIPTEN__
            EM_ASM({
                console.log("TextInputWidget focus changed to: " + ($0 ? "focused" : "unfocused"));
            }, focused);
#endif
            onFocusChanged.emit(focused);
        }
    }
    
    void TextInputWidget::setPlaceholder(const std::string& placeholder) {
        config_.placeholder = placeholder;
    }
    
    // Widget interface methods
    int TextInputWidget::getWidth() const {
        return config_.width;
    }
    
    int TextInputWidget::getHeight() const {
        return config_.height;
    }
    
    void TextInputWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.x = x;
        config_.y = y;
    }
    
    int TextInputWidget::getX() const {
        return config_.x;
    }
    
    int TextInputWidget::getY() const {
        return config_.y;
    }
    
    void TextInputWidget::resize(int width, int height) {
        config_.width = width;
        config_.height = height;
    }
    
    // Helper functions
    std::shared_ptr<TextInputWidget> TextInput(const TextInputConfig& config, bool addToManager) {
        auto widget = std::make_shared<TextInputWidget>(config);
        
        if (addToManager) {
            addWidget(widget);
        }
        
        return widget;
    }
    
    TextInputConfig DefaultTextInputConfig() {
        return {
            .x = 0,
            .y = 0,
            .width = 200,
            .height = 30,
            .backgroundColor = 0xFFFFFF,  // White
            .borderColor = 0x888888,      // Gray
            .focusBorderColor = 0x0066CC, // Blue
            .textColor = 0x000000,        // Black
            .cursorColor = 0x000000,      // Black
            .placeholder = "",
            .textSize = 16,
            .fontType = FontType::Bitmap,
            .borderWidth = 1,
            .padding = 4,
            .maxLength = 256
        };
    }
}
