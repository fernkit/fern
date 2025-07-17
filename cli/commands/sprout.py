"""
Fern Sprout Command - Create new Fern project
"""

import os
import sys
import shutil
from pathlib import Path

# Add utils to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from utils.colors import print_header, print_success, print_error, print_warning, print_info
from utils.config import config

class SproutCommand:
    """Create a new Fern project"""
    
    def execute(self, args):
        if len(args) == 0:
            print_error("Project name is required")
            print_info("Usage: fern sprout <project_name>")
            return
        
        project_name = args[0]
        
        # Validate project name
        if not self._is_valid_project_name(project_name):
            print_error(f"Invalid project name: {project_name}")
            print_info("Project name should only contain letters, numbers, and underscores")
            return
        
        # Check if directory already exists
        original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
        project_path = Path(original_cwd) / project_name
        if project_path.exists():
            print_error(f"Directory '{project_name}' already exists")
            return
        
        print_header(f"Creating Fern project: {project_name}")
        
        try:
            self._create_project_structure(project_name)
            self._create_project_files(project_name)
            print_success(f"🌱 Project '{project_name}' created successfully!")
            
            # Show next steps
            self._show_next_steps(project_name)
            
        except Exception as e:
            print_error(f"Failed to create project: {str(e)}")
    
    def _is_valid_project_name(self, name):
        """Validate project name"""
        return name.replace('_', '').replace('-', '').isalnum()
    
    def _create_project_structure(self, project_name):
        """Create the project directory structure"""
        # Use the original working directory if available
        original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
        project_root = Path(original_cwd) / project_name
        
        # Create main directories
        directories = [
            project_root / "lib",
            project_root / "web",
            project_root / "linux", 
            project_root / "assets",
            project_root / "examples",
            project_root / ".fern"
        ]
        
        for directory in directories:
            directory.mkdir(parents=True, exist_ok=True)
            print_info(f"Created directory: {directory}")
    
    def _create_project_files(self, project_name):
        """Create project configuration and starter files"""
        # Use the original working directory if available
        original_cwd = os.environ.get('ORIGINAL_CWD', os.getcwd())
        project_root = Path(original_cwd) / project_name
        
        # Create fern.yaml configuration
        config_content = f"""name: {project_name}
version: 1.0.0
description: A new Fern project

dependencies:
  fern: ^0.1.0

platforms:
  web:
    enabled: true
    port: 3000
  linux:
    enabled: true
    
build:
  incremental: true
  optimize: false
"""
        
        (project_root / "fern.yaml").write_text(config_content)
        
        # Create main.cpp
        main_cpp_content = f"""#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void draw() {{
    // Clear background
    Draw::fill(Colors::DarkGray);
    
    // Draw title
    DrawText::drawText("Welcome to {project_name}!", 50, 50, 3, Colors::White);
    
    // Draw subtitle
    DrawText::drawText("Your Fern project is ready!", 50, 100, 2, Colors::LightGray);
    
    // Draw instructions
    DrawText::drawText("Edit lib/main.cpp to start coding", 50, 150, 1, Colors::Cyan);
    DrawText::drawText("Run 'fern fire' to see changes", 50, 170, 1, Colors::Cyan);
}}

int main() {{
    std::cout << "🌿 Starting {project_name}..." << std::endl;
    
    // Initialize Fern
    Fern::initialize();
    
    // Set up render callback
    Fern::setDrawCallback(draw);
    
    // Start the application
    Fern::startRenderLoop();
    
    return 0;
}}
"""
        
        (project_root / "lib" / "main.cpp").write_text(main_cpp_content)
        
        # Create README.md
        readme_content = """# """ + project_name + """

A new Fern project created with `fern sprout`.

## Getting Started

1. **Run the project:**
   ```bash
   fern fire
   ```

2. **Build for web:**
   ```bash
   fern fire -p web
   ```

3. **Build for Linux:**
   ```bash
   fern fire -p linux
   ```

## Project Structure

```
""" + project_name + """/
├── lib/           # Main source code
├── web/           # Web platform specific files
│   └── template.html  # Customizable HTML template for web builds
├── linux/         # Linux platform specific files
├── assets/        # Images, fonts, etc.
├── examples/      # Example code
└── fern.yaml      # Project configuration
```

## Web Template Customization

The `web/template.html` file is used as the HTML template for web builds. You can customize:

- Page title and meta tags
- CSS styling and layout
- Additional JavaScript libraries
- Canvas container structure

The template uses `{{{ SCRIPT }}}` as a placeholder for the generated JavaScript code.

## Learn More

- [Fern Documentation](https://github.com/your-repo/fern)
- [Examples](./examples/)
"""
        
        (project_root / "README.md").write_text(readme_content)
        
        # Create web template
        self._create_web_template(project_root, project_name)
        
        # Create .gitignore
        gitignore_content = """# Build outputs
build/
*.o
*.a
*.so
*.dll
*.exe

# Platform specific
web/build/
linux/build/

# IDE
.vscode/
.idea/
*.swp
*.swo

# OS
.DS_Store
Thumbs.db
"""
        
        (project_root / ".gitignore").write_text(gitignore_content)
        
        print_info("Created project files:")
        print_info("  ├── fern.yaml")
        print_info("  ├── README.md")
        print_info("  ├── .gitignore")
        print_info("  ├── lib/main.cpp")
        print_info("  └── web/template.html")
    
    def _create_web_template(self, project_root, project_name):
        """Create a customizable web template"""
        try:
            # Copy the template.html from the main fern directory
            template_source = Path(__file__).parent.parent.parent / "template.html"
            template_dest = project_root / "web" / "template.html"
            
            if template_source.exists():
                # Read the template and customize it
                template_content = template_source.read_text()
                
                # Replace title with project name
                template_content = template_content.replace(
                    "<title>Fern Application</title>",
                    "<title>" + project_name + "</title>"
                )
                
                # Write to project web directory
                template_dest.write_text(template_content)
                print_info("Created web template: " + str(template_dest))
            else:
                # Create a basic template if source doesn't exist
                template_dest.write_text("""<!doctype html>
<html lang="en-us">
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>""" + project_name + """</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            width: 100%;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: #333;
        }
        #canvas-container {
            width: 100%;
            height: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        canvas {
            display: block;
            max-width: 100%;
            max-height: 100%;
        }
    </style>
</head>
<body>
    <div id="canvas-container">
        <canvas id="canvas" oncontextmenu="event.preventDefault()"></canvas>
    </div>
    <script type='text/javascript'>
        var Module = {
            canvas: document.getElementById('canvas')
        };
    </script>
    {{{ SCRIPT }}}
</body>
</html>""")
                print_info("Created basic web template: " + str(template_dest))
        except Exception as e:
            print_error("Error creating web template: " + str(e))
            # Create a minimal template as fallback
            template_dest.write_text("""<!doctype html>
<html><head><title>""" + project_name + """</title></head>
<body><canvas id="canvas"></canvas>
<script>var Module = {canvas: document.getElementById('canvas')};</script>
{{{ SCRIPT }}}
</body></html>""")
            print_info("Created minimal web template: " + str(template_dest))
    
    def _show_next_steps(self, project_name):
        """Show what to do next"""
        print()
        print_header("Next Steps")
        print_info("1. cd " + project_name)
        print_info("2. fern fire                 # Run your project")
        print_info("3. fern bloom                # Check system health")
        print_info("4. fern prepare web          # Build for web")
        print()
        print_success("Happy coding! 🌿")
