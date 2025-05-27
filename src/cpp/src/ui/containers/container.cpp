#include "../../include/fern/ui/containers/container.hpp"
#include "../../include/fern/graphics/primitives.hpp"
#include "../../include/fern/core/canvas.hpp"

namespace Fern {
    Container::Container(int x, int y, int width, int height, uint32_t color)
        : x_(x), y_(y), width_(width), height_(height), color_(color) {}
        
    void Container::render() {
        Draw::rect(x_, y_, width_, height_, color_);
    }
    
    void BasicContainer(uint32_t color, int x, int y, int width, int height) {
        Fern::Container container(x, y, width, height, color);
        container.render();
    }
    
    void CenteredContainer(int width, int height, uint32_t color) {
        if (!globalCanvas) return;
        
        int x = (globalCanvas->getWidth() - width) / 2;
        int y = (globalCanvas->getHeight() - height) / 2;
        
        Container(color, x, y, width, height);
    }
    
    void LinearGradientContainer(int x, int y, int width, int height, const LinearGradient& gradient) {
        if (!globalCanvas) return;
        
        if (gradient.isVertical()) {
            for (int row = 0; row < height; row++) {
                float pos = (float)row / height;
                uint32_t color = gradient.colorAt(pos);
                Draw::rect(x, y + row, width, 1, color);
            }
        } else {
            for (int col = 0; col < width; col++) {
                float pos = (float)col / width;
                uint32_t color = gradient.colorAt(pos);
                Draw::rect(x + col, y, 1, height, color);
            }
        }
    }
}