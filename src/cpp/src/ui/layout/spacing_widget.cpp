#include "fern/ui/layout/layout.hpp"

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

std::shared_ptr<SpacingWidget> SizedBox(int width, int height) {
    return std::make_shared<SpacingWidget>(width, height);
}

} // namespace Fern