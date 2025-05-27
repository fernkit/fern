#include "../../include/fern/ui/widgets/button_widget.hpp"
#include "../../include/fern/core/input.hpp"
#include "../../include/fern/core/widget_manager.hpp"
#include "../../include/fern/graphics/primitives.hpp"
#include "../../include/fern/text/font.hpp"
#include <cstring>
#include <memory>
namespace Fern {
    ButtonWidget::ButtonWidget(const ButtonConfig& config)
        : config_(config) {}
    
    void ButtonWidget::render() {
        uint32_t buttonColor = config_.normalColor;
        if (isHovered_) {
            buttonColor = isPressed_ ? config_.pressColor : config_.hoverColor;
        }
        
        Draw::rect(x_, y_, config_.width, config_.height, buttonColor);
        
        if (!config_.label.empty()) {
            int textWidth = config_.label.length() * 8 * config_.textScale;
            int textX = x_ + (config_.width - textWidth) / 2;
            int textY = y_ + (config_.height - 8 * config_.textScale) / 2;
            
            DrawText::drawText(config_.label.c_str(), textX, textY, config_.textScale, config_.textColor);
        }
    }       
    
    const auto& input = Input::getState();

    
    bool ButtonWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        bool wasPressed = isPressed_;

        isHovered_ = input.mouseX >= x_ && 
                    input.mouseX < x_ + config_.width &&
                    input.mouseY >= y_ &&
                    input.mouseY < y_ + config_.height;
        
        isPressed_ = isHovered_ && input.mouseDown;
        
        if (wasHovered != isHovered_) {
            onHover.emit(isHovered_);
        }

        if (wasPressed != isPressed_) {
            onPress.emit(isPressed_);
        }

        if (isHovered_ && input.mouseClicked) {
            onClick.emit();
            return true;
        }
        
        return false;
    }
    
    std::shared_ptr<ButtonWidget> Button(const ButtonConfig& config, bool addToManager) {
        auto button = std::make_shared<ButtonWidget>(config);
        if (config.onClick) {
            button->onClick.connect(config.onClick);
        }
        if (addToManager) {
            addWidget(button);
        }
        
        return button;
    }

    int ButtonWidget::getWidth() const {
        return config_.width;
    }

    int ButtonWidget::getHeight() const {
        return config_.height;
    }

    void ButtonWidget::setPosition(int x, int y) {
        x_ = x;        
        y_ = y; 
        config_.x = x;
        config_.y = y;
    }

    int ButtonWidget::getX() const {
        return config_.x;
    }

    int ButtonWidget::getY() const {
        return config_.y;
    }

    void ButtonWidget::resize(int width, int height) {
        config_.width = width;
        config_.height = height;
    }
}