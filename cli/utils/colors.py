"""
Color utilities for terminal output
"""

class Colors:
    # ANSI color codes
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    MAGENTA = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    END = '\033[0m'

def print_colored(text, color=Colors.WHITE, bold=False):
    """Print colored text to terminal"""
    prefix = Colors.BOLD if bold else ""
    print(f"{prefix}{color}{text}{Colors.END}")

def print_success(text):
    """Print success message in green"""
    print_colored(f"✓ {text}", Colors.GREEN)

def print_error(text):
    """Print error message in red"""
    print_colored(f"✗ {text}", Colors.RED)

def print_warning(text):
    """Print warning message in yellow"""
    print_colored(f"⚠ {text}", Colors.YELLOW)

def print_info(text):
    """Print info message in blue"""
    print_colored(f"ℹ {text}", Colors.BLUE)

def print_header(text):
    """Print header with decoration"""
    print_colored(f"\n🌿 {text}", Colors.CYAN, bold=True)
    print_colored("=" * (len(text) + 3), Colors.CYAN)
