#pragma once

#include "../../core/types.hpp"
#include <functional>

namespace Fern {
    class Widget {
    public:
        Widget() = default;
        virtual ~Widget() = default;
        virtual void render() = 0;
        virtual bool handleInput(const InputState& input) = 0;
        
        virtual void setPosition(int x, int y) { 
            if (x_ != x || y_ != y) {
                x_ = x; 
                y_ = y;
                markDirty();
            }
        }
        
        virtual int getX() const { return x_; }
        virtual int getY() const { return y_; }
        
        virtual void resize(int width, int height) { 
            if (width_ != width || height_ != height) {
                width_ = width; 
                height_ = height;
                markDirty();
            }
        }
        
        virtual int getWidth() const { return width_; }
        virtual int getHeight() const { return height_; }
        
        // Dirty checking system
        void markDirty() { isDirty_ = true; }
        void markClean() { isDirty_ = false; }
        bool isDirty() const { return isDirty_; }
        
    protected:
        int x_ = 0;
        int y_ = 0;
        int width_ = 0;
        int height_ = 0;
        bool isDirty_ = true;  // Start dirty for initial render
    };
}