#!/usr/bin/env python3
"""
Setup script for Fern CLI
"""

from setuptools import setup, find_packages
from pathlib import Path

# Read README for long description
readme_path = Path(__file__).parent / "README.md"
long_description = readme_path.read_text(encoding="utf-8") if readme_path.exists() else ""

# Read version from CLI
version = "0.1.0"

setup(
    name="fern-ui",
    version=version,
    description="UI Framework CLI - Flutter-like development for C++",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Rishi Ahuja",
    author_email="www.rishiahuja@gmail.com",
    url="https://github.com/fernkit/fern",
    packages=find_packages(where="cli"),
    package_dir={"": "cli"},
    entry_points={
        "console_scripts": [
            "fern=fern_cli:main",
        ],
    },
    install_requires=[
        "colorama>=0.4.0",
        "pyyaml>=6.0",
        "click>=8.0",
    ],
    python_requires=">=3.7",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Topic :: Software Development :: Libraries :: Application Frameworks",
        "Topic :: Software Development :: User Interfaces",
    ],
    keywords="ui framework c++ flutter gui development",
    include_package_data=True,
    package_data={
        "": ["templates/**/*", "*.yaml", "*.yml"],
    },
    zip_safe=False,
)
