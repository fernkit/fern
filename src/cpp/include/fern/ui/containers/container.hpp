#pragma once
#include "../widgets/widget.hpp"
#include "../../graphics/colors.hpp"
#include <memory>
#include <vector>

namespace Fern {
    class LinearGradient {
    public:
        enum Direction { HORIZONTAL, VERTICAL };
        
        struct GradientStop {
            uint32_t color;
            float position;  // 0.0 to 1.0
        };
        
        LinearGradient(const std::vector<GradientStop>& stops, Direction direction = HORIZONTAL) 
            : stops_(stops), direction_(direction) {}
        
        uint32_t colorAt(float position) const;
        bool isVertical() const { return direction_ == VERTICAL; }
        
    private:
        std::vector<GradientStop> stops_;
        Direction direction_;
    };

    // Base container widget
    class ContainerWidget : public Widget {
    public:
        ContainerWidget(uint32_t color, int x = 0, int y = 0, int width = 0, int height = 0);
        ContainerWidget(uint32_t color, int x, int y, int width, int height, 
                     std::shared_ptr<Widget> child);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        void setChild(std::shared_ptr<Widget> child);
        
    protected:
        uint32_t color_;
        std::shared_ptr<Widget> child_;
    };

    // Centered container widget
    class CenteredContainerWidget : public ContainerWidget {
    public:
        CenteredContainerWidget(uint32_t color, int width, int height);
        CenteredContainerWidget(uint32_t color, int width, int height, 
                             std::shared_ptr<Widget> child);
    };

    // Gradient container widget
    class GradientContainerWidget : public Widget {
    public:
        GradientContainerWidget(int x, int y, int width, int height, 
                            const LinearGradient& gradient);
        GradientContainerWidget(int x, int y, int width, int height, 
                            const LinearGradient& gradient,
                            std::shared_ptr<Widget> child);
                            
        void render() override;
        bool handleInput(const InputState& input) override;
        void setChild(std::shared_ptr<Widget> child);
        
    private:
        LinearGradient gradient_;
        std::shared_ptr<Widget> child_;
    };

    // Factory functions
    std::shared_ptr<ContainerWidget> Container(
        uint32_t color, int x, int y, int width, int height, 
        bool addToManager = true, 
        std::shared_ptr<Widget> child = nullptr);
    
    std::shared_ptr<CenteredContainerWidget> CenteredContainer(
        uint32_t color, int width, int height,
        bool addToManager = true,
        std::shared_ptr<Widget> child = nullptr);
    
    std::shared_ptr<GradientContainerWidget> GradientContainer(
        int x, int y, int width, int height,
        const LinearGradient& gradient, 
        bool addToManager = true,
        std::shared_ptr<Widget> child = nullptr);
}