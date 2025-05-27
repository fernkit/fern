#pragma once

// Include all component headers
#include "core/canvas.hpp"
#include "core/input.hpp"
#include "graphics/primitives.hpp"
#include "graphics/colors.hpp"
#include "text/font.hpp"
#include "ui/text_widget.hpp"
#include "ui/circle_widget.hpp"
#include "ui/line_widget.hpp"
#include "ui/button.hpp"
#include "core/widget_manager.hpp"
#include "ui/layout.hpp"

namespace Fern {
    void initialize(uint32_t* pixelBuffer, int width, int height);

    // new automatic overloads deprecating manual buffers
    void initialize();
    void initialize(int width, int height); 

    // Get current canvas dimensions
    int getWidth();
    int getHeight();
    Point getCanvasSize();

    void startRenderLoop();
    void setDrawCallback(std::function<void()> callback);
}