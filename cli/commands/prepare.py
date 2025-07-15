"""
Fern Prepare Command - Build for different platforms
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info
from utils.system import ProjectDetector, BuildSystem

class PrepareCommand:
    """Build Fern project for different platforms"""
    
    def execute(self, args):
        if len(args) == 0:
            print_error("Platform is required")
            print_info("Usage: fern prepare <platform>")
            print_info("Supported platforms: web, linux")
            return
        
        platform = args[0].lower()
        
        if platform not in ['web', 'linux']:
            print_error(f"Unsupported platform: {platform}")
            print_info("Supported platforms: web, linux")
            return
        
        # Check if we're in a Fern project
        project_root = ProjectDetector.find_project_root()
        if not project_root:
            print_error("Not in a Fern project directory")
            print_info("Run 'fern sprout <project_name>' to create a new project")
            return
        
        print_header(f"Building for {platform.title()}")
        
        build_system = BuildSystem(project_root)
        
        if platform == 'web':
            self._build_web(build_system)
        elif platform == 'linux':
            self._build_linux(build_system)
    
    def _build_web(self, build_system):
        """Build for web platform using Emscripten"""
        if not shutil.which("emcc"):
            print_error("Emscripten not found")
            print_info("Please install Emscripten:")
            print_info("  git clone https://github.com/emscripten-core/emsdk.git")
            print_info("  cd emsdk && ./emsdk install latest && ./emsdk activate latest")
            return
        
        try:
            # Get project structure
            structure = ProjectDetector.get_project_structure(build_system.project_root)
            main_file = structure['lib'] / 'main.cpp'
            
            if not main_file.exists():
                print_error("No main.cpp found in lib directory")
                return
            
            # Create web build directory
            web_build_dir = structure['web'] / 'build'
            web_build_dir.mkdir(parents=True, exist_ok=True)
            
            # Get Fern root directory
            fern_root = Path(__file__).parent.parent.parent
            
            print_info("Compiling with Emscripten...")
            
            # Emscripten compile command
            cmd = [
                "emcc",
                "-std=c++17",
                f"-I{fern_root}/src/cpp/include",
                f"-I{fern_root}/src/cpp/src",
                str(main_file),
                f"{fern_root}/src/cpp/src/ui/widgets/*.cpp",
                f"{fern_root}/src/cpp/src/ui/layout/*.cpp",
                f"{fern_root}/src/cpp/src/core/*.cpp",
                f"{fern_root}/src/cpp/src/graphics/*.cpp",
                f"{fern_root}/src/cpp/src/text/*.cpp",
                f"{fern_root}/src/cpp/src/font/*.cpp",
                f"{fern_root}/src/cpp/src/platform/*.cpp",
                f"{fern_root}/src/cpp/src/fern.cpp",
                "-o", str(web_build_dir / "index.html"),
                "-s", "USE_GLFW=3",
                "-s", "WASM=1",
                "-s", "ALLOW_MEMORY_GROWTH=1",
                "--shell-file", str(fern_root / "template.html")
            ]
            
            result = subprocess.run(cmd, capture_output=True, text=True, shell=True)
            
            if result.returncode != 0:
                print_error("Web build failed:")
                print(result.stderr)
                return
            
            print_success("Web build successful!")
            print_info(f"Output: {web_build_dir}/index.html")
            print_info("To serve locally, run: python -m http.server 8000")
            
        except Exception as e:
            print_error(f"Web build error: {str(e)}")
    
    def _build_linux(self, build_system):
        """Build for Linux platform"""
        if not shutil.which("g++") and not shutil.which("clang++"):
            print_error("C++ compiler not found")
            print_info("Please install g++ or clang++:")
            print_info("  Ubuntu/Debian: sudo apt-get install build-essential")
            print_info("  CentOS/RHEL: sudo yum groupinstall 'Development Tools'")
            return
        
        try:
            # Get project structure
            structure = ProjectDetector.get_project_structure(build_system.project_root)
            main_file = structure['lib'] / 'main.cpp'
            
            if not main_file.exists():
                print_error("No main.cpp found in lib directory")
                return
            
            # Create linux build directory
            linux_build_dir = structure['linux'] / 'build'
            linux_build_dir.mkdir(parents=True, exist_ok=True)
            
            # Get Fern root directory
            fern_root = Path(__file__).parent.parent.parent
            
            print_info("Compiling for Linux...")
            
            # G++ compile command
            cmd = [
                "g++",
                "-std=c++17",
                "-O2",  # Optimize for release
                f"-I{fern_root}/src/cpp/include",
                f"-I{fern_root}/src/cpp/src",
                str(main_file),
                f"{fern_root}/src/cpp/src/ui/widgets/*.cpp",
                f"{fern_root}/src/cpp/src/ui/layout/*.cpp",
                f"{fern_root}/src/cpp/src/core/*.cpp",
                f"{fern_root}/src/cpp/src/graphics/*.cpp",
                f"{fern_root}/src/cpp/src/text/*.cpp",
                f"{fern_root}/src/cpp/src/font/*.cpp",
                f"{fern_root}/src/cpp/src/platform/*.cpp",
                f"{fern_root}/src/cpp/src/fern.cpp",
                "-o", str(linux_build_dir / "app"),
                "-lX11", "-lGL", "-lGLU", "-lglfw", "-lfreetype", "-lpthread"
            ]
            
            result = subprocess.run(cmd, capture_output=True, text=True, shell=True)
            
            if result.returncode != 0:
                print_error("Linux build failed:")
                print(result.stderr)
                return
            
            print_success("Linux build successful!")
            print_info(f"Output: {linux_build_dir}/app")
            print_info(f"To run: {linux_build_dir}/app")
            
        except Exception as e:
            print_error(f"Linux build error: {str(e)}")
