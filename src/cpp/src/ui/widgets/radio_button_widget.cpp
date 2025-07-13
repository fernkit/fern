#include "../../../include/fern/ui/widgets/radio_button_widget.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <string>

extern Fern::Canvas* globalCanvas;

namespace Fern {
    RadioButtonWidget::RadioButtonWidget(const RadioButtonConfig& config)
        : config_(config), selected_(config.isSelected()) {
        setPosition(config.getX(), config.getY());
        
        // Calculate width and height based on text and radio button
        int textWidth = calculateTextWidth();
        int totalWidth = config_.getStyle().getRadius() * 2 + config_.getStyle().getSpacing() + textWidth;
        int totalHeight = std::max(config_.getStyle().getRadius() * 2, config_.getStyle().getFontSize() * 8);
        
        resize(totalWidth, totalHeight);
    }
    
    void RadioButtonWidget::render() {
        const auto& style = config_.getStyle();
        int radioX = x_ + style.getRadius();
        int radioY = y_ + style.getRadius();
        
        // Draw radio button background
        uint32_t bgColor = isHovered_ ? style.getHoverColor() : style.getBackgroundColor();
        Draw::circle(radioX, radioY, style.getRadius(), bgColor);
        
        // Draw border
        for (int i = 0; i < style.getBorderWidth(); ++i) {
            Draw::circle(radioX, radioY, style.getRadius() + i, style.getBorderColor());
        }
        
        // Draw selection indicator
        if (selected_) {
            int innerRadius = style.getRadius() - 3;
            if (innerRadius > 0) {
                Draw::circle(radioX, radioY, innerRadius, style.getSelectedColor());
            }
        }
        
        // Draw text
        renderText();
    }
    
    bool RadioButtonWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        
        // Check if mouse is over the radio button or text
        isHovered_ = (input.mouseX >= x_ && input.mouseX < x_ + width_ &&
                     input.mouseY >= y_ && input.mouseY < y_ + height_);
        
        // Handle click
        if (input.mouseClicked && isHovered_) {
            setSelected(true);
            return true;
        }
        
        return false;
    }
    
    void RadioButtonWidget::setSelected(bool selected) {
        if (selected_ != selected) {
            selected_ = selected;
            onSelectionChanged.emit(selected_);
            
            if (selected_) {
                onSelected.emit();
                
                // Notify group to unselect other buttons
                if (group_) {
                    group_->selectButton(std::shared_ptr<RadioButtonWidget>(this, [](RadioButtonWidget*){}));
                }
            }
        }
    }
    
    void RadioButtonWidget::setText(const std::string& text) {
        config_.text(text);
        
        // Recalculate size
        int textWidth = calculateTextWidth();
        int totalWidth = config_.getStyle().getRadius() * 2 + config_.getStyle().getSpacing() + textWidth;
        resize(totalWidth, height_);
    }
    
    void RadioButtonWidget::renderText() {
        const auto& style = config_.getStyle();
        if (config_.getText().empty()) return;
        
        int textX = x_ + style.getRadius() * 2 + style.getSpacing();
        int textY = y_ + (height_ - style.getFontSize() * 8) / 2;
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, config_.getText(), textX, textY, 
                           style.getFontSize(), style.getTextColor());
        } else {
            DrawText::drawText(config_.getText().c_str(), textX, textY, 
                             style.getFontSize(), style.getTextColor());
        }
    }
    
    bool RadioButtonWidget::isPointInRadioButton(int x, int y) const {
        const auto& style = config_.getStyle();
        int radioX = x_ + style.getRadius();
        int radioY = y_ + style.getRadius();
        
        int dx = x - radioX;
        int dy = y - radioY;
        return (dx * dx + dy * dy) <= (style.getRadius() * style.getRadius());
    }
    
    int RadioButtonWidget::calculateTextWidth() const {
        const auto& style = config_.getStyle();
        if (config_.getText().empty()) return 0;
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            return Font::getTextWidth(config_.getText(), style.getFontSize(), FontType::TTF);
        } else {
            // Bitmap font calculation
            int charWidth = style.getFontSize() * 6 / 8;
            return config_.getText().length() * charWidth;
        }
    }
    
    // Widget interface implementation
    int RadioButtonWidget::getWidth() const {
        return width_;
    }
    
    int RadioButtonWidget::getHeight() const {
        return height_;
    }
    
    void RadioButtonWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int RadioButtonWidget::getX() const {
        return config_.getX();
    }
    
    int RadioButtonWidget::getY() const {
        return config_.getY();
    }
    
    void RadioButtonWidget::resize(int width, int height) {
        width_ = width;
        height_ = height;
    }
    
    // RadioButtonGroup implementation
    void RadioButtonGroup::addButton(std::shared_ptr<RadioButtonWidget> button) {
        buttons_.push_back(button);
        button->setGroup(std::shared_ptr<RadioButtonGroup>(this, [](RadioButtonGroup*){}));
        
        // If this is the first button or it's already selected, select it
        if (buttons_.size() == 1 || button->isSelected()) {
            selectButton(button);
        }
    }
    
    void RadioButtonGroup::selectButton(std::shared_ptr<RadioButtonWidget> button) {
        // Unselect current selection
        if (selected_) {
            selected_->setSelected(false);
        }
        
        // Select new button
        selected_ = button;
        if (selected_) {
            selected_->setSelected(true);
        }
        
        onSelectionChanged.emit(selected_);
    }
    
    std::shared_ptr<RadioButtonWidget> RadioButtonGroup::getSelected() const {
        return selected_;
    }
    
    // Factory functions
    std::shared_ptr<RadioButtonWidget> RadioButton(const RadioButtonConfig& config, bool addToManager) {
        auto radioButton = std::make_shared<RadioButtonWidget>(config);
        
        if (addToManager) {
            addWidget(radioButton);
        }
        
        return radioButton;
    }
    
    std::shared_ptr<RadioButtonGroup> RadioGroup() {
        return std::make_shared<RadioButtonGroup>();
    }
    
    // Preset configurations
    namespace RadioButtonPresets {
        RadioButtonConfig Default(int x, int y, const std::string& text, const std::string& group) {
            return RadioButtonConfig(x, y, text, group);
        }
        
        RadioButtonConfig Modern(int x, int y, const std::string& text, const std::string& group) {
            return RadioButtonConfig(x, y, text, group)
                .style(RadioButtonStyle()
                    .backgroundColor(0xFFFFFFFF)
                    .borderColor(0xFF007BFF)
                    .selectedColor(0xFF007BFF)
                    .textColor(0xFF212529)
                    .hoverColor(0xFFF8F9FA)
                    .borderWidth(2)
                    .radius(10)
                    .spacing(12)
                    .fontSize(2));
        }
        
        RadioButtonConfig Compact(int x, int y, const std::string& text, const std::string& group) {
            return RadioButtonConfig(x, y, text, group)
                .style(RadioButtonStyle()
                    .backgroundColor(0xFFFFFFFF)
                    .borderColor(0xFF6C757D)
                    .selectedColor(0xFF28A745)
                    .textColor(0xFF495057)
                    .hoverColor(0xFFE9ECEF)
                    .borderWidth(1)
                    .radius(6)
                    .spacing(6)
                    .fontSize(1));
        }
    }
}
