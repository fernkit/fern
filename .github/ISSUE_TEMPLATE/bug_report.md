---
name: Bug Report
about: Create a report to help us improve Fern
title: '[BUG] '
labels: 'bug'
assignees: ''
---

## Bug Description
**A clear and concise description of what the bug is.**

## Steps to Reproduce
**Steps to reproduce the behavior:**
1. Go to '...'
2. Click on '...'
3. Scroll down to '...'
4. See error

## Expected Behavior
**A clear and concise description of what you expected to happen.**

## Actual Behavior
**A clear and concise description of what actually happened.**

## Code Sample
**Please provide a minimal code sample that reproduces the issue:**

```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Your code that demonstrates the bug
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback([]() { Draw::fill(Colors::Black); });
    Fern::startRenderLoop();
    return 0;
}
```

## Screenshots/GIFs
**If applicable, add screenshots or GIFs to help explain your problem.**

## Environment
**Please complete the following information:**
- **OS**: [e.g. Ubuntu 22.04, macOS 13.0, Windows 11]
- **Compiler**: [e.g. GCC 11.2, Clang 14.0, MSVC 2022]
- **Fern Version**: [e.g. 1.0.0, commit hash if using development version]
- **CMake Version**: [e.g. 3.22.1]

## Additional Context
**Add any other context about the problem here.**

## Possible Solution
**If you have ideas on how to fix this, please describe them here.**

## Related Issues
**Link to any related issues or pull requests.**

---

**Before submitting:**
- [ ] I have checked the [documentation](https://github.com/fernkit/fern/blob/master/README.md)
- [ ] I have searched for similar issues
- [ ] I have provided a minimal code sample
- [ ] I have included my environment details
