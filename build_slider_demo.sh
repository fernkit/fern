#!/bin/bash

# Quick build script for Fern C++ examples
# This sources emscripten environment and builds examples

echo "Building Fern C++ slider demo..."

# Source emscripten environment
source /home/rishi/emsdk/emsdk_env.sh

# Build the slider demo
cd /home/rishi/git/fern
./fern-cli.sh --build --cpp examples/cpp/slider_demo.cpp

echo "Build complete! Open dist/cpp/slider_demo.html in a browser."
echo "Or run: python3 -m http.server 8000"
echo "Then visit: http://localhost:8000/dist/cpp/slider_demo.html"
