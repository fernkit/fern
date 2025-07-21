"""
Fern Fire Command - Run single file or project
"""

import os
import sys
import subprocess
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info
from utils.system import ProjectDetector, BuildSystem
from utils.config import config

class FireCommand:
    """Run Fern code - single file or project"""
    
    def execute(self, args):
        # Parse arguments for platform and file
        platform = "linux"  # default platform
        file_path = None
        
        # Parse arguments
        i = 0
        while i < len(args):
            arg = args[i]
            if arg in ["--platform", "-p"]:
                if i + 1 < len(args):
                    platform = args[i + 1]
                    i += 2
                else:
                    print_error("--platform requires a value (linux, web)")
                    return
            elif arg in ["--help", "-h"]:
                self._show_help()
                return
            else:
                # Assume it's a file path
                file_path = arg
                i += 1
        
        # Validate platform
        if platform not in ["linux", "web"]:
            print_error(f"Unsupported platform: {platform}")
            print_info("Supported platforms: linux, web")
            return
        
        if file_path:
            # File specified, run single file
            self._run_single_file(file_path, platform)
        else:
            # No file specified, try to run current project
            self._run_project(platform)
    
    def _show_help(self):
        """Show help for fire command"""
        print_header("Fern Fire Command")
        print()
        print_info("Usage:")
        print("  fern fire [options] [file]")
        print()
        print_info("Options:")
        print("  -p, --platform <platform>   Target platform (linux, web)")
        print("  -h, --help                  Show this help message")
        print()
        print_info("Examples:")
        print("  fern fire                   # Run current project for Linux")
        print("  fern fire -p web            # Run current project for web")
        print("  fern fire main.cpp          # Run single file for Linux")
        print("  fern fire -p web main.cpp   # Run single file for web")
        print()
        print_info("Platforms:")
        print("  linux    Build and run native Linux application")
        print("  web      Build and serve web application")
    
    def _run_project(self, platform="linux"):
        """Run current Fern project"""
        print_header(f"Running Fern Project ({platform})")
        
        # Check if we're in a Fern project
        project_root = ProjectDetector.find_project_root()
        if not project_root:
            print_error("Not in a Fern project directory")
            print_info("Run 'fern sprout <project_name>' to create a new project")
            return
        
        print_info(f"Found Fern project at: {project_root}")
        
        # Get project structure
        structure = ProjectDetector.get_project_structure(project_root)
        
        # Find main.cpp in lib directory
        main_file = structure['lib'] / 'main.cpp'
        if not main_file.exists():
            print_error("No main.cpp found in lib directory")
            print_info("Create lib/main.cpp with your Fern code")
            return
        
        # Build and run
        build_system = BuildSystem(project_root)
        
        print_info(f"Building project for {platform}...")
        if platform == "web":
            if self._build_project_web(build_system, main_file):
                print_success("Build successful!")
                self._run_web_project(project_root)
            else:
                print_error("Build failed")
        else:  # linux
            if self._build_project_linux(build_system, main_file):
                print_success("Build successful!")
                self._run_executable(project_root / "build" / "main")
            else:
                print_error("Build failed")
    
    def _run_single_file(self, file_path, platform="linux"):
        """Run a single Fern file"""
        print_header(f"Running {file_path} ({platform})")
        
        # Use original working directory if available
        original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
        
        # Resolve file path relative to original working directory
        if not os.path.isabs(file_path):
            file_path = Path(original_cwd) / file_path
        else:
            file_path = Path(file_path)
            
        if not file_path.exists():
            print_error(f"File not found: {file_path}")
            return
        
        if file_path.suffix not in ['.cpp', '.cxx', '.cc']:
            print_error(f"Unsupported file type: {file_path.suffix}")
            print_info("Supported types: .cpp, .cxx, .cc")
            return
        
        # Build single file
        print_info(f"Building single file for {platform}...")
        if platform == "web":
            if self._build_single_file_web(file_path):
                print_success("Build successful!")
                self._run_web_file(file_path)
            else:
                print_error("Build failed")
        else:  # linux
            if self._build_single_file_linux(file_path):
                print_success("Build successful!")
                # Run the executable from build directory
                original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
                build_dir = Path(original_cwd) / "build"
                executable = build_dir / (file_path.stem + "_temp")
                self._run_executable(executable)
            else:
                print_error("Build failed")
    
    def _build_project_linux(self, build_system, main_file):
        """Build a Fern project for Linux"""
        try:
            # Check if Fern is installed globally
            if not config.is_fern_installed():
                print_error("Fern C++ library is not installed globally")
                print_info("Run './install.sh' from the Fern source directory to install")
                return False
            
            # Create build directory
            build_dir = build_system.project_root / "build"
            build_dir.mkdir(exist_ok=True)
            
            # Build command using global configuration
            cmd = ["g++"]
            
            # Add build flags
            cmd.extend(config.get_build_flags())
            
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
            cmd.extend(["-o", str(build_dir / "main")])
            
            print_info("Compiling...")
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Compilation failed:")
                print(result.stderr)
                return False
            
            return True
            
        except Exception as e:
            print_error(f"Build error: {str(e)}")
            return False
    
    def _build_project_web(self, build_system, main_file):
        """Build a Fern project for web using Emscripten"""
        try:
            # Check if Emscripten is available
            result = subprocess.run(["emcc", "--version"], capture_output=True, text=True)
            if result.returncode != 0:
                print_error("Emscripten not found. Please install and activate Emscripten.")
                print_info("See installation tips: fern bloom")
                return False
                
            # Create build directory
            build_dir = build_system.project_root / "build"
            build_dir.mkdir(exist_ok=True)
            
            # Build command using Emscripten
            cmd = ["emcc"]
            
            # Add Emscripten flags
            cmd.extend(["-std=c++17", "-O2"])
            cmd.extend(["-s", "WASM=1"])
            cmd.extend(["-s", "ALLOW_MEMORY_GROWTH=1"])
            cmd.extend(["-s", "USE_WEBGL2=1"])
            cmd.extend(["-s", "EXPORTED_FUNCTIONS=['_main']"])
            cmd.extend(["-s", "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"])
            
            # === START OF FIX ===
            # For web builds, we need to compile Fern source files with Emscripten
            
            # Find the Fern source directory
            fern_source = None
            original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
            
            potential_sources = [
                Path(original_cwd).parent / "src" / "cpp", # Running from inside a fern clone
                Path.home() / ".fern" / "src" / "cpp"      # A potential future location
            ]
            
            for src_path in potential_sources:
                # A simple check to see if it looks like the source directory
                if src_path.exists() and (src_path / "include" / "fern").exists():
                    fern_source = src_path
                    print_info(f"Found Fern source for web build at: {fern_source}")
                    break

            if not fern_source:
                print_error("Fern source files not found for web compilation.")
                print_info("Ensure you are running 'fern fire' from within the cloned Fern repository for web builds to work.")
                return False

            # Add the source include path for web builds (instead of the global installed headers)
            cmd.extend(["-I", str(fern_source / "include")])

            # Add the project's own source file
            cmd.append(str(main_file))

            # Add Fern library source files to be compiled
            patterns = ["core/*.cpp", "graphics/*.cpp", "text/*.cpp", "font/*.cpp", "ui/**/*.cpp"]
            for pattern in patterns:
                for src_file in fern_source.glob(f"src/{pattern}"):
                    cmd.append(str(src_file))
            
            # Add platform-specific files for web
            cmd.append(str(fern_source / "src/platform/web_renderer.cpp"))
            cmd.append(str(fern_source / "src/platform/platform_factory.cpp"))
            cmd.append(str(fern_source / "src/fern.cpp"))

            # === END OF FIX ===
            
            # Check for custom template
            project_template = build_system.project_root / "web" / "template.html"
            if project_template.exists():
                # Use custom template
                cmd.extend(["--shell-file", str(project_template)])
            
            # Add output file
            cmd.extend(["-o", str(build_dir / "main.html")])
            
            print_info("Compiling for web...")
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Web compilation failed:")
                print(result.stderr)
                return False
                
            return True
        except Exception as e:
            print_error(f"Web build error: {str(e)}")
            return False
    
    def _build_single_file_linux(self, file_path):
        """Build a single Fern file for Linux"""
        try:
            # Check if Fern is installed globally
            if not config.is_fern_installed():
                print_error("Fern C++ library is not installed globally")
                print_info("Run './install.sh' from the Fern source directory to install")
                return False
            
            # Create a build directory in the original working directory
            original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
            build_dir = Path(original_cwd) / "build"
            build_dir.mkdir(exist_ok=True)
            
            # Output executable name in build directory
            output_file = build_dir / (file_path.stem + "_temp")
            
            # Build command using global configuration
            cmd = ["g++"]
            
            # Add build flags
            cmd.extend(config.get_build_flags())
            
            # Add include paths
            for include_path in config.get_include_paths():
                cmd.extend(["-I", include_path])
            
            # Add source file
            cmd.append(str(file_path))
            
            # Add library paths
            for lib_path in config.get_library_paths():
                cmd.extend(["-L", lib_path])
            
            # Add libraries
            for lib in config.get_libraries():
                cmd.extend(["-l", lib])
            
            # Add output
            cmd.extend(["-o", str(output_file)])
            
            print_info("Compiling...")
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Compilation failed:")
                print(result.stderr)
                return False
            
            return True
            
        except Exception as e:
            print_error(f"Build error: {str(e)}")
            return False
    
    def _build_single_file_web(self, file_path):
        """Build a single Fern file for web using Emscripten"""
        try:
            # Check if Emscripten is available
            result = subprocess.run(["emcc", "--version"], capture_output=True, text=True)
            if result.returncode != 0:
                print_error("Emscripten not found. Please install and activate Emscripten.")
                print_info("See installation tips: fern bloom")
                return False
            
            # Check if Fern is installed globally
            if not config.is_fern_installed():
                print_error("Fern C++ library is not installed globally")
                print_info("Run './install.sh' from the Fern source directory to install")
                return False
            
            # Create a build directory in the original working directory
            original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
            build_dir = Path(original_cwd) / "build"
            build_dir.mkdir(exist_ok=True)
            
            # Output HTML file name
            output_file = build_dir / (file_path.stem + "_temp.html")
            
            # Build command using Emscripten
            cmd = ["emcc"]
            
            # Add Emscripten flags
            cmd.extend(["-std=c++17", "-O2"])
            cmd.extend(["-s", "WASM=1"])
            cmd.extend(["-s", "ALLOW_MEMORY_GROWTH=1"])
            cmd.extend(["-s", "USE_WEBGL2=1"])
            cmd.extend(["-s", "EXPORTED_FUNCTIONS=['_main']"])
            cmd.extend(["-s", "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"])
            
            # For web builds, we'll add the include path from the source directory
            # Don't use the installed headers as they conflict with source compilation
            
            # Add source file
            cmd.append(str(file_path))
            
            # For web builds, we need to compile Fern source files with Emscripten
            # Try to find the source files in common locations
            fern_source = None
            
            # Look for source files in common locations
            # Get the original working directory where the user ran the command
            original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
            
            potential_sources = [
                Path(original_cwd) / "src" / "cpp",  # Running from Fern repo root
                Path(__file__).parent.parent.parent / "src" / "cpp",  # Local development
                Path("/usr/local/src/fern/src/cpp"),  # System-wide source
                Path("/opt/fern/src/cpp"),  # Alternative system location
                Path.home() / ".fern" / "src" / "cpp"  # User source backup
            ]
            
            for src_path in potential_sources:
                if src_path.exists() and (src_path / "src").exists():
                    fern_source = src_path
                    break
            
            if not fern_source:
                print_error("Fern source files not found for web compilation")
                print_info("Web builds require access to Fern source files")
                print_info("Please ensure you're running from the Fern source directory")
                return False
            
            # Add the source include path for web builds (instead of installed headers)
            cmd.extend(["-I", str(fern_source / "include")])
            
            # Add Fern source files for web compilation
            patterns = ["core/*.cpp", "graphics/*.cpp", "text/*.cpp", "font/*.cpp", "ui/**/*.cpp"]
            for pattern in patterns:
                for src_file in fern_source.glob(f"src/{pattern}"):
                    cmd.append(str(src_file))
            
            # Add web platform files
            web_renderer = fern_source / "src/platform/web_renderer.cpp"
            if web_renderer.exists():
                cmd.append(str(web_renderer))
            else:
                print_warning("Web renderer not found, web build may not work correctly")
            
            cmd.append(str(fern_source / "src/platform/platform_factory.cpp"))
            cmd.append(str(fern_source / "src/fern.cpp"))
            
            # Check for custom template in current directory or use default
            local_template = Path(original_cwd) / "template.html"
            global_template = Path(__file__).parent.parent.parent / "template.html"
            
            if local_template.exists():
                # Use local template
                cmd.extend(["--shell-file", str(local_template)])
            elif global_template.exists():
                # Use global template
                cmd.extend(["--shell-file", str(global_template)])
            
            # Add output
            cmd.extend(["-o", str(output_file)])
            
            print_info("Compiling for web...")
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error("Web compilation failed:")
                print(result.stderr)
                return False
            
            return True
            
        except Exception as e:
            print_error(f"Web build error: {str(e)}")
            return False
    
    def _run_web_project(self, project_root):
        """Run web project by starting a local server"""
        try:
            build_dir = project_root / "build"
            html_file = build_dir / "main.html"
            
            if not html_file.exists():
                print_error(f"Web build not found: {html_file}")
                return
            
            print_info("Starting local web server...")
            print_success("🔥 Fern Fire started (web)!")
            print()
            print_info(f"Open your browser to: http://localhost:8000/main.html")
            print_info("Press Ctrl+C to stop the server")
            print()
            
            # Start simple HTTP server
            import http.server
            import socketserver
            import threading
            import webbrowser
            import time
            
            os.chdir(build_dir)
            
            def start_server():
                with socketserver.TCPServer(("", 8000), http.server.SimpleHTTPRequestHandler) as httpd:
                    httpd.serve_forever()
            
            server_thread = threading.Thread(target=start_server, daemon=True)
            server_thread.start()
            
            # Wait a moment then open browser
            time.sleep(1)
            webbrowser.open("http://localhost:8000/main.html")
            
            # Keep running until interrupted
            try:
                while True:
                    time.sleep(1)
            except KeyboardInterrupt:
                print_info("\\nStopping web server...")
                
        except Exception as e:
            print_error(f"Error running web project: {str(e)}")
    
    def _run_web_file(self, file_path):
        """Run web file by starting a local server"""
        try:
            # Get original working directory for build location
            original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
            build_dir = Path(original_cwd) / "build"
            html_file = build_dir / (file_path.stem + "_temp.html")
            
            if not html_file.exists():
                print_error(f"Web build not found: {html_file}")
                return
            
            print_info("Starting local web server...")
            print_success("🔥 Fern Fire started (web)!")
            print()
            print_info(f"Open your browser to: http://localhost:8000/{html_file.name}")
            print_info("Press Ctrl+C to stop the server")
            print()
            
            # Start simple HTTP server
            import http.server
            import socketserver
            import threading
            import webbrowser
            import time
            
            os.chdir(build_dir)
            
            def start_server():
                with socketserver.TCPServer(("", 8000), http.server.SimpleHTTPRequestHandler) as httpd:
                    httpd.serve_forever()
            
            server_thread = threading.Thread(target=start_server, daemon=True)
            server_thread.start()
            
            # Wait a moment then open browser
            time.sleep(1)
            webbrowser.open(f"http://localhost:8000/{html_file.name}")
            
            # Keep running until interrupted
            try:
                while True:
                    time.sleep(1)
            except KeyboardInterrupt:
                print_info("\\nStopping web server...")
                
        except Exception as e:
            print_error(f"Error running web file: {str(e)}")
    
    def _run_executable(self, executable_path):
        """Run the compiled executable"""
        try:
            if not Path(executable_path).exists():
                print_error(f"Executable not found: {executable_path}")
                return
            
            print_info(f"Running {executable_path}...")
            print_success("🔥 Fern Fire started!")
            print()
            
            # Run the executable
            subprocess.run([str(executable_path)], check=True)
            
        except subprocess.CalledProcessError as e:
            print_error(f"Runtime error: {e}")
        except KeyboardInterrupt:
            print_info("\\nStopped by user")
        except Exception as e:
            print_error(f"Error running executable: {str(e)}")
    
    def _cleanup_temp_files(self, file_path):
        """Clean up temporary files"""
        temp_file = file_path.parent / (file_path.stem + "_temp")
        if temp_file.exists():
            temp_file.unlink()
            print_info("Cleaned up temporary files")
        
        # Also clean up web temp files
        web_temp_files = [
            file_path.parent / (file_path.stem + "_temp.html"),
            file_path.parent / (file_path.stem + "_temp.js"),
            file_path.parent / (file_path.stem + "_temp.wasm")
        ]
        
        for temp_file in web_temp_files:
            if temp_file.exists():
                temp_file.unlink()
                print_info(f"Cleaned up web temp file: {temp_file.name}")
