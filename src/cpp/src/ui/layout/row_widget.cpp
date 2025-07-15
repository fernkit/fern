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
    markDirty();
}

void RowWidget::addAll(const std::vector<std::shared_ptr<Widget>>& children) {
    children_.insert(children_.end(), children.begin(), children.end());
    arrangeChildren();
    markDirty();
}

void RowWidget::arrangeChildren() {
    if (children_.empty()) return;
    
    int totalFixedWidth = 0;
    int totalFlex = 0;
    int spacingWidgetsWidth = 0;

    for (auto& child : children_) {
         auto expanded = std::dynamic_pointer_cast<ExpandedWidget>(child);
        auto spacing = std::dynamic_pointer_cast<SpacingWidget>(child);

        if (spacing && child->getWidth() == 0) {
            spacingWidgetsWidth += child->getHeight();
            continue;
        }

        if (expanded) {
            totalFlex += expanded->getFlex();
        } else if (child->getWidth() > 0) {
            totalFixedWidth += child->getWidth();
        }
    }
    
    int availableWidth = width_ - totalFixedWidth - spacingWidgetsWidth;
    if (availableWidth < 0) availableWidth = 0;
    
    int startX = x_;
    int spacing = 0;
    int remainingSpace = width_ - totalFixedWidth - spacingWidgetsWidth;

    switch (mainAxisAlignment_) {
        case MainAxisAlignment::Start:
            break;
        case MainAxisAlignment::Center:
            startX = x_ + remainingSpace / 2;
            break;
        case MainAxisAlignment::End:
            startX = x_ + remainingSpace;
            break;
        case MainAxisAlignment::SpaceBetween:
            if (children_.size() > 1)
                spacing = remainingSpace / (children_.size() - 1);
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
    
    int currentX = startX;
    
        for (auto& child : children_) {
        auto expanded = std::dynamic_pointer_cast<ExpandedWidget>(child);
        auto spacingWidget = std::dynamic_pointer_cast<SpacingWidget>(child);

        if (spacingWidget && child->getWidth() == 0) {
            currentX += child->getHeight();
            continue;
        }

        int childWidth = 0;
        if (expanded) {
            childWidth = (totalFlex > 0) ? (availableWidth * expanded->getFlex()) / totalFlex : 0;
        } else {
            childWidth = child->getWidth() > 0 ? child->getWidth() : 0;
        }

        int childY = y_;
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
                childY = y_;
                break;
        }

        child->setPosition(currentX, childY);

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