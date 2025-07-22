#include "../../../include/fern/ui/widgets/text_input_widget.hpp"
#include "../../../include/fern/core/input.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

extern Fern::Canvas* globalCanvas;

namespace Fern {
    TextInputWidget::TextInputWidget(const TextInputConfig& config)
        : config_(config), text_(""), cursorPosition_(0), isFocused_(false), 
          showCursor_(true), cursorBlinkTimer_(0) {
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
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
        Draw::rect(x_, y_, config_.getWidth(), config_.getHeight(), config_.getStyle().getBackgroundColor());
    }
    
    void TextInputWidget::renderBorder() {
        const auto& style = config_.getStyle();
        uint32_t borderColor = isFocused_ ? style.getFocusBorderColor() : style.getBorderColor();
        
        if (style.getBorderWidth() > 0) {
            // Top border
            Draw::rect(x_, y_, config_.getWidth(), style.getBorderWidth(), borderColor);
            // Bottom border
            Draw::rect(x_, y_ + config_.getHeight() - style.getBorderWidth(), 
                      config_.getWidth(), style.getBorderWidth(), borderColor);
            // Left border
            Draw::rect(x_, y_, style.getBorderWidth(), config_.getHeight(), borderColor);
            // Right border
            Draw::rect(x_ + config_.getWidth() - style.getBorderWidth(), y_, 
                      style.getBorderWidth(), config_.getHeight(), borderColor);
        }
    }
    
    void TextInputWidget::renderText() {
        const auto& style = config_.getStyle();
        std::string displayText = text_;
        bool isPlaceholder = false;
        
        // Show placeholder if empty and not focused
        if (displayText.empty() && !isFocused_ && !config_.getPlaceholder().empty()) {
            displayText = config_.getPlaceholder();
            isPlaceholder = true;
        }
        
        if (!displayText.empty()) {
            int textX = x_ + style.getPadding() + style.getBorderWidth();
            int textY = y_ + style.getPadding() + style.getBorderWidth();
            
            // Calculate available width for text (excluding padding and borders)
            int availableWidth = config_.getWidth() - 2 * (style.getPadding() + style.getBorderWidth());
            
            // For placeholder, just clip it
            if (isPlaceholder) {
                std::string clippedText = clipTextToWidth(displayText, availableWidth);
                uint32_t textColor = 0x888888; // Gray for placeholder
                
                if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
                    Font::renderTTF(globalCanvas, clippedText, textX, textY, 
                                   style.getFontSize(), textColor, style.getTTFFontName());
                } else {
                    DrawText::drawText(clippedText.c_str(), textX, textY, style.getFontSize(), textColor);
                }
            } else {
                // For actual text, implement scrolling to show cursor
                std::string visibleText = getVisibleText(displayText, availableWidth);
                uint32_t textColor = style.getTextColor();
                
                if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
                    Font::renderTTF(globalCanvas, visibleText, textX, textY, 
                                   style.getFontSize(), textColor, style.getTTFFontName());
                } else {
                    DrawText::drawText(visibleText.c_str(), textX, textY, style.getFontSize(), textColor);
                }
            }
        }
    }
    
    void TextInputWidget::renderCursor() {
        if (!isFocused_) return;
        
        const auto& style = config_.getStyle();
        int cursorX = getCursorX();
        int cursorY = y_ + style.getPadding() + style.getBorderWidth();
        
        // Calculate cursor height based on font type and size
        int cursorHeight;
        if (style.getFontType() == FontType::TTF) {
            cursorHeight = Font::getTextHeight(style.getFontSize(), FontType::TTF);
        } else {
            cursorHeight = style.getFontSize() * 8; // Bitmap font: size * 8 pixels
        }
        
        // Draw a vertical line cursor with thickness of 2 for better visibility
        for (int i = 0; i < 2; i++) {
            Draw::line(cursorX + i, cursorY, cursorX + i, cursorY + cursorHeight, 1, style.getCursorColor());
        }
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
            // Debug output for all platforms
            std::cout << "TextInputWidget received text: '" << input.textInput << "'" << std::endl;
            
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
        if (text_.length() + text.length() <= config_.getMaxLength()) {
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
        const auto& style = config_.getStyle();
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            return Font::getTextWidth(text, style.getFontSize(), FontType::TTF);
        } else {
            // Bitmap font: calculate width correctly based on character types
            int width = 0;
            int scale = style.getFontSize();
            
            for (char c : text) {
                if (c == ' ') {
                    width += 4 * scale;  // Space is 4 pixels wide
                } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                    width += 8 * scale;  // Regular characters are 8 pixels wide
                } else {
                    width += 4 * scale;  // Unsupported characters are 4 pixels wide
                }
            }
            return width;
        }
    }
    
    int TextInputWidget::getCursorX() const {
        const auto& style = config_.getStyle();
        int baseX = x_ + style.getPadding() + style.getBorderWidth();
        
        if (cursorPosition_ == 0) {
            return baseX;
        }
        
        // Calculate available width for text
        int availableWidth = config_.getWidth() - 2 * (style.getPadding() + style.getBorderWidth());
        
        // Get the visible text
        std::string visibleText = getVisibleText(text_, availableWidth);
        
        // Find where the visible text starts in the original text
        size_t visibleStart = 0;
        if (getTextWidth(text_) > availableWidth) {
            // Find the start position of visible text
            for (size_t i = 0; i <= text_.length(); i++) {
                std::string candidate = text_.substr(i);
                if (getVisibleText(candidate, availableWidth) == visibleText) {
                    visibleStart = i;
                    break;
                }
            }
        }
        
        // Calculate cursor position relative to visible text
        if (cursorPosition_ < visibleStart) {
            return baseX; // Cursor is before visible text, show at start
        }
        
        size_t relativeCursorPos = cursorPosition_ - visibleStart;
        if (relativeCursorPos > visibleText.length()) {
            relativeCursorPos = visibleText.length();
        }
        
        std::string textToCursor = visibleText.substr(0, relativeCursorPos);
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
        return x >= x_ && x < x_ + config_.getWidth() &&
               y >= y_ && y < y_ + config_.getHeight();
    }
    
    void TextInputWidget::setText(const std::string& text) {
        if (text.length() <= config_.getMaxLength()) {
            text_ = text;
            cursorPosition_ = std::min(cursorPosition_, text_.length());
            onTextChanged.emit(text_);
        }
    }
    
    void TextInputWidget::setFocus(bool focused) {
        if (isFocused_ != focused) {
            isFocused_ = focused;
            showCursor_ = focused;
            
            std::cout << "TextInputWidget focus changed to: " << (focused ? "focused" : "unfocused") << std::endl;
            
#ifdef __EMSCRIPTEN__
            EM_ASM({
                console.log("TextInputWidget focus changed to: " + ($0 ? "focused" : "unfocused"));
            }, focused);
#endif
            onFocusChanged.emit(focused);
        }
    }
    
    void TextInputWidget::setPlaceholder(const std::string& placeholder) {
        // Note: with the new class-based config, we'd need to update the config
        // For now, we'll create a new config - this could be improved
    }
    
    // Widget interface methods
    int TextInputWidget::getWidth() const {
        return config_.getWidth();
    }
    
    int TextInputWidget::getHeight() const {
        return config_.getHeight();
    }
    
    void TextInputWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int TextInputWidget::getX() const {
        return config_.getX();
    }
    
    int TextInputWidget::getY() const {
        return config_.getY();
    }
    
    void TextInputWidget::resize(int width, int height) {
        config_.setSize(width, height);
    }
    
    // Helper functions
    std::shared_ptr<TextInputWidget> TextInput(const TextInputConfig& config, bool addToManager) {
        auto widget = std::make_shared<TextInputWidget>(config);
        
        if (addToManager) {
            addWidget(widget);
        }
        
        return widget;
    }
    
    // Preset configurations
    namespace TextInputPresets {
        TextInputConfig Default(int x, int y, int width, int height) {
            return TextInputConfig(x, y, width, height)
                .placeholder("Enter text...")
                .style(TextInputStyle()
                    .backgroundColor(0xFFFFFF)
                    .borderColor(0x888888)
                    .focusBorderColor(0x0066CC)
                    .textColor(0x000000)
                    .fontSize(2)  // Good for bitmap font
                    .useBitmapFont());
        }
        
        TextInputConfig Modern(int x, int y, int width, int height) {
            return TextInputConfig(x, y, width, height)
                .placeholder("Type here...")
                .style(TextInputStyle()
                    .backgroundColor(0xF8F9FA)  // Light gray
                    .borderColor(0xDEE2E6)      // Light border
                    .focusBorderColor(0x007BFF)  // Modern blue
                    .textColor(0x212529)        // Dark gray text
                    .fontSize(2)
                    .borderWidth(2)
                    .padding(8)
                    .useBitmapFont());
        }
        
        TextInputConfig WithTTF(int x, int y, const std::string& fontName, int width, int height) {
            return TextInputConfig(x, y, width, height)
                .placeholder("TTF Font Input...")
                .style(TextInputStyle()
                    .backgroundColor(0xFFFFFF)
                    .borderColor(0x666666)
                    .focusBorderColor(0x0066CC)
                    .textColor(0x000000)
                    .fontSize(24)  // TTF needs larger sizes
                    .borderWidth(2)
                    .padding(8)
                    .useTTFFont(fontName));
        }
    }
    
    std::string TextInputWidget::clipTextToWidth(const std::string& text, int maxWidth) const {
        if (text.empty()) return text;
        
        // If the full text fits, return it
        if (getTextWidth(text) <= maxWidth) {
            return text;
        }
        
        // Find the maximum number of characters that fit
        std::string result;
        int currentWidth = 0;
        const auto& style = config_.getStyle();
        int scale = style.getFontSize();
        
        for (char c : text) {
            int charWidth;
            if (c == ' ') {
                charWidth = 4 * scale;
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                charWidth = 8 * scale;
            } else {
                charWidth = 4 * scale;
            }
            
            if (currentWidth + charWidth > maxWidth) {
                break;
            }
            
            result += c;
            currentWidth += charWidth;
        }
        
        return result;
    }
    
    std::string TextInputWidget::getVisibleText(const std::string& text, int availableWidth) const {
        if (text.empty()) return text;
        
        // If the full text fits, return it
        if (getTextWidth(text) <= availableWidth) {
            return text;
        }
        
        // Ensure cursor is always visible - calculate visible text based on cursor position
        size_t startPos = 0;
        size_t endPos = text.length();
        
        // If cursor is at the end (most common case when typing), show text ending at cursor
        if (cursorPosition_ >= text.length()) {
            // Start from the end and work backwards to find maximum visible text
            while (startPos < text.length()) {
                std::string candidate = text.substr(startPos);
                if (getTextWidth(candidate) <= availableWidth) {
                    break;
                }
                startPos++;
            }
            return text.substr(startPos);
        }
        
        // For cursor in middle, try to center it in the visible area
        size_t cursorPos = std::min(cursorPosition_, text.length());
        
        // Calculate ideal start position (cursor roughly in middle of visible area)
        size_t idealStart = 0;
        if (cursorPos > 0) {
            // Estimate how many characters fit in half the available width
            int halfWidth = availableWidth / 2;
            size_t charCount = 0;
            int currentWidth = 0;
            
            // Count characters from cursor backwards
            for (size_t i = cursorPos; i > 0 && currentWidth < halfWidth; i--) {
                char c = text[i - 1];
                int charWidth = getCharWidth(c);
                if (currentWidth + charWidth > halfWidth) break;
                currentWidth += charWidth;
                charCount++;
            }
            
            idealStart = cursorPos - charCount;
        }
        
        // Find the actual start position that fits
        startPos = idealStart;
        while (startPos < text.length()) {
            // Try to include the cursor position
            endPos = std::max(cursorPos + 1, startPos + 1);
            
            // Extend as much as possible
            while (endPos < text.length()) {
                std::string candidate = text.substr(startPos, endPos - startPos + 1);
                if (getTextWidth(candidate) > availableWidth) {
                    break;
                }
                endPos++;
            }
            
            // Check if cursor is visible
            if (startPos <= cursorPos && cursorPos <= endPos) {
                break;
            }
            
            startPos++;
        }
        
        return text.substr(startPos, endPos - startPos);
    }
    
    int TextInputWidget::getCharWidth(char c) const {
        const auto& style = config_.getStyle();
        int scale = style.getFontSize();
        
        if (c == ' ') {
            return 4 * scale;
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            return 8 * scale;
        } else {
            return 4 * scale;
        }
    }
}

