#pragma once

#include "widget.hpp"
#include <string>
#include <functional>
#include <memory>
#include "../core/signal.hpp"

namespace Fern {
    struct ButtonConfig {
        int x;
        int y;
        int width;
        int height;
        uint32_t normalColor;
        uint32_t hoverColor;
        uint32_t pressColor;
        std::string label;
        int textScale;
        uint32_t textColor;
        std::function<void()> onClick; // deprecated
    };
    
    class ButtonWidget : public Widget {
    public:
        ButtonWidget(const ButtonConfig& config);
        
        void render() override;
        bool handleInput(const InputState& input) override;

        int getWidth() const override;
        int getHeight() const override;
        void setPosition(int x, int y) override;
        int getX() const override;
        int getY() const override;
        void resize(int width, int height) override;
    

        Signal<> onClick;       
        Signal<bool> onHover;   
        Signal<bool> onPress;   
        
    private:
        ButtonConfig config_;
        bool isHovered_ = false;
        bool isPressed_ = false;
    };
    
    std::shared_ptr<ButtonWidget> Button(const ButtonConfig& config, bool addToManager = true);
}