"""
Fern Bloom Command - System health check (like flutter doctor)
"""

import sys
import os
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info
from utils.system import SystemChecker
from utils.config import config

class BloomCommand:
    """Check system health and dependencies"""
    
    def execute(self, args):
        print_header("Fern System Health Check")
        
        checker = SystemChecker()
        checks = checker.run_all_checks()
        
        # Add Fern-specific checks
        checks.extend(self._run_fern_checks())
        
        print_info("System Check Results:")
        print()
        
        passed = 0
        failed = 0
        
        for name, success, message in checks:
            if success:
                print_success(f"{name}: {message}")
                passed += 1
            else:
                print_error(f"{name}: {message}")
                failed += 1
        
        print()
        print_info(f"Health Check Summary: {passed} passed, {failed} failed")
        
        if failed == 0:
            print_success("🌿 Fern environment is healthy!")
        else:
            print_warning("Some issues detected. Please install missing dependencies.")
            
        # Provide installation suggestions
        self._show_installation_tips()
    
    def _run_fern_checks(self):
        """Run Fern-specific health checks"""
        checks = []
        
        # Check if Fern C++ library is installed
        if config.is_fern_installed():
            checks.append(("Fern C++ Library", True, "Installed globally"))
        else:
            checks.append(("Fern C++ Library", False, "Not installed globally"))
        
        # Check templates directory
        templates_path = config.get_templates_path()
        if templates_path.exists() and any(templates_path.iterdir()):
            checks.append(("Fern Templates", True, f"Available at {templates_path}"))
        else:
            checks.append(("Fern Templates", False, "No templates found"))
        
        # Check configuration
        config_file = config.config_file
        if config_file.exists():
            checks.append(("Fern Configuration", True, f"Found at {config_file}"))
        else:
            checks.append(("Fern Configuration", False, "Configuration file missing"))
        
        return checks
    
    def _show_installation_tips(self):
        """Show installation tips for common issues"""
        print()
        print_header("Installation Tips")
        
        print_info("To install Fern globally:")
        print("  cd /path/to/fern")
        print("  ./install.sh")
        print()
        
        print_info("System Dependencies:")
        print()
        print_info("Ubuntu/Debian:")
        print("  sudo apt-get update")
        print("  sudo apt-get install build-essential pkg-config cmake")
        print("  sudo apt-get install libx11-dev libxext-dev libfontconfig1-dev libfreetype6-dev")
        
        print_info("CentOS/RHEL:")
        print("  sudo yum groupinstall 'Development Tools'")
        print("  sudo yum install cmake pkgconfig libX11-devel libXext-devel fontconfig-devel freetype-devel")
        
        print_info("Arch Linux:")
        print("  sudo pacman -S base-devel cmake pkg-config libx11 libxext fontconfig freetype2")
        
        print_info("Emscripten (for web builds):")
        print("  git clone https://github.com/emscripten-core/emsdk.git")
        print("  cd emsdk")
        print("  ./emsdk install latest")
        print("  ./emsdk activate latest")
        print("  source ./emsdk_env.sh")

        print()
        print_info("After installing dependencies, run:")
        print("  fern bloom")
        print("  to verify your installation.")
