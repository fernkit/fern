#pragma once
#include "widget.hpp"
#include <memory>

namespace Fern {
    class LineWidget : public Widget {
    public:
        LineWidget(Point start, Point end, int thickness, uint32_t color);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Setters
        void setStart(Point start);
        void setEnd(Point end);
        void setThickness(int thickness);
        void setColor(uint32_t color);
        
        // Getters
        Point getStart() const { return start_; }
        Point getEnd() const { return end_; }
        int getThickness() const { return thickness_; }
        uint32_t getColor() const { return color_; }
        
    private:
        Point start_;
        Point end_;
        int thickness_;
        uint32_t color_;
    };
    
    // Factory function
    std::shared_ptr<LineWidget> Line(Point start, Point end, int thickness, uint32_t color);
}