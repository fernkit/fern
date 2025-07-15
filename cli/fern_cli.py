#!/usr/bin/env python3
"""
Fern CLI - Flutter-like command line interface for Fern framework
Usage: fern [command] [options]

Commands:
  bloom                    Check system health (like flutter doctor)
  sprout <project_name>    Create new Fern project
  fire [file]             Run single file or project
  prepare <platform>       Build for target platform
  install <package>        Install Fern packages
  templates               Manage project templates
  help                    Show this help message
"""

import sys
import os
import argparse
import subprocess
from pathlib import Path

# Add the CLI modules to Python path
CLI_DIR = Path(__file__).parent
sys.path.insert(0, str(CLI_DIR))

from commands.bloom import BloomCommand
from commands.sprout import SproutCommand
from commands.fire import FireCommand
from commands.prepare import PrepareCommand
from commands.install import InstallCommand
from commands.templates import TemplatesCommand
from utils.colors import Colors, print_colored

class FernCLI:
    def __init__(self):
        self.commands = {
            'bloom': BloomCommand(),
            'sprout': SproutCommand(),
            'fire': FireCommand(),
            'prepare': PrepareCommand(),
            'install': InstallCommand(),
            'templates': TemplatesCommand(),
        }
    
    def run(self, args):
        if len(args) == 0 or args[0] in ['help', '-h', '--help']:
            self.show_help()
            return
        
        command_name = args[0]
        command_args = args[1:]
        
        if command_name not in self.commands:
            print_colored(f"Unknown command: {command_name}", Colors.RED)
            print_colored("Run 'fern help' to see available commands", Colors.YELLOW)
            return
        
        try:
            self.commands[command_name].execute(command_args)
        except Exception as e:
            print_colored(f"Error executing {command_name}: {str(e)}", Colors.RED)
            sys.exit(1)
    
    def show_help(self):
        print_colored("🌿 Fern CLI - Modern UI Framework", Colors.GREEN)
        print_colored("=" * 40, Colors.CYAN)
        print(__doc__)
        
        print_colored("\nExamples:", Colors.CYAN)
        print("  fern bloom                    # Check system health")
        print("  fern sprout my_app            # Create new project")
        print("  fern fire main.cpp            # Run single file")
        print("  fern fire                     # Run current project")
        print("  fern prepare web              # Build for web")
        print("  fern prepare linux            # Build for linux")

def main():
    cli = FernCLI()
    cli.run(sys.argv[1:])

if __name__ == "__main__":
    main()
