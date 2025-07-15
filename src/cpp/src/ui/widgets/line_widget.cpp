#include "../../../include/fern/ui/widgets/line_widget.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include <cmath>

namespace Fern {
    LineWidget::LineWidget(Point start, Point end, int thickness, uint32_t color)
        : start_(start), end_(end), thickness_(thickness), color_(color) {}
        
    void LineWidget::render() {
        Draw::line(start_.x, start_.y, end_.x, end_.y, thickness_, color_);
    }
    
    bool LineWidget::handleInput(const InputState& input) {
        return false;
    }
    
    void LineWidget::setStart(Point start) {
        if (start_.x != start.x || start_.y != start.y) {
            start_ = start;
            markDirty();
        }
    }
    
    void LineWidget::setEnd(Point end) {
        if (end_.x != end.x || end_.y != end.y) {
            end_ = end;
            markDirty();
        }
    }
    
    void LineWidget::setThickness(int thickness) {
        if (thickness_ != thickness) {
            thickness_ = thickness;
            markDirty();
        }
    }
    
    void LineWidget::setColor(uint32_t color) {
        if (color_ != color) {
            color_ = color;
            markDirty();
        }
    }
    
    std::shared_ptr<LineWidget> Line(Point start, Point end, int thickness, uint32_t color, bool addToManager) {
        auto widget = std::make_shared<LineWidget>(start, end, thickness, color);
        if(addToManager){
            addWidget(widget);
        }
        return widget;
    }
}