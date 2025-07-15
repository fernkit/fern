#include "../../../include/fern/ui/widgets/circle_widget.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/core/widget_manager.hpp"

namespace Fern {
    CircleWidget::CircleWidget(int radius, Point position, uint32_t color)
        : radius_(radius), position_(position), color_(color) {
        x_ = position.x;
        y_ = position.y;
    }
    
    // don't use positions!! 
    // the position of each widget is now maintaine by the parent class!
    void CircleWidget::render() {
        Draw::circle(x_ + radius_, y_ + radius_, radius_, color_);
    }
    
    bool CircleWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        
        int dx = input.mouseX - x_;
        int dy = input.mouseY - y_;
        isHovered_ = (dx*dx + dy*dy) <= (radius_ * radius_);
        
        // Mark dirty if hover state changed
        if (wasHovered != isHovered_) {
            markDirty();
            onHover.emit(isHovered_);
        }
        
        if (isHovered_ && input.mouseClicked) {
            onClick.emit();
            return true;  
        }
        
        return isHovered_;
    }
    
    void CircleWidget::setRadius(int radius) {
        if (radius_ != radius) {
            radius_ = radius;
            markDirty();
        }
    }
    
    void CircleWidget::setPosition(const Point& position){
        Widget::setPosition(position.x, position.y);  // update base x_, y_ fields
        position_.x = position.x;      // update any internal center field
        position_.y = position.y;
    }
    
    void CircleWidget::setColor(uint32_t color) {
        if (color_ != color) {
            color_ = color;
            markDirty();
        }
    }
    
    std::shared_ptr<CircleWidget> Circle(int radius, Point position, uint32_t color, bool addToManager) {
        auto widget = std::make_shared<CircleWidget>(radius, position, color);
        if(addToManager){addWidget(widget);}
        return widget;
    }
}