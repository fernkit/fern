#pragma once

#include "../core/canvas.hpp"
#include <cstdint>

namespace Fern {
    namespace Draw {
        void fill(uint32_t color);
        void rect(int x, int y, int width, int height, uint32_t color);
        void roundedRect(int x, int y, int width, int height, int radius, uint32_t color);
        void roundedRectBorder(int x, int y, int width, int height, int radius, int borderWidth, uint32_t color);
        void circle(int cx, int cy, int radius, uint32_t color);
        void line(int x1, int y1, int x2, int y2, int thickness, uint32_t color);
    }
}