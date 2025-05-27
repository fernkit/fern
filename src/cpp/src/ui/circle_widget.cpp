#include "../../include/fern/ui/circle_widget.hpp"
#include "../../include/fern/graphics/primitives.hpp"
#include "../../include/fern/core/widget_manager.hpp"

namespace Fern {
    CircleWidget::CircleWidget(int radius, Point position, uint32_t color)
        : radius_(radius), position_(position), color_(color) {}
        
    void CircleWidget::render() {
        Draw::circle(position_.x, position_.y, radius_, color_);
    }
    
    bool CircleWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        
        int dx = input.mouseX - position_.x;
        int dy = input.mouseY - position_.y;
        isHovered_ = (dx*dx + dy*dy) <= (radius_ * radius_);
        
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
    
    void CircleWidget::setPosition(Point position) {
        position_ = position;
    }
    
    void CircleWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    std::shared_ptr<CircleWidget> Circle(int radius, Point position, uint32_t color) {
        auto widget = std::make_shared<CircleWidget>(radius, position, color);
        addWidget(widget);
        return widget;
    }
}