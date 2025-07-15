#pragma once

#include "core/canvas.hpp"
#include "core/input.hpp"
#include "graphics/primitives.hpp"
#include "graphics/colors.hpp"
#include "text/font.hpp"
#include "ui/widgets/text_widget.hpp"
#include "ui/widgets/circle_widget.hpp"
#include "ui/widgets/line_widget.hpp"
#include "ui/widgets/button_widget.hpp"
#include "ui/widgets/text_input_widget.hpp"
#include "ui/widgets/slider_widget.hpp"
#include "ui/widgets/radio_button_widget.hpp"
#include "ui/widgets/dropdown_widget.hpp"
#include "ui/widgets/circular_indicator_widget.hpp"
#include "ui/widgets/progress_bar_widget.hpp"
#include "core/widget_manager.hpp"
#include "core/scene_manager.hpp"
#include "ui/layout/layout.hpp"
#include "ui/containers/container.hpp"

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