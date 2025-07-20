#!/bin/bash
#
# Development script for Fern CLI
# This runs the CLI directly from source without needing to reinstall
#

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Set the original working directory as an environment variable
export ORIGINAL_CWD="$(pwd)"

# Run the CLI from the source directory
python3 "$SCRIPT_DIR/cli/terra_cli.py" "$@"
