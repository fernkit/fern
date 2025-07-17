# Contributing to Fern

Thank you for your interest in contributing to Fern! This document provides guidelines and information about how to contribute to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Documentation](#documentation)
- [Community](#community)

## Code of Conduct

This project and everyone participating in it is governed by our Code of Conduct. By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

### Our Pledge

We pledge to make participation in our project a harassment-free experience for everyone, regardless of age, body size, disability, ethnicity, gender identity and expression, level of experience, nationality, personal appearance, race, religion, or sexual identity and orientation.

## Getting Started

### Prerequisites

- **C++ Compiler**: GCC 8+ or Clang 10+
- **CMake**: Version 3.10 or higher
- **Git**: For version control
- **Platform Dependencies**:
  - **Linux**: X11 development libraries, OpenGL
  - **macOS**: Xcode command line tools
  - **Windows**: Visual Studio 2019+ or MinGW

### Development Setup

1. **Fork the repository** on GitHub
2. **Clone your fork**:
   ```bash
   git clone https://github.com/your-username/fern.git
   cd fern
   ```

3. **Build the project**:
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

4. **Run examples**:
   ```bash
   # Run a simple example
   ./fern-cli.sh examples/cpp/basic_counter_example.cpp
   
   # Or manually compile and run
   g++ -I src/cpp/include examples/cpp/basic_counter_example.cpp -o example
   ./example
   ```

5. **Set up your development environment**:
   ```bash
   # Create a new branch for your feature
   git checkout -b feature/your-feature-name
   ```

## How to Contribute

### Reporting Bugs

Before creating bug reports, please check the issue list to avoid duplicates. When creating a bug report, include:

- **Clear title** describing the issue
- **Detailed description** of the problem
- **Steps to reproduce** the issue
- **Expected vs actual behavior**
- **Environment details** (OS, compiler, Fern version)
- **Code samples** that demonstrate the issue
- **Screenshots or GIFs** if applicable

Use our [Bug Report Template](.github/ISSUE_TEMPLATE/bug_report.md).

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

- **Clear title** describing the enhancement
- **Detailed description** of the proposed feature
- **Use case** explaining why this would be valuable
- **Possible implementation** approach if you have ideas
- **Examples** of how the feature would be used

Use our [Feature Request Template](.github/ISSUE_TEMPLATE/feature_request.md).

### Contributing Code

1. **Pick an issue** from our issue tracker or create a new one
2. **Comment on the issue** to let others know you're working on it
3. **Create a branch** from `main` for your work
4. **Make your changes** following our coding standards
5. **Add tests** for new functionality
6. **Update documentation** as needed
7. **Submit a pull request**

## Pull Request Process

### Before Submitting

- [ ] **Code compiles** without warnings
- [ ] **All tests pass** (if applicable)
- [ ] **Code follows** our style guidelines
- [ ] **Documentation updated** for public API changes
- [ ] **Examples added/updated** for new features
- [ ] **Commit messages** are clear and descriptive

### PR Checklist

- [ ] **Title** clearly describes the change
- [ ] **Description** explains what and why
- [ ] **Links to issue** if applicable
- [ ] **Screenshots/GIFs** for UI changes
- [ ] **Breaking changes** are documented
- [ ] **Backwards compatibility** considered

### Review Process

1. **Automated checks** will run on your PR
2. **Maintainer review** will be requested
3. **Address feedback** promptly
4. **Squash commits** before merge (if requested)
5. **Celebrate** your contribution! 🎉

## Coding Standards

### C++ Style Guide

We follow a modified version of the Google C++ Style Guide:

#### File Organization
```cpp
// Headers first, then implementation
#include <fern/fern.hpp>
#include <standard_library.hpp>
#include "local_header.hpp"

using namespace Fern;  // Only in .cpp files, not headers
```

#### Naming Conventions
```cpp
// Classes: PascalCase
class TextWidget { };

// Functions and variables: camelCase
void setupUI();
int fontSize = 16;

// Constants: UPPER_SNAKE_CASE
const int MAX_WIDGETS = 1000;

// Private members: trailing underscore
class Button {
private:
    int width_;
    std::string label_;
};
```

#### Formatting
- **Indentation**: 4 spaces (no tabs)
- **Line length**: 100 characters max
- **Braces**: Opening brace on same line
```cpp
if (condition) {
    doSomething();
} else {
    doSomethingElse();
}
```

#### Widget Development Guidelines

```cpp
class MyWidget : public Widget {
public:
    // Constructor with configuration
    MyWidget(const MyWidgetConfig& config);
    
    // Required overrides
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Public signals
    Signal<> onMyEvent;

private:
    MyWidgetConfig config_;
};

// Always provide a factory function
std::shared_ptr<MyWidget> MyWidget(
    const MyWidgetConfig& config, 
    bool addToManager = true
);
```

## Testing

### Running Tests
```bash
# Build and run all tests
./build.sh test

# Run specific examples
./fern-cli.sh examples/cpp/basic_counter_example.cpp
```

### Writing Tests
- **Unit tests** for individual components
- **Integration tests** for widget interactions
- **Visual tests** for rendering correctness
- **Platform tests** for cross-platform compatibility

## Documentation

### API Documentation
- Use **Doxygen comments** for all public APIs
- Include **usage examples** in header comments
- Document **parameter constraints** and **return values**

### User Documentation
- Update **README.md** for major features
- Add **examples** in `examples/` directory
- Update **docs/** for comprehensive guides

## Community

### Communication Channels
- **GitHub Issues**: Bug reports and feature requests
- **GitHub Discussions**: General questions and ideas
- **Pull Requests**: Code review and collaboration

### Getting Help
- Check the **documentation** first
- Search **existing issues** for similar problems
- Ask questions in **GitHub Discussions**

## License

By contributing, you agree that your contributions will be licensed under the project's MIT License.

## Questions?

Don't hesitate to ask! We're here to help:
- Create an issue for bugs or feature requests
- Start a discussion for questions or ideas
- Reach out to maintainers for guidance

Thank you for contributing to Fern! 🌿