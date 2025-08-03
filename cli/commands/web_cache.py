"""
Fern Web Cache Command - Manage web build cache
"""

import os
import sys
import shutil
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info

class WebCacheCommand:
    """Manage Fern web build cache"""
    
    def execute(self, args):
        # Parse arguments
        action = "status"  # default action
        
        if len(args) > 0:
            action = args[0]
        
        if action in ["--help", "-h"]:
            self._show_help()
            return
        
        if action == "status":
            self._show_status()
        elif action == "clear":
            self._clear_cache()
        elif action == "rebuild":
            self._rebuild_cache()
        else:
            print_error(f"Unknown action: {action}")
            self._show_help()
    
    def _show_help(self):
        """Show help for web-cache command"""
        print_header("Fern Web Cache Command")
        print()
        print_info("Usage:")
        print("  fern web-cache [action]")
        print()
        print_info("Actions:")
        print("  status     Show cache status (default)")
        print("  clear      Clear the web build cache")
        print("  rebuild    Force rebuild of the web library cache")
        print("  -h, --help Show this help message")
        print()
        print_info("Description:")
        print("The web cache stores precompiled Fern library files to speed up web builds.")
        print("When you run 'fern fire -p web', Fern compiles your code against a cached")
        print("web library instead of recompiling the entire Fern source every time.")
    
    def _show_status(self):
        """Show web cache status"""
        print_header("Fern Web Cache Status")
        
        cache_dir = Path.home() / ".fern" / "cache" / "web"
        lib_file = cache_dir / "libfern_web.a"
        
        if not cache_dir.exists():
            print_warning("Web cache directory does not exist")
            print_info(f"Expected location: {cache_dir}")
            print_info("Cache will be created automatically on first web build")
            return
        
        if not lib_file.exists():
            print_warning("Web library cache not found")
            print_info(f"Expected file: {lib_file}")
            print_info("Cache will be created automatically on first web build")
            return
        
        # Get cache file info
        stat = lib_file.stat()
        size_mb = stat.st_size / (1024 * 1024)
        
        print_success("Web cache is available")
        print_info(f"Cache location: {cache_dir}")
        print_info(f"Library file: {lib_file.name}")
        print_info(f"Size: {size_mb:.1f} MB")
        print_info(f"Last modified: {self._format_time(stat.st_mtime)}")
        
        # Check source directory for comparison
        fern_source = self._find_fern_source()
        if fern_source:
            print_info(f"Source location: {fern_source}")
            
            # Check if cache is up to date
            if self._is_cache_outdated(lib_file, fern_source):
                print_warning("Cache may be outdated (source files newer than cache)")
                print_info("Cache will be automatically rebuilt on next web build")
            else:
                print_success("Cache is up to date")
        else:
            print_warning("Fern source not found - cache may not work")
    
    def _clear_cache(self):
        """Clear the web cache"""
        print_header("Clearing Fern Web Cache")
        
        cache_dir = Path.home() / ".fern" / "cache" / "web"
        
        if not cache_dir.exists():
            print_info("Cache directory does not exist, nothing to clear")
            return
        
        try:
            # Remove the entire cache directory
            shutil.rmtree(cache_dir)
            print_success("Web cache cleared successfully")
            print_info("Cache will be recreated automatically on next web build")
        except Exception as e:
            print_error(f"Failed to clear cache: {str(e)}")
    
    def _rebuild_cache(self):
        """Force rebuild of the web cache"""
        print_header("Rebuilding Fern Web Cache")
        
        # First clear the cache
        cache_dir = Path.home() / ".fern" / "cache" / "web"
        if cache_dir.exists():
            try:
                shutil.rmtree(cache_dir)
                print_info("Cleared existing cache")
            except Exception as e:
                print_error(f"Failed to clear existing cache: {str(e)}")
                return
        
        # Now force a rebuild by importing and using the fire command's library builder
        try:
            from fire import FireCommand
            fire_cmd = FireCommand()
            
            fern_source = fire_cmd._find_fern_source()
            if not fern_source:
                print_error("Cannot rebuild cache: Fern source not found")
                return
            
            lib_file = fire_cmd._ensure_fern_web_library(fern_source)
            if lib_file:
                print_success("Web cache rebuilt successfully")
                print_info(f"Cache location: {lib_file}")
            else:
                print_error("Failed to rebuild web cache")
                
        except Exception as e:
            print_error(f"Failed to rebuild cache: {str(e)}")
            print_info("You can rebuild the cache by running a web build: fern fire -p web <file>")
    
    def _find_fern_source(self):
        """Find the Fern source directory"""
        cli_dir = Path(__file__).parent.parent.parent
        
        potential_sources = [
            Path.home() / ".fern",
            cli_dir,
            Path("/usr/local/src/fern"),
            Path.home() / ".fern" / "src"
        ]
        
        for src_path in potential_sources:
            cpp_src = src_path / "src" / "cpp"
            if (cpp_src.exists() and 
                (cpp_src / "include" / "fern").exists() and
                (cpp_src / "src").exists()):
                return cpp_src
        return None
    
    def _is_cache_outdated(self, lib_file, fern_source):
        """Check if cache is outdated compared to source files"""
        lib_mtime = lib_file.stat().st_mtime
        
        # Check source file patterns
        patterns = ["core/*.cpp", "graphics/*.cpp", "text/*.cpp", "font/*.cpp", "ui/**/*.cpp"]
        
        for pattern in patterns:
            for src_file in fern_source.glob(f"src/{pattern}"):
                if src_file.stat().st_mtime > lib_mtime:
                    return True
        
        # Check platform files
        platform_files = [
            fern_source / "src/platform/web_renderer.cpp",
            fern_source / "src/platform/platform_factory.cpp",
            fern_source / "src/fern.cpp"
        ]
        
        for platform_file in platform_files:
            if platform_file.exists() and platform_file.stat().st_mtime > lib_mtime:
                return True
        
        return False
    
    def _format_time(self, timestamp):
        """Format timestamp for display"""
        import datetime
        dt = datetime.datetime.fromtimestamp(timestamp)
        return dt.strftime("%Y-%m-%d %H:%M:%S")
