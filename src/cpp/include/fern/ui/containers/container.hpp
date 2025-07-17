#pragma once
#include "../widgets/widget.hpp"
#include "../../graphics/colors.hpp"
#include <memory>
#include <vector>

namespace Fern {
    /**
     * @brief Linear gradient utility for creating color gradients
     * 
     * LinearGradient allows creating smooth color transitions along horizontal
     * or vertical directions using multiple color stops.
     * 
     * @example Creating a simple blue to red gradient:
     * @code
     * LinearGradient gradient({
     *     {Colors::Blue, 0.0f},
     *     {Colors::Red, 1.0f}
     * }, LinearGradient::HORIZONTAL);
     * @endcode
     */
    class LinearGradient {
    public:
        /**
         * @brief Direction of the gradient
         */
        enum Direction { HORIZONTAL, VERTICAL };
        
        /**
         * @brief A color stop in the gradient
         */
        struct GradientStop {
            uint32_t color;    ///< Color in ARGB format
            float position;    ///< Position from 0.0 to 1.0
        };
        
        /**
         * @brief Construct a new Linear Gradient
         * 
         * @param stops Vector of color stops defining the gradient
         * @param direction Direction of the gradient (default: HORIZONTAL)
         */
        LinearGradient(const std::vector<GradientStop>& stops, Direction direction = HORIZONTAL) 
            : stops_(stops), direction_(direction) {}
        
        /**
         * @brief Get the interpolated color at a specific position
         * 
         * @param position Position from 0.0 to 1.0
         * @return Interpolated color in ARGB format
         */
        uint32_t colorAt(float position) const;
        
        /**
         * @brief Check if gradient is vertical
         * 
         * @return true if vertical, false if horizontal
         */
        bool isVertical() const { return direction_ == VERTICAL; }
        
    private:
        std::vector<GradientStop> stops_;  ///< Color stops defining the gradient
        Direction direction_;              ///< Direction of the gradient
    };

    /**
     * @brief Base container widget for holding and styling child widgets
     * 
     * ContainerWidget provides a colored background for child widgets and handles
     * positioning and input forwarding. It can contain a single child widget
     * and provides basic container functionality.
     * 
     * @example Creating a container with a child widget:
     * @code
     * auto textWidget = Text("Hello", 10, 10, Colors::White);
     * auto container = Container(Colors::Blue, 50, 50, 200, 100, textWidget);
     * @endcode
     */
    class ContainerWidget : public Widget {
    public:
        /**
         * @brief Construct a new Container Widget without a child
         * 
         * @param color Background color in ARGB format
         * @param x X position (default: 0)
         * @param y Y position (default: 0)
         * @param width Width in pixels (default: 0)
         * @param height Height in pixels (default: 0)
         */
        ContainerWidget(uint32_t color, int x = 0, int y = 0, int width = 0, int height = 0);
        
        /**
         * @brief Construct a new Container Widget with a child
         * 
         * @param color Background color in ARGB format
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param child Child widget to contain
         */
        ContainerWidget(uint32_t color, int x, int y, int width, int height, 
                     std::shared_ptr<Widget> child);
        
        /**
         * @brief Render the container and its child
         */
        void render() override;
        
        /**
         * @brief Handle input and forward to child
         * 
         * @param input Current input state
         * @return true if input was handled
         */
        bool handleInput(const InputState& input) override;
        
        /**
         * @brief Set or replace the child widget
         * 
         * @param child New child widget
         */
        void setChild(std::shared_ptr<Widget> child);
        
    protected:
        uint32_t color_;                    ///< Background color
        std::shared_ptr<Widget> child_;     ///< Child widget
    };

    /**
     * @brief Container widget that centers itself on the screen
     * 
     * CenteredContainerWidget automatically positions itself at the center
     * of the screen and provides a colored background for its child widget.
     * 
     * @example Creating a centered container:
     * @code
     * auto centered = CenteredContainer(Colors::Gray, 300, 200, textWidget);
     * @endcode
     */
    class CenteredContainerWidget : public ContainerWidget {
    public:
        /**
         * @brief Construct a new Centered Container Widget without a child
         * 
         * @param color Background color in ARGB format
         * @param width Width in pixels
         * @param height Height in pixels
         */
        CenteredContainerWidget(uint32_t color, int width, int height);
        
        /**
         * @brief Construct a new Centered Container Widget with a child
         * 
         * @param color Background color in ARGB format
         * @param width Width in pixels
         * @param height Height in pixels
         * @param child Child widget to contain
         */
        CenteredContainerWidget(uint32_t color, int width, int height, 
                             std::shared_ptr<Widget> child);
    };

    /**
     * @brief Container widget with gradient background
     * 
     * GradientContainerWidget provides a linear gradient background for child widgets.
     * The gradient can be horizontal or vertical and supports multiple color stops.
     * 
     * @example Creating a gradient container:
     * @code
     * LinearGradient gradient({
     *     {Colors::Blue, 0.0f},
     *     {Colors::Purple, 0.5f},
     *     {Colors::Red, 1.0f}
     * }, LinearGradient::VERTICAL);
     * auto gradientBox = GradientContainer(0, 0, 300, 200, gradient, textWidget);
     * @endcode
     */
    class GradientContainerWidget : public Widget {
    public:
        /**
         * @brief Construct a new Gradient Container Widget without a child
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param gradient Linear gradient for the background
         */
        GradientContainerWidget(int x, int y, int width, int height, 
                            const LinearGradient& gradient);
        
        /**
         * @brief Construct a new Gradient Container Widget with a child
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param gradient Linear gradient for the background
         * @param child Child widget to contain
         */
        GradientContainerWidget(int x, int y, int width, int height, 
                            const LinearGradient& gradient,
                            std::shared_ptr<Widget> child);
                            
        /**
         * @brief Render the gradient background and child
         */
        void render() override;
        
        /**
         * @brief Handle input and forward to child
         * 
         * @param input Current input state
         * @return true if input was handled
         */
        bool handleInput(const InputState& input) override;
        
        /**
         * @brief Set or replace the child widget
         * 
         * @param child New child widget
         */
        void setChild(std::shared_ptr<Widget> child);
        
    private:
        LinearGradient gradient_;           ///< Background gradient
        std::shared_ptr<Widget> child_;     ///< Child widget
    };

    /**
     * @brief Factory function to create a container widget
     * 
     * @param color Background color in ARGB format
     * @param x X position
     * @param y Y position
     * @param width Width in pixels
     * @param height Height in pixels
     * @param child Optional child widget (default: nullptr)
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<ContainerWidget> Shared pointer to created container
     */
    std::shared_ptr<ContainerWidget> Container(
        uint32_t color, int x, int y, int width, int height,         
        std::shared_ptr<Widget> child = nullptr, bool addToManager = false);
    
    /**
     * @brief Factory function to create a centered container widget
     * 
     * @param color Background color in ARGB format
     * @param width Width in pixels
     * @param height Height in pixels
     * @param addToManager Whether to add to widget manager (default: false)
     * @param child Optional child widget (default: nullptr)
     * @return std::shared_ptr<CenteredContainerWidget> Shared pointer to created container
     */
    std::shared_ptr<CenteredContainerWidget> CenteredContainer(
        uint32_t color, int width, int height,
        bool addToManager = false,
        std::shared_ptr<Widget> child = nullptr);
    
    /**
     * @brief Factory function to create a gradient container widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width in pixels
     * @param height Height in pixels
     * @param gradient Linear gradient for the background
     * @param addToManager Whether to add to widget manager (default: false)
     * @param child Optional child widget (default: nullptr)
     * @return std::shared_ptr<GradientContainerWidget> Shared pointer to created container
     */
    std::shared_ptr<GradientContainerWidget> GradientContainer(
        int x, int y, int width, int height,
        const LinearGradient& gradient, 
        bool addToManager = false,
        std::shared_ptr<Widget> child = nullptr);
}