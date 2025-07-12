cmake_minimum_required(VERSION 3.10)
project(FernCpp)

set(CMAKE_CXX_STANDARD 14)

# Find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/fern/*.hpp")

# Include directories
include_directories(include)

# Library target
add_library(fern STATIC ${SOURCES} ${HEADERS})

# Emscripten-specific settings
if(EMSCRIPTEN)
    set_target_properties(fern PROPERTIES
        LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS='[_main, _fernUpdateMousePosition, _fernUpdateMouseButton, _webRendererMouseMove, _webRendererMouseClick, _webRendererResize, _webRendererKeyEvent, _webRendererTextInput]' -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]'")
endif()