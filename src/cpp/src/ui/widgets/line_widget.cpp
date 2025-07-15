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
        start_ = start;
    }
    
    void LineWidget::setEnd(Point end) {
        end_ = end;
    }
    
    void LineWidget::setThickness(int thickness) {
        thickness_ = thickness;
    }
    
    void LineWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    std::shared_ptr<LineWidget> Line(Point start, Point end, int thickness, uint32_t color, bool addToManager) {
        auto widget = std::make_shared<LineWidget>(start, end, thickness, color);
        if(addToManager){
            addWidget(widget);
        }
        return widget;
    }
}