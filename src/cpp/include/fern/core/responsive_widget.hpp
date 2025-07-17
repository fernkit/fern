#pragma once

namespace Fern {
    /**
     * Interface for widgets that can respond to window resize events
     */
    class ResponsiveWidget {
    public:
        virtual ~ResponsiveWidget() = default;
        
        /**
         * Called when the window is resized
         * @param newWidth New window width
         * @param newHeight New window height
         */
        virtual void onWindowResize(int newWidth, int newHeight) = 0;
    };
}
