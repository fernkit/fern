#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"
#include "../../../include/fern/fern.hpp"


namespace Fern {

RowWidget::RowWidget(int x, int y, int width, int height)
    : LayoutWidget(x, y, width, height) {}

void RowWidget::add(std::shared_ptr<Widget> child) {
    children_.push_back(child);
    arrangeChildren();
}

void RowWidget::addAll(const std::vector<std::shared_ptr<Widget>>& children) {
    children_.insert(children_.end(), children.begin(), children.end());
    arrangeChildren();
}

void RowWidget::arrangeChildren() {
    if (children_.empty()) return;
    
    int totalFixedWidth = 0;
    int expandableCount = 0;
    
    for (auto& child : children_) {
        if (child->getWidth() > 0) {
            totalFixedWidth += child->getWidth();
        } else {
            expandableCount++;
        }
    }
    
    int expandableWidth = 0;
    if (expandableCount > 0) {
        expandableWidth = (width_ - totalFixedWidth) / expandableCount;
    }
    
    int currentX = x_;
    
    for (auto& child : children_) {
        int childWidth = child->getWidth() > 0 ? child->getWidth() : expandableWidth;
        int childY = y_ + (height_ - child->getHeight()) / 2;  
        child->setPosition(currentX, childY);
        
        if (child->getHeight() <= 0) {
            child->resize(childWidth, height_);
        } else {
            child->resize(childWidth, child->getHeight());
        }
        
        currentX += childWidth;
    }
}

std::shared_ptr<RowWidget> Row(const std::vector<std::shared_ptr<Widget>>& children, bool addToManager) {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto row = std::make_shared<RowWidget>(0, 0, screenWidth, screenHeight);
    
    if (!children.empty()) {
        row->addAll(children);
    }
    
    if(addToManager){addWidget(row);}
    
    return row;
}

} // namespace Fern