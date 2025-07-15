#!/bin/bash
#
# Global installation script for Fern UI Framework
# This script installs both the CLI and C++ library globally
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Helper functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if running as root
check_root() {
    if [[ $EUID -eq 0 ]]; then
        log_error "This script should not be run as root"
        exit 1
    fi
}

# Check dependencies
check_dependencies() {
    log_info "Checking dependencies..."
    
    # Check Python
    if ! command -v python3 &> /dev/null; then
        log_error "Python 3 is required but not installed"
        exit 1
    fi
    
    # Check pip
    if ! command -v pip3 &> /dev/null; then
        log_error "pip3 is required but not installed"
        exit 1
    fi
    
    # Check C++ compiler
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        log_error "C++ compiler (g++ or clang++) is required but not installed"
        exit 1
    fi
    
    # Check CMake
    if ! command -v cmake &> /dev/null; then
        log_error "CMake is required but not installed"
        exit 1
    fi
    
    # Check pkg-config
    if ! command -v pkg-config &> /dev/null; then
        log_error "pkg-config is required but not installed"
        exit 1
    fi
    
    log_success "All dependencies are installed"
}

# Install Python CLI (Terra)
install_cli() {
    log_info "Installing Terra CLI..."
    
    # Check if Terra CLI is already installed
    TERRA_DIR="$HOME/.fern/terra"
    
    if [ -d "$TERRA_DIR" ]; then
        log_info "Terra CLI directory exists, updating..."
        cd "$TERRA_DIR"
        git pull origin master
        cd - > /dev/null
    else
        log_info "Cloning Terra CLI repository..."
        mkdir -p "$HOME/.fern"
        cd "$HOME/.fern"
        git clone https://github.com/fernkit/terra.git
        cd - > /dev/null
    fi
    
    # Create ~/.local/bin directory if it doesn't exist
    mkdir -p "$HOME/.local/bin"
    
    # Create a simple launcher script
    cat > "$HOME/.local/bin/fern" << 'EOF'
#!/usr/bin/env python3
import sys
import os
from pathlib import Path

# Add Terra CLI directory to path
TERRA_CLI_DIR = Path.home() / ".fern" / "terra" / "cli"
sys.path.insert(0, str(TERRA_CLI_DIR))

from terra_cli import main

if __name__ == "__main__":
    main()
EOF
    
    # Make it executable
    chmod +x "$HOME/.local/bin/fern"
    
    # Check if ~/.local/bin is in PATH
    if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
        log_warning "~/.local/bin is not in your PATH"
        log_warning "Add the following line to your ~/.bashrc or ~/.zshrc:"
        echo "export PATH=\"\$HOME/.local/bin:\$PATH\""
        
        # Temporarily add to PATH for this session
        export PATH="$HOME/.local/bin:$PATH"
    fi
    
    log_success "Terra CLI installed successfully"
}

# Install C++ library
install_cpp_library() {
    log_info "Installing Fern C++ library..."
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure with CMake
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$HOME/.local" ..
    
    # Build
    make -j$(nproc)
    
    # Install
    make install
    
    cd ..
    
    log_success "Fern C++ library installed successfully"
}

# Create global templates directory
setup_global_templates() {
    log_info "Setting up global templates..."
    
    TEMPLATES_DIR="$HOME/.fern/templates"
    mkdir -p "$TEMPLATES_DIR"
    
    # Copy default templates
    if [ -d "cli/templates" ]; then
        cp -r cli/templates/* "$TEMPLATES_DIR/"
    fi
    
    log_success "Global templates installed to $TEMPLATES_DIR"
}

# Create global configuration
setup_global_config() {
    log_info "Setting up global configuration..."
    
    CONFIG_DIR="$HOME/.fern"
    mkdir -p "$CONFIG_DIR"
    
    cat > "$CONFIG_DIR/config.yaml" << EOF
# Fern UI Framework Global Configuration
version: "0.1.0"
cpp_library_path: "$HOME/.local"
templates_path: "$HOME/.fern/templates"
default_template: "basic"
build:
  default_flags: ["-std=c++17", "-O2"]
  debug_flags: ["-std=c++17", "-g", "-O0"]
  include_paths: ["$HOME/.local/include"]
  library_paths: ["$HOME/.local/lib"]
  libraries: ["fern", "X11", "Xext", "fontconfig", "freetype"]
EOF
    
    log_success "Global configuration created at $CONFIG_DIR/config.yaml"
}

# Main installation function
main() {
    echo "========================================"
    echo "Fern UI Framework Global Installation"
    echo "Terra CLI + Fern Core Framework"
    echo "========================================"
    
    check_root
    check_dependencies
    
    # Install CLI
    install_cli
    
    # Install C++ library
    install_cpp_library
    
    # Setup global templates and config
    setup_global_templates
    setup_global_config
    
    echo "========================================"
    log_success "Fern UI Framework with Terra CLI installed successfully!"
    echo "========================================"
    
    echo ""
    echo "You can now use the following commands:"
    echo "  fern bloom     - Check system health"
    echo "  fern sprout    - Create new project"
    echo "  fern fire      - Run project"
    echo "  fern prepare   - Build project"
    echo "  fern templates - Manage templates"
    echo ""
    
    # Test installation
    log_info "Testing installation..."
    if command -v fern &> /dev/null; then
        log_success "Terra CLI is available as 'fern' command"
        fern bloom
    else
        log_warning "Terra CLI might not be in PATH. You may need to restart your terminal."
    fi
}

# Run main function
main "$@"
