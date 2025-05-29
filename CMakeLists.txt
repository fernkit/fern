cmake_minimum_required(VERSION 3.10)
project(FernGraphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Platform detection
if(EMSCRIPTEN)
    set(PLATFORM_NAME "Web")
    add_definitions(-D__EMSCRIPTEN__)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_NAME "Linux")
    add_definitions(-D__linux__)
elseif(WIN32)
    set(PLATFORM_NAME "Windows")
    add_definitions(-DWIN32)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

message(STATUS "Building for platform: ${PLATFORM_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Include directories
include_directories(src/cpp/include)

# Find core source files
file(GLOB_RECURSE CORE_SOURCES "src/cpp/src/*.cpp")
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/platform/.*")

# Platform-specific sources
set(PLATFORM_SOURCES
    src/cpp/src/platform/platform_factory.cpp
)

if(EMSCRIPTEN)
    list(APPEND PLATFORM_SOURCES src/cpp/src/platform/web_renderer.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND PLATFORM_SOURCES src/cpp/src/platform/linux_renderer.cpp)
endif()

# Create Fern library
add_library(fern STATIC ${CORE_SOURCES} ${PLATFORM_SOURCES})

# Platform-specific settings
if(EMSCRIPTEN)
    set_target_properties(fern PROPERTIES
        COMPILE_FLAGS "-s USE_WEBGL2=1"
        LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 -s EXPORTED_FUNCTIONS='[_main]' -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]'"
    )
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(fern ${X11_LIBRARIES})
    target_include_directories(fern PRIVATE ${X11_INCLUDE_DIR})
endif()

# Build examples
file(GLOB EXAMPLE_SOURCES "examples/cpp/*.cpp")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_NAME} fern)
    
    if(EMSCRIPTEN)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES
            SUFFIX ".html"
            LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1"
        )
    endif()
    
    message(STATUS "Added example: ${EXAMPLE_NAME}")
endforeach()

# Install targets
install(TARGETS fern DESTINATION lib)
install(DIRECTORY src/cpp/include/ DESTINATION include)