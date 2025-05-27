#pragma once

#include "widget.hpp"
#include "../graphics/colors.hpp"

namespace Fern {
    class Container : public Widget {
    public:
        Container(int x, int y, int width, int height, uint32_t color);
        
        void render() override;
        bool handleInput(const InputState&) override { return false; }
        
    private:
        int x_;
        int y_;
        int width_;
        int height_;
        uint32_t color_;
    };
    
    // Factory functions
    void BasicContainer(uint32_t color, int x, int y, int width, int height);
    void CenteredContainer(int width, int height, uint32_t color);
    void LinearGradientContainer(int x, int y, int width, int height, const LinearGradient& gradient);
}