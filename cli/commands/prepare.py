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
from utils.config import config

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
        """Build for web platform using Emscripten with optimized caching"""
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
                print_info("Create lib/main.cpp with your Fern code")
                return
            
            # Create web build directory
            web_build_dir = structure['web'] / 'build'
            web_build_dir.mkdir(parents=True, exist_ok=True)
            
            # Find the Fern source directory (same logic as fire command)
            fern_source = self._find_fern_source()
            if not fern_source:
                return
            
            # Check if we have a precompiled Fern web library, or build one
            fern_web_lib = self._ensure_fern_web_library(fern_source)
            if not fern_web_lib:
                return
            
            print_info("Compiling with Emscripten...")
            
            # Build command using Emscripten - uses precompiled library
            cmd = ["emcc"]
            
            # Add Emscripten flags for production build
            cmd.extend(["-std=c++17", "-O3"])  # O3 for production
            cmd.extend(["-s", "WASM=1"])
            cmd.extend(["-s", "ALLOW_MEMORY_GROWTH=1"])
            cmd.extend(["-s", "USE_WEBGL2=1"])
            cmd.extend(["-s", "EXPORTED_FUNCTIONS=['_main']"])
            cmd.extend(["-s", "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"])
            
            # Add the source include path
            cmd.extend(["-I", str(fern_source / "include")])

            # Add the project's main file
            cmd.append(str(main_file))

            # Link against the precompiled Fern web library
            cmd.append(str(fern_web_lib))
            
            # Check for custom template
            project_template = structure['web'] / "template.html"
            global_template = Path(__file__).parent.parent.parent / "template.html"
            
            if project_template.exists():
                cmd.extend(["--shell-file", str(project_template)])
            elif global_template.exists():
                cmd.extend(["--shell-file", str(global_template)])
            
            # Add output file
            cmd.extend(["-o", str(web_build_dir / "index.html")])
            
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Web build failed:")
                print(result.stderr)
                return
            
            print_success("Web build successful!")
            print_info(f"Output: {web_build_dir}/index.html")
            print_info(f"Files generated:")
            print_info(f"  • {web_build_dir}/index.html")
            print_info(f"  • {web_build_dir}/index.js")
            print_info(f"  • {web_build_dir}/index.wasm")
            print_info("")
            print_info("To serve locally:")
            print_info(f"  cd {web_build_dir}")
            print_info("  python3 -m http.server 8000")
            print_info("  Open: http://localhost:8000/index.html")
            
        except Exception as e:
            print_error(f"Web build error: {str(e)}")
    
    def _build_linux(self, build_system):
        """Build for Linux platform"""
        if not config.is_fern_installed():
            print_error("Fern C++ library is not installed globally")
            print_info("Run './install.sh' from the Fern source directory to install")
            return
        
        try:
            # Get project structure
            structure = ProjectDetector.get_project_structure(build_system.project_root)
            main_file = structure['lib'] / 'main.cpp'
            
            if not main_file.exists():
                print_error("No main.cpp found in lib directory")
                print_info("Create lib/main.cpp with your Fern code")
                return
            
            # Create linux build directory
            linux_build_dir = structure['linux'] / 'build'
            linux_build_dir.mkdir(parents=True, exist_ok=True)
            
            print_info("Compiling for Linux...")
            
            # Build command using global configuration
            cmd = ["g++"]
            
            # Add production build flags
            cmd.extend(["-std=c++17", "-O3", "-DNDEBUG"])  # O3 + NDEBUG for production
            
            # Add include paths
            for include_path in config.get_include_paths():
                cmd.extend(["-I", include_path])
            
            # Add source file
            cmd.append(str(main_file))
            
            # Add library paths
            for lib_path in config.get_library_paths():
                cmd.extend(["-L", lib_path])
            
            # Add libraries
            for lib in config.get_libraries():
                cmd.extend(["-l", lib])
            
            # Add output
            output_file = linux_build_dir / build_system.project_root.name
            cmd.extend(["-o", str(output_file)])
            
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Linux build failed:")
                print(result.stderr)
                return
            
            # Make executable
            output_file.chmod(0o755)
            
            print_success("Linux build successful!")
            print_info(f"Output: {output_file}")
            print_info(f"Size: {output_file.stat().st_size / 1024:.1f} KB")
            print_info("")
            print_info("To run:")
            print_info(f"  {output_file}")
            print_info("")
            print_info("To distribute:")
            print_info(f"  cp {output_file} /path/to/distribution/")
            
        except Exception as e:
            print_error(f"Linux build error: {str(e)}")
    
    def _find_fern_source(self):
        """Find the Fern source directory for web builds"""
        # Get the directory where the CLI is located (should be the Fern repo)
        cli_dir = Path(__file__).parent.parent.parent  # Go up from cli/commands/prepare.py to repo root
        
        potential_sources = [
            Path.home() / ".fern",  # Global source installation (primary location)
            cli_dir,  # The Fern repository root where the CLI is located
            Path(os.getcwd()),  # Current working directory (if run from Fern repo)
            Path(os.environ.get('ORIGINAL_CWD', os.getcwd())).parent,  # Parent of original working dir
            Path("/usr/local/src/fern"),  # System-wide source location
            Path.home() / ".fern" / "src"  # Alternative user location
        ]
        
        for src_path in potential_sources:
            # Check if this looks like the Fern source directory
            cpp_src = src_path / "src" / "cpp"
            if (cpp_src.exists() and 
                (cpp_src / "include" / "fern").exists() and
                (cpp_src / "src").exists()):
                print_info(f"Found Fern source for web build at: {cpp_src}")
                return cpp_src

        print_error("Fern source files not found for web compilation.")
        print_info("Web builds require access to Fern source files.")
        print_info("Run './install.sh' from the Fern repository to install source files globally.")
        return None

    def _ensure_fern_web_library(self, fern_source):
        """Ensure a precompiled Fern web library exists, building it if necessary"""
        # Create a cache directory for precompiled web libraries
        cache_dir = Path.home() / ".fern" / "cache" / "web"
        cache_dir.mkdir(parents=True, exist_ok=True)
        
        # Check if we need to rebuild by comparing source timestamps
        lib_file = cache_dir / "libfern_web.a"
        needs_rebuild = True
        
        if lib_file.exists():
            lib_mtime = lib_file.stat().st_mtime
            
            # Check if any source file is newer than the library
            source_newer = False
            patterns = ["core/*.cpp", "graphics/*.cpp", "text/*.cpp", "font/*.cpp", "ui/**/*.cpp"]
            
            for pattern in patterns:
                for src_file in fern_source.glob(f"src/{pattern}"):
                    if src_file.stat().st_mtime > lib_mtime:
                        source_newer = True
                        break
                if source_newer:
                    break
            
            # Also check platform files
            platform_files = [
                fern_source / "src/platform/web_renderer.cpp",
                fern_source / "src/platform/platform_factory.cpp", 
                fern_source / "src/fern.cpp"
            ]
            
            for platform_file in platform_files:
                if platform_file.exists() and platform_file.stat().st_mtime > lib_mtime:
                    source_newer = True
                    break
            
            needs_rebuild = source_newer
        
        if needs_rebuild:
            print_info("Building Fern web library (this may take a moment)...")
            
            # Collect all source files
            source_files = []
            patterns = ["core/*.cpp", "graphics/*.cpp", "text/*.cpp", "font/*.cpp", "ui/**/*.cpp"]
            for pattern in patterns:
                for src_file in fern_source.glob(f"src/{pattern}"):
                    source_files.append(src_file)
            
            # Add platform files
            platform_files = [
                fern_source / "src/platform/web_renderer.cpp",
                fern_source / "src/platform/platform_factory.cpp",
                fern_source / "src/fern.cpp"
            ]
            
            for platform_file in platform_files:
                if platform_file.exists():
                    source_files.append(platform_file)
            
            # Compile each source file to an object file
            object_files = []
            
            try:
                for i, src_file in enumerate(source_files):
                    obj_file = cache_dir / f"obj_{i}.o"
                    object_files.append(obj_file)
                    
                    # Compile individual source file
                    cmd = [
                        "emcc", "-std=c++17", "-O2", "-c",
                        "-I", str(fern_source / "include"),
                        str(src_file),
                        "-o", str(obj_file)
                    ]
                    
                    result = subprocess.run(cmd, capture_output=True, text=True)
                    if result.returncode != 0:
                        print_error(f"Failed to compile {src_file.name}:")
                        print(result.stderr)
                        return None
                
                # Create static library from object files
                cmd = ["emar", "rcs", str(lib_file)] + [str(obj) for obj in object_files]
                result = subprocess.run(cmd, capture_output=True, text=True)
                
                if result.returncode != 0:
                    print_error("Failed to create Fern web library:")
                    print(result.stderr)
                    return None
                
                # Clean up object files
                for obj_file in object_files:
                    if obj_file.exists():
                        obj_file.unlink()
                        
                print_success("Fern web library built successfully!")
                
            except Exception as e:
                print_error(f"Error building Fern web library: {str(e)}")
                return None
        else:
            print_info("Using cached Fern web library")
        
        return lib_file
