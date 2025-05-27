#include "../../../include/fern/ui/layout/layout.hpp"
#include "../../../include/fern/fern.hpp"

namespace Fern {
    ColumnWidget::ColumnWidget(int x, int y, int width, int height)
        : LayoutWidget(x, y, width, height) {}
    
    void ColumnWidget::add(std::shared_ptr<Widget> child) {
        children_.push_back(child);
        arrangeChildren();
    }
    
    void ColumnWidget::addAll(const std::vector<std::shared_ptr<Widget>>& children) {
        children_.insert(children_.end(), children.begin(), children.end());
        arrangeChildren();
    }
    
    void ColumnWidget::arrangeChildren() {
        if (children_.empty()) return;
        
        int totalFixedHeight = 0;
        int numExpandable = 0;
        
        // For fixed-height widgets, we add up their heights
        // For expandable widgets (height <= 0), we count how many there are
        for (auto& child : children_) {
            if (child->getHeight() > 0) {
                totalFixedHeight += child->getHeight();
            } else {
                numExpandable++;
            }
        }
        
        // Calculate remaining space for expandable children and distribute equally.
        int remainingHeight = height_ - totalFixedHeight;
        int expandableHeight = numExpandable > 0 ? remainingHeight / numExpandable : 0;
        
        // Position each child vertically.
        int currentY = y_;
        
        for (auto& child : children_) {
            int childHeight = child->getHeight() > 0 ? child->getHeight() : expandableHeight;
            int childX = x_ + (width_ - child->getWidth()) / 2;
            child->setPosition(childX, currentY);
            
            if (child->getWidth() <= 0) {
                child->resize(width_, childHeight);
            } else {
                child->resize(child->getWidth(), childHeight);
            }
            
            currentY += childHeight;
        }
    }
    
    std::shared_ptr<ColumnWidget> Column(const std::vector<std::shared_ptr<Widget>>& children, bool addToManager) {
        int screenWidth = Fern::getWidth();
        int screenHeight = Fern::getHeight();
        
        auto column = std::make_shared<ColumnWidget>(0, 0, screenWidth, screenHeight);
        
        if (!children.empty()) {
            column->addAll(children);
        }
        if(addToManager){addWidget(column);}

        return column;
    }
}