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
        if len(args) == 0:
            # No file specified, try to run current project
            self._run_project()
        else:
            # File specified, run single file
            file_path = args[0]
            self._run_single_file(file_path)
    
    def _run_project(self):
        """Run current Fern project"""
        print_header("Running Fern Project")
        
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
        
        print_info("Building project...")
        if self._build_project(build_system, main_file):
            print_success("Build successful!")
            self._run_executable(project_root / "build" / "main")
        else:
            print_error("Build failed")
    
    def _run_single_file(self, file_path):
        """Run a single Fern file"""
        print_header(f"Running {file_path}")
        
        file_path = Path(file_path)
        if not file_path.exists():
            print_error(f"File not found: {file_path}")
            return
        
        if file_path.suffix not in ['.cpp', '.cxx', '.cc']:
            print_error(f"Unsupported file type: {file_path.suffix}")
            print_info("Supported types: .cpp, .cxx, .cc")
            return
        
        # Build single file
        print_info("Building single file...")
        if self._build_single_file(file_path):
            print_success("Build successful!")
            # Run the executable
            executable = file_path.parent / (file_path.stem + "_temp")
            self._run_executable(executable)
        else:
            print_error("Build failed")
    
    def _build_project(self, build_system, main_file):
        """Build a Fern project"""
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
    
    def _build_single_file(self, file_path):
        """Build a single Fern file"""
        try:
            # Check if Fern is installed globally
            if not config.is_fern_installed():
                print_error("Fern C++ library is not installed globally")
                print_info("Run './install.sh' from the Fern source directory to install")
                return False
            
            # Output executable name
            output_file = file_path.parent / (file_path.stem + "_temp")
            
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
