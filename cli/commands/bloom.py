"""
Fern Bloom Command - System health check
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
        
        # Parse arguments
        show_troubleshooting = "--troubleshoot" in args or "-t" in args
        
        checker = SystemChecker()
        checks = checker.run_all_checks()
        
        # Add Fern-specific checks
        checks.extend(self._run_fern_checks())
        
        print_info("System Check Results:")
        print()
        
        passed = 0
        failed = 0
        critical_failed = 0
        
        # Define critical checks that prevent Fern from working
        critical_checks = [
            "C++ Compiler (g++)", 
            "pkg-config",
            "Fern C++ Library"
        ]
        
        for name, success, message in checks:
            if success:
                print_success(f"{name}: {message}")
                passed += 1
            else:
                # Check if this is a critical failure
                if any(critical in name for critical in critical_checks):
                    print_error(f"{name}: {message}")
                    critical_failed += 1
                else:
                    print_warning(f"{name}: {message} (optional)")
                failed += 1
        
        print()
        print_info(f"Health Check Summary: {passed} passed, {failed} failed")
        
        if failed == 0:
            print_success("🌿 Fern environment is healthy and ready to use!")
        elif critical_failed > 0:
            print_error("Critical dependencies missing. Fern may not work properly.")
            print_info("Run 'fern bloom --troubleshoot' for installation help.")
        else:
            print_warning("Some optional dependencies missing. Core functionality available.")
            print_info("Run 'fern bloom --troubleshoot' for optimization tips.")
            
        # Show troubleshooting only when requested or when critical issues exist
        if show_troubleshooting or critical_failed > 0:
            self._show_installation_tips(critical_failed > 0)
    
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
    
    def _show_installation_tips(self, critical_issues=False):
        """Show installation tips for common issues"""
        print()
        if critical_issues:
            print_header("Critical Issues - Installation Required")
            print_error("Fern cannot function without these dependencies.")
        else:
            print_header("Troubleshooting & Optimization")
            print_info("Optional improvements for better Fern experience.")
        
        print()
        print_info("System Dependencies by Platform:")
        print()
        
        print_info("Ubuntu/Debian:")
        print("  sudo apt-get update")
        print("  sudo apt-get install build-essential pkg-config cmake make")
        print("  sudo apt-get install libx11-dev libxext-dev libfontconfig1-dev libfreetype6-dev")
        print()
        
        print_info("CentOS/RHEL/Fedora:")
        print("  sudo dnf groupinstall 'Development Tools'")
        print("  sudo dnf install cmake pkgconfig make libX11-devel libXext-devel fontconfig-devel freetype-devel")
        print()
        
        print_info("Arch Linux:")
        print("  sudo pacman -S base-devel cmake pkg-config make libx11 libxext fontconfig freetype2")
        print()
        
        if critical_issues:
            print_info("After installing system dependencies:")
            print("  cd /path/to/fern")
            print("  ./install.sh")
            print()
        
        print_info("For Web Development (Emscripten):")
        print("  git clone https://github.com/emscripten-core/emsdk.git")
        print("  cd emsdk")
        print("  ./emsdk install latest")
        print("  ./emsdk activate latest")
        print("  source ./emsdk_env.sh")
        print()

        print_info("Verify Installation:")
        print("  fern bloom          # Check system health")
        print("  fern sprout myapp   # Create test project")
        print("  cd myapp && fern fire  # Test build and run")
        print()
        
        if not critical_issues:
            print_info("Command Line Options:")
            print("  fern bloom --troubleshoot   # Show this help anytime")
            print("  fern bloom -t               # Short form")
            print("  fern --help                 # General Fern help")
