#include "../../../include/fern/ui/widgets/slider_widget.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include <algorithm>
#include <cmath>
#include <string>

namespace Fern {
    SliderWidget::SliderWidget(const SliderConfig& config)
        : config_(config), currentValue_(config.getInitialValue()) {
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
        clampValue();
    }
    
    void SliderWidget::render() {
        const auto& style = config_.getStyle();
        const int trackHeight = config_.getHeight() / 3;
        const int trackY = y_ + (config_.getHeight() - trackHeight) / 2;
        
        // Draw track background
        Draw::rect(x_, trackY, config_.getWidth(), trackHeight, style.getTrackColor());
        
        // Draw filled portion
        int fillWidth = valueToScreen(currentValue_) - x_;
        if (fillWidth > 0) {
            Draw::rect(x_, trackY, fillWidth, trackHeight, style.getFillColor());
        }
        
        // Draw thumb
        int thumbX = valueToScreen(currentValue_);
        int thumbY = y_ + config_.getHeight() / 2;
        uint32_t thumbColor = isThumbHovered_ ? style.getThumbHoverColor() : style.getThumbColor();
        Draw::circle(thumbX, thumbY, style.getThumbRadius(), thumbColor);
        
        // Draw value text if enabled
        if (style.getShowValue()) {
            std::string valueText = std::to_string(static_cast<int>(currentValue_));
            int textX = x_ + config_.getWidth() + 10;
            int textY = y_ + (config_.getHeight() - 8 * style.getTextScale()) / 2;
            DrawText::drawText(valueText.c_str(), textX, textY, style.getTextScale(), style.getTextColor());
        }
    }
    
    bool SliderWidget::handleInput(const InputState& input) {
        bool wasHovered = isThumbHovered_;
        bool wasDragging = isDragging_;
        
        // Check if mouse is over the thumb
        isThumbHovered_ = isPointInThumb(input.mouseX, input.mouseY);
        
        // Handle mouse click to start dragging
        if (input.mouseClicked) {
            if (isThumbHovered_) {
                isDragging_ = true;
            } else {
                // Click on track - jump to that position
                if (input.mouseX >= x_ && input.mouseX <= x_ + config_.getWidth() &&
                    input.mouseY >= y_ && input.mouseY <= y_ + config_.getHeight()) {
                    float newValue = screenToValue(input.mouseX);
                    setValue(newValue);
                    return true;
                }
            }
        }
        
        // Handle dragging
        if (isDragging_) {
            if (input.mouseDown) {
                float newValue = screenToValue(input.mouseX);
                setValue(newValue);
            } else {
                isDragging_ = false;
            }
        }
        
        // Emit signals for state changes
        if (wasDragging != isDragging_) {
            onDragging.emit(isDragging_);
        }
        
        // Return true if we're handling input (dragging or clicked in our area)
        return isDragging_ || (input.mouseClicked && 
                              input.mouseX >= x_ && input.mouseX <= x_ + config_.getWidth() &&
                              input.mouseY >= y_ && input.mouseY <= y_ + config_.getHeight());
    }
    
    void SliderWidget::setValue(float value) {
        float oldValue = currentValue_;
        currentValue_ = value;
        clampValue();
        
        if (oldValue != currentValue_) {
            onValueChanged.emit(currentValue_);
        }
    }
    
    void SliderWidget::setRange(float minValue, float maxValue) {
        config_.setPosition(config_.getX(), config_.getY()); // Preserve position
        config_.setSize(config_.getWidth(), config_.getHeight()); // Preserve size
        // Note: We can't directly modify the range in the config, so we'll store it internally
        // This is a limitation of the current design that could be improved
        clampValue();
    }
    
    float SliderWidget::screenToValue(int screenX) const {
        // Convert screen position to value
        int relativeX = screenX - x_;
        relativeX = std::max(0, std::min(relativeX, config_.getWidth()));
        
        float ratio = static_cast<float>(relativeX) / static_cast<float>(config_.getWidth());
        return config_.getMinValue() + ratio * (config_.getMaxValue() - config_.getMinValue());
    }
    
    int SliderWidget::valueToScreen(float value) const {
        // Convert value to screen position
        float ratio = (value - config_.getMinValue()) / (config_.getMaxValue() - config_.getMinValue());
        ratio = std::max(0.0f, std::min(1.0f, ratio));
        return x_ + static_cast<int>(ratio * config_.getWidth());
    }
    
    bool SliderWidget::isPointInThumb(int x, int y) const {
        int thumbX = valueToScreen(currentValue_);
        int thumbY = y_ + config_.getHeight() / 2;
        
        int dx = x - thumbX;
        int dy = y - thumbY;
        return (dx * dx + dy * dy) <= (config_.getStyle().getThumbRadius() * config_.getStyle().getThumbRadius());
    }
    
    void SliderWidget::clampValue() {
        currentValue_ = std::max(config_.getMinValue(), std::min(currentValue_, config_.getMaxValue()));
    }
    
    // Widget interface implementation
    int SliderWidget::getWidth() const {
        const auto& style = config_.getStyle();
        return config_.getWidth() + (style.getShowValue() ? 50 : 0); // Extra space for text
    }
    
    int SliderWidget::getHeight() const {
        return config_.getHeight();
    }
    
    void SliderWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int SliderWidget::getX() const {
        return config_.getX();
    }
    
    int SliderWidget::getY() const {
        return config_.getY();
    }
    
    void SliderWidget::resize(int width, int height) {
        config_.setSize(width, height);
    }
    
    // Factory function
    std::shared_ptr<SliderWidget> Slider(const SliderConfig& config, bool addToManager) {
        auto slider = std::make_shared<SliderWidget>(config);
        
        if (addToManager) {
            addWidget(slider);
        }
        
        return slider;
    }
    
    // Preset configurations
    namespace SliderPresets {
        SliderConfig Default(int x, int y, int width, int height) {
            return SliderConfig(x, y, width, height);
        }
        
        SliderConfig Volume(int x, int y, int width, int height) {
            return SliderConfig(x, y, width, height)
                .range(0.0f, 100.0f)
                .initialValue(50.0f)
                .style(SliderStyle()
                    .trackColor(0xFF606060)
                    .fillColor(0xFF00AA00)
                    .thumbColor(0xFFFFFFFF)
                    .thumbHoverColor(0xFFE0E0E0)
                    .thumbRadius(12)
                    .showValue(false));
        }
        
        SliderConfig Brightness(int x, int y, int width, int height) {
            return SliderConfig(x, y, width, height)
                .range(0.0f, 100.0f)
                .initialValue(75.0f)
                .style(SliderStyle()
                    .trackColor(0xFF404040)
                    .fillColor(0xFFFFDD00)
                    .thumbColor(0xFFFFA500)
                    .thumbHoverColor(0xFFFFBF00)
                    .thumbRadius(10)
                    .showValue(false));
        }
        
        SliderConfig ColorComponent(int x, int y, int width, int height) {
            return SliderConfig(x, y, width, height)
                .range(0.0f, 255.0f)
                .initialValue(128.0f)
                .style(SliderStyle()
                    .trackColor(0xFF303030)
                    .fillColor(0xFF0080FF)
                    .thumbColor(0xFFFFFFFF)
                    .thumbHoverColor(0xFFCCCCCC)
                    .thumbRadius(8)
                    .showValue(true)
                    .textScale(1));
        }
    }
}
