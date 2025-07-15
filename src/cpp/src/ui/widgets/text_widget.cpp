#include "../../../include/fern/ui/widgets/text_widget.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include "../../../include/fern/core/widget_manager.hpp"

extern Fern::Canvas* globalCanvas;

namespace Fern {

    TextWidget::TextWidget(Point position, const std::string& text, int size, uint32_t color, 
                        bool addToManager, FontType fontType)
        : Widget(),
        text_(text), size_(size), color_(color), fontType_(fontType) {
        
        setPosition(position.x, position.y);
        resize(0, 0);  
        updateDimensions();
        
        if (addToManager) {
            addWidget(std::shared_ptr<TextWidget>(this, [](TextWidget*){}));
        }
    }
        
    void TextWidget::render() {
        switch (fontType_) {
            case FontType::TTF:
                renderTTF();
                break;
            case FontType::Bitmap:
            default:
                renderBitmap();
                break;
        }
    }
    
    void TextWidget::renderTTF() {
        if (Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, text_, x_, y_, size_, color_);
        } else {
            renderBitmap();
        }
    }
    
    void TextWidget::renderBitmap() {
        DrawText::drawText(text_.c_str(), x_, y_, size_, color_);
    }
    
    void TextWidget::setText(const std::string& text) {
        if (text_ != text) {
            text_ = text;
            updateDimensions();
            markDirty();
        }
    }
    
    void TextWidget::setSize(int size) {
        if (size_ != size) {
            size_ = size;
            updateDimensions();
            markDirty();
        }
    }
    
    void TextWidget::setColor(uint32_t color) {
        if (color_ != color) {
            color_ = color;
            markDirty();
        }
    }
    
    void TextWidget::updateDimensions() {
        int textWidth, textHeight;
        
        // Calculate dimensions based on font type
        if (fontType_ == FontType::TTF && Font::hasTTFFont()) {
            textWidth = Font::getTextWidth(text_, size_, FontType::TTF);
            textHeight = Font::getTextHeight(size_, FontType::TTF);
        } else {
            // Use bitmap font calculation - match the actual rendering logic
            int charWidth = 8 * (size_ > 0 ? size_ : 1);  // 8 pixels per character scaled by size
            int spaceWidth = 4 * (size_ > 0 ? size_ : 1); // 4 pixels for space scaled by size
            
            textWidth = 0;
            for (char c : text_) {
                if (c == ' ') {
                    textWidth += spaceWidth;
                } else {
                    textWidth += charWidth;
                }
            }
            textHeight = size_ > 0 ? size_ : 1;   // Ensure minimum height
        }
        
        resize(textWidth, textHeight);
    }
    
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                     int size, uint32_t color, bool addToManager,
                                     FontType fontType) {
        auto widget = std::make_shared<TextWidget>(position, text, size, color, false, fontType);
        
        if (addToManager) {
            addWidget(widget);
        }
        
        return widget;
    }
}