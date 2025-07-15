#include "../../include/fern/core/widget_manager.hpp"
#include "../../include/fern/ui/layout/layout.hpp"

namespace Fern {

void WidgetManager::updateAll(const InputState& input) {
    bool inputHandled = false;
    for (auto it = widgets_.rbegin(); it != widgets_.rend(); ++it) {
        updateWidget(*it, input, inputHandled);
    }
}

void WidgetManager::updateWidget(std::shared_ptr<Widget> widget, const InputState& input, bool& inputHandled) {
    if (!inputHandled) {
        inputHandled = widget->handleInput(input);
    }
    
    // Check if this widget is a layout and recursively update its children
    if (auto layout = std::dynamic_pointer_cast<LayoutWidget>(widget)) {
        for (auto& child : layout->getChildren()) {
            updateWidget(child, input, inputHandled);
        }
    }
}

} // namespace Fern
