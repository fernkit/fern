#include "../../../include/fern/ui/widgets/progress_bar_widget.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <string>

extern Fern::Canvas* globalCanvas;

namespace Fern {
    ProgressBarWidget::ProgressBarWidget(const ProgressBarConfig& config)
        : config_(config), currentValue_(config.getCurrentValue()) {
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
        clampValue();
    }
    
    void ProgressBarWidget::render() {
        const auto& style = config_.getStyle();
        
        // Draw border if enabled
        if (style.getBorderWidth() > 0) {
            for (int i = 0; i < style.getBorderWidth(); ++i) {
                Draw::rect(x_ - i, y_ - i, 
                          config_.getWidth() + 2 * i, 
                          config_.getHeight() + 2 * i, 
                          style.getBorderColor());
            }
        }
        
        // Draw background
        Draw::rect(x_, y_, config_.getWidth(), config_.getHeight(), style.getBackgroundColor());
        
        // Draw fill based on current value
        float percentage = getPercentage();
        int fillWidth = static_cast<int>((percentage / 100.0f) * config_.getWidth());
        
        if (fillWidth > 0) {
            Draw::rect(x_, y_, fillWidth, config_.getHeight(), style.getFillColor());
        }
        
        // Draw percentage text if enabled
        if (style.getShowPercentage()) {
            renderText();
        }
    }
    
    bool ProgressBarWidget::handleInput(const InputState& input) {
        // Progress bars are typically not interactive
        return false;
    }
    
    void ProgressBarWidget::setValue(float value) {
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
    
    void ProgressBarWidget::setRange(float minValue, float maxValue) {
        config_.setPosition(config_.getX(), config_.getY());
        config_.setSize(config_.getWidth(), config_.getHeight());
        clampValue();
    }
    
    float ProgressBarWidget::getPercentage() const {
        if (config_.getMaxValue() == config_.getMinValue()) {
            return 0.0f;
        }
        return ((currentValue_ - config_.getMinValue()) / 
                (config_.getMaxValue() - config_.getMinValue())) * 100.0f;
    }
    
    void ProgressBarWidget::clampValue() {
        currentValue_ = std::max(config_.getMinValue(), 
                                std::min(currentValue_, config_.getMaxValue()));
    }
    
    void ProgressBarWidget::renderText() {
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
        
        int textX = x_ + (config_.getWidth() - textWidth) / 2;
        int textY = y_ + (config_.getHeight() - textHeight) / 2;
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, percentageText, textX, textY, 
                           style.getFontSize(), style.getTextColor());
        } else {
            DrawText::drawText(percentageText.c_str(), textX, textY, 
                             style.getFontSize(), style.getTextColor());
        }
    }
    
    // Widget interface implementation
    int ProgressBarWidget::getWidth() const {
        return config_.getWidth() + 2 * config_.getStyle().getBorderWidth();
    }
    
    int ProgressBarWidget::getHeight() const {
        return config_.getHeight() + 2 * config_.getStyle().getBorderWidth();
    }
    
    void ProgressBarWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int ProgressBarWidget::getX() const {
        return config_.getX();
    }
    
    int ProgressBarWidget::getY() const {
        return config_.getY();
    }
    
    void ProgressBarWidget::resize(int width, int height) {
        config_.setSize(width, height);
    }
    
    // Factory function
    std::shared_ptr<ProgressBarWidget> ProgressBar(const ProgressBarConfig& config, bool addToManager) {
        auto progressBar = std::make_shared<ProgressBarWidget>(config);
        
        if (addToManager) {
            addWidget(progressBar);
        }
        
        return progressBar;
    }
    
    // Preset configurations
    namespace ProgressBarPresets {
        ProgressBarConfig Default(int x, int y, int width, int height) {
            return ProgressBarConfig(x, y, width, height);
        }
        
        ProgressBarConfig Loading(int x, int y, int width, int height) {
            return ProgressBarConfig(x, y, width, height)
                .range(0.0f, 100.0f)
                .value(0.0f)
                .style(ProgressBarStyle()
                    .backgroundColor(0xFF333333)
                    .fillColor(0xFF007BFF)
                    .borderColor(0xFF666666)
                    .textColor(0xFFFFFFFF)
                    .borderWidth(1)
                    .showPercentage(true));
        }
        
        ProgressBarConfig Health(int x, int y, int width, int height) {
            return ProgressBarConfig(x, y, width, height)
                .range(0.0f, 100.0f)
                .value(100.0f)
                .style(ProgressBarStyle()
                    .backgroundColor(0xFF602020)
                    .fillColor(0xFF00CC00)
                    .borderColor(0xFF404040)
                    .textColor(0xFFFFFFFF)
                    .borderWidth(2)
                    .showPercentage(false));
        }
        
        ProgressBarConfig Download(int x, int y, int width, int height) {
            return ProgressBarConfig(x, y, width, height)
                .range(0.0f, 100.0f)
                .value(0.0f)
                .style(ProgressBarStyle()
                    .backgroundColor(0xFFE9ECEF)
                    .fillColor(0xFF28A745)
                    .borderColor(0xFF6C757D)
                    .textColor(0xFF212529)
                    .borderWidth(1)
                    .showPercentage(true)
                    .fontSize(2));
        }
    }
}
