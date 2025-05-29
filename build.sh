#!/bin/bash
# filepath: /home/rishi/graph_c/build.sh

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
SRC_DIR="$PROJECT_ROOT/src/cpp"
EXAMPLES_DIR="$PROJECT_ROOT/examples/cpp"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
PLATFORM=""
TARGET=""
BUILD_TYPE="Release"
CLEAN=false
VERBOSE=false

show_help() {
    echo "Fern Graphics Library Build System"
    echo ""
    echo "Usage: $0 [OPTIONS] <platform> [target]"
    echo ""
    echo "Platforms:"
    echo "  linux    - Build for Linux (X11)"
    echo "  web      - Build for Web (Emscripten)"
    echo "  all      - Build for all platforms"
    echo ""
    echo "Targets (optional):"
    echo "  platform_test    - Build platform test example"
    echo "  expanded_example - Build expanded widget example"
    echo "  [filename.cpp]   - Build specific example file"
    echo ""
    echo "Options:"
    echo "  -d, --debug      Build in debug mode"
    echo "  -c, --clean      Clean build directory first"
    echo "  -v, --verbose    Verbose output"
    echo "  -h, --help       Show this help"
    echo ""
    echo "Examples:"
    echo "  $0 linux platform_test"
    echo "  $0 web"
    echo "  $0 all expanded_example"
    echo "  $0 --clean --debug linux platform_test"
}

log() {
    echo -e "${BLUE}[BUILD]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    exit 1
}

success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        linux|web|all)
            PLATFORM="$1"
            shift
            ;;
        *.cpp)
            TARGET="$1"
            shift
            ;;
        *)
            if [[ -z "$TARGET" ]]; then
                TARGET="$1"
            else
                error "Unknown argument: $1"
            fi
            shift
            ;;
    esac
done

if [[ -z "$PLATFORM" ]]; then
    error "Platform not specified. Use 'linux', 'web', or 'all'"
fi

# Clean if requested
if [[ "$CLEAN" == true ]]; then
    log "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directories
mkdir -p "$BUILD_DIR/linux"
mkdir -p "$BUILD_DIR/web"

# Find source files
CORE_SOURCES=$(find "$SRC_DIR/src" -name "*.cpp" | grep -v platform | tr '\n' ' ')
PLATFORM_FACTORY="$SRC_DIR/src/platform/platform_factory.cpp"
LINUX_RENDERER="$SRC_DIR/src/platform/linux_renderer.cpp"
WEB_RENDERER="$SRC_DIR/src/platform/web_renderer.cpp"

build_linux() {
    log "Building for Linux (X11)..."
    
    # Check for X11 development libraries
    if ! pkg-config --exists x11; then
        warning "X11 development libraries not found. Install with:"
        warning "  Ubuntu/Debian: sudo apt-get install libx11-dev"
        warning "  Fedora/RHEL:   sudo dnf install libX11-devel"
        warning "  Arch:          sudo pacman -S libx11"
    fi
    
    local LINUX_FLAGS="-std=c++17 -D__linux__ -I$SRC_DIR/include"
    local LINUX_LIBS="-lX11"
    
    if [[ "$BUILD_TYPE" == "Debug" ]]; then
        LINUX_FLAGS="$LINUX_FLAGS -g -O0 -DDEBUG"
    else
        LINUX_FLAGS="$LINUX_FLAGS -O3 -DNDEBUG"
    fi
    
    if [[ "$VERBOSE" == true ]]; then
        LINUX_FLAGS="$LINUX_FLAGS -v"
    fi
    
    # Build library components first
    local LINUX_SOURCES="$CORE_SOURCES $PLATFORM_FACTORY $LINUX_RENDERER"
    
    if [[ -n "$TARGET" ]]; then
        # Build specific target
        local TARGET_FILE=""
        if [[ "$TARGET" == *.cpp ]]; then
            TARGET_FILE="$EXAMPLES_DIR/$TARGET"
        else
            TARGET_FILE="$EXAMPLES_DIR/${TARGET}.cpp"
        fi
        
        if [[ ! -f "$TARGET_FILE" ]]; then
            error "Target file not found: $TARGET_FILE"
        fi
        
        local OUTPUT_NAME=$(basename "$TARGET_FILE" .cpp)
        log "Building Linux target: $OUTPUT_NAME"
        
        g++ $LINUX_FLAGS $LINUX_SOURCES "$TARGET_FILE" $LINUX_LIBS -o "$BUILD_DIR/linux/$OUTPUT_NAME"
        success "Linux build complete: $BUILD_DIR/linux/$OUTPUT_NAME"
    else
        # Build all examples
        log "Building all Linux examples..."
        for example in "$EXAMPLES_DIR"/*.cpp; do
            if [[ -f "$example" ]]; then
                local example_name=$(basename "$example" .cpp)
                log "  Building $example_name..."
                g++ $LINUX_FLAGS $LINUX_SOURCES "$example" $LINUX_LIBS -o "$BUILD_DIR/linux/$example_name"
            fi
        done
        success "All Linux examples built"
    fi
}

build_web() {
    log "Building for Web (Emscripten)..."
    
    # Check for Emscripten
    if ! command -v emcc &> /dev/null; then
        error "Emscripten not found. Install from: https://emscripten.org/docs/getting_started/downloads.html"
    fi
    
    local WEB_FLAGS="-std=c++17 -D__EMSCRIPTEN__ -I$SRC_DIR/include"
    local WEB_SETTINGS="-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1"
    WEB_SETTINGS="$WEB_SETTINGS -s EXPORTED_FUNCTIONS='[_main]'"
    WEB_SETTINGS="$WEB_SETTINGS -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]'"
    
    if [[ "$BUILD_TYPE" == "Debug" ]]; then
        WEB_FLAGS="$WEB_FLAGS -g -O0 -DDEBUG"
        WEB_SETTINGS="$WEB_SETTINGS -s ASSERTIONS=1"
    else
        WEB_FLAGS="$WEB_FLAGS -O3 -DNDEBUG"
    fi
    
    # Build library components
    local WEB_SOURCES="$CORE_SOURCES $PLATFORM_FACTORY $WEB_RENDERER"
    
    if [[ -n "$TARGET" ]]; then
        # Build specific target
        local TARGET_FILE=""
        if [[ "$TARGET" == *.cpp ]]; then
            TARGET_FILE="$EXAMPLES_DIR/$TARGET"
        else
            TARGET_FILE="$EXAMPLES_DIR/${TARGET}.cpp"
        fi
        
        if [[ ! -f "$TARGET_FILE" ]]; then
            error "Target file not found: $TARGET_FILE"
        fi
        
        local OUTPUT_NAME=$(basename "$TARGET_FILE" .cpp)
        log "Building Web target: $OUTPUT_NAME"
        
        emcc $WEB_FLAGS $WEB_SOURCES "$TARGET_FILE" $WEB_SETTINGS -o "$BUILD_DIR/web/$OUTPUT_NAME.html"
        success "Web build complete: $BUILD_DIR/web/$OUTPUT_NAME.html"
        
        log "To run: cd $BUILD_DIR/web && python3 -m http.server 8000"
        log "Then open: http://localhost:8000/$OUTPUT_NAME.html"
    else
        # Build all examples
        log "Building all Web examples..."
        for example in "$EXAMPLES_DIR"/*.cpp; do
            if [[ -f "$example" ]]; then
                local example_name=$(basename "$example" .cpp)
                log "  Building $example_name..."
                emcc $WEB_FLAGS $WEB_SOURCES "$example" $WEB_SETTINGS -o "$BUILD_DIR/web/$example_name.html"
            fi
        done
        success "All Web examples built"
        log "To run examples: cd $BUILD_DIR/web && python3 -m http.server 8000"
    fi
}

# Main build logic
case "$PLATFORM" in
    linux)
        build_linux
        ;;
    web)
        build_web
        ;;
    all)
        build_linux
        echo ""
        build_web
        ;;
    *)
        error "Unknown platform: $PLATFORM"
        ;;
esac

success "Build process completed!"