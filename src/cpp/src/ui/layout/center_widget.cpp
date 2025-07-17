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
    
    if (child->getWidth() <= 0 || child->getHeight() <= 0) {
        if (width_ > 0 && child->getWidth() <= 0) {
            child->resize(width_, child->getHeight());
        }
        if (height_ > 0 && child->getHeight() <= 0) {
            child->resize(child->getWidth(), height_);
        }
    }
    
    int childX = x_;
    int childY = y_;
    
    if (width_ > 0 && child->getWidth() > 0) {
        childX = x_ + (width_ - child->getWidth()) / 2;
    }
    
    if (height_ > 0 && child->getHeight() > 0) {
        childY = y_ + (height_ - child->getHeight()) / 2;
    }
    
    child->setPosition(childX, childY);
}

void CenterWidget::onWindowResize(int newWidth, int newHeight) {
    // Update this widget's dimensions to match the new window size
    resize(newWidth, newHeight);
    // Re-arrange children to center them in the new dimensions
    arrangeChildren();
}

std::shared_ptr<CenterWidget> Center(std::shared_ptr<Widget> child, bool addToManager) {
    // Create center widget with default dimensions - will inherit from parent
    auto center = std::make_shared<CenterWidget>(0, 0, 0, 0);
    
    if (child) {
        center->add(child);
    }
    
    // Only add to manager if specified
    if (addToManager) {
        addWidget(center);
    }
    
    return center;
}

} // namespace Fern