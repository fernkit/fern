# Fern UI Framework - Installation Guide

Fern is a modern UI framework for C++ inspired by Flutter. This guide will help you install Fern globally on your system.

## Quick Install

```bash
# Clone the repository
git clone https://github.com/fern-ui/fern.git
cd fern

# Run global installation
./install.sh
```

## What Gets Installed

The installation script will:

1. **Install Fern CLI** - Python command-line interface
2. **Install Fern C++ Library** - Static library with headers
3. **Setup Global Templates** - Project templates
4. **Create Global Configuration** - Default build settings

## System Requirements

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config python3 python3-pip
sudo apt-get install libx11-dev libxext-dev libfontconfig1-dev libfreetype6-dev
```

### CentOS/RHEL
```bash
sudo yum groupinstall 'Development Tools'
sudo yum install cmake pkgconfig python3 python3-pip
sudo yum install libX11-devel libXext-devel fontconfig-devel freetype-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel cmake pkg-config python python-pip
sudo pacman -S libx11 libxext fontconfig freetype2
```

## Installation Locations

After installation, files will be located at:

- **CLI**: `~/.local/bin/fern`
- **C++ Library**: `~/.local/lib/libfern.a`
- **Headers**: `~/.local/include/fern/`
- **Templates**: `~/.fern/templates/`
- **Configuration**: `~/.fern/config.yaml`

## Verify Installation

```bash
# Check system health
fern bloom

# Create a test project
fern sprout my_test_app
cd my_test_app
fern fire
```

## Usage

### Create New Project
```bash
fern sprout my_app
cd my_app
```

### Run Project
```bash
fern fire
```

### Run Single File
```bash
fern fire my_file.cpp
```

### Build for Distribution
```bash
fern prepare linux
```

### Check System Health
```bash
fern bloom
```

## Global Configuration

The global configuration file at `~/.fern/config.yaml` contains:

```yaml
version: "0.1.0"
cpp_library_path: "/home/user/.local"
templates_path: "/home/user/.fern/templates"
default_template: "basic"
build:
  default_flags: ["-std=c++17", "-O2"]
  debug_flags: ["-std=c++17", "-g", "-O0"]
  include_paths: ["/home/user/.local/include"]
  library_paths: ["/home/user/.local/lib"]
  libraries: ["fern", "X11", "Xext", "fontconfig", "freetype"]
```

## Benefits of Global Installation

1. **No Source Code Modification**: Projects only contain your application code
2. **Fast Project Creation**: No need to copy library files to each project
3. **Consistent Builds**: All projects use the same Fern version
4. **Easy Updates**: Update Fern once, all projects benefit
5. **Clean Project Structure**: Projects are focused on your application logic

## Troubleshooting

### CLI Not Found
If `fern` command is not found, add `~/.local/bin` to your PATH:

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### Missing Dependencies
Run `fern bloom` to check for missing system dependencies.

### Permission Issues
Don't run the installation script as root. It installs to user directories.

## Uninstallation

```bash
# Remove CLI
pip3 uninstall fern-ui

# Remove C++ library
rm -rf ~/.local/lib/libfern.a
rm -rf ~/.local/include/fern/

# Remove configuration
rm -rf ~/.fern/
```

## Development

For development, install in editable mode:

```bash
pip3 install -e .
```

This allows you to modify the CLI code and see changes immediately.
