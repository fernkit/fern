#include "fern/ui/containers/container.hpp"
#include "fern/graphics/primitives.hpp"
#include "fern/core/canvas.hpp"
#include "fern/core/widget_manager.hpp"
#include "fern/core/input.hpp"

namespace Fern {
    uint32_t LinearGradient::colorAt(float position) const {
        if (stops_.empty()) return 0;
        if (stops_.size() == 1) return stops_[0].color;
        
        size_t nextStopIdx = 0;
        while (nextStopIdx < stops_.size() && stops_[nextStopIdx].position < position) {
            nextStopIdx++;
        }
        
        if (nextStopIdx == 0) return stops_[0].color;
        if (nextStopIdx >= stops_.size()) return stops_.back().color;
        
        const GradientStop& prevStop = stops_[nextStopIdx - 1];
        const GradientStop& nextStop = stops_[nextStopIdx];
        
        float range = nextStop.position - prevStop.position;
        if (range <= 0) return prevStop.color;
        
        float t = (position - prevStop.position) / range;
        
        uint8_t prevA = (prevStop.color >> 24) & 0xFF;
        uint8_t prevR = (prevStop.color >> 16) & 0xFF;
        uint8_t prevG = (prevStop.color >> 8) & 0xFF;
        uint8_t prevB = prevStop.color & 0xFF;
        
        uint8_t nextA = (nextStop.color >> 24) & 0xFF;
        uint8_t nextR = (nextStop.color >> 16) & 0xFF;
        uint8_t nextG = (nextStop.color >> 8) & 0xFF;
        uint8_t nextB = nextStop.color & 0xFF;
        
        uint8_t a = prevA + t * (nextA - prevA);
        uint8_t r = prevR + t * (nextR - prevR);
        uint8_t g = prevG + t * (nextG - prevG);
        uint8_t b = prevB + t * (nextB - prevB);
        
        return (a << 24) | (r << 16) | (g << 8) | b;
    }
    
    ContainerWidget::ContainerWidget(uint32_t color, int x, int y, int width, int height)
        : color_(color) {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
    }
    
    ContainerWidget::ContainerWidget(uint32_t color, int x, int y, int width, int height, 
                                   std::shared_ptr<Widget> child)
        : ContainerWidget(color, x, y, width, height) {
        child_ = child;
    }
    
    void ContainerWidget::render() {
        Draw::rect(x_, y_, width_, height_, color_);
        
        if (child_) {
            child_->render();
        }
    }
    
    bool ContainerWidget::handleInput(const InputState& input) {
        if (child_) {
            return child_->handleInput(input);
        }
        
        return false;
    }
    
    void ContainerWidget::setChild(std::shared_ptr<Widget> child) {
        child_ = child;
        
        if (child_) {
            child_->setPosition(x_, y_);
            
            if (child_->getWidth() <= 0) {
                child_->resize(width_, child_->getHeight());
            }
            if (child_->getHeight() <= 0) {
                child_->resize(child_->getWidth(), height_);
            }
        }
    }
    
    std::shared_ptr<ContainerWidget> Container(
        uint32_t color, int x, int y, int width, int height, 
        bool addToManager, 
        std::shared_ptr<Widget> child) {
        
        auto container = std::make_shared<ContainerWidget>(color, x, y, width, height);
        
        if (child) {
            container->setChild(child);
        }
        
        if (addToManager) {
            addWidget(container);
        }
        
        return container;
    }
}