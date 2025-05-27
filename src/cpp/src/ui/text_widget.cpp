#include "../../include/fern/ui/text_widget.hpp"
#include "../../include/fern/text/font.hpp"
#include "../../include/fern/core/widget_manager.hpp"

namespace Fern {
    TextWidget::TextWidget(Point position, const std::string& text, int scale, uint32_t color)
        : position_(position), text_(text), scale_(scale), color_(color) {}
        
    void TextWidget::render() {
        DrawText::drawText(text_.c_str(), position_.x, position_.y, scale_, color_);
    }
    
    bool TextWidget::handleInput(const InputState& input) {
        return false;
    }
    
    void TextWidget::setText(const std::string& text) {
        text_ = text;
    }
    
    void TextWidget::setPosition(const Point& position) {
        position_ = position;
    }
    
    void TextWidget::setScale(int scale) {
        scale_ = scale;
    }
    
    void TextWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                     int scale, uint32_t color) {
        auto widget = std::make_shared<TextWidget>(position, text, scale, color);
        addWidget(widget);
        return widget;
    }
}