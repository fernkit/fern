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
elseif(APPLE)
    set(PLATFORM_NAME "macOS")
    add_definitions(-D__APPLE__)
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
elseif(APPLE)
    list(APPEND PLATFORM_SOURCES src/cpp/src/platform/macos_renderer.mm)
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
elseif(APPLE)
    # macOS requires Cocoa framework
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    target_link_libraries(fern PUBLIC ${COCOA_FRAMEWORK})
    
    # Find fontconfig and freetype on macOS
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    
    # Add library directories to global search path
    link_directories(${FONTCONFIG_LIBRARY_DIRS} ${FREETYPE_LIBRARY_DIRS})
    
    target_link_libraries(fern PUBLIC ${FONTCONFIG_LIBRARIES} ${FREETYPE_LIBRARIES})
    target_include_directories(fern PUBLIC ${FONTCONFIG_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
    target_compile_options(fern PUBLIC ${FONTCONFIG_CFLAGS_OTHER} ${FREETYPE_CFLAGS_OTHER})
    
    # Set Objective-C++ flags for .mm files
    set_source_files_properties(
        src/cpp/src/platform/macos_renderer.mm
        PROPERTIES COMPILE_FLAGS "-x objective-c++ -fobjc-arc"
    )
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(fern PUBLIC ${X11_LIBRARIES})
    target_include_directories(fern PUBLIC ${X11_INCLUDE_DIR})
    
    # Find fontconfig and freetype on Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    
    # Add library directories to global search path
    link_directories(${FONTCONFIG_LIBRARY_DIRS} ${FREETYPE_LIBRARY_DIRS})
    
    target_link_libraries(fern PUBLIC ${FONTCONFIG_LIBRARIES} ${FREETYPE_LIBRARIES})
    target_include_directories(fern PUBLIC ${FONTCONFIG_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
    target_compile_options(fern PUBLIC ${FONTCONFIG_CFLAGS_OTHER} ${FREETYPE_CFLAGS_OTHER})
endif()

# Build examples
file(GLOB_RECURSE EXAMPLE_SOURCES "examples/cpp/*.cpp")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    
    # Check if file is not empty
    file(SIZE ${EXAMPLE_SOURCE} FILE_SIZE)
    if(FILE_SIZE GREATER 0)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
        target_link_libraries(${EXAMPLE_NAME} fern)
        
        if(EMSCRIPTEN)
            set_target_properties(${EXAMPLE_NAME} PROPERTIES
                SUFFIX ".html"
                LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1"
            )
        endif()
        
        message(STATUS "Added example: ${EXAMPLE_NAME}")
    else()
        message(STATUS "Skipped empty example: ${EXAMPLE_NAME}")
    endif()
endforeach()

# Install targets
install(TARGETS fern 
    EXPORT FernTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY src/cpp/include/ DESTINATION include 
    FILES_MATCHING PATTERN "*.hpp"
)

# Install pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fern.pc.in ${CMAKE_CURRENT_BINARY_DIR}/fern.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fern.pc DESTINATION lib/pkgconfig)

# Install export targets
install(EXPORT FernTargets
    FILE FernTargets.cmake
    DESTINATION lib/cmake/Fern
)

# Create CMake config files for find_package support
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FernConfigVersion.cmake"
    VERSION "0.1.0"
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FernConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FernConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Fern
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/FernConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/FernConfigVersion.cmake"
    DESTINATION lib/cmake/Fern
)