#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"
#include "../../../include/fern/fern.hpp"


namespace Fern {

PaddingWidget::PaddingWidget(int x, int y, int width, int height,
                           int left, int top, int right, int bottom)
    : LayoutWidget(x, y, width, height),
      paddingLeft_(left), paddingTop_(top), 
      paddingRight_(right), paddingBottom_(bottom) {}

void PaddingWidget::add(std::shared_ptr<Widget> child) {
    children_.clear();
    children_.push_back(child);
    arrangeChildren();
}

void PaddingWidget::arrangeChildren() {
    if (children_.empty()) return;
    
    auto& child = children_[0];
    
    int childX = x_ + paddingLeft_;
    int childY = y_ + paddingTop_;
    int childWidth = width_ - paddingLeft_ - paddingRight_;
    int childHeight = height_ - paddingTop_ - paddingBottom_;
    
    child->setPosition(childX, childY);
    child->resize(childWidth, childHeight);
}


std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, int all, bool addToManager) {
    return Padding(child, all, all, all, all, addToManager);
}

std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child,
                                     int left, int top, int right, int bottom, bool addToManager) {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto padding = std::make_shared<PaddingWidget>(0, 0, screenWidth, screenHeight,
                                                left, top, right, bottom);
    
    if (child) {
        padding->add(child);
    }
    
    if(addToManager){addWidget(padding);}
    
    return padding;
}

} // namespace Fern