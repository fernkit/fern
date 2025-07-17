# Contributing to Fern UI Framework

## Overview

Thank you for your interest in contributing to the Fern UI Framework! This guide will help you get started with contributing code, documentation, examples, and bug reports.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Development Setup](#development-setup)
3. [Code Standards](#code-standards)
4. [Testing Guidelines](#testing-guidelines)
5. [Submitting Changes](#submitting-changes)
6. [Documentation Contributions](#documentation-contributions)
7. [Bug Reports](#bug-reports)
8. [Feature Requests](#feature-requests)
9. [Community Guidelines](#community-guidelines)

## Getting Started

### Prerequisites

- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2019+)
- CMake 3.15 or higher
- Git for version control
- For web builds: Emscripten SDK

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
```bash
git clone https://github.com/yourusername/fern.git
cd fern
```
3. Add the upstream repository:
```bash
git remote add upstream https://github.com/fernkit/fern.git
```

## Development Setup

### Building the Project

```bash
# Linux/macOS build
./build.sh linux

# Web build
./build.sh web

# Clean build
rm -rf build && ./build.sh linux
```

### Running Tests

```bash
# Build and run examples
cd build
make
./examples/basic_example

# Test specific widgets
./examples/button_test
./examples/layout_test
```

### Development Workflow

1. Create a feature branch:
```bash
git checkout -b feature/your-feature-name
```

2. Make your changes
3. Test thoroughly
4. Commit with clear messages
5. Push and create a pull request

## Code Standards

### C++ Style Guidelines

#### Header Files
- Use `#pragma once` for header guards
- Include comprehensive docstrings for all public APIs
- Follow the established widget pattern for new widgets

```cpp
/**
 * @brief Brief description of the class
 * 
 * Detailed description explaining the purpose, usage patterns,
 * and any important considerations.
 * 
 * @example Basic usage:
 * @code
 * auto widget = MyWidget(config);
 * widget->setValue(42);
 * @endcode
 */
class MyWidget : public Widget {
public:
    /**
     * @brief Constructor description
     * @param param Parameter description
     */
    MyWidget(const MyConfig& config);
    
    // ... rest of class
};
```

#### Code Formatting
- Use 4 spaces for indentation (no tabs)
- Opening braces on same line for functions and classes
- Clear, descriptive variable names
- Consistent naming conventions:
  - Classes: `PascalCase`
  - Functions/methods: `camelCase`
  - Variables: `camelCase`
  - Private members: `camelCase_` (trailing underscore)
  - Constants: `UPPER_CASE`

#### Widget Development Pattern
When creating new widgets, follow this pattern:

1. **Style Configuration Class**: Fluent interface for styling
2. **Config Class**: Combines position, content, and style
3. **Widget Class**: Inherits from Widget, implements render() and handleInput()
4. **Factory Function**: Convenient creation function
5. **Preset Namespace**: Common configurations

### Documentation Standards

#### Docstring Requirements
All public APIs must have comprehensive docstrings:

```cpp
/**
 * @brief Brief one-line description
 * 
 * Detailed explanation of functionality, behavior, and usage.
 * Include information about parameters, return values, and any
 * side effects or important considerations.
 * 
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * 
 * @example Basic usage:
 * @code
 * auto result = myFunction(value1, value2);
 * @endcode
 * 
 * @note Any important notes or warnings
 * @see Related functions or classes
 */
```

#### Example Code
- All examples must compile and run correctly
- Include error handling where appropriate
- Show both basic and advanced usage patterns
- Add comments explaining key concepts

## Testing Guidelines

### Widget Testing
Each widget should be tested with:

1. **Basic functionality test**
2. **Style configuration test**
3. **Event handling test**
4. **Layout integration test**
5. **Cross-platform compatibility test**

### Example Testing Pattern
```cpp
// Test basic widget creation
void testWidgetCreation() {
    auto config = MyWidgetConfig(0, 0, 100, 50);
    auto widget = MyWidget(config);
    assert(widget->getWidth() == 100);
    assert(widget->getHeight() == 50);
}

// Test style configuration
void testWidgetStyling() {
    MyWidgetStyle style;
    style.color(Colors::Red).border(2);
    
    auto config = MyWidgetConfig(0, 0, 100, 50).style(style);
    auto widget = MyWidget(config);
    // Verify styling was applied correctly
}
```

### Integration Testing
- Test widgets within layout systems
- Verify event handling chains
- Test cross-platform compatibility
- Performance testing for complex UIs

## Submitting Changes

### Pull Request Process

1. **Update documentation** for any API changes
2. **Add or update tests** for new functionality
3. **Ensure all tests pass** on both Linux and web platforms
4. **Update examples** if introducing new features
5. **Write clear commit messages**

### Commit Message Format
```
type(scope): brief description

Detailed explanation of the changes, including:
- What was changed and why
- Any breaking changes
- References to issues or discussions

Fixes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

### Code Review Guidelines
- Be respectful and constructive
- Focus on code quality and maintainability
- Suggest improvements clearly
- Test the changes locally when possible

## Documentation Contributions

### Adding New Documentation
1. Follow the established structure in `docs/README.md`
2. Include practical examples and code samples
3. Add links to related documentation
4. Update the main documentation index

### Documentation Standards
- Clear, concise writing
- Practical examples that work
- Screenshots/diagrams where helpful
- Consistent formatting and style
- Regular updates to keep content current

## Bug Reports

### Before Reporting
1. Check existing issues for duplicates
2. Test with the latest version
3. Try to reproduce with minimal code
4. Gather system information

### Bug Report Template
```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Create widget with '...'
2. Call method '...'
3. See error

**Expected behavior**
A clear description of what you expected to happen.

**Code Example**
```cpp
// Minimal code that reproduces the issue
```

**Environment**
- OS: [e.g., Ubuntu 20.04]
- Compiler: [e.g., GCC 9.3]
- Fern version: [e.g., v1.0.0]
- Build type: [Linux/Web]

**Additional context**
Add any other context about the problem here.
```

## Feature Requests

### Proposing New Features
1. Check existing issues and discussions
2. Describe the use case clearly
3. Provide example API design
4. Consider implementation complexity
5. Discuss with maintainers before large changes

### Feature Request Template
```markdown
**Feature Description**
A clear description of the proposed feature.

**Use Case**
Describe the problem this feature would solve.

**Proposed API**
```cpp
// Example of how the feature might be used
auto widget = NewWidget(config);
```

**Alternative Solutions**
Describe alternatives you've considered.

**Additional Context**
Any other context, mockups, or examples.
```

## Community Guidelines

### Code of Conduct
- Be welcoming and inclusive
- Be respectful and professional
- Focus on constructive feedback
- Help others learn and grow
- Follow the project's technical standards

### Communication Channels
- GitHub Issues: Bug reports, feature requests
- GitHub Discussions: General questions, ideas
- Pull Request Reviews: Code feedback
- Documentation: Inline comments and guides

### Getting Help
- Check the documentation first
- Search existing issues
- Ask specific, detailed questions
- Provide minimal reproduction cases
- Be patient and respectful

## Development Resources

### Useful Tools
- **Debuggers**: GDB, LLDB, Visual Studio debugger
- **Static Analysis**: Clang-tidy, Cppcheck
- **Memory Tools**: Valgrind, AddressSanitizer
- **Build Tools**: CMake, Ninja
- **Documentation**: Doxygen for API docs

### Learning Resources
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)
- [CMake Documentation](https://cmake.org/documentation/)
- [Emscripten Documentation](https://emscripten.org/docs/)

### Project Architecture
- Study existing widget implementations
- Understand the layout system design
- Learn the event/signal system
- Review cross-platform abstractions

---

Thank you for contributing to Fern! Your efforts help make the framework better for everyone.

## Related Documentation

- [Architecture Guide](architecture.md)
- [Testing Guide](testing.md)
- [Performance Guide](performance.md)
- [Getting Started](../getting-started.md)
- [API Reference](../api-reference.md)
