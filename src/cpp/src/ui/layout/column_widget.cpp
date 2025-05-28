#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"
#include <cstdio>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Fern {

ColumnWidget::ColumnWidget(int x, int y, int width, int height,
                         MainAxisAlignment mainAlignment,
                         CrossAxisAlignment crossAlignment)
    : LayoutWidget(x, y, width, height), 
      mainAxisAlignment_(mainAlignment),
      crossAxisAlignment_(crossAlignment) {}

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
    
    for (auto& child : children_) {
        // Special case for SizedBox
        if (std::dynamic_pointer_cast<SpacingWidget>(child) && child->getHeight() == 0) {
            totalFixedHeight += child->getWidth();  // Use width for vertical spacing
            continue;
        }
        
        if (child->getHeight() > 0) {
            totalFixedHeight += child->getHeight();
        } else {
            numExpandable++;
        }
    }
    
    int expandableHeight = 0;
    if (numExpandable > 0) {
        expandableHeight = (height_ - totalFixedHeight) / numExpandable;
    }
    
    // Calculate spacing based on alignment
    int startY = y_;
    int spacing = 0;
    int remainingSpace = height_ - totalFixedHeight;
    
    switch (mainAxisAlignment_) {
        case MainAxisAlignment::Start:
            // Default - start at y_
            break;
            
        case MainAxisAlignment::Center:
            startY = y_ + remainingSpace / 2;
            break;
            
        case MainAxisAlignment::End:
            startY = y_ + remainingSpace;
            break;
            
        case MainAxisAlignment::SpaceBetween:
            if (children_.size() > 1) {
                spacing = remainingSpace / (children_.size() - 1);
            }
            break;
            
        case MainAxisAlignment::SpaceAround:
            if (children_.size() > 0) {
                spacing = remainingSpace / children_.size();
                startY = y_ + spacing / 2;
            }
            break;
            
        case MainAxisAlignment::SpaceEvenly:
            if (children_.size() + 1 > 0) {
                spacing = remainingSpace / (children_.size() + 1);
                startY = y_ + spacing;
            }
            break;
    }
    
    // Position children with the calculated alignment
    int currentY = startY;
    
    for (auto& child : children_) {
        // Special case for SizedBox
        if (std::dynamic_pointer_cast<SpacingWidget>(child) && child->getHeight() == 0) {
            currentY += child->getWidth();  // Use width for vertical spacing
            continue;
        }
        
        int childHeight = child->getHeight() > 0 ? child->getHeight() : expandableHeight;
        
        // Calculate X position based on cross axis alignment
        int childX = x_;
        
        // Apply cross axis alignment
        switch (crossAxisAlignment_) {
            case CrossAxisAlignment::Start:
                childX = x_;
                break;
                
            case CrossAxisAlignment::Center:
                if (child->getWidth() > 0 && child->getWidth() < width_) {
                    childX = x_ + (width_ - child->getWidth()) / 2;
                }
                break;
                
            case CrossAxisAlignment::End:
                if (child->getWidth() > 0) {
                    childX = x_ + width_ - child->getWidth();
                }
                break;
                
            case CrossAxisAlignment::Stretch:
                // For stretch, we'll set the width in the resize call
                childX = x_;
                break;
        }
        
        child->setPosition(childX, currentY);
        
        // Handle sizing based on cross axis alignment
        if (crossAxisAlignment_ == CrossAxisAlignment::Stretch) {
            child->resize(width_, childHeight);
        } else if (child->getWidth() <= 0) {
            child->resize(width_, childHeight);
        } else {
            child->resize(child->getWidth(), childHeight);
        }
        
        currentY += childHeight + spacing;
    }
}

std::shared_ptr<ColumnWidget> Column(
    const std::vector<std::shared_ptr<Widget>>& children, 
    bool addToManager,
    MainAxisAlignment mainAlignment,
    CrossAxisAlignment crossAlignment) 
{
    auto column = std::make_shared<ColumnWidget>(0, 0, 0, 0, mainAlignment, crossAlignment);
    
    if (!children.empty()) {
        column->addAll(children);
    }
    
    if (addToManager) {
        addWidget(column);
    }
    
    return column;
}

} // namespace Fern