#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace Fern
{
    /**
     * @brief Template class for event signaling and callback management
     * 
     * Signal provides a type-safe event system for connecting callbacks (slots)
     * to events. When a signal is emitted, all connected slots are called with
     * the provided arguments. This is commonly used for UI event handling.
     * 
     * @tparam Args Types of arguments passed when the signal is emitted
     * 
     * @example Basic signal usage:
     * @code
     * Signal<int, std::string> mySignal;
     * 
     * auto id = mySignal.connect([](int value, const std::string& text) {
     *     std::cout << "Received: " << value << ", " << text << std::endl;
     * });
     * 
     * mySignal.emit(42, "Hello"); // Calls the connected function
     * mySignal.disconnect(id);    // Remove the connection
     * @endcode
     * 
     * @example Widget event handling:
     * @code
     * button->onClick.connect([]() {
     *     std::cout << "Button clicked!" << std::endl;
     * });
     * @endcode
     */
    template <typename... Args>
    class Signal{
    public: 
        /**
         * @brief Unique identifier for signal connections
         */
        using ConnectionID = size_t;
        
        /**
         * @brief Function type for signal slots
         */
        using SlotFunction = std::function<void(Args...)>;
        
        /**
         * @brief Connect a callback function to the signal
         * 
         * @param slot Function to call when signal is emitted
         * @return ConnectionID Unique ID for this connection (use for disconnect)
         */
        ConnectionID connect(SlotFunction slot) {
            ConnectionID id = nextID_++;
            slots_.push_back({id, slot});
            return id;
        }
        
        /**
         * @brief Emit the signal, calling all connected slots
         * 
         * @param args Arguments to pass to all connected functions
         */
        void emit(Args... args) const {
            for (const auto& slot: slots_){
                slot.second(args...);
            }            
        }
        
        /**
         * @brief Disconnect a specific slot from the signal
         * 
         * @param id ConnectionID returned from connect()
         */
        void disconnect(ConnectionID id) {
            slots_.erase(
                std::remove_if(slots_.begin(), slots_.end(),
                    [id](const auto& pair) { return pair.first == id; }),
                slots_.end()
            );
        }
        
    private:
        std::vector<std::pair<ConnectionID, SlotFunction>> slots_;  ///< Connected slots with their IDs
        ConnectionID nextID_ = 0;  ///< Next available connection ID
    };    
} // namespace Fern
