/**
 * @file canvas.hpp
 * @brief Core canvas class for pixel-level rendering operations
 * 
 * This file defines the Canvas class, which provides low-level access to the
 * pixel buffer used for rendering. The canvas abstracts the underlying pixel
 * buffer and provides safe, efficient pixel manipulation methods.
 */

#pragma once

#include "types.hpp"
#include <cstdint>

namespace Fern {
    /**
     * @brief Low-level canvas for pixel-based rendering
     * 
     * The Canvas class provides direct access to a pixel buffer for low-level
     * rendering operations. It manages a 2D array of 32-bit RGBA pixels and
     * provides methods for safe pixel manipulation, bounds checking, and buffer management.
     * 
     * All pixels are stored in 32-bit RGBA format (0xAABBGGRR) where:
     * - AA = Alpha (transparency)
     * - BB = Blue component  
     * - GG = Green component
     * - RR = Red component
     * 
     * @example Basic usage:
     * @code
     * uint32_t* buffer = new uint32_t[800 * 600];
     * Canvas canvas(buffer, 800, 600);
     * 
     * canvas.clear(0xFF000000);  // Clear to black
     * canvas.setPixel(100, 50, 0xFFFF0000);  // Set red pixel
     * 
     * uint32_t pixel = canvas.getPixel(100, 50);  // Read pixel
     * @endcode
     */
    class Canvas {
    public:
        /**
         * @brief Construct a new Canvas with the given pixel buffer
         * 
         * Creates a canvas that operates on the provided pixel buffer.
         * The buffer must remain valid for the lifetime of the canvas.
         * 
         * @param buffer Pointer to 32-bit RGBA pixel array
         * @param width Canvas width in pixels
         * @param height Canvas height in pixels
         * 
         * @warning The pixel buffer must be at least (width * height * 4) bytes
         * @note The canvas does not take ownership of the buffer
         */
        Canvas(uint32_t* buffer, int width, int height);
        
        /**
         * @brief Clear the entire canvas with a solid color
         * 
         * Fills all pixels in the canvas with the specified color value.
         * This is an efficient way to reset the canvas for a new frame.
         * 
         * @param color 32-bit RGBA color value (0xAABBGGRR format)
         * 
         * @example
         * @code
         * canvas.clear(0xFF000000);  // Clear to opaque black
         * canvas.clear(0x00000000);  // Clear to transparent
         * @endcode
         */
        void clear(uint32_t color);
        
        /**
         * @brief Set a single pixel to the specified color
         * 
         * Sets the pixel at the given coordinates to the specified color.
         * Coordinates are automatically bounds-checked for safety.
         * 
         * @param x X-coordinate (0 to width-1)
         * @param y Y-coordinate (0 to height-1)  
         * @param color 32-bit RGBA color value
         * 
         * @note Coordinates outside the canvas bounds are silently ignored
         */
        void setPixel(int x, int y, uint32_t color);
        
        /**
         * @brief Get the color of a single pixel
         * 
         * Retrieves the color value of the pixel at the given coordinates.
         * Returns 0 (transparent black) for coordinates outside the canvas bounds.
         * 
         * @param x X-coordinate (0 to width-1)
         * @param y Y-coordinate (0 to height-1)
         * @return uint32_t 32-bit RGBA color value, or 0 if out of bounds
         */
        uint32_t getPixel(int x, int y) const;
        
        /**
         * @brief Get the canvas width in pixels
         * 
         * @return int Canvas width in pixels
         */
        int getWidth() const { return width_; }
        
        /**
         * @brief Get the canvas height in pixels
         * 
         * @return int Canvas height in pixels
         */
        int getHeight() const { return height_; }
        
        /**
         * @brief Get direct access to the pixel buffer
         * 
         * Returns a pointer to the underlying pixel buffer for advanced operations.
         * Use with caution and ensure proper bounds checking.
         * 
         * @return uint32_t* Pointer to the pixel buffer array
         * 
         * @warning Direct buffer access bypasses bounds checking
         * @note The buffer contains (width * height) 32-bit pixels
         */
        uint32_t* getBuffer() const { return buffer_; }
        
    private:
        uint32_t* buffer_;  ///< Pointer to the pixel buffer
        int width_;         ///< Canvas width in pixels  
        int height_;        ///< Canvas height in pixels
    };
    
    /**
     * @brief Global canvas instance used by the framework
     * 
     * This global canvas instance is used by all drawing operations in Fern.
     * It is automatically initialized when Fern::initialize() is called.
     * 
     * @warning Do not modify this directly unless you know what you're doing
     */
    extern Canvas* globalCanvas;
}