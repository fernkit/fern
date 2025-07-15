#pragma once
#include "../ui/widgets/widget.hpp"
#include "../core/input.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace Fern
{
    // Forward declaration
    class LayoutWidget;
    
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
        void updateAll(const InputState& input);

         void renderAll() {
            for (auto& widget : widgets_) {
                widget->render();
                // Note: We still keep the dirty flag for future optimizations
                // like layout recalculation, but basic rendering happens every frame
            }
        }

        void clear() {
            widgets_.clear();
        }

    private:
    private:
        WidgetManager() = default;
        void updateWidget(std::shared_ptr<Widget> widget, const InputState& input, bool& inputHandled);
        std::vector<std::shared_ptr<Widget>> widgets_;    
    };

    inline void addWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().addWidget(widget);
    }
    
    inline void removeWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().removeWidget(widget);
    }
} // namespace Fern
