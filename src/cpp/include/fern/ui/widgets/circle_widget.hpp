#pragma once
#include "widget.hpp"
#include <memory>
#include "../../core/signal.hpp"

namespace Fern {
    class CircleWidget : public Widget {
    public:
        CircleWidget(int radius, Point position, uint32_t color);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        void setRadius(int radius);
        void setPosition(const Point& position);
        void setColor(uint32_t color);
        
        int getRadius() const { return radius_; }
        Point getPosition() const { return position_; }
        uint32_t getColor() const { return color_; }
        int getWidth() const override { return radius_ * 2; }
        int getHeight() const override { return radius_ * 2; }
        Signal<> onClick; 
        Signal<bool> onHover; 
        
    private:
        int radius_;
        Point position_;
        uint32_t color_;
        bool isHovered_ = false;
    };
    
    std::shared_ptr<CircleWidget> Circle(int radius, Point position, uint32_t color, bool addToManager = false);
}