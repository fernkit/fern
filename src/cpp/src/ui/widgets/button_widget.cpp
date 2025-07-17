#include "../../../include/fern/ui/widgets/button_widget.hpp"
#include "../../../include/fern/core/input.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include <cstring>
#include <memory>
namespace Fern {
    ButtonWidget::ButtonWidget(const ButtonConfig& config)
        : config_(config) {
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
    }
    
    void ButtonWidget::render() {
        renderBackground();
        renderBorder();
        renderText();
    }
    
    void ButtonWidget::renderBackground() {
        uint32_t buttonColor = config_.getStyle().getNormalColor();
        if (isHovered_) {
            buttonColor = isPressed_ ? config_.getStyle().getPressColor() : config_.getStyle().getHoverColor();
        }
        
        int borderRadius = config_.getStyle().getBorderRadius();
        if (borderRadius > 0) {
            Draw::roundedRect(x_, y_, config_.getWidth(), config_.getHeight(), borderRadius, buttonColor);
        } else {
            Draw::rect(x_, y_, config_.getWidth(), config_.getHeight(), buttonColor);
        }
    }
    
    void ButtonWidget::renderBorder() {
        if (config_.getStyle().getBorderWidth() > 0) {
            int borderWidth = config_.getStyle().getBorderWidth();
            uint32_t borderColor = config_.getStyle().getBorderColor();
            int borderRadius = config_.getStyle().getBorderRadius();
            
            if (borderRadius > 0) {
                // Use rounded rectangle border
                Draw::roundedRectBorder(x_ - borderWidth, y_ - borderWidth, 
                                      config_.getWidth() + 2 * borderWidth, 
                                      config_.getHeight() + 2 * borderWidth, 
                                      borderRadius + borderWidth, borderWidth, borderColor);
            } else {
                // Draw border as 4 lines (since we don't have rectOutline)
                Draw::line(x_ - borderWidth, y_ - borderWidth, x_ + config_.getWidth() + borderWidth, y_ - borderWidth, borderWidth, borderColor); // Top
                Draw::line(x_ - borderWidth, y_ + config_.getHeight() + borderWidth, x_ + config_.getWidth() + borderWidth, y_ + config_.getHeight() + borderWidth, borderWidth, borderColor); // Bottom
                Draw::line(x_ - borderWidth, y_ - borderWidth, x_ - borderWidth, y_ + config_.getHeight() + borderWidth, borderWidth, borderColor); // Left
                Draw::line(x_ + config_.getWidth() + borderWidth, y_ - borderWidth, x_ + config_.getWidth() + borderWidth, y_ + config_.getHeight() + borderWidth, borderWidth, borderColor); // Right
            }
        }
    }
    
    void ButtonWidget::renderText() {
        if (!config_.getLabel().empty()) {
            int textScale = config_.getStyle().getTextScale();
            int textWidth = config_.getLabel().length() * 8 * textScale;
            int textX = x_ + (config_.getWidth() - textWidth) / 2;
            int textY = y_ + (config_.getHeight() - 8 * textScale) / 2;
            
            DrawText::drawText(config_.getLabel().c_str(), textX, textY, textScale, config_.getStyle().getTextColor());
        }
    }
    
    bool ButtonWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        bool wasPressed = isPressed_;

        isHovered_ = input.mouseX >= x_ && 
                    input.mouseX < x_ + config_.getWidth() &&
                    input.mouseY >= y_ &&
                    input.mouseY < y_ + config_.getHeight();
        
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
        if (addToManager) {
            addWidget(button);
        }
        
        return button;
    }

    int ButtonWidget::getWidth() const {
        return config_.getWidth();
    }

    int ButtonWidget::getHeight() const {
        return config_.getHeight();
    }

    void ButtonWidget::setPosition(int x, int y) {
        x_ = x;        
        y_ = y; 
        config_.setPosition(x, y);
    }

    int ButtonWidget::getX() const {
        return config_.getX();
    }

    int ButtonWidget::getY() const {
        return config_.getY();
    }

    void ButtonWidget::resize(int width, int height) {
        config_.setSize(width, height);
    }
    
    void ButtonWidget::setConfig(const ButtonConfig& config) {
        config_ = config;
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
    }
    
    void ButtonWidget::setLabel(const std::string& label) {
        config_.label(label);
    }
    
    void ButtonWidget::autoSizeToContent(int padding) {
        if (!config_.getLabel().empty()) {
            int textWidth = calculateTextWidth(config_.getLabel(), config_.getStyle().getTextScale());
            int textHeight = calculateTextHeight(config_.getStyle().getTextScale());
            
            int newWidth = textWidth + padding * 2;
            int newHeight = textHeight + padding;
            
            config_.setSize(newWidth, newHeight);
        }
    }
    
    int ButtonWidget::calculateTextWidth(const std::string& text, int textScale) {
        // Each character is 8 pixels wide in bitmap font
        return text.length() * 8 * textScale;
    }
    
    int ButtonWidget::calculateTextHeight(int textScale) {
        // Each character is 8 pixels tall in bitmap font
        return 8 * textScale;
    }
    
    // Preset configurations
    namespace ButtonPresets {
        ButtonConfig Primary(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFF007BFF)    // Bootstrap primary blue
                    .hoverColor(0xFF0056B3)     // Darker blue on hover
                    .pressColor(0xFF004085)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
        
        ButtonConfig Secondary(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFF6C757D)    // Bootstrap secondary gray
                    .hoverColor(0xFF545B62)     // Darker gray on hover
                    .pressColor(0xFF494F54)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
        
        ButtonConfig Success(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFF28A745)    // Bootstrap success green
                    .hoverColor(0xFF1E7E34)     // Darker green on hover
                    .pressColor(0xFF155724)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
        
        ButtonConfig Danger(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFFDC3545)    // Bootstrap danger red
                    .hoverColor(0xFFC82333)     // Darker red on hover
                    .pressColor(0xFFBD2130)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
        
        ButtonConfig Warning(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFFFFC107)    // Bootstrap warning yellow
                    .hoverColor(0xFFE0A800)     // Darker yellow on hover
                    .pressColor(0xFFD39E00)     // Even darker on press
                    .textColor(0xFF212529)      // Dark text for contrast
                    .textScale(2));
        }
        
        ButtonConfig Info(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFF17A2B8)    // Bootstrap info cyan
                    .hoverColor(0xFF138496)     // Darker cyan on hover
                    .pressColor(0xFF117A8B)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
        
        ButtonConfig Light(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFFF8F9FA)    // Bootstrap light gray
                    .hoverColor(0xFFE2E6EA)     // Darker gray on hover
                    .pressColor(0xFFDAE0E5)     // Even darker on press
                    .textColor(0xFF212529)      // Dark text for contrast
                    .textScale(2)
                    .border(1, 0xFFDEE2E6));    // Light border
        }
        
        ButtonConfig Dark(int x, int y, int width, int height, const std::string& label) {
            return ButtonConfig(x, y, width, height, label)
                .style(ButtonStyle()
                    .normalColor(0xFF343A40)    // Bootstrap dark gray
                    .hoverColor(0xFF23272B)     // Darker gray on hover
                    .pressColor(0xFF1D2124)     // Even darker on press
                    .textColor(0xFFFFFFFF)      // White text
                    .textScale(2));
        }
    }
}