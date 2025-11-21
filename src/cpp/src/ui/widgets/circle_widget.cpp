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
    // the position of each widget is now maintained by the parent class!
    void CircleWidget::render() {
        Draw::circle(x_ + radius_, y_ + radius_, radius_, color_);
    }
    
    bool CircleWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        
        // Calculate distance from mouse to circle center
        // Circle center is at (x_ + radius_, y_ + radius_) since x_, y_ is top-left corner
        int centerX = x_ + radius_;
        int centerY = y_ + radius_;
        int dx = input.mouseX - centerX;
        int dy = input.mouseY - centerY;
        int distanceSquared = dx*dx + dy*dy;
        
        isHovered_ = distanceSquared <= (radius_ * radius_);
        
        if (wasHovered != isHovered_) {
            onHover.emit(isHovered_);
        }
        
        if (isHovered_ && input.mouseClicked) {
            onClick.emit();
            return true;  
        }
        
        return isHovered_;
    }
    
    void CircleWidget::setRadius(int radius) {
        radius_ = radius;
    }
    
    void CircleWidget::setPosition(const Point& position){
        Widget::setPosition(position.x, position.y);  // update base x_, y_ fields
        position_.x = position.x;      // update any internal center field
        position_.y = position.y;
    }
    
    void CircleWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    std::shared_ptr<CircleWidget> Circle(int radius, Point position, uint32_t color, bool addToManager) {
        auto widget = std::make_shared<CircleWidget>(radius, position, color);
        if(addToManager){addWidget(widget);}
        return widget;
    }
}