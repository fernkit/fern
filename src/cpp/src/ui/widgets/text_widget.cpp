#include "../../../include/fern/ui/widgets/text_widget.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/graphics/colors.hpp"
#include <iostream>

extern Fern::Canvas* globalCanvas;

namespace Fern {

    // Modern constructor with configuration
    TextWidget::TextWidget(const TextConfig& config)
        : Widget(),
        text_(config.getText()), 
        size_(config.getStyle().getFontSize()), 
        color_(config.getStyle().getColor()),
        fontType_(config.getStyle().getFontType()),
        config_(config) {
        
        setPosition(config.getX(), config.getY());
        resize(0, 0);  
        updateDimensions();
    }

    // Legacy constructor for backward compatibility
    TextWidget::TextWidget(Point position, const std::string& text, int size, uint32_t color, 
                        FontType fontType)
        : Widget(),
        text_(text), size_(size), color_(color), fontType_(fontType),
        config_(TextConfig(position.x, position.y, text)
                .style(TextStyle().color(color).fontSize(size).useBitmapFont())) {
        
        setPosition(position.x, position.y);
        resize(0, 0);  
        updateDimensions();
    }
        
    void TextWidget::render() {
        renderBackground();
        renderShadow();
        renderMainText();
    }
    
    void TextWidget::renderBackground() {
        if (config_.getStyle().hasBackground()) {
            int padding = config_.getStyle().getPadding();
            Draw::rect(x_ - padding, y_ - padding, 
                      width_ + 2 * padding, height_ + 2 * padding,
                      config_.getStyle().getBackgroundColor());
        }
    }
    
    void TextWidget::renderShadow() {
        if (config_.getStyle().hasShadow()) {
            int offset = config_.getStyle().getShadowOffset();
            switch (fontType_) {
                case FontType::TTF:
                    if (Font::hasTTFFont()) {
                        Font::renderTTF(globalCanvas, text_, x_ + offset, y_ + offset, 
                                       size_, config_.getStyle().getShadowColor());
                    } else {
                        DrawText::drawText(text_.c_str(), x_ + offset, y_ + offset, 
                                         size_, config_.getStyle().getShadowColor());
                    }
                    break;
                case FontType::Bitmap:
                default:
                    DrawText::drawText(text_.c_str(), x_ + offset, y_ + offset, 
                                     size_, config_.getStyle().getShadowColor());
                    break;
            }
        }
    }
    
    void TextWidget::renderMainText() {
        switch (fontType_) {
            case FontType::TTF:
                renderTTF();
                break;
            case FontType::Bitmap:
            default:
                renderBitmap();
                break;
        }
    }
    
    void TextWidget::renderTTF() {
        if (Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, text_, x_, y_, size_, color_);
        } else {
            renderBitmap();
        }
    }
    
    void TextWidget::renderBitmap() {
        DrawText::drawText(text_.c_str(), x_, y_, size_, color_);
    }
    
    void TextWidget::setText(const std::string& text) {
        text_ = text;
        updateDimensions();
    }
    
    void TextWidget::setSize(int size) {
        size_ = size;
        updateDimensions();
    }
    
    void TextWidget::setColor(uint32_t color) {
        color_ = color;
    }
    
    void TextWidget::setConfig(const TextConfig& config) {
        config_ = config;
        text_ = config.getText();
        size_ = config.getStyle().getFontSize();
        color_ = config.getStyle().getColor();
        fontType_ = config.getStyle().getFontType();
        setPosition(config.getX(), config.getY());
        updateDimensions();
    }
    
    void TextWidget::updateDimensions() {
        int textWidth, textHeight;
        
        // Calculate dimensions based on font type
        if (fontType_ == FontType::TTF && Font::hasTTFFont()) {
            textWidth = Font::getTextWidth(text_, size_, FontType::TTF);
            textHeight = Font::getTextHeight(size_, FontType::TTF);
        } else {
            // Use bitmap font calculation that matches the actual drawing
            textWidth = 0;
            for (char c : text_) {
                if (c == ' ') {
                    textWidth += 4 * size_;  // Spaces are 4 * scale pixels
                } else {
                    textWidth += 8 * size_;  // Characters are 8 * scale pixels
                }
            }
            textHeight = 8 * size_;  // Bitmap font height is 8 * scale pixels
        }
        
        resize(textWidth, textHeight);
    }
    
    // Modern factory function with configuration
    std::shared_ptr<TextWidget> Text(const TextConfig& config, bool addToManager) {
        auto widget = std::make_shared<TextWidget>(config);
        
        if (addToManager) {
            addWidget(widget);
        }
        
        return widget;
    }
    
    // Legacy factory function for backward compatibility
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                     int size, uint32_t color, bool addToManager,
                                     FontType fontType) {
        auto widget = std::make_shared<TextWidget>(position, text, size, color, fontType);
        
        if (addToManager) {
            addWidget(widget);
        }
        
        return widget;
    }
    
    // Preset configurations
    namespace TextPresets {
        TextConfig Title(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(4)
                    .color(Colors::White)
                    .useBitmapFont());
        }
        
        TextConfig Subtitle(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(3)
                    .color(Colors::LightGray)
                    .useBitmapFont());
        }
        
        TextConfig Body(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(2)
                    .color(Colors::White)
                    .useBitmapFont());
        }
        
        TextConfig Caption(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(1)
                    .color(Colors::Gray)
                    .useBitmapFont());
        }
        
        TextConfig Button(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(2)
                    .color(Colors::White)
                    .backgroundColor(Colors::Blue)
                    .padding(8)
                    .useBitmapFont());
        }
        
        TextConfig Error(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(2)
                    .color(Colors::Red)
                    .useBitmapFont());
        }
        
        TextConfig Success(int x, int y, const std::string& text) {
            return TextConfig(x, y, text)
                .style(TextStyle()
                    .fontSize(2)
                    .color(Colors::Green)
                    .useBitmapFont());
        }
    }
}