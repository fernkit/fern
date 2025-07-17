"""
Fern LSP Command - Language Server Protocol management
"""

import os
import sys
import signal
import subprocess
import shutil
import tempfile
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info
from utils.config import config

class LSPCommand:
    """Manage Gleeb LSP server for Fern development"""
    
    def execute(self, args):
        if len(args) == 0:
            self._show_help()
            return
        
        subcommand = args[0]
        
        if subcommand == "start":
            self._start_server(args[1:])
        elif subcommand == "stop":
            self._stop_server()
        elif subcommand == "status":
            self._show_status()
        elif subcommand == "config":
            self._configure_editor(args[1:])
        elif subcommand == "install":
            self._install_lsp()
        elif subcommand == "restart":
            self._restart_server()
        else:
            print_error(f"Unknown LSP command: {subcommand}")
            self._show_help()
    
    def _show_help(self):
        """Show LSP command help"""
        print_header("LSP Commands")
        print_info("fern lsp start     - Start Gleeb LSP server")
        print_info("fern lsp stop      - Stop Gleeb LSP server")
        print_info("fern lsp status    - Show LSP server status")
        print_info("fern lsp config    - Configure editor integration")
        print_info("fern lsp install   - Install/reinstall LSP server")
        print_info("fern lsp restart   - Restart LSP server")
        print()
        print_info("Examples:")
        print_info("  fern lsp start --background")
        print_info("  fern lsp config --editor vscode")
        print_info("  fern lsp status")
    
    def _start_server(self, args):
        """Start Gleeb LSP server"""
        gleeb_path = self._get_gleeb_path()
        
        if not gleeb_path:
            print_error("Gleeb LSP server not found")
            print_info("Run 'fern lsp install' to install it")
            return
        
        if self._is_server_running():
            print_warning("LSP server is already running")
            return
        
        # Parse arguments
        background = "--background" in args or "-b" in args
        port = None
        
        # Extract port if specified
        for i, arg in enumerate(args):
            if arg == "--port" and i + 1 < len(args):
                try:
                    port = int(args[i + 1])
                except ValueError:
                    print_error("Invalid port number")
                    return
        
        try:
            cmd = ["node", str(gleeb_path)]
            if port:
                cmd.extend(["--port", str(port)])
            else:
                cmd.append("--stdio")
            
            if background:
                print_info("Starting LSP server in background...")
                process = subprocess.Popen(
                    cmd,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    start_new_session=True
                )
                
                # Save PID
                pid_file = self._get_pid_file()
                pid_file.parent.mkdir(parents=True, exist_ok=True)
                with open(pid_file, 'w') as f:
                    f.write(str(process.pid))
                
                print_success(f"LSP server started (PID: {process.pid})")
                if port:
                    print_info(f"Server listening on port {port}")
                else:
                    print_info("Server using stdio communication")
            else:
                print_info("Starting LSP server in foreground...")
                print_info("Press Ctrl+C to stop")
                subprocess.run(cmd)
                
        except Exception as e:
            print_error(f"Failed to start LSP server: {e}")
    
    def _stop_server(self):
        """Stop Gleeb LSP server"""
        if not self._is_server_running():
            print_warning("LSP server is not running")
            return
        
        try:
            pid_file = self._get_pid_file()
            with open(pid_file, 'r') as f:
                pid = int(f.read().strip())
            
            os.kill(pid, signal.SIGTERM)
            pid_file.unlink()
            
            print_success("LSP server stopped")
            
        except Exception as e:
            print_error(f"Failed to stop LSP server: {e}")
    
    def _restart_server(self):
        """Restart Gleeb LSP server"""
        print_info("Restarting LSP server...")
        self._stop_server()
        import time
        time.sleep(1)  # Give it a moment to stop
        self._start_server(["--background"])
    
    def _show_status(self):
        """Show LSP server status"""
        print_header("LSP Server Status")
        
        gleeb_path = self._get_gleeb_path()
        is_running = self._is_server_running()
        
        print_info(f"Gleeb LSP installed: {'Yes' if gleeb_path else 'No'}")
        if gleeb_path:
            print_info(f"Server path: {gleeb_path}")
        
        print_info(f"Server running: {'Yes' if is_running else 'No'}")
        
        if is_running:
            try:
                pid_file = self._get_pid_file()
                with open(pid_file, 'r') as f:
                    pid = int(f.read().strip())
                print_info(f"Process ID: {pid}")
            except:
                pass
        
        # Check VS Code configuration
        vscode_config = self._check_vscode_config()
        print_info(f"VS Code configured: {'Yes' if vscode_config else 'No'}")
        
        # Check dependencies
        node_available = self._check_node_js()
        print_info(f"Node.js available: {'Yes' if node_available else 'No'}")
        
        if not gleeb_path:
            print()
            print_warning("To install Gleeb LSP, run: fern lsp install")
        elif not is_running:
            print()
            print_info("To start LSP server, run: fern lsp start")
    
    def _configure_editor(self, args):
        """Configure editor integration"""
        editor = "vscode"  # Default
        
        # Parse editor argument
        for i, arg in enumerate(args):
            if arg == "--editor" and i + 1 < len(args):
                editor = args[i + 1]
                break
        
        if editor == "vscode":
            self._configure_vscode()
        else:
            print_error(f"Editor '{editor}' not supported yet")
            print_info("Supported editors: vscode")
    
    def _configure_vscode(self):
        """Configure VS Code for Gleeb LSP"""
        print_info("Configuring VS Code for Gleeb LSP...")
        
        vscode_helper = Path.home() / ".local" / "bin" / "gleeb-configure-vscode"
        
        if not vscode_helper.exists():
            print_error("VS Code configuration helper not found")
            print_info("Please reinstall Fern to get the latest tools")
            return
        
        try:
            subprocess.run([str(vscode_helper)], check=True)
            print_success("VS Code configured successfully")
            print_info("Install the 'Gleeb LSP for Fern UI' extension from VS Code marketplace")
            print_info("Extension ID: fernkit.gleeb-lsp")
        except subprocess.CalledProcessError as e:
            print_error(f"Failed to configure VS Code: {e}")
    
    def _install_lsp(self):
        """Install or reinstall Gleeb LSP"""
        print_info("Installing Gleeb LSP...")
        
        # Check if we're in development mode (local directories exist)
        if (Path(__file__).parent.parent.parent / "gleeb").exists():
            print_info("Development mode: Using local Gleeb LSP")
            self._install_local_lsp()
        else:
            print_info("Production mode: Cloning from GitHub")
            self._install_remote_lsp()
    
    def _install_local_lsp(self):
        """Install LSP from local directory"""
        try:
            gleeb_source = Path(__file__).parent.parent.parent / "gleeb"
            gleeb_dest = Path.home() / ".fern" / "gleeb"
            
            print_info("Copying Gleeb LSP source...")
            if gleeb_dest.exists():
                shutil.rmtree(gleeb_dest)
            
            shutil.copytree(gleeb_source, gleeb_dest)
            
            # Install dependencies and build
            self._build_lsp(gleeb_dest)
            
            print_success("Gleeb LSP installed successfully")
            
        except Exception as e:
            print_error(f"Failed to install LSP: {e}")
    
    def _install_remote_lsp(self):
        """Install LSP from GitHub"""
        try:
            with tempfile.TemporaryDirectory() as temp_dir:
                print_info("Cloning Gleeb LSP from GitHub...")
                
                # Clone repository
                subprocess.run([
                    "git", "clone", 
                    "https://github.com/fernkit/gleeb.git",
                    str(Path(temp_dir) / "gleeb")
                ], check=True)
                
                # Copy to destination
                gleeb_dest = Path.home() / ".fern" / "gleeb"
                if gleeb_dest.exists():
                    shutil.rmtree(gleeb_dest)
                
                shutil.copytree(Path(temp_dir) / "gleeb", gleeb_dest)
                
                # Install dependencies and build
                self._build_lsp(gleeb_dest)
                
                print_success("Gleeb LSP installed successfully")
                
        except subprocess.CalledProcessError:
            print_error("Failed to clone Gleeb LSP from GitHub")
            print_info("Please check your internet connection and try again")
        except Exception as e:
            print_error(f"Failed to install LSP: {e}")
    
    def _build_lsp(self, gleeb_dir):
        """Build LSP server"""
        print_info("Installing dependencies...")
        subprocess.run(["npm", "install"], cwd=gleeb_dir, check=True)
        
        print_info("Building LSP server...")
        subprocess.run(["npm", "run", "build"], cwd=gleeb_dir, check=True)
        
        # Create launcher script
        launcher_path = Path.home() / ".local" / "bin" / "gleeb-lsp"
        launcher_path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(launcher_path, 'w') as f:
            f.write(f"""#!/bin/bash
# Gleeb LSP Server launcher
GLEEB_DIR="{gleeb_dir}"
if [ ! -d "$GLEEB_DIR" ]; then
    echo "Error: Gleeb LSP not found at $GLEEB_DIR"
    exit 1
fi
cd "$GLEEB_DIR"
exec node dist/server.js "$@"
""")
        
        launcher_path.chmod(0o755)
        print_info("Created launcher script")
    
    def _get_gleeb_path(self):
        """Get path to Gleeb LSP server"""
        gleeb_path = Path.home() / ".fern" / "gleeb" / "dist" / "server.js"
        return gleeb_path if gleeb_path.exists() else None
    
    def _get_pid_file(self):
        """Get path to PID file"""
        return Path.home() / ".fern" / "gleeb" / "server.pid"
    
    def _is_server_running(self):
        """Check if LSP server is running"""
        pid_file = self._get_pid_file()
        
        if not pid_file.exists():
            return False
        
        try:
            with open(pid_file, 'r') as f:
                pid = int(f.read().strip())
            
            # Check if process exists
            os.kill(pid, 0)
            return True
            
        except (OSError, ValueError):
            # Process doesn't exist, clean up stale PID file
            if pid_file.exists():
                pid_file.unlink()
            return False
    
    def _check_vscode_config(self):
        """Check if VS Code is configured"""
        vscode_settings = Path.home() / ".config" / "Code" / "User" / "settings.json"
        
        if not vscode_settings.exists():
            return False
        
        try:
            import json
            with open(vscode_settings) as f:
                settings = json.load(f)
            
            return "gleeb.enable" in settings
        except:
            return False
    
    def _check_node_js(self):
        """Check if Node.js is available"""
        try:
            subprocess.run(["node", "--version"], 
                         capture_output=True, check=True)
            return True
        except:
            return False
