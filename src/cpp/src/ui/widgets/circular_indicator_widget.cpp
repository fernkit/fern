#include "../../../include/fern/ui/widgets/circular_indicator_widget.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <cmath>
#include <string>

extern Fern::Canvas* globalCanvas;

namespace Fern {
    CircularIndicatorWidget::CircularIndicatorWidget(const CircularIndicatorConfig& config)
        : config_(config), currentValue_(config.getCurrentValue()) {
        setPosition(config.getX(), config.getY());
        resize(config.getRadius() * 2, config.getRadius() * 2);
        clampValue();
    }
    
    void CircularIndicatorWidget::render() {
        const auto& style = config_.getStyle();
        int centerX = x_ + config_.getRadius();
        int centerY = y_ + config_.getRadius();
        
        // Draw background circle
        Draw::circle(centerX, centerY, config_.getRadius(), style.getBackgroundColor());
        
        // Draw border if enabled
        if (style.getBorderWidth() > 0) {
            for (int i = 0; i < style.getBorderWidth(); ++i) {
                // Draw border as concentric circles (simplified)
                Draw::circle(centerX, centerY, config_.getRadius() + i, style.getBorderColor());
            }
        }
        
        // Draw progress arc
        float percentage = getPercentage();
        if (percentage > 0.0f) {
            float endAngle = style.getStartAngle() + (percentage / 100.0f) * 360.0f;
            if (!style.isClockwise()) {
                endAngle = style.getStartAngle() - (percentage / 100.0f) * 360.0f;
            }
            renderArc(centerX, centerY, config_.getRadius() - style.getThickness() / 2, 
                     style.getStartAngle(), endAngle, style.getFillColor());
        }
        
        // Draw percentage text if enabled
        if (style.getShowPercentage()) {
            renderText();
        }
    }
    
    bool CircularIndicatorWidget::handleInput(const InputState& input) {
        // Circular indicators are typically not interactive
        return false;
    }
    
    void CircularIndicatorWidget::setValue(float value) {
        float oldValue = currentValue_;
        currentValue_ = value;
        clampValue();
        
        if (oldValue != currentValue_) {
            onValueChanged.emit(currentValue_);
            
            // Check if we reached 100%
            if (getPercentage() >= 100.0f && oldValue < config_.getMaxValue()) {
                onComplete.emit();
            }
        }
    }
    
    void CircularIndicatorWidget::setRange(float minValue, float maxValue) {
        config_.setPosition(config_.getX(), config_.getY());
        config_.setRadius(config_.getRadius());
        clampValue();
    }
    
    float CircularIndicatorWidget::getPercentage() const {
        if (config_.getMaxValue() == config_.getMinValue()) {
            return 0.0f;
        }
        return ((currentValue_ - config_.getMinValue()) / 
                (config_.getMaxValue() - config_.getMinValue())) * 100.0f;
    }
    
    void CircularIndicatorWidget::clampValue() {
        currentValue_ = std::max(config_.getMinValue(), 
                                std::min(currentValue_, config_.getMaxValue()));
    }
    
    void CircularIndicatorWidget::renderArc(int centerX, int centerY, int radius, 
                                           float startAngle, float endAngle, uint32_t color) {
        // Simplified arc rendering using lines
        const float step = 2.0f; // Degree step for smoother arc
        float currentAngle = startAngle;
        
        while (currentAngle <= endAngle) {
            float radians = currentAngle * M_PI / 180.0f;
            int x1 = centerX + static_cast<int>((radius - config_.getStyle().getThickness() / 2) * cos(radians));
            int y1 = centerY + static_cast<int>((radius - config_.getStyle().getThickness() / 2) * sin(radians));
            int x2 = centerX + static_cast<int>((radius + config_.getStyle().getThickness() / 2) * cos(radians));
            int y2 = centerY + static_cast<int>((radius + config_.getStyle().getThickness() / 2) * sin(radians));
            
            Draw::line(x1, y1, x2, y2, 2, color);
            currentAngle += step;
        }
    }
    
    void CircularIndicatorWidget::renderText() {
        const auto& style = config_.getStyle();
        std::string percentageText = std::to_string(static_cast<int>(getPercentage())) + "%";
        
        // Calculate text position (centered)
        int textWidth, textHeight;
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            textWidth = Font::getTextWidth(percentageText, style.getFontSize(), FontType::TTF);
            textHeight = Font::getTextHeight(style.getFontSize(), FontType::TTF);
        } else {
            // Bitmap font calculation
            int charWidth = style.getFontSize() * 6 / 8;
            textWidth = percentageText.length() * charWidth;
            textHeight = style.getFontSize();
        }
        
        int textX = x_ + config_.getRadius() - textWidth / 2;
        int textY = y_ + config_.getRadius() - textHeight / 2;
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, percentageText, textX, textY, 
                           style.getFontSize(), style.getTextColor());
        } else {
            DrawText::drawText(percentageText.c_str(), textX, textY, 
                             style.getFontSize(), style.getTextColor());
        }
    }
    
    // Widget interface implementation
    int CircularIndicatorWidget::getWidth() const {
        return config_.getRadius() * 2 + 2 * config_.getStyle().getBorderWidth();
    }
    
    int CircularIndicatorWidget::getHeight() const {
        return config_.getRadius() * 2 + 2 * config_.getStyle().getBorderWidth();
    }
    
    void CircularIndicatorWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int CircularIndicatorWidget::getX() const {
        return config_.getX();
    }
    
    int CircularIndicatorWidget::getY() const {
        return config_.getY();
    }
    
    void CircularIndicatorWidget::resize(int width, int height) {
        // For circular widgets, use the smaller dimension as radius
        int newRadius = std::min(width, height) / 2;
        config_.setRadius(newRadius);
    }
    
    // Factory function
    std::shared_ptr<CircularIndicatorWidget> CircularIndicator(const CircularIndicatorConfig& config, bool addToManager) {
        auto indicator = std::make_shared<CircularIndicatorWidget>(config);
        
        if (addToManager) {
            addWidget(indicator);
        }
        
        return indicator;
    }
    
    // Preset configurations
    namespace CircularIndicatorPresets {
        CircularIndicatorConfig Default(int x, int y, int radius) {
            return CircularIndicatorConfig(x, y, radius);
        }
        
        CircularIndicatorConfig Loading(int x, int y, int radius) {
            return CircularIndicatorConfig(x, y, radius)
                .range(0.0f, 100.0f)
                .value(0.0f)
                .style(CircularIndicatorStyle()
                    .backgroundColor(0xFF333333)
                    .fillColor(0xFF007BFF)
                    .borderColor(0xFF666666)
                    .textColor(0xFFFFFFFF)
                    .borderWidth(1)
                    .thickness(8)
                    .showPercentage(true)
                    .clockwise(true)
                    .startAngle(-90.0f)); // Start from top
        }
        
        CircularIndicatorConfig Health(int x, int y, int radius) {
            return CircularIndicatorConfig(x, y, radius)
                .range(0.0f, 100.0f)
                .value(100.0f)
                .style(CircularIndicatorStyle()
                    .backgroundColor(0xFF602020)
                    .fillColor(0xFF00CC00)
                    .borderColor(0xFF404040)
                    .textColor(0xFFFFFFFF)
                    .borderWidth(2)
                    .thickness(6)
                    .showPercentage(false)
                    .clockwise(true)
                    .startAngle(-90.0f));
        }
        
        CircularIndicatorConfig Battery(int x, int y, int radius) {
            return CircularIndicatorConfig(x, y, radius)
                .range(0.0f, 100.0f)
                .value(75.0f)
                .style(CircularIndicatorStyle()
                    .backgroundColor(0xFF2C2C2C)
                    .fillColor(0xFF28A745)
                    .borderColor(0xFF555555)
                    .textColor(0xFFFFFFFF)
                    .borderWidth(1)
                    .thickness(4)
                    .showPercentage(true)
                    .clockwise(true)
                    .startAngle(-90.0f)
                    .fontSize(1));
        }
    }
}
