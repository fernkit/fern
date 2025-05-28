#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"

namespace Fern {

SpacingWidget::SpacingWidget(int width, int height) {
    width_ = width;
    height_ = height;
}

void SpacingWidget::render() {
}

bool SpacingWidget::handleInput(const InputState& input) {
    return false;
}

std::shared_ptr<SpacingWidget> SizedBox(int width, int height, bool addToManager) {
    auto spacingWidget = std::make_shared<SpacingWidget>(width, height);
    
    if (addToManager) {
        addWidget(spacingWidget);
    }
    
    return spacingWidget;
}

} // namespace Fern