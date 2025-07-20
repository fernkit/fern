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
    int totalFlex = 0;
    int spacingWidgetsHeight = 0;
    
    for (auto& child : children_) {
        auto expanded = std::dynamic_pointer_cast<ExpandedWidget>(child);
        auto spacingWidget = std::dynamic_pointer_cast<SpacingWidget>(child);

        if (spacingWidget) {
            // In a column, spacing widget height creates vertical space
            spacingWidgetsHeight += child->getHeight();
            continue;
        }

        if (expanded) {
            totalFlex += expanded->getFlex();
        } else {
            // Count actual or default height for fixed widgets
            int widgetHeight = child->getHeight() > 0 ? child->getHeight() : 40; // Default 40px
            totalFixedHeight += widgetHeight;
        }
    }
    
    int availableHeight = height_ - totalFixedHeight - spacingWidgetsHeight;
    if (availableHeight < 0) availableHeight = 0;
    
    int startY = y_;
    int spacingBetween = 0; 
    int remainingSpace = height_ - totalFixedHeight - spacingWidgetsHeight;
    
    // Add minimum spacing between widgets if no explicit spacing is provided
    int minSpacing = 5; // 5px minimum spacing between widgets
    
    switch (mainAxisAlignment_) {
        case MainAxisAlignment::Start:
            spacingBetween = minSpacing;
            break;
            
        case MainAxisAlignment::Center:
            startY = y_ + remainingSpace / 2;
            break;
            
        case MainAxisAlignment::End:
            startY = y_ + remainingSpace;
            break;
            
        case MainAxisAlignment::SpaceBetween:
            if (children_.size() > 1) {
                spacingBetween = remainingSpace / (children_.size() - 1);
            }
            break;
            
        case MainAxisAlignment::SpaceAround:
            if (children_.size() > 0) {
                spacingBetween = remainingSpace / children_.size();
                startY = y_ + spacingBetween / 2;
            }
            break;
            
        case MainAxisAlignment::SpaceEvenly:
            if (children_.size() + 1 > 0) {
                spacingBetween = remainingSpace / (children_.size() + 1);
                startY = y_ + spacingBetween;
            }
            break;
    }
    
    int currentY = startY;
    
    for (auto& child : children_) {
        auto expanded = std::dynamic_pointer_cast<ExpandedWidget>(child);
        auto spacingWidget = std::dynamic_pointer_cast<SpacingWidget>(child);

        if (spacingWidget) {
            // In a column, spacing widget height creates vertical space
            currentY += child->getHeight();
            continue;
        }
        
        int childHeight = 0;
        if (expanded) {
            childHeight = (totalFlex > 0) ? (availableHeight * expanded->getFlex()) / totalFlex : 0;
        } else {
            // If widget doesn't have explicit height, give it a reasonable default
            childHeight = child->getHeight() > 0 ? child->getHeight() : 40; // Default 40px height
        }
        
        int childX = x_;
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
                childX = x_;
                break;
        }
        
        child->setPosition(childX, currentY);
        
        if (crossAxisAlignment_ == CrossAxisAlignment::Stretch) {
            child->resize(width_, childHeight);
        } else if (child->getWidth() <= 0) {
            child->resize(width_, childHeight);
        } else {
            child->resize(child->getWidth(), childHeight);
        }
        
        currentY += childHeight + spacingBetween; 
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