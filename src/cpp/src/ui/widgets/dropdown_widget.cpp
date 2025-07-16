#include "../../../include/fern/ui/widgets/dropdown_widget.hpp"
#include "../../../include/fern/core/widget_manager.hpp"
#include "../../../include/fern/graphics/primitives.hpp"
#include "../../../include/fern/text/font.hpp"
#include "../../../include/fern/font/font.hpp"
#include <algorithm>
#include <string>

extern Fern::Canvas* globalCanvas;

namespace Fern {
    DropdownWidget::DropdownWidget(const DropdownConfig& config)
        : config_(config), selectedIndex_(config.getSelectedIndex()) {
        setPosition(config.getX(), config.getY());
        resize(config.getWidth(), config.getHeight());
    }
    
    void DropdownWidget::render() {
        renderMainButton();
        
        if (isOpen_) {
            renderDropdown();
        }
    }
    
    bool DropdownWidget::handleInput(const InputState& input) {
        bool wasHovered = isHovered_;
        
        // Check if mouse is over main button
        isHovered_ = isPointInMainButton(input.mouseX, input.mouseY);
        
        if (input.mouseClicked) {
            if (isHovered_) {
                // Toggle dropdown
                if (isOpen_) {
                    close();
                } else {
                    open();
                }
                return true;
            } else if (isOpen_) {
                // Check if clicked in dropdown
                if (isPointInDropdown(input.mouseX, input.mouseY)) {
                    int itemIndex = getItemIndexAtPoint(input.mouseX, input.mouseY);
                    if (itemIndex >= 0 && itemIndex < static_cast<int>(config_.getItems().size())) {
                        setSelectedIndex(itemIndex);
                        close();
                        return true;
                    }
                } else {
                    // Clicked outside - close dropdown
                    close();
                    return false; // Don't consume the click so other widgets can handle it
                }
            }
        }
        
        // Handle hover for dropdown items
        if (isOpen_) {
            int newHoveredIndex = -1;
            if (isPointInDropdown(input.mouseX, input.mouseY)) {
                newHoveredIndex = getItemIndexAtPoint(input.mouseX, input.mouseY);
            }
            hoveredItemIndex_ = newHoveredIndex;
        }
        
        return isOpen_ || isHovered_;
    }
    
    void DropdownWidget::setSelectedIndex(int index) {
        if (index >= -1 && index < static_cast<int>(config_.getItems().size()) && selectedIndex_ != index) {
            selectedIndex_ = index;
            onSelectionChanged.emit(selectedIndex_);
            
            if (selectedIndex_ >= 0) {
                onItemSelected.emit(config_.getItems()[selectedIndex_]);
            }
        }
    }
    
    const DropdownItem* DropdownWidget::getSelectedItem() const {
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(config_.getItems().size())) {
            return &config_.getItems()[selectedIndex_];
        }
        return nullptr;
    }
    
    void DropdownWidget::addItem(const std::string& text, const std::string& value) {
        config_.addItem(text, value);
    }
    
    void DropdownWidget::clearItems() {
        config_.items(std::vector<DropdownItem>());
        selectedIndex_ = -1;
    }
    
    void DropdownWidget::setItems(const std::vector<DropdownItem>& items) {
        config_.items(items);
        if (selectedIndex_ >= static_cast<int>(items.size())) {
            selectedIndex_ = -1;
        }
    }
    
    void DropdownWidget::open() {
        if (!isOpen_) {
            isOpen_ = true;
            onOpenStateChanged.emit(true);
        }
    }
    
    void DropdownWidget::close() {
        if (isOpen_) {
            isOpen_ = false;
            hoveredItemIndex_ = -1;
            onOpenStateChanged.emit(false);
        }
    }
    
    void DropdownWidget::renderMainButton() {
        const auto& style = config_.getStyle();
        
        // Draw background
        uint32_t bgColor = isHovered_ ? style.getHoverColor() : style.getBackgroundColor();
        Draw::rect(x_, y_, config_.getWidth(), config_.getHeight(), bgColor);
        
        // Draw border
        for (int i = 0; i < style.getBorderWidth(); ++i) {
            // Top border
            Draw::rect(x_ - i, y_ - i, config_.getWidth() + 2 * i, 1, style.getBorderColor());
            // Bottom border
            Draw::rect(x_ - i, y_ + config_.getHeight() + i - 1, config_.getWidth() + 2 * i, 1, style.getBorderColor());
            // Left border
            Draw::rect(x_ - i, y_ - i, 1, config_.getHeight() + 2 * i, style.getBorderColor());
            // Right border
            Draw::rect(x_ + config_.getWidth() + i - 1, y_ - i, 1, config_.getHeight() + 2 * i, style.getBorderColor());
        }
        
        // Draw text
        std::string displayText;
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(config_.getItems().size())) {
            displayText = config_.getItems()[selectedIndex_].text;
        } else {
            displayText = config_.getPlaceholder();
        }
        
        int textX = x_ + style.getPadding();
        int textY = y_ + (config_.getHeight() - style.getFontSize() * 8) / 2;
        renderText(displayText, textX, textY, style.getTextColor());
        
        // Draw dropdown arrow
        renderArrow();
    }
    
    void DropdownWidget::renderDropdown() {
        const auto& style = config_.getStyle();
        const auto& items = config_.getItems();
        
        if (items.empty()) return;
        
        int dropdownY = y_ + config_.getHeight();
        int dropdownHeight = getDropdownHeight();
        int itemHeight = getItemHeight();
        
        // Draw dropdown background
        Draw::rect(x_, dropdownY, config_.getWidth(), dropdownHeight, style.getDropdownBackgroundColor());
        
        // Draw dropdown border
        for (int i = 0; i < style.getBorderWidth(); ++i) {
            // Top border
            Draw::rect(x_ - i, dropdownY - i, config_.getWidth() + 2 * i, 1, style.getBorderColor());
            // Bottom border
            Draw::rect(x_ - i, dropdownY + dropdownHeight + i - 1, config_.getWidth() + 2 * i, 1, style.getBorderColor());
            // Left border
            Draw::rect(x_ - i, dropdownY - i, 1, dropdownHeight + 2 * i, style.getBorderColor());
            // Right border
            Draw::rect(x_ + config_.getWidth() + i - 1, dropdownY - i, 1, dropdownHeight + 2 * i, style.getBorderColor());
        }
        
        // Draw items
        int visibleItems = std::min(static_cast<int>(items.size()), style.getMaxVisibleItems());
        for (int i = 0; i < visibleItems; ++i) {
            int itemY = dropdownY + i * itemHeight;
            
            // Draw item background if hovered
            if (i == hoveredItemIndex_) {
                Draw::rect(x_, itemY, config_.getWidth(), itemHeight, style.getHoverColor());
            }
            
            // Draw item text
            int textX = x_ + style.getPadding();
            int textY = itemY + (itemHeight - style.getFontSize() * 8) / 2;
            renderText(items[i].text, textX, textY, style.getTextColor());
        }
    }
    
    void DropdownWidget::renderArrow() {
        const auto& style = config_.getStyle();
        
        // Simple arrow using lines
        int arrowX = x_ + config_.getWidth() - 20;
        int arrowY = y_ + config_.getHeight() / 2;
        
        if (isOpen_) {
            // Up arrow
            Draw::line(arrowX - 4, arrowY + 2, arrowX, arrowY - 2, 2, style.getTextColor());
            Draw::line(arrowX, arrowY - 2, arrowX + 4, arrowY + 2, 2, style.getTextColor());
        } else {
            // Down arrow
            Draw::line(arrowX - 4, arrowY - 2, arrowX, arrowY + 2, 2, style.getTextColor());
            Draw::line(arrowX, arrowY + 2, arrowX + 4, arrowY - 2, 2, style.getTextColor());
        }
    }
    
    void DropdownWidget::renderText(const std::string& text, int x, int y, uint32_t color) {
        const auto& style = config_.getStyle();
        
        // Calculate available width for text (excluding padding and arrow space)
        int availableWidth = config_.getWidth() - style.getPadding() * 2 - 30; // 30 for arrow
        
        // Clip text if it's too long
        std::string displayText = text;
        if (calculateTextWidth(text) > availableWidth) {
            // Find the maximum number of characters that fit
            std::string clippedText;
            for (size_t i = 0; i < text.length(); ++i) {
                std::string candidate = text.substr(0, i + 1);
                if (calculateTextWidth(candidate) > availableWidth) {
                    break;
                }
                clippedText = candidate;
            }
            displayText = clippedText;
        }
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            Font::renderTTF(globalCanvas, displayText, x, y, style.getFontSize(), color);
        } else {
            DrawText::drawText(displayText.c_str(), x, y, style.getFontSize(), color);
        }
    }
    
    bool DropdownWidget::isPointInMainButton(int x, int y) const {
        return x >= x_ && x < x_ + config_.getWidth() &&
               y >= y_ && y < y_ + config_.getHeight();
    }
    
    bool DropdownWidget::isPointInDropdown(int x, int y) const {
        if (!isOpen_) return false;
        
        int dropdownY = y_ + config_.getHeight();
        int dropdownHeight = getDropdownHeight();
        
        return x >= x_ && x < x_ + config_.getWidth() &&
               y >= dropdownY && y < dropdownY + dropdownHeight;
    }
    
    int DropdownWidget::getItemIndexAtPoint(int x, int y) const {
        if (!isPointInDropdown(x, y)) return -1;
        
        int dropdownY = y_ + config_.getHeight();
        int itemHeight = getItemHeight();
        int relativeY = y - dropdownY;
        
        int itemIndex = relativeY / itemHeight;
        const auto& items = config_.getItems();
        int visibleItems = std::min(static_cast<int>(items.size()), config_.getStyle().getMaxVisibleItems());
        
        if (itemIndex >= 0 && itemIndex < visibleItems) {
            return itemIndex;
        }
        
        return -1;
    }
    
    int DropdownWidget::calculateTextWidth(const std::string& text) const {
        const auto& style = config_.getStyle();
        
        if (style.getFontType() == FontType::TTF && Font::hasTTFFont()) {
            return Font::getTextWidth(text, style.getFontSize(), FontType::TTF);
        } else {
            // Bitmap font calculation - more accurate
            int charWidth = 8 * style.getFontSize(); // Each char is 8 pixels wide scaled
            return text.length() * charWidth;
        }
    }
    
    int DropdownWidget::calculateOptimalWidth() const {
        const auto& style = config_.getStyle();
        const auto& items = config_.getItems();
        
        int maxWidth = 0;
        
        // Check placeholder width
        if (!config_.getPlaceholder().empty()) {
            maxWidth = std::max(maxWidth, calculateTextWidth(config_.getPlaceholder()));
        }
        
        // Check all item widths
        for (const auto& item : items) {
            maxWidth = std::max(maxWidth, calculateTextWidth(item.text));
        }
        
        // Add padding and arrow space
        return maxWidth + style.getPadding() * 2 + 30; // 30 pixels for arrow
    }
    
    int DropdownWidget::getDropdownHeight() const {
        const auto& style = config_.getStyle();
        const auto& items = config_.getItems();
        
        int visibleItems = std::min(static_cast<int>(items.size()), style.getMaxVisibleItems());
        return visibleItems * getItemHeight();
    }
    
    int DropdownWidget::getItemHeight() const {
        const auto& style = config_.getStyle();
        return style.getFontSize() * 8 + style.getPadding() * 2;
    }
    
    // Widget interface implementation
    int DropdownWidget::getWidth() const {
        return config_.getWidth();
    }
    
    int DropdownWidget::getHeight() const {
        int baseHeight = config_.getHeight();
        if (isOpen_) {
            baseHeight += getDropdownHeight();
        }
        return baseHeight;
    }
    
    void DropdownWidget::setPosition(int x, int y) {
        x_ = x;
        y_ = y;
        config_.setPosition(x, y);
    }
    
    int DropdownWidget::getX() const {
        return config_.getX();
    }
    
    int DropdownWidget::getY() const {
        return config_.getY();
    }
    
    void DropdownWidget::resize(int width, int height) {
        config_.setSize(width, height);
    }
    
    // Factory function
    std::shared_ptr<DropdownWidget> Dropdown(const DropdownConfig& config, bool addToManager) {
        auto dropdown = std::make_shared<DropdownWidget>(config);
        
        if (addToManager) {
            addWidget(dropdown);
        }
        
        return dropdown;
    }
    
    // Preset configurations
    namespace DropdownPresets {
        DropdownConfig Default(int x, int y, int width, int height) {
            return DropdownConfig(x, y, width, height);
        }
        
        DropdownConfig Modern(int x, int y, int width, int height) {
            return DropdownConfig(x, y, width, height)
                .placeholder("Choose an option...")
                .style(DropdownStyle()
                    .backgroundColor(0xFFFFFFFF)
                    .borderColor(0xFF007BFF)
                    .selectedColor(0xFF007BFF)
                    .textColor(0xFF212529)
                    .hoverColor(0xFFF8F9FA)
                    .dropdownBackgroundColor(0xFFFFFFFF)
                    .borderWidth(2)
                    .padding(12)
                    .maxVisibleItems(6)
                    .fontSize(2));
        }
        
        DropdownConfig Compact(int x, int y, int width, int height) {
            return DropdownConfig(x, y, width, height)
                .placeholder("Select...")
                .style(DropdownStyle()
                    .backgroundColor(0xFFF8F9FA)
                    .borderColor(0xFF6C757D)
                    .selectedColor(0xFF28A745)
                    .textColor(0xFF495057)
                    .hoverColor(0xFFE9ECEF)
                    .dropdownBackgroundColor(0xFFFFFFFF)
                    .borderWidth(1)
                    .padding(6)
                    .maxVisibleItems(4)
                    .fontSize(1));
        }
    }
}