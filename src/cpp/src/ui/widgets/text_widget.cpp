#include "../../include/fern/ui/widgets/text_widget.hpp"
#include "../../include/fern/text/font.hpp"
#include "../../include/fern/core/widget_manager.hpp"

namespace Fern {
    TextWidget::TextWidget(Point position, const std::string& text, int scale, uint32_t color)
        : position_(position), text_(text), scale_(scale), color_(color) {}
        
    void TextWidget::render() {
        // don't use positions!! 
        // the position of each widget is now maintaine by the parent class!
        DrawText::drawText(text_.c_str(), x_, y_, scale_, color_);
    }
    
    bool TextWidget::handleInput(const InputState& input) {
        return false;
    }
    
    void TextWidget::setText(const std::string& text) {
        text_ = text;
    }
    
    void TextWidget::setPosition(const Point& position) {
        Widget::setPosition(position.x, position.y);  // update base x_, y_ fields
        position_ = Point(position.x, position.y);    // update internal position_ field
    }
    
    void TextWidget::setScale(int scale) {
        scale_ = scale;
    }
    
    void TextWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                     int scale, uint32_t color, bool addToManager) {
        auto widget = std::make_shared<TextWidget>(position, text, scale, color);
        if(addToManager){addWidget(widget);}
        return widget;
    }
}