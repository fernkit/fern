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
    
    # Create Terra CLI directory
    CLI_DIR="$HOME/.fern/cli"
    mkdir -p "$CLI_DIR"
    
    # Copy CLI files from local or clone from GitHub
    if [ -d "cli" ]; then
        log_info "Using local Terra CLI source..."
        cp -r cli/* "$CLI_DIR/"
    else
        log_info "Cloning Terra CLI from GitHub..."
        TEMP_DIR=$(mktemp -d)
        cd "$TEMP_DIR"
        
        if git clone https://github.com/fernkit/terra.git; then
            cp -r terra/cli/* "$CLI_DIR/"
            cd - > /dev/null
            rm -rf "$TEMP_DIR"
        else
            log_error "Failed to clone Terra CLI from GitHub"
            log_error "Please check your internet connection and try again"
            cd - > /dev/null
            rm -rf "$TEMP_DIR"
            return 1
        fi
    fi
    
    # Create ~/.local/bin directory if it doesn't exist
    mkdir -p "$HOME/.local/bin"
    
    # Create global launcher script
    cat > "$HOME/.local/bin/fern" << 'EOF'
#!/bin/bash
# Fern CLI launcher
CLI_DIR="$HOME/.fern/cli"
if [ ! -f "$CLI_DIR/terra_cli.py" ]; then
    echo "Error: Terra CLI not found at $CLI_DIR"
    exit 1
fi
# Set the original working directory as an environment variable
export ORIGINAL_CWD="$(pwd)"
cd "$CLI_DIR"
python3 terra_cli.py "$@"
EOF
    chmod +x "$HOME/.local/bin/fern"
    
    # Create alias for 'terra' command
    cat > "$HOME/.local/bin/terra" << 'EOF'
#!/bin/bash
# Terra CLI alias
exec "$HOME/.local/bin/fern" "$@"
EOF
    chmod +x "$HOME/.local/bin/terra"
    
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

# Install Gleeb LSP Server
install_gleeb_lsp() {
    log_info "Installing Gleeb LSP Server..."
    
    # Check if Node.js is installed
    if ! command -v node &> /dev/null; then
        log_info "Installing Node.js..."
        
        # For Ubuntu/Debian
        if command -v apt-get &> /dev/null; then
            log_info "Installing Node.js via apt..."
            curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
            sudo apt-get install -y nodejs
        # For CentOS/RHEL/Fedora
        elif command -v dnf &> /dev/null; then
            log_info "Installing Node.js via dnf..."
            sudo dnf install -y npm nodejs
        elif command -v yum &> /dev/null; then
            log_info "Installing Node.js via yum..."
            curl -fsSL https://rpm.nodesource.com/setup_18.x | sudo bash -
            sudo yum install -y nodejs
        # For macOS
        elif command -v brew &> /dev/null; then
            log_info "Installing Node.js via homebrew..."
            brew install node
        # For Arch Linux
        elif command -v pacman &> /dev/null; then
            log_info "Installing Node.js via pacman..."
            sudo pacman -S nodejs npm
        else
            log_warning "Could not install Node.js automatically"
            log_warning "Please install Node.js 16+ manually and re-run this script"
            return 1
        fi
    fi
    
    # Verify Node.js installation
    if ! command -v node &> /dev/null; then
        log_error "Node.js installation failed"
        return 1
    fi
    
    # Check Node.js version
    node_version=$(node --version | cut -d'v' -f2 | cut -d'.' -f1)
    if [ "$node_version" -lt 16 ]; then
        log_warning "Node.js version is too old (found: $node_version, required: 16+)"
        log_warning "Please update Node.js manually"
        return 1
    fi
    
    log_success "Node.js $(node --version) found"
    
    # Install Gleeb LSP
    GLEEB_DIR="$HOME/.fern/gleeb"
    mkdir -p "$GLEEB_DIR"
    
    # Clone Gleeb LSP from GitHub if not present locally
    if [ -d "gleeb" ]; then
        log_info "Using local Gleeb LSP source..."
        cp -r gleeb/* "$GLEEB_DIR/"
    else
        log_info "Cloning Gleeb LSP from GitHub..."
        TEMP_DIR=$(mktemp -d)
        cd "$TEMP_DIR"
        
        if git clone https://github.com/fernkit/gleeb.git; then
            cp -r gleeb/* "$GLEEB_DIR/"
            cd - > /dev/null
            rm -rf "$TEMP_DIR"
        else
            log_error "Failed to clone Gleeb LSP from GitHub"
            log_error "Please check your internet connection and try again"
            cd - > /dev/null
            rm -rf "$TEMP_DIR"
            return 1
        fi
    fi
    
    # Install dependencies and build
    log_info "Installing Gleeb LSP dependencies..."
    cd "$GLEEB_DIR"
    npm install
    
    log_info "Building Gleeb LSP..."
    npm run build
    cd - > /dev/null
    
    # Create global launcher script
    cat > "$HOME/.local/bin/gleeb-lsp" << 'EOF'
#!/bin/bash
# Gleeb LSP Server launcher
GLEEB_DIR="$HOME/.fern/gleeb"
if [ ! -d "$GLEEB_DIR" ]; then
    echo "Error: Gleeb LSP not found at $GLEEB_DIR"
    exit 1
fi
cd "$GLEEB_DIR"
node dist/server.js "$@"
EOF
    chmod +x "$HOME/.local/bin/gleeb-lsp"
    
    # Create VS Code configuration helper
    cat > "$HOME/.local/bin/gleeb-configure-vscode" << 'EOF'
#!/bin/bash
# Configure VS Code for Gleeb LSP
VSCODE_SETTINGS="$HOME/.config/Code/User/settings.json"
VSCODE_SETTINGS_INSIDERS="$HOME/.config/Code - Insiders/User/settings.json"

configure_vscode_settings() {
    local settings_file="$1"
    mkdir -p "$(dirname "$settings_file")"
    
    # Create settings file if it doesn't exist
    if [ ! -f "$settings_file" ]; then
        echo '{}' > "$settings_file"
    fi
    
    # Add Gleeb LSP configuration
    python3 -c "
import json
import os
import sys

settings_file = sys.argv[1]
gleeb_config = {
    'gleeb.enable': True,
    'gleeb.server.command': os.path.expanduser('~/.local/bin/gleeb-lsp'),
    'gleeb.server.args': ['--stdio'],
    'gleeb.diagnostics.enable': True,
    'gleeb.completion.enable': True,
    'gleeb.hover.enable': True,
    'gleeb.maxNumberOfProblems': 100,
    'gleeb.trace.server': 'off'
}

try:
    with open(settings_file, 'r') as f:
        settings = json.load(f)
except:
    settings = {}

settings.update(gleeb_config)

with open(settings_file, 'w') as f:
    json.dump(settings, f, indent=2)

print(f'✅ VS Code configured for Gleeb LSP: {settings_file}')
" "$settings_file"
}

# Configure regular VS Code
if [ -d "$HOME/.config/Code" ]; then
    configure_vscode_settings "$VSCODE_SETTINGS"
fi

# Configure VS Code Insiders
if [ -d "$HOME/.config/Code - Insiders" ]; then
    configure_vscode_settings "$VSCODE_SETTINGS_INSIDERS"
fi

# Check if any VS Code was configured
if [ ! -d "$HOME/.config/Code" ] && [ ! -d "$HOME/.config/Code - Insiders" ]; then
    echo "⚠️  VS Code not found. Install VS Code and run this command again."
    echo "   Or manually add Gleeb LSP configuration to your settings.json"
fi
EOF
    chmod +x "$HOME/.local/bin/gleeb-configure-vscode"
    
    log_success "Gleeb LSP installed successfully"
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
    
    # Install Gleeb LSP
    install_gleeb_lsp
    
    # Setup global templates and config
    setup_global_templates
    setup_global_config
    
    echo "========================================"
    log_success "Fern UI Framework with Terra CLI installed successfully!"
    echo "========================================"
    
    echo ""
    echo "You can now use the following commands:"
    echo "  fern bloom        - Check system health"
    echo "  fern sprout       - Create new project"
    echo "  fern fire         - Run project"
    echo "  fern prepare      - Build project"
    echo "  fern templates    - Manage templates"
    echo "  fern lsp start    - Start LSP server"
    echo "  fern lsp stop     - Stop LSP server"
    echo "  fern lsp config   - Configure editor"
    echo "  fern lsp status   - Check LSP status"
    echo ""
    echo "LSP Tools:"
    echo "  gleeb-lsp                  - Start Gleeb LSP server"
    echo "  gleeb-configure-vscode     - Configure VS Code for Gleeb LSP"
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
