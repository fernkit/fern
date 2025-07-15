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
        text_ = text;
        updateDimensions();
    }
    
    void TextWidget::setSize(int size) {
        size_ = size;
        updateDimensions();
    }
    
    void TextWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    void TextWidget::updateDimensions() {
        int textWidth, textHeight;
        
        // Calculate dimensions based on font type
        if (fontType_ == FontType::TTF && Font::hasTTFFont()) {
            textWidth = Font::getTextWidth(text_, size_, FontType::TTF);
            textHeight = Font::getTextHeight(size_, FontType::TTF);
        } else {
            // Use simple bitmap font calculation
            int charWidth = size_ * 6 / 8;
            textWidth = text_.length() * charWidth;
            textHeight = size_;
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