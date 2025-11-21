#!/usr/bin/env python3
"""
Global configuration management for Fern CLI
"""

import os
import json
from pathlib import Path
from typing import Dict, Any, Optional

class FernConfig:
    """Manages global Fern configuration"""
    
    def __init__(self):
        self.config_dir = Path.home() / ".fern"
        self.config_file = self.config_dir / "config.json"
        self.templates_dir = self.config_dir / "templates"
        
        # Default configuration
        self.default_config = {
            "version": "0.1.0",
            "cpp_library_path": str(Path.home() / ".local"),
            "templates_path": str(self.templates_dir),
            "default_template": "basic",
            "build": {
                "default_flags": ["-std=c++17", "-O2"],
                "debug_flags": ["-std=c++17", "-g", "-O0"],
                "include_paths": [str(Path.home() / ".local" / "include")],
                "library_paths": [str(Path.home() / ".local" / "lib")],
                "libraries": ["fern", "X11", "Xext", "fontconfig", "freetype"]
            }
        }
        
        self._config = None
    
    def ensure_config_exists(self):
        """Ensure configuration directory and file exist"""
        self.config_dir.mkdir(parents=True, exist_ok=True)
        self.templates_dir.mkdir(parents=True, exist_ok=True)
        
        if not self.config_file.exists():
            with open(self.config_file, 'w') as f:
                json.dump(self.default_config, f, indent=2)
    
    def load_config(self) -> Dict[str, Any]:
        """Load configuration from file"""
        if self._config is None:
            self.ensure_config_exists()
            
            try:
                with open(self.config_file, 'r') as f:
                    self._config = json.load(f)
            except (FileNotFoundError, json.JSONDecodeError):
                self._config = self.default_config.copy()
        
        return self._config
    
    def save_config(self, config: Dict[str, Any]):
        """Save configuration to file"""
        self.ensure_config_exists()
        
        with open(self.config_file, 'w') as f:
            json.dump(config, f, indent=2)
        
        self._config = config
    
    def get(self, key: str, default: Any = None) -> Any:
        """Get configuration value"""
        config = self.load_config()
        
        # Support nested keys like "build.default_flags"
        keys = key.split('.')
        value = config
        
        for k in keys:
            if isinstance(value, dict) and k in value:
                value = value[k]
            else:
                return default
        
        return value
    
    def set(self, key: str, value: Any):
        """Set configuration value"""
        config = self.load_config()
        
        # Support nested keys like "build.default_flags"
        keys = key.split('.')
        current = config
        
        for k in keys[:-1]:
            if k not in current:
                current[k] = {}
            current = current[k]
        
        current[keys[-1]] = value
        self.save_config(config)
    
    def get_cpp_library_path(self) -> Path:
        """Get C++ library installation path"""
        return Path(self.get("cpp_library_path", str(Path.home() / ".local")))
    
    def get_templates_path(self) -> Path:
        """Get templates directory path"""
        return Path(self.get("templates_path", str(self.templates_dir)))
    
    def get_build_flags(self, debug: bool = False) -> list:
        """Get build flags for C++ compilation"""
        if debug:
            return self.get("build.debug_flags", ["-std=c++17", "-g", "-O0"])
        else:
            return self.get("build.default_flags", ["-std=c++17", "-O2"])
    
    def get_include_paths(self) -> list:
        """Get include paths for C++ compilation"""
        return self.get("build.include_paths", [str(Path.home() / ".local" / "include")])
    
    def get_library_paths(self) -> list:
        """Get library paths for C++ linking"""
        return self.get("build.library_paths", [str(Path.home() / ".local" / "lib")])
    
    def get_libraries(self) -> list:
        """Get libraries to link against"""
        return self.get("build.libraries", ["fern", "X11", "Xext", "fontconfig", "freetype"])
    
    def is_fern_installed(self) -> bool:
        """Check if Fern C++ library is installed"""
        lib_path = self.get_cpp_library_path()
        include_path = lib_path / "include" / "fern"
        lib_file = lib_path / "lib" / "libfern.a"
        
        return include_path.exists() and lib_file.exists()

# Global config instance
config = FernConfig()
