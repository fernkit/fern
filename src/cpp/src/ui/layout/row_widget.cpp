#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"
#include "../../../include/fern/fern.hpp"

namespace Fern {

RowWidget::RowWidget(int x, int y, int width, int height, 
                    MainAxisAlignment mainAlignment,
                    CrossAxisAlignment crossAlignment)
    : LayoutWidget(x, y, width, height), 
      mainAxisAlignment_(mainAlignment),
      crossAxisAlignment_(crossAlignment) {}

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
    
    // First pass: calculate total width
    for (auto& child : children_) {
        // Special case for SizedBox
        if (std::dynamic_pointer_cast<SpacingWidget>(child) && child->getWidth() == 0) {
            totalFixedWidth += child->getHeight();  // Use height for horizontal spacing
            continue;
        }
        
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
    
    // Calculate spacing based on alignment
    int startX = x_;
    int spacing = 0;
    int remainingSpace = width_ - totalFixedWidth;
    
    switch (mainAxisAlignment_) {
        case MainAxisAlignment::Start:
            // Default - start at x_
            break;
            
        case MainAxisAlignment::Center:
            startX = x_ + remainingSpace / 2;
            break;
            
        case MainAxisAlignment::End:
            startX = x_ + remainingSpace;
            break;
            
        case MainAxisAlignment::SpaceBetween:
            if (children_.size() > 1) {
                spacing = remainingSpace / (children_.size() - 1);
            }
            break;
            
        case MainAxisAlignment::SpaceAround:
            if (children_.size() > 0) {
                spacing = remainingSpace / children_.size();
                startX = x_ + spacing / 2;
            }
            break;
            
        case MainAxisAlignment::SpaceEvenly:
            if (children_.size() + 1 > 0) {
                spacing = remainingSpace / (children_.size() + 1);
                startX = x_ + spacing;
            }
            break;
    }
    
    // Position children with the calculated alignment
    int currentX = startX;
    
    for (auto& child : children_) {
        // Special case for SizedBox
        if (std::dynamic_pointer_cast<SpacingWidget>(child) && child->getWidth() == 0) {
            currentX += child->getHeight();  // Use height for horizontal spacing
            continue;
        }
        
        int childWidth = child->getWidth() > 0 ? child->getWidth() : expandableWidth;
        
        // Calculate Y position based on cross axis alignment
        int childY = y_;
        
        // Apply cross axis alignment
        switch (crossAxisAlignment_) {
            case CrossAxisAlignment::Start:
                childY = y_;
                break;
                
            case CrossAxisAlignment::Center:
                if (child->getHeight() > 0 && child->getHeight() < height_) {
                    childY = y_ + (height_ - child->getHeight()) / 2;
                }
                break;
                
            case CrossAxisAlignment::End:
                if (child->getHeight() > 0) {
                    childY = y_ + height_ - child->getHeight();
                }
                break;
                
            case CrossAxisAlignment::Stretch:
                // For stretch, we'll set the height in the resize call
                childY = y_;
                break;
        }
        
        child->setPosition(currentX, childY);
        
        // Handle sizing based on cross axis alignment
        if (crossAxisAlignment_ == CrossAxisAlignment::Stretch) {
            child->resize(childWidth, height_);
        } else if (child->getHeight() <= 0) {
            child->resize(childWidth, height_);
        } else {
            child->resize(childWidth, child->getHeight());
        }
        
        currentX += childWidth + spacing;
    }
}

std::shared_ptr<RowWidget> Row(
    const std::vector<std::shared_ptr<Widget>>& children, 
    bool addToManager,
    MainAxisAlignment mainAlignment,
    CrossAxisAlignment crossAlignment) 
{
    auto row = std::make_shared<RowWidget>(0, 0, 0, 0, mainAlignment, crossAlignment);
    
    if (!children.empty()) {
        row->addAll(children);
    }
    
    if (addToManager) {
        addWidget(row);
    }
    
    return row;
}

} // namespace Fern