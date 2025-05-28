#include "fern/ui/layout/layout.hpp"
#include "fern/core/widget_manager.hpp"

namespace Fern {
    
    ExpandedWidget::ExpandedWidget(std::shared_ptr<Widget> child, int flex)
        : LayoutWidget(0, 0, 0, 0), flex_(flex) {
        if (child) {
            children_.push_back(child);
        }
    }
    
    void ExpandedWidget::render() {
        if (!children_.empty()) {
            children_[0]->render();
        }
    }
    
    void ExpandedWidget::arrangeChildren() {
        if (!children_.empty()) {
            children_[0]->resize(width_, height_);
            children_[0]->setPosition(x_, y_);
        }
    }
    
    std::shared_ptr<ExpandedWidget> Expanded(
        std::shared_ptr<Widget> child, 
        int flex,
        bool addToManager
    ) {
        auto expanded = std::make_shared<ExpandedWidget>(child, flex);
        
        if (addToManager) {
            addWidget(expanded);
        }
        
        return expanded;
    }
}