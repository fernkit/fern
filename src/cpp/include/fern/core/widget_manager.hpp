#pragma once
#include "../ui/widgets/widget.hpp"
#include "../core/input.hpp"
#include "responsive_widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

// Forward declarations
namespace Fern {
    int getWidth();
    int getHeight();
}

namespace Fern
{
    class WidgetManager {
    public: 
        static WidgetManager& getInstance(){
            static WidgetManager instance;
            return instance;
        }

        void addWidget(std::shared_ptr<Widget> widget){
             widgets_.push_back(widget);
        }

        void removeWidget(std::shared_ptr<Widget> widget) {
            widgets_.erase(
                std::remove_if(widgets_.begin(), widgets_.end(),
                    [&widget](const auto& w) { return w == widget; }),
                widgets_.end()
            );
        }

        // for proper Z handling, the update has been reversed
        void updateAll(const InputState& input) {
            bool inputHandled = false;
            for (auto it = widgets_.rbegin(); it != widgets_.rend(); ++it) {
                if (!inputHandled) {
                    inputHandled = (*it)->handleInput(input);
                }
            }
        }

         void renderAll() {
            for (auto& widget : widgets_) {
                widget->render();
            }
        }

        void clear() {
            widgets_.clear();
        }

        // Responsiveness support
        void onWindowResize(int newWidth, int newHeight) {
            // Notify all widgets about the resize
            for (auto& widget : widgets_) {
                // Check if widget has responsive capabilities
                auto responsiveWidget = dynamic_cast<ResponsiveWidget*>(widget.get());
                if (responsiveWidget) {
                    responsiveWidget->onWindowResize(newWidth, newHeight);
                }
            }
        }
        
        void refreshLayout() { 
            // Use external canvas dimensions
            onWindowResize(getWidth(), getHeight()); 
        }
        
        // Get current canvas dimensions (implemented inline to avoid dependency issues)
        int getCanvasWidth() const { return 800; }  // Will be updated by external calls
        int getCanvasHeight() const { return 600; } // Will be updated by external calls

    private:
        WidgetManager() = default;
        std::vector<std::shared_ptr<Widget>> widgets_;    
    };

    inline void addWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().addWidget(widget);
    }
    
    inline void removeWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().removeWidget(widget);
    }
} // namespace Fern
