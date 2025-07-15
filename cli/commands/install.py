"""
Fern Install Command - Install packages and dependencies
"""

import os
import sys
import subprocess
import requests
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info

class InstallCommand:
    """Install Fern packages and dependencies"""
    
    def execute(self, args):
        if len(args) == 0:
            print_error("Package name is required")
            print_info("Usage: fern install <package_name>")
            print_info("Example: fern install audio")
            return
        
        package_name = args[0]
        
        print_header(f"Installing {package_name}")
        
        # Check if we're in a Fern project
        project_root = self._find_project_root()
        if not project_root:
            print_error("Not in a Fern project directory")
            print_info("Run 'fern sprout <project_name>' to create a new project")
            return
        
        # Install the package
        if self._install_package(package_name, project_root):
            print_success(f"Package '{package_name}' installed successfully!")
        else:
            print_error(f"Failed to install package '{package_name}'")
    
    def _find_project_root(self):
        """Find the root of a Fern project"""
        current = Path('.').resolve()
        while current != current.parent:
            if (current / "fern.yaml").exists() or (current / "fern.toml").exists():
                return current
            current = current.parent
        return None
    
    def _install_package(self, package_name, project_root):
        """Install a Fern package"""
        try:
            # For now, we'll implement a simple package system
            # In the future, this could fetch from a package registry
            
            if package_name == "audio":
                return self._install_audio_package(project_root)
            elif package_name == "networking":
                return self._install_networking_package(project_root)
            elif package_name == "json":
                return self._install_json_package(project_root)
            else:
                print_error(f"Unknown package: {package_name}")
                self._show_available_packages()
                return False
                
        except Exception as e:
            print_error(f"Installation error: {str(e)}")
            return False
    
    def _install_audio_package(self, project_root):
        """Install audio package"""
        print_info("Installing audio package...")
        
        # Create package directory
        package_dir = project_root / "packages" / "audio"
        package_dir.mkdir(parents=True, exist_ok=True)
        
        # Create audio header
        audio_header = """#pragma once
#include <string>

namespace Fern {
    class Audio {
    public:
        static void playSound(const std::string& filename);
        static void playMusic(const std::string& filename);
        static void stopMusic();
        static void setVolume(float volume);
    };
}
"""
        
        (package_dir / "audio.hpp").write_text(audio_header)
        
        # Create audio implementation
        audio_impl = """#include "audio.hpp"
#include <iostream>

namespace Fern {
    void Audio::playSound(const std::string& filename) {
        std::cout << "Playing sound: " << filename << std::endl;
        // TODO: Implement actual audio playback
    }
    
    void Audio::playMusic(const std::string& filename) {
        std::cout << "Playing music: " << filename << std::endl;
        // TODO: Implement actual music playback
    }
    
    void Audio::stopMusic() {
        std::cout << "Stopping music" << std::endl;
        // TODO: Implement music stopping
    }
    
    void Audio::setVolume(float volume) {
        std::cout << "Setting volume: " << volume << std::endl;
        // TODO: Implement volume control
    }
}
"""
        
        (package_dir / "audio.cpp").write_text(audio_impl)
        
        print_info("Audio package installed at packages/audio/")
        return True
    
    def _install_networking_package(self, project_root):
        """Install networking package"""
        print_info("Installing networking package...")
        
        # Create package directory
        package_dir = project_root / "packages" / "networking"
        package_dir.mkdir(parents=True, exist_ok=True)
        
        # Create networking header
        networking_header = """#pragma once
#include <string>
#include <functional>

namespace Fern {
    class Http {
    public:
        static void get(const std::string& url, std::function<void(const std::string&)> callback);
        static void post(const std::string& url, const std::string& data, std::function<void(const std::string&)> callback);
    };
}
"""
        
        (package_dir / "networking.hpp").write_text(networking_header)
        
        print_info("Networking package installed at packages/networking/")
        return True
    
    def _install_json_package(self, project_root):
        """Install JSON package"""
        print_info("Installing JSON package...")
        
        # Create package directory
        package_dir = project_root / "packages" / "json"
        package_dir.mkdir(parents=True, exist_ok=True)
        
        # Create JSON header
        json_header = """#pragma once
#include <string>
#include <map>

namespace Fern {
    class Json {
    public:
        static std::map<std::string, std::string> parse(const std::string& json);
        static std::string stringify(const std::map<std::string, std::string>& data);
    };
}
"""
        
        (package_dir / "json.hpp").write_text(json_header)
        
        print_info("JSON package installed at packages/json/")
        return True
    
    def _show_available_packages(self):
        """Show available packages"""
        print_info("Available packages:")
        print("  audio      - Audio playback support")
        print("  networking - HTTP client support")
        print("  json       - JSON parsing support")
