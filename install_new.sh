#!/bin/bash
#
# Fern UI Framework - Enhanced Installation Script
# This script installs the complete Fern ecosystem including CLI and LSP
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Animation and UI helpers
FERN_LOGO="🌿"
SPINNER_CHARS="⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏"

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

log_step() {
    echo -e "${PURPLE}[STEP]${NC} $1"
}

# Spinner function
spinner() {
    local pid=$1
    local delay=0.1
    local spinstr=$SPINNER_CHARS
    
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c] " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b"
    done
    printf "     \b\b\b\b\b"
}

# Progress bar function
progress_bar() {
    local current=$1
    local total=$2
    local width=40
    local percentage=$((current * 100 / total))
    local completed=$((current * width / total))
    
    printf "\r["
    for ((i=0; i<completed; i++)); do printf "#"; done
    for ((i=completed; i<width; i++)); do printf "-"; done
    printf "] %d%%" $percentage
}

# Welcome screen
show_welcome() {
    clear
    echo -e "${CYAN}════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}                                                                ${NC}"
    echo -e "${CYAN}                    ${FERN_LOGO} FERN UI FRAMEWORK ${FERN_LOGO}                    ${NC}"
    echo -e "${CYAN}                                                                ${NC}"
    echo -e "${CYAN}                    Modern C++ UI Framework                    ${NC}"
    echo -e "${CYAN}                                                                ${NC}"
    echo -e "${CYAN}════════════════════════════════════════════════════════════════${NC}"
    echo
    echo -e "${BLUE}This installer will set up:${NC}"
    echo -e "  • Fern UI Framework (C++ library)"
    echo -e "  • Terra CLI (developer tools)"
    echo -e "  • Gleeb LSP (language server)"
    echo -e "  • VS Code integration"
    echo
    echo -e "${YELLOW}Installation will take 3-5 minutes depending on your system.${NC}"
    echo
}

# Interactive confirmation
confirm_installation() {
    echo -e "${CYAN}Would you like to continue with the installation? [Y/n]${NC}"
    read -r response
    if [[ "$response" =~ ^[Nn]$ ]]; then
        echo -e "${YELLOW}Installation cancelled.${NC}"
        exit 0
    fi
}

# Check if running as root
check_root() {
    if [[ $EUID -eq 0 ]]; then
        log_error "This script should not be run as root"
        log_info "Please run as a regular user. The script will prompt for sudo when needed."
        exit 1
    fi
}

# Check system requirements
check_system() {
    log_step "Checking system requirements..."
    
    # Check OS
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
    else
        log_error "Unsupported operating system: $OSTYPE"
        exit 1
    fi
    
    # Check required tools
    local missing_tools=()
    
    for tool in git curl cmake python3 make g++; do
        if ! command -v "$tool" &> /dev/null; then
            missing_tools+=("$tool")
        fi
    done
    
    if [ ${#missing_tools[@]} -ne 0 ]; then
        log_error "Missing required tools: ${missing_tools[*]}"
        log_info "Please install them using your package manager:"
        if [[ "$OS" == "linux" ]]; then
            echo "  Ubuntu/Debian: sudo apt install ${missing_tools[*]}"
            echo "  CentOS/RHEL:   sudo yum install ${missing_tools[*]}"
            echo "  Arch Linux:    sudo pacman -S ${missing_tools[*]}"
        elif [[ "$OS" == "macos" ]]; then
            echo "  macOS:         brew install ${missing_tools[*]}"
        fi
        exit 1
    fi
    
    log_success "System requirements met"
}

# Create installation directories
create_directories() {
    log_step "Creating installation directories..."
    
    INSTALL_DIR="$HOME/.fern"
    BIN_DIR="$HOME/.local/bin"
    CONFIG_DIR="$HOME/.config/fern"
    
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$BIN_DIR"
    mkdir -p "$CONFIG_DIR"
    mkdir -p "$INSTALL_DIR/tmp"
    
    log_success "Directories created"
}

# Install Node.js if needed
install_nodejs() {
    log_step "Checking Node.js installation..."
    
    if command -v node &> /dev/null; then
        node_version=$(node --version | cut -d'v' -f2 | cut -d'.' -f1)
        if [ "$node_version" -ge 16 ]; then
            log_success "Node.js $(node --version) found"
            return 0
        else
            log_warning "Node.js version too old (found: $node_version, required: 16+)"
        fi
    fi
    
    log_info "Installing Node.js..."
    
    # Use NodeSource repository for better version control
    if [[ "$OS" == "linux" ]]; then
        if command -v apt-get &> /dev/null; then
            curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - &> /dev/null
            sudo apt-get install -y nodejs &> /dev/null
        elif command -v dnf &> /dev/null; then
            curl -fsSL https://rpm.nodesource.com/setup_18.x | sudo bash - &> /dev/null
            sudo dnf install -y nodejs npm &> /dev/null
        elif command -v yum &> /dev/null; then
            curl -fsSL https://rpm.nodesource.com/setup_18.x | sudo bash - &> /dev/null
            sudo yum install -y nodejs &> /dev/null
        elif command -v pacman &> /dev/null; then
            sudo pacman -S --noconfirm nodejs npm &> /dev/null
        fi
    elif [[ "$OS" == "macos" ]]; then
        if command -v brew &> /dev/null; then
            brew install node &> /dev/null
        else
            log_error "Homebrew not found. Please install Node.js manually"
            exit 1
        fi
    fi
    
    # Verify installation
    if command -v node &> /dev/null; then
        log_success "Node.js $(node --version) installed successfully"
    else
        log_error "Node.js installation failed"
        exit 1
    fi
}

# Clone and install Terra CLI
install_terra_cli() {
    log_step "Installing Terra CLI..."
    
    cd "$INSTALL_DIR/tmp"
    
    # Clone Terra CLI from GitHub
    if [ -d "terra" ]; then
        rm -rf terra
    fi
    
    log_info "Cloning Terra CLI from GitHub..."
    git clone https://github.com/fernkit/terra.git &> /dev/null
    
    if [ ! -d "terra" ]; then
        log_error "Failed to clone Terra CLI repository"
        exit 1
    fi
    
    # Install Terra CLI
    cd terra
    
    # Copy CLI files
    cp -r cli/* "$INSTALL_DIR/terra/"
    
    # Create launcher script
    cat > "$BIN_DIR/fern" << 'EOF'
#!/bin/bash
# Fern CLI launcher
TERRA_CLI="$HOME/.fern/terra/terra_cli.py"
if [ ! -f "$TERRA_CLI" ]; then
    echo "Error: Terra CLI not found at $TERRA_CLI"
    exit 1
fi
python3 "$TERRA_CLI" "$@"
EOF
    chmod +x "$BIN_DIR/fern"
    
    # Create alias for 'terra' command
    cat > "$BIN_DIR/terra" << 'EOF'
#!/bin/bash
# Terra CLI alias
exec "$HOME/.local/bin/fern" "$@"
EOF
    chmod +x "$BIN_DIR/terra"
    
    log_success "Terra CLI installed successfully"
}

# Clone and install Gleeb LSP
install_gleeb_lsp() {
    log_step "Installing Gleeb LSP..."
    
    cd "$INSTALL_DIR/tmp"
    
    # Clone Gleeb LSP from GitHub
    if [ -d "gleeb" ]; then
        rm -rf gleeb
    fi
    
    log_info "Cloning Gleeb LSP from GitHub..."
    git clone https://github.com/fernkit/gleeb.git &> /dev/null
    
    if [ ! -d "gleeb" ]; then
        log_error "Failed to clone Gleeb LSP repository"
        exit 1
    fi
    
    # Install Gleeb LSP
    cd gleeb
    
    # Copy source files
    cp -r * "$INSTALL_DIR/gleeb/"
    
    # Install dependencies
    cd "$INSTALL_DIR/gleeb"
    log_info "Installing dependencies..."
    npm install &> /dev/null &
    spinner $!
    
    # Build LSP server
    log_info "Building LSP server..."
    npm run build &> /dev/null &
    spinner $!
    
    # Create launcher script
    cat > "$BIN_DIR/gleeb-lsp" << 'EOF'
#!/bin/bash
# Gleeb LSP Server launcher
GLEEB_DIR="$HOME/.fern/gleeb"
if [ ! -d "$GLEEB_DIR" ]; then
    echo "Error: Gleeb LSP not found at $GLEEB_DIR"
    exit 1
fi
cd "$GLEEB_DIR"
exec node dist/server.js "$@"
EOF
    chmod +x "$BIN_DIR/gleeb-lsp"
    
    log_success "Gleeb LSP installed successfully"
}

# Install Fern C++ library
install_fern_library() {
    log_step "Installing Fern C++ library..."
    
    # Build and install the library
    log_info "Building Fern library..."
    mkdir -p build
    cd build
    cmake .. &> /dev/null
    make -j$(nproc) &> /dev/null &
    spinner $!
    
    # Install library
    sudo make install &> /dev/null
    
    log_success "Fern C++ library installed"
}

# Create global configuration
create_config() {
    log_step "Creating global configuration..."
    
    cat > "$CONFIG_DIR/config.yaml" << EOF
# Fern UI Framework Configuration
fern:
  version: "$(git describe --tags --always)"
  install_date: "$(date -Iseconds)"
  
paths:
  install_dir: "$INSTALL_DIR"
  bin_dir: "$BIN_DIR"
  config_dir: "$CONFIG_DIR"
  
terra:
  cli_path: "$INSTALL_DIR/terra/terra_cli.py"
  
gleeb:
  lsp_path: "$INSTALL_DIR/gleeb/dist/server.js"
  server_command: "$BIN_DIR/gleeb-lsp"
  
build:
  default_template: "basic"
  cmake_options: "-DCMAKE_BUILD_TYPE=Release"
EOF
    
    log_success "Configuration created"
}

# Configure VS Code integration
configure_vscode() {
    log_step "Configuring VS Code integration..."
    
    # Create VS Code configuration helper
    cat > "$BIN_DIR/gleeb-configure-vscode" << 'EOF'
#!/bin/bash
# Configure VS Code for Gleeb LSP
VSCODE_SETTINGS="$HOME/.config/Code/User/settings.json"
VSCODE_SETTINGS_INSIDERS="$HOME/.config/Code - Insiders/User/settings.json"

configure_vscode_settings() {
    local settings_file="$1"
    mkdir -p "$(dirname "$settings_file")"
    
    if [ ! -f "$settings_file" ]; then
        echo '{}' > "$settings_file"
    fi
    
    python3 -c "
import json
import os
import sys

settings_file = sys.argv[1]
with open(settings_file, 'r') as f:
    try:
        settings = json.load(f)
    except json.JSONDecodeError:
        settings = {}

# Add Gleeb LSP configuration
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

settings.update(gleeb_config)

with open(settings_file, 'w') as f:
    json.dump(settings, f, indent=2)
" "$settings_file"
    
    echo "VS Code configured for Gleeb LSP"
}

# Configure both regular and insiders versions
if [ -d "$HOME/.config/Code" ]; then
    configure_vscode_settings "$VSCODE_SETTINGS"
fi

if [ -d "$HOME/.config/Code - Insiders" ]; then
    configure_vscode_settings "$VSCODE_SETTINGS_INSIDERS"
fi

echo "VS Code integration configured"
echo "Install the Gleeb LSP extension from the VS Code marketplace for best experience"
EOF
    chmod +x "$BIN_DIR/gleeb-configure-vscode"
    
    log_success "VS Code integration configured"
}

# Update PATH
update_path() {
    log_step "Updating PATH..."
    
    # Add to various shell profiles
    local shell_profiles=("$HOME/.bashrc" "$HOME/.zshrc" "$HOME/.profile")
    local path_line='export PATH="$HOME/.local/bin:$PATH"'
    
    for profile in "${shell_profiles[@]}"; do
        if [ -f "$profile" ]; then
            if ! grep -q "$path_line" "$profile"; then
                echo "" >> "$profile"
                echo "# Added by Fern UI Framework installer" >> "$profile"
                echo "$path_line" >> "$profile"
            fi
        fi
    done
    
    # Update current session
    export PATH="$HOME/.local/bin:$PATH"
    
    log_success "PATH updated"
}

# Cleanup temporary files
cleanup() {
    log_step "Cleaning up..."
    
    if [ -d "$INSTALL_DIR/tmp" ]; then
        rm -rf "$INSTALL_DIR/tmp"
    fi
    
    log_success "Cleanup completed"
}

# Installation summary
show_summary() {
    echo
    echo -e "${CYAN}════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}                                                                ${NC}"
    echo -e "${CYAN}                    ${FERN_LOGO} INSTALLATION COMPLETE ${FERN_LOGO}                    ${NC}"
    echo -e "${CYAN}                                                                ${NC}"
    echo -e "${CYAN}════════════════════════════════════════════════════════════════${NC}"
    echo
    echo -e "${GREEN}Successfully installed:${NC}"
    echo -e "  ✓ Fern UI Framework (C++ library)"
    echo -e "  ✓ Terra CLI (developer tools)"
    echo -e "  ✓ Gleeb LSP (language server)"
    echo -e "  ✓ VS Code integration"
    echo
    echo -e "${BLUE}Next steps:${NC}"
    echo -e "  1. Restart your terminal or run: source ~/.bashrc"
    echo -e "  2. Create a new project: fern sprout my_app"
    echo -e "  3. Configure VS Code: gleeb-configure-vscode"
    echo -e "  4. Install VS Code extension from marketplace"
    echo
    echo -e "${YELLOW}Documentation:${NC}"
    echo -e "  • Quick start: fern --help"
    echo -e "  • Examples: ~/.fern/examples/"
    echo -e "  • GitHub: https://github.com/fernkit/fern"
    echo
    echo -e "${PURPLE}Happy coding with Fern! ${FERN_LOGO}${NC}"
}

# Main installation flow
main() {
    show_welcome
    confirm_installation
    
    local steps=(
        "check_root"
        "check_system"
        "create_directories"
        "install_nodejs"
        "install_terra_cli"
        "install_gleeb_lsp"
        "install_fern_library"
        "create_config"
        "configure_vscode"
        "update_path"
        "cleanup"
    )
    
    local step_count=0
    local total_steps=${#steps[@]}
    
    echo -e "${BLUE}Starting installation...${NC}"
    echo
    
    for step in "${steps[@]}"; do
        ((step_count++))
        progress_bar $step_count $total_steps
        echo
        $step
    done
    
    echo
    show_summary
}

# Run main function
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
