#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include "../core/types.hpp"

namespace Fern {
    
    class PlatformRenderer {
    public:
        virtual ~PlatformRenderer() = default;
        
        virtual void initialize(int width, int height) = 0;
        virtual void present(uint32_t* pixelBuffer, int width, int height) = 0;
        virtual void shutdown() = 0;
        
        virtual void setTitle(const std::string& title) = 0;
        virtual void setSize(int width, int height) = 0;
        virtual bool shouldClose() = 0;
        
        virtual void pollEvents() = 0;
        virtual void setMouseCallback(std::function<void(int, int)> callback) = 0;
        virtual void setClickCallback(std::function<void(bool)> callback) = 0;
        virtual void setResizeCallback(std::function<void(int, int)> callback) = 0;
        
        // Keyboard callbacks
        virtual void setKeyCallback(std::function<void(KeyCode, bool)> callback) = 0;
        virtual void setTextInputCallback(std::function<void(const std::string&)> callback) = 0;
        
        virtual std::string getPlatformName() = 0;
    };
    
    std::unique_ptr<PlatformRenderer> createRenderer();
}