#!/bin/bash
# Fern Graphics CLI - A tool for compiling and serving WebAssembly graphics applications

VERSION="1.1.0"
PORT=8000

# Define color codes for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

find_cpp_sources() {
    local sources=""
    # Find all .cpp files in the src/cpp directory
    for file in $(find src/cpp/src -name "*.cpp"); do
        sources="$sources $file"
    done
    echo "$sources"
}

# Function to open URL in browser
open_browser() {
    local url=$1
    
    # Determine the OS and use appropriate command
    case "$(uname -s)" in
        Darwin*)    # macOS
            open "$url"
            ;;
        Linux*)     # Linux
            # Try different browsers
            if command -v xdg-open &> /dev/null; then
                xdg-open "$url" &> /dev/null &
            elif command -v gnome-open &> /dev/null; then
                gnome-open "$url" &> /dev/null &
            elif command -v firefox &> /dev/null; then
                firefox "$url" &> /dev/null &
            elif command -v google-chrome &> /dev/null; then
                google-chrome "$url" &> /dev/null &
            elif command -v chromium-browser &> /dev/null; then
                chromium-browser "$url" &> /dev/null &
            else
                echo -e "${YELLOW}Unable to open browser automatically. Please open the URL manually: ${url}${NC}"
                return 1
            fi
            ;;
        CYGWIN*|MINGW*|MSYS*)  # Windows
            start "$url"
            ;;
        *)
            echo -e "${YELLOW}Unable to open browser automatically. Please open the URL manually: ${url}${NC}"
            return 1
            ;;
    esac
    
    return 0
}

# Display help information
show_help() {
    echo -e "${BLUE}Fern Graphics CLI v${VERSION}${NC}"
    echo "A tool for compiling and serving WebAssembly graphics applications"
    echo ""
    echo "Usage:"
    echo "  fern [options] [file.c|file.cpp]"
    echo ""
    echo "Options:"
    echo "  --help              Show this help information"
    echo "  --version           Display version information"
    echo "  --serve             Compile and serve (default if no option specified)"
    echo "  --build             Only compile, don't start server"
    echo "  --port <number>     Use custom port for server (default: 8000)"
    echo "  --c                 Force C mode (default for .c files)"
    echo "  --cpp               Force C++ mode (default for .cpp files)"
    echo ""
    echo "Examples:"
    echo "  fern example.c                 # Compile and serve C example"
    echo "  fern example.cpp               # Compile and serve C++ example"
    echo "  fern --c example.cpp           # Force compile CPP file in C mode"
    echo "  fern --cpp example.c           # Force compile C file in C++ mode"
    echo "  fern --build main.c            # Just compile main.c without serving"
    echo "  fern --port 9000 example.c     # Use port 9000 for the server"
    echo ""
    echo -e "${YELLOW}Note: The C++ implementation is still in development and may not be fully functional${NC}"
}

# Display version information
show_version() {
    echo -e "${BLUE}Fern Graphics CLI v${VERSION}${NC}"
}

# Parse command line arguments
parse_args() {
    SERVE=true
    FILE=""
    FORCE_C=false
    FORCE_CPP=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --help)
                show_help
                exit 0
                ;;
            --version)
                show_version
                exit 0
                ;;
            --serve)
                SERVE=true
                shift
                ;;
            --build)
                SERVE=false
                shift
                ;;
            --c)
                FORCE_C=true
                FORCE_CPP=false
                shift
                ;;
            --cpp)
                FORCE_CPP=true
                FORCE_C=false
                shift
                ;;
            --port)
                if [[ -z "$2" || "$2" =~ ^- ]]; then
                    echo -e "${RED}Error: Port number is required for --port option${NC}"
                    exit 1
                fi
                PORT="$2"
                shift 2
                ;;
            --port=*)
                PORT="${1#*=}"
                shift
                ;;
            -*)
                echo -e "${RED}Error: Unknown option $1${NC}"
                show_help
                exit 1
                ;;
            *)
                if [[ -z "$FILE" ]]; then
                    FILE="$1"
                    shift
                else
                    echo -e "${RED}Error: Too many arguments. Only one source file can be specified.${NC}"
                    show_help
                    exit 1
                fi
                ;;
        esac
    done
    
    # If no file specified, check for defaults
    if [[ -z "$FILE" ]]; then
        for default_file in "main.cpp" "main.c" "example.cpp" "example.c"; do
            if [[ -f "$default_file" ]]; then
                FILE="$default_file"
                echo -e "${YELLOW}No file specified, using default: ${FILE}${NC}"
                break
            fi
        done
        
        if [[ -z "$FILE" ]]; then
            echo -e "${RED}Error: No source file specified and no default files found.${NC}"
            exit 1
        fi
    fi
    
    # Determine if it's a C or C++ file based on extension
    if [[ "$FORCE_CPP" = true ]]; then
        IS_CPP=true
        echo -e "${YELLOW}Forcing C++ mode${NC}"
    elif [[ "$FORCE_C" = true ]]; then
        IS_CPP=false
        echo -e "${YELLOW}Forcing C mode${NC}"
    elif [[ "$FILE" == *.cpp ]] || [[ "$FILE" == *.cc ]] || [[ "$FILE" == *.cxx ]]; then
        IS_CPP=true
        echo -e "${BLUE}Detected C++ file extension${NC}"
    else
        IS_CPP=false
        echo -e "${BLUE}Detected C file extension${NC}"
    fi
}

# Check for required dependencies
check_dependencies() {
    # Check for emcc (Emscripten)
    if ! command -v emcc &> /dev/null; then
        echo -e "${RED}Error: Emscripten compiler (emcc) not found.${NC}"
        echo "Please install Emscripten: https://emscripten.org/docs/getting_started/downloads.html"
        exit 1
    fi
    
    # Check for Python (for HTTP server)
    if ! command -v python3 &> /dev/null; then
        echo -e "${RED}Error: Python 3 not found.${NC}"
        echo "Please install Python 3: https://www.python.org/downloads/"
        exit 1
    fi
    
    # Extra warning for C++ mode
    if [[ "$IS_CPP" = true ]]; then
        echo -e "${YELLOW}⚠️ Warning: The C++ implementation is still in development and may not be fully functional.${NC}"
        echo -e "${YELLOW}See the migration guide for details: docs/migration-guide.md${NC}"
    fi
}

# Create a default template.html if none exists
create_default_template() {
    cat > template.html << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fern Graphics Application</title>
    <style>
        body { margin: 0; background-color: #f0f0f0; font-family: Arial, sans-serif; }
        canvas { display: block; margin: 20px auto; border: 1px solid #ccc; }
        .container { max-width: 800px; margin: 0 auto; padding: 20px; }
        h1 { color: #333; text-align: center; }
        .implementation-badge {
            position: absolute;
            top: 10px;
            right: 10px;
            padding: 5px 10px;
            border-radius: 5px;
            color: white;
            font-weight: bold;
        }
        .c-implementation {
            background-color: #007acc;
        }
        .cpp-implementation {
            background-color: #f14e32;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Fern Graphics</h1>
        <div class="implementation-badge ${IS_CPP && "cpp-implementation" || "c-implementation"}">
            ${IS_CPP && "C++ Implementation" || "C Implementation"}
        </div>
        {{{ CANVAS }}}
    </div>
</body>
</html>
EOF
    echo -e "${GREEN}Created default template.html${NC}"
}

# Create index.html in the dist directory for easier navigation
create_index_html() {
    local app_name=$1
    local dist_dir=$2
    
    cat > "$dist_dir/index.html" << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fern Graphics Applications</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            margin: 20px; 
            background: #f8f8f8;
        }
        h1 { 
            color: #333; 
            border-bottom: 2px solid #ddd;
            padding-bottom: 10px;
        }
        .implementation { 
            margin-bottom: 20px;
            padding: 15px;
            background: white;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        h2 {
            margin-top: 0;
            color: #555;
        }
        a { 
            color: #0066cc; 
            text-decoration: none;
            display: inline-block;
            margin: 5px 0;
            padding: 8px 15px;
            background: #f0f0f0;
            border-radius: 4px;
        }
        a:hover { 
            background: #e0e0e0;
        }
        .c-impl h2 { color: #007acc; }
        .cpp-impl h2 { color: #f14e32; }
        .status-badge {
            display: inline-block;
            padding: 3px 8px;
            border-radius: 4px;
            font-size: 12px;
            font-weight: bold;
            margin-left: 10px;
            vertical-align: middle;
        }
        .stable { background: #4CAF50; color: white; }
        .dev { background: #FF9800; color: white; }
    </style>
</head>
<body>
    <h1>Fern Graphics Applications</h1>
    
    <div class="implementation c-impl">
        <h2>C Implementation <span class="status-badge stable">Stable</span></h2>
        <p><a href="${app_name}.html">${app_name}</a></p>
    </div>
    
    <div class="implementation cpp-impl">
        <h2>C++ Implementation <span class="status-badge dev">Development</span></h2>
        <p>No examples available yet</p>
    </div>
</body>
</html>
EOF
    echo -e "${GREEN}Created index.html in $dist_dir directory${NC}"
}

run_interactive_server() {
    local source_file=$1
    local port=$2
    local output_base=$(basename "$source_file" .${source_file##*.})
    local dist_dir
    if [[ "$IS_CPP" = true ]]; then
        dist_dir="dist/cpp"
    else
        dist_dir="dist/c"
    fi
    local url="http://localhost:${port}/${dist_dir}/${output_base}.html"
    local server_pid
    local original_dir=$(pwd) 

    # Start server in background
    python3 -m http.server ${port} &
    server_pid=$!
    
    # Open browser
    open_browser "$url"
    
    echo -e "${GREEN}Server running at ${url}${NC}"
    echo -e "${YELLOW}Interactive mode enabled. Available commands:${NC}"
    echo "  r - Recompile and reload"
    echo "  c - Clear console"
    echo "  q - Quit server"
    echo "  h - Show this help"
    
    # Read input in a loop
    while true; do
        # Read a single character without requiring Enter
        read -n 1 -s cmd
        
        case "$cmd" in
            r|R)
                echo -e "${YELLOW}Recompiling...${NC}"
                compile_app "$source_file"
                echo -e "${GREEN}Recompiled! Refresh your browser to see changes.${NC}"
                ;;
            c|C)
                clear
                echo -e "${GREEN}Server running at ${url}${NC}"
                echo -e "${YELLOW}Interactive mode. Press 'h' for help.${NC}"
                ;;
            q|Q)
                echo -e "${YELLOW}Shutting down server...${NC}"
                kill $server_pid
                exit 0
                ;;
            h|H)
                echo -e "${YELLOW}Available commands:${NC}"
                echo "  r - Recompile and reload"
                echo "  c - Clear console"
                echo "  q - Quit server"
                echo "  h - Show this help"
                ;;
            *)
                # Ignore other keystrokes
                ;;
        esac
    done
}

compile_app() {
    local source_file=$1
    local output_base=$(basename "$source_file" .${source_file##*.})
    
    # Check if source file exists
    if [[ ! -f "$source_file" ]]; then
        echo -e "${RED}Error: $source_file not found in current directory.${NC}"
        exit 1
    fi
    
    # Check if template.html exists
    if [[ ! -f "template.html" ]]; then
        echo -e "${YELLOW}Warning: template.html not found. Creating a default template...${NC}"
        create_default_template
    fi
    
    # Determine output directory based on implementation
    local dist_dir
        if [[ "$IS_CPP" = true ]]; then
            dist_dir="dist/cpp"
        else
            dist_dir="dist/c"
        fi
    
    # Create dist directory if it doesn't exist
    if [[ ! -d "$dist_dir" ]]; then
        echo -e "${YELLOW}Creating $dist_dir directory...${NC}"
        mkdir -p "$dist_dir"
    fi
    
    # Set compiler flags based on implementation
    local std_flag="-std=gnu99"
    local include_path=""

    if [[ "$IS_CPP" = true ]]; then
        std_flag="-std=gnu++14"
        include_path="-I src/cpp/include"
        echo -e "${YELLOW}Compiling $source_file with C++ implementation...${NC}"
    else
        echo -e "${YELLOW}Compiling $source_file with C implementation...${NC}"
    fi
    
    # Compile with emscripten
    if [[ "$IS_CPP" = true ]]; then
        std_flag="-std=gnu++14"
        include_path="-I src/cpp/include"        
        # Get all C++ source files
        cpp_sources=$(find_cpp_sources)
        
        # Compile with all C++ source files
        emcc "$source_file" $cpp_sources -o "$dist_dir/${output_base}.html" --shell-file template.html \
            $std_flag $include_path \
            -s WASM=1 \
            -s EXPORTED_RUNTIME_METHODS=['cwrap','HEAPU8'] \
            -s ALLOW_MEMORY_GROWTH=1 \
            -s NO_DISABLE_EXCEPTION_CATCHING \
            --embed-file fonts
    else
        echo -e "${YELLOW}Compiling $source_file with C implementation...${NC}"
        
        # Original C compilation command
        emcc "$source_file" -o "$dist_dir/${output_base}.html" --shell-file template.html \
            $std_flag $include_path \
            -s WASM=1 \
            -s EXPORTED_RUNTIME_METHODS=['cwrap','HEAPU8'] \
            -s ALLOW_MEMORY_GROWTH=1 \
            -s NO_DISABLE_EXCEPTION_CATCHING
    fi   

    if [[ $? -eq 0 ]]; then
        echo -e "${GREEN}Compilation successful!${NC}"
        
        # Create a simple index.html if it doesn't exist to help navigation
        if [[ ! -f "$dist_dir/index.html" ]] || [[ "$output_base" != "index" ]]; then
            create_index_html "$output_base" "$dist_dir"
        else 
            # Add example to existing index if not already there
            if [[ "$IS_CPP" = true ]]; then
                if ! grep -q "${output_base}.html" "$dist_dir/index.html" || ! grep -q "C++ Implementation" "$dist_dir/index.html"; then
                    sed -i 's|<p>No examples available yet</p>|<p><a href="'${output_base}'.html">'${output_base}'</a></p>|' "$dist_dir/index.html"
                fi
            else
                if ! grep -q "${output_base}.html" "$dist_dir/index.html"; then
                    sed -i 's|<h2>C Implementation|<p><a href="'${output_base}'.html">'${output_base}'</a></p>\n        <h2>C Implementation|' "$dist_dir/index.html"
                fi
            fi
        fi
        
        echo -e "${GREEN}Output: $dist_dir/${output_base}.html${NC}"
        return 0
    else
        echo -e "${RED}Compilation failed.${NC}"
        exit 1
    fi
}

# Updated main function
main() {
    parse_args "$@"
    check_dependencies
    
    # Compile the application
    compile_app "$FILE"
    
    # Start server if --serve (default) was specified
    if [[ "$SERVE" == true ]]; then
        echo -e "${YELLOW}Starting interactive development server on port ${PORT}...${NC}"
        run_interactive_server "$FILE" "$PORT"
    fi
}

# Run the script
main "$@"