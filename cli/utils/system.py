"""
System utilities for Fern CLI
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path
from .colors import print_success, print_error, print_warning, print_info

class SystemChecker:
    """Check system dependencies and health"""
    
    def __init__(self):
        self.checks = []
    
    def check_command(self, command, name, required=True):
        """Check if a command is available"""
        if shutil.which(command):
            self.checks.append((name, True, f"{command} found"))
            return True
        else:
            self.checks.append((name, False, f"{command} not found"))
            return False
    
    def check_file(self, filepath, name, required=True):
        """Check if a file exists"""
        if Path(filepath).exists():
            self.checks.append((name, True, f"Found at {filepath}"))
            return True
        else:
            self.checks.append((name, False, f"Not found at {filepath}"))
            return False
    
    def check_directory(self, dirpath, name, required=True):
        """Check if a directory exists"""
        if Path(dirpath).is_dir():
            self.checks.append((name, True, f"Found at {dirpath}"))
            return True
        else:
            self.checks.append((name, False, f"Not found at {dirpath}"))
            return False
    
    def run_all_checks(self):
        """Run all system checks"""
        print_info("Running system health checks...")
        
        # Check C++ compiler
        self.check_command("g++", "C++ Compiler (g++)")
        self.check_command("clang++", "C++ Compiler (clang++)", required=False)
        
        # Check Emscripten for web builds
        self.check_command("emcc", "Emscripten (Web builds)", required=False)
        
        # Check system libraries
        self.check_command("pkg-config", "pkg-config")
        
        # Check X11 for Linux
        if sys.platform.startswith('linux'):
            self.check_command("pkg-config --exists x11", "X11 Development Libraries", required=False)
        
        # Check Fern installation
        fern_root = Path(__file__).parent.parent.parent
        self.check_directory(fern_root / "src", "Fern Source Code")
        self.check_directory(fern_root / "examples", "Fern Examples")
        
        return self.checks

class ProjectDetector:
    """Detect Fern project structure"""
    
    @staticmethod
    def is_fern_project(path="."):
        """Check if current directory is a Fern project"""
        project_path = Path(path)
        return (project_path / "fern.yaml").exists() or (project_path / "fern.toml").exists()
    
    @staticmethod
    def find_project_root(start_path="."):
        """Find the root of a Fern project"""
        current = Path(start_path).resolve()
        while current != current.parent:
            if ProjectDetector.is_fern_project(current):
                return current
            current = current.parent
        return None
    
    @staticmethod
    def get_project_structure(project_root):
        """Get the structure of a Fern project"""
        root = Path(project_root)
        structure = {
            'lib': root / "lib",
            'web': root / "web",
            'linux': root / "linux",
            'examples': root / "examples",
            'assets': root / "assets",
            'config': root / "fern.yaml" if (root / "fern.yaml").exists() else root / "fern.toml"
        }
        return structure

class BuildSystem:
    """Handle build operations"""
    
    def __init__(self, project_root):
        self.project_root = Path(project_root)
        self.fern_root = Path(__file__).parent.parent.parent
    
    def get_source_files(self, directory):
        """Get all C++ source files in a directory"""
        cpp_files = []
        for ext in ['*.cpp', '*.cxx', '*.cc']:
            cpp_files.extend(Path(directory).glob(f"**/{ext}"))
        return cpp_files
    
    def needs_rebuild(self, source_files, target):
        """Check if rebuild is needed based on file timestamps"""
        if not Path(target).exists():
            return True
        
        target_time = Path(target).stat().st_mtime
        for source in source_files:
            if Path(source).stat().st_mtime > target_time:
                return True
        return False
    
    def build_web(self, main_file=None):
        """Build for web platform using Emscripten"""
        if not shutil.which("emcc"):
            print_error("Emscripten not found. Please install Emscripten for web builds.")
            return False
        
        # Implementation for web build
        print_info("Building for web platform...")
        return True
    
    def build_linux(self, main_file=None):
        """Build for Linux platform"""
        if not shutil.which("g++") and not shutil.which("clang++"):
            print_error("C++ compiler not found. Please install g++ or clang++.")
            return False
        
        # Implementation for Linux build
        print_info("Building for Linux platform...")
        return True
