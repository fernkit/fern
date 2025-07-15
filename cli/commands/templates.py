"""
Fern Templates Command - Manage project templates
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info

class TemplatesCommand:
    """Manage Fern project templates"""
    
    def execute(self, args):
        if len(args) == 0:
            self._show_help()
            return
        
        subcommand = args[0]
        
        if subcommand == "list":
            self._list_templates()
        elif subcommand == "install":
            if len(args) < 2:
                print_error("Template name/URL is required")
                print_info("Usage: fern templates install <template_name_or_url>")
                return
            self._install_template(args[1])
        elif subcommand == "create":
            if len(args) < 3:
                print_error("Template name and project name are required")
                print_info("Usage: fern templates create <template_name> <project_name>")
                return
            self._create_from_template(args[1], args[2])
        else:
            print_error(f"Unknown subcommand: {subcommand}")
            self._show_help()
    
    def _show_help(self):
        """Show templates help"""
        print_header("Fern Templates")
        print("Usage: fern templates <command> [options]")
        print()
        print("Commands:")
        print("  list                        List available templates")
        print("  install <name_or_url>       Install a template")
        print("  create <template> <name>    Create project from template")
        print()
        print("Examples:")
        print("  fern templates list")
        print("  fern templates install game")
        print("  fern templates install https://github.com/user/fern-template")
        print("  fern templates create game my_game")
    
    def _list_templates(self):
        """List available templates"""
        print_header("Available Templates")
        
        # Get templates directory
        templates_dir = self._get_templates_dir()
        
        if not templates_dir.exists():
            print_info("No templates installed")
            print_info("Install templates with: fern templates install <template>")
            return
        
        # List installed templates
        templates = [d for d in templates_dir.iterdir() if d.is_dir()]
        
        if not templates:
            print_info("No templates installed")
            return
        
        print_info("Installed templates:")
        for template in templates:
            # Read template info if available
            info_file = template / "template.yaml"
            if info_file.exists():
                # TODO: Parse template info
                print(f"  📦 {template.name}")
            else:
                print(f"  📦 {template.name}")
        
        print()
        print_info("Official templates:")
        print("  📦 basic      - Basic Fern application")
        print("  📦 game       - Game development template")
        print("  📦 dashboard  - Dashboard/admin template")
        print("  📦 mobile     - Mobile-style UI template")
    
    def _install_template(self, template_source):
        """Install a template from name or URL"""
        print_header(f"Installing template: {template_source}")
        
        templates_dir = self._get_templates_dir()
        templates_dir.mkdir(parents=True, exist_ok=True)
        
        if template_source.startswith("http"):
            # Install from URL
            self._install_from_url(template_source, templates_dir)
        else:
            # Install from official templates
            self._install_official_template(template_source, templates_dir)
    
    def _install_from_url(self, url, templates_dir):
        """Install template from Git URL"""
        try:
            # Extract template name from URL
            template_name = url.split("/")[-1].replace(".git", "")
            template_path = templates_dir / template_name
            
            if template_path.exists():
                print_warning(f"Template '{template_name}' already exists")
                return
            
            print_info(f"Cloning from {url}...")
            
            # Clone the repository
            result = subprocess.run([
                "git", "clone", url, str(template_path)
            ], capture_output=True, text=True)
            
            if result.returncode != 0:
                print_error(f"Failed to clone template: {result.stderr}")
                return
            
            # Remove .git directory
            git_dir = template_path / ".git"
            if git_dir.exists():
                shutil.rmtree(git_dir)
            
            print_success(f"Template '{template_name}' installed successfully!")
            
        except Exception as e:
            print_error(f"Installation error: {str(e)}")
    
    def _install_official_template(self, template_name, templates_dir):
        """Install official template"""
        # Official templates repository (example)
        official_templates = {
            "basic": "https://github.com/fern-ui/template-basic.git",
            "game": "https://github.com/fern-ui/template-game.git",
            "dashboard": "https://github.com/fern-ui/template-dashboard.git",
            "mobile": "https://github.com/fern-ui/template-mobile.git"
        }
        
        if template_name not in official_templates:
            print_error(f"Unknown official template: {template_name}")
            print_info("Available official templates: " + ", ".join(official_templates.keys()))
            return
        
        # For now, create a simple template locally
        self._create_local_template(template_name, templates_dir)
    
    def _create_local_template(self, template_name, templates_dir):
        """Create a local template (placeholder for official templates)"""
        template_path = templates_dir / template_name
        template_path.mkdir(parents=True, exist_ok=True)
        
        # Create template structure
        if template_name == "game":
            self._create_game_template(template_path)
        elif template_name == "dashboard":
            self._create_dashboard_template(template_path)
        else:
            self._create_basic_template(template_path)
        
        print_success(f"Template '{template_name}' created successfully!")
    
    def _create_game_template(self, template_path):
        """Create game template"""
        # Create game-specific main.cpp
        main_cpp = """#include <fern/fern.hpp>
#include <iostream>
#include <cmath>

using namespace Fern;

// Game state
struct Player {
    float x = 400;
    float y = 300;
    float speed = 200;
};

Player player;

void update(float deltaTime) {
    // Simple player movement
    if (Input::isKeyPressed(KeyCode::Left)) {
        player.x -= player.speed * deltaTime;
    }
    if (Input::isKeyPressed(KeyCode::Right)) {
        player.x += player.speed * deltaTime;
    }
    if (Input::isKeyPressed(KeyCode::Up)) {
        player.y -= player.speed * deltaTime;
    }
    if (Input::isKeyPressed(KeyCode::Down)) {
        player.y += player.speed * deltaTime;
    }
}

void draw() {
    // Clear background
    Draw::fill(Colors::DarkBlue);
    
    // Draw player
    Draw::circle(player.x, player.y, 20, Colors::Yellow);
    
    // Draw UI
    DrawText::drawText("Use arrow keys to move", 10, 10, 1, Colors::White);
    DrawText::drawText("Game Template", 10, 30, 2, Colors::Cyan);
}

int main() {
    std::cout << "🎮 Starting Fern Game..." << std::endl;
    
    Fern::initialize();
    Fern::setUpdateCallback(update);
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
"""
        
        (template_path / "main.cpp").write_text(main_cpp)
        
        # Create template info
        template_info = """name: Fern Game Template
description: Template for creating games with Fern
author: Fern Team
version: 1.0.0
"""
        
        (template_path / "template.yaml").write_text(template_info)
    
    def _create_dashboard_template(self, template_path):
        """Create dashboard template"""
        # Dashboard template implementation
        pass
    
    def _create_basic_template(self, template_path):
        """Create basic template"""
        # Basic template implementation
        pass
    
    def _create_from_template(self, template_name, project_name):
        """Create a new project from template"""
        print_header(f"Creating '{project_name}' from template '{template_name}'")
        
        # Get template path
        templates_dir = self._get_templates_dir()
        template_path = templates_dir / template_name
        
        if not template_path.exists():
            print_error(f"Template '{template_name}' not found")
            print_info("Install it first: fern templates install " + template_name)
            return
        
        # Check if project already exists
        if Path(project_name).exists():
            print_error(f"Directory '{project_name}' already exists")
            return
        
        try:
            # Copy template to new project
            shutil.copytree(template_path, project_name)
            
            # Remove template.yaml from project
            template_file = Path(project_name) / "template.yaml"
            if template_file.exists():
                template_file.unlink()
            
            print_success(f"Project '{project_name}' created from template '{template_name}'!")
            print_info(f"cd {project_name}")
            print_info("fern fire")
            
        except Exception as e:
            print_error(f"Error creating project: {str(e)}")
    
    def _get_templates_dir(self):
        """Get templates directory"""
        fern_home = Path.home() / ".fern"
        return fern_home / "templates"
