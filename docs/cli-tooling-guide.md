# CLI Tooling Design Guide

## What is a CLI Tool?

A CLI (Command Line Interface) tool is a program that users interact with through text commands in a terminal. Good CLI tools help developers by automating repetitive tasks, providing shortcuts, and standardizing workflows.

## Types of CLI Architecture

### 1. **Single Binary Approach**
- **Description**: One executable file that handles all commands
- **Examples**: `git`, `npm`, `docker`
- **Pros**: 
  - Single file to distribute
  - Consistent behavior across commands
  - Easy to install and manage
- **Cons**: 
  - Can become large and slow to start
  - All features loaded even if not used

### 2. **Multi-Script Approach**
- **Description**: Multiple separate scripts for different tasks
- **Examples**: Old Unix tools, custom build scripts
- **Pros**:
  - Fast startup (only loads what's needed)
  - Easy to develop and debug individual features
  - Can use different languages for different tasks
- **Cons**:
  - Hard to distribute
  - Inconsistent behavior between scripts
  - PATH management issues

### 3. **Hybrid Approach** (Recommended)
- **Description**: Main dispatcher with separate modules/plugins
- **Examples**: `cargo`, `kubectl`, `heroku`
- **Pros**:
  - Best of both worlds
  - Plugin architecture allows extensions
  - Consistent interface with specialized backends
- **Cons**:
  - More complex to implement initially

## Designing the Fern CLI

### Current State Analysis
The existing `fern-cli.sh` is a bash script that:
- Compiles C++ code to WebAssembly
- Serves files via HTTP
- Opens browser automatically
- Handles different build configurations

### Proposed Architecture: **Hybrid Approach**

```
fern (main binary)
├── fern new <project-name>         # Project scaffolding
├── fern build [--watch]            # Incremental compilation
├── fern serve [--port=8000]        # Development server
├── fern test                       # Run tests
├── fern clean                      # Clean build artifacts
└── fern deploy                     # Deploy to web
```

### Implementation Strategy

#### Phase 1: Core CLI Structure
Create a main `fern` binary that:
1. **Parses command-line arguments**
2. **Dispatches to appropriate handlers**
3. **Provides help and error handling**
4. **Manages configuration**

#### Phase 2: Project Scaffolding
Implement `fern new` command:
- Create project from templates
- Initialize build configuration
- Set up directory structure
- Generate example code

#### Phase 3: Incremental Build System
Implement `fern build` command:
- Track file dependencies
- Only rebuild changed files
- Cache intermediate results
- Support watch mode for development

#### Phase 4: Development Server
Implement `fern serve` command:
- Hot reload on file changes
- Automatic browser opening
- Development middleware
- Error overlay

## Implementation Options

### Option 1: Pure C++ Binary
**Pros**: 
- Native performance
- Single dependency
- Consistent with main project

**Cons**:
- Complex argument parsing
- Platform-specific builds needed
- File system operations are verbose

### Option 2: Rust Binary (Recommended)
**Pros**:
- Excellent CLI libraries (`clap`, `tokio`)
- Cross-platform by default
- Great performance
- Rich ecosystem for file operations

**Cons**:
- Additional language in project
- Learning curve for team

### Option 3: Node.js/TypeScript
**Pros**:
- Easy to distribute via npm
- Rich ecosystem
- Good for web-focused tool

**Cons**:
- Requires Node.js runtime
- Slower startup
- JavaScript quirks

### Option 4: Go Binary
**Pros**:
- Fast compilation
- Single binary output
- Good CLI libraries
- Easy cross-compilation

**Cons**:
- Another language to maintain
- Different toolchain

## Recommended Implementation: Rust CLI

### Project Structure
```
fern-cli/
├── Cargo.toml
├── src/
│   ├── main.rs              # Entry point & command dispatch
│   ├── commands/
│   │   ├── mod.rs
│   │   ├── new.rs           # Project scaffolding
│   │   ├── build.rs         # Incremental compilation
│   │   ├── serve.rs         # Development server
│   │   └── clean.rs         # Clean build artifacts
│   ├── templates/           # Project templates
│   │   ├── basic/
│   │   ├── game/
│   │   └── dashboard/
│   ├── build/
│   │   ├── mod.rs
│   │   ├── dependency.rs    # Dependency tracking
│   │   ├── compiler.rs      # Wrapper around g++/emcc
│   │   └── watcher.rs       # File system watching
│   └── config/
│       ├── mod.rs
│       └── project.rs       # Project configuration
└── templates/               # Project templates
    ├── basic/
    │   ├── src/main.cpp
    │   ├── CMakeLists.txt
    │   └── README.md
    └── game/
        ├── src/game.cpp
        ├── assets/
        └── README.md
```

### Dependencies (Cargo.toml)
```toml
[dependencies]
clap = "4.0"           # Command line argument parsing
tokio = "1.0"          # Async runtime
serde = "1.0"          # Serialization
serde_json = "1.0"     # JSON handling
notify = "5.0"         # File system watching
walkdir = "2.0"        # Directory traversal
reqwest = "0.11"       # HTTP client
tower = "0.4"          # Web server
axum = "0.6"           # Web framework
```

### Core CLI Implementation

#### main.rs
```rust
use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(name = "fern")]
#[command(about = "Fern UI Framework CLI")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Create a new Fern project
    New {
        /// Project name
        name: String,
        /// Project template
        #[arg(short, long, default_value = "basic")]
        template: String,
    },
    /// Build the project
    Build {
        /// Watch for changes
        #[arg(short, long)]
        watch: bool,
        /// Build for production
        #[arg(short, long)]
        release: bool,
    },
    /// Serve the project locally
    Serve {
        /// Port to serve on
        #[arg(short, long, default_value = "8000")]
        port: u16,
        /// Auto-open browser
        #[arg(long)]
        open: bool,
    },
    /// Clean build artifacts
    Clean,
    /// Run tests
    Test,
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cli = Cli::parse();
    
    match &cli.command {
        Commands::New { name, template } => {
            commands::new::run(name, template).await?;
        }
        Commands::Build { watch, release } => {
            commands::build::run(*watch, *release).await?;
        }
        Commands::Serve { port, open } => {
            commands::serve::run(*port, *open).await?;
        }
        Commands::Clean => {
            commands::clean::run().await?;
        }
        Commands::Test => {
            commands::test::run().await?;
        }
    }
    
    Ok(())
}
```

### Incremental Build System

#### build/dependency.rs
```rust
use std::collections::HashMap;
use std::path::Path;
use std::time::SystemTime;

pub struct DependencyTracker {
    dependencies: HashMap<String, Vec<String>>,
    timestamps: HashMap<String, SystemTime>,
}

impl DependencyTracker {
    pub fn new() -> Self {
        Self {
            dependencies: HashMap::new(),
            timestamps: HashMap::new(),
        }
    }
    
    pub fn needs_rebuild(&self, target: &str) -> bool {
        // Check if target exists
        if !Path::new(target).exists() {
            return true;
        }
        
        // Check if any dependencies are newer
        if let Some(deps) = self.dependencies.get(target) {
            for dep in deps {
                if self.is_newer(dep, target) {
                    return true;
                }
            }
        }
        
        false
    }
    
    pub fn add_dependency(&mut self, target: &str, dependency: &str) {
        self.dependencies
            .entry(target.to_string())
            .or_insert_with(Vec::new)
            .push(dependency.to_string());
    }
    
    fn is_newer(&self, file1: &str, file2: &str) -> bool {
        // Compare file modification times
        // Return true if file1 is newer than file2
        match (
            std::fs::metadata(file1).and_then(|m| m.modified()),
            std::fs::metadata(file2).and_then(|m| m.modified()),
        ) {
            (Ok(time1), Ok(time2)) => time1 > time2,
            (Ok(_), Err(_)) => true,  // dependency exists, target doesn't
            _ => false,
        }
    }
}
```

#### commands/build.rs
```rust
use crate::build::DependencyTracker;
use std::process::Command;
use notify::{Watcher, RecursiveMode, Event};

pub async fn run(watch: bool, release: bool) -> Result<(), Box<dyn std::error::Error>> {
    let mut tracker = DependencyTracker::new();
    
    // Initial build
    build_project(&mut tracker, release).await?;
    
    if watch {
        println!("Watching for changes...");
        watch_files(&mut tracker, release).await?;
    }
    
    Ok(())
}

async fn build_project(tracker: &mut DependencyTracker, release: bool) -> Result<(), Box<dyn std::error::Error>> {
    // Scan for source files
    let cpp_files = find_cpp_files("src/")?;
    let target = if release { "dist/app.js" } else { "build/app.js" };
    
    // Check if rebuild needed
    let mut needs_rebuild = false;
    for file in &cpp_files {
        tracker.add_dependency(target, file);
        if tracker.needs_rebuild(target) {
            needs_rebuild = true;
            break;
        }
    }
    
    if !needs_rebuild {
        println!("✅ Up to date");
        return Ok(());
    }
    
    println!("🔨 Building...");
    
    // Build command
    let mut cmd = Command::new("emcc");
    cmd.args(&cpp_files);
    cmd.args(&["-o", target]);
    cmd.args(&["-std=c++14"]);
    cmd.args(&["-I", "src/cpp/include"]);
    
    if release {
        cmd.args(&["-O3", "-DNDEBUG"]);
    } else {
        cmd.args(&["-O0", "-g"]);
    }
    
    let output = cmd.output()?;
    
    if output.status.success() {
        println!("✅ Build successful");
    } else {
        eprintln!("❌ Build failed:");
        eprintln!("{}", String::from_utf8_lossy(&output.stderr));
        return Err("Build failed".into());
    }
    
    Ok(())
}

async fn watch_files(tracker: &mut DependencyTracker, release: bool) -> Result<(), Box<dyn std::error::Error>> {
    let (tx, rx) = std::sync::mpsc::channel();
    let mut watcher = notify::recommended_watcher(tx)?;
    
    watcher.watch("src/".as_ref(), RecursiveMode::Recursive)?;
    
    loop {
        match rx.recv() {
            Ok(event) => {
                if let Ok(event) = event {
                    if event.kind.is_modify() {
                        println!("📁 File changed, rebuilding...");
                        if let Err(e) = build_project(tracker, release).await {
                            eprintln!("Build error: {}", e);
                        }
                    }
                }
            }
            Err(e) => {
                eprintln!("Watch error: {}", e);
                break;
            }
        }
    }
    
    Ok(())
}

fn find_cpp_files(dir: &str) -> Result<Vec<String>, Box<dyn std::error::Error>> {
    let mut files = Vec::new();
    
    for entry in walkdir::WalkDir::new(dir) {
        let entry = entry?;
        if let Some(ext) = entry.path().extension() {
            if ext == "cpp" || ext == "cc" || ext == "cxx" {
                files.push(entry.path().to_string_lossy().to_string());
            }
        }
    }
    
    Ok(files)
}
```

## Project Templates

### Basic Template Structure
```
templates/basic/
├── src/
│   └── main.cpp
├── assets/
│   └── fonts/
├── CMakeLists.txt
├── .gitignore
└── README.md
```

### Template main.cpp
```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Your UI code here
    auto title = Text(Point(50, 50), "Hello Fern!", 3, Colors::Blue);
    auto button = Button({
        .x = 50, .y = 150, .width = 200, .height = 50,
        .normalColor = Colors::Green,
        .hoverColor = Colors::LightGreen,
        .pressColor = Colors::DarkGreen,
        .label = "Click Me!",
        .textScale = 2,
        .textColor = Colors::White
    });
    
    button->onClick.connect([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    addWidget(title);
    addWidget(button);
}

int main() {
    setupUI();
    return 0;
}
```

## Benefits of This Approach

### 1. **Incremental Compilation**
- Only rebuild changed files
- Dependency tracking prevents unnecessary work
- Watch mode for instant feedback

### 2. **Project Scaffolding**
- Multiple templates for different use cases
- Consistent project structure
- Pre-configured build settings

### 3. **Developer Experience**
- Single command to start: `fern new my-project`
- Automatic browser opening
- Hot reload during development

### 4. **Performance**
- Native binary (fast startup)
- Parallel compilation where possible
- Efficient file watching

## Installation and Distribution

### Option 1: Direct Binary Distribution
- Release binaries for each platform
- Users download and install manually
- Simple but manual process

### Option 2: Package Managers
- **Cargo**: `cargo install fern-cli`
- **npm**: `npm install -g fern-cli`
- **Homebrew**: `brew install fern-cli`
- **apt/yum**: Platform-specific packages

### Option 3: curl/wget Installer
```bash
curl -fsSL https://fern.dev/install.sh | sh
```

## Migration from Current System

### Phase 1: Parallel Implementation
- Keep existing `fern-cli.sh` working
- Implement new CLI alongside
- Test with beta users

### Phase 2: Feature Parity
- Ensure new CLI supports all existing features
- Add new incremental build features
- Performance testing

### Phase 3: Migration
- Update documentation
- Deprecate old CLI
- Provide migration guide

## Conclusion

A well-designed CLI tool can dramatically improve the developer experience with Fern. The hybrid approach with Rust implementation provides:

1. **Fast, native performance**
2. **Incremental compilation** that only rebuilds what changed
3. **Project scaffolding** for quick starts
4. **Excellent developer experience** with hot reload
5. **Cross-platform support** out of the box

This approach scales well and can be extended with plugins and additional features as the framework grows.

---

*This guide provides a comprehensive approach to building a modern CLI tool for Fern UI Framework*
*Last updated: 2025-07-15*
