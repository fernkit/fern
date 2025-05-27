#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"
#include "../../../include/fern/fern.hpp"

namespace Fern {

CenterWidget::CenterWidget(int x, int y, int width, int height)
    : LayoutWidget(x, y, width, height) {}

void CenterWidget::add(std::shared_ptr<Widget> child) {
    children_.clear();
    children_.push_back(child);
    arrangeChildren();
}

void CenterWidget::arrangeChildren() {
    if (children_.empty()) return;
    
    auto& child = children_[0];
    
    int childX = x_ + (width_ - child->getWidth()) / 2;
    int childY = y_ + (height_ - child->getHeight()) / 2;
    
    child->setPosition(childX, childY);
}

std::shared_ptr<CenterWidget> Center(std::shared_ptr<Widget> child) {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto center = std::make_shared<CenterWidget>(0, 0, screenWidth, screenHeight);
    
    if (child) {
        center->add(child);
    }
    
    addWidget(center);
    
    return center;
}

} // namespace Fern