## Description
**Briefly describe what this PR does and why it's needed.**

Fixes #(issue number)

## Type of Change
**Mark the relevant option:**
- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to change)
- [ ] Documentation update
- [ ] Code cleanup/refactoring
- [ ] Performance improvement
- [ ] Build/infrastructure change

## Changes Made
**Detailed list of changes:**
- Change 1
- Change 2
- Change 3

## Code Examples
**If this adds or changes public APIs, show how to use them:**

```cpp
#include <fern/fern.hpp>

using namespace Fern;

void setupUI() {
    // Example demonstrating the new feature or fixed functionality
    auto widget = NewWidget(Config(/*...*/));
    addWidget(widget);
}
```

## Testing
**How has this been tested?**
- [ ] Unit tests added/updated
- [ ] Integration tests added/updated
- [ ] Manual testing performed
- [ ] Examples updated and tested
- [ ] Cross-platform testing (if applicable)

**Test Results:**
- Tested on: [OS/Compiler combinations]
- Examples verified: [list of examples tested]

## Screenshots
**If this changes UI or visual behavior, include before/after screenshots or GIFs:**

| Before | After |
|--------|-------|
| ![before](url) | ![after](url) |

## Documentation
**Documentation changes made:**
- [ ] API documentation updated (Doxygen comments)
- [ ] User documentation updated (`docs/` folder)
- [ ] README.md updated (if needed)
- [ ] Examples added/updated
- [ ] Migration guide added (for breaking changes)

## Checklist
**Before requesting review:**
- [ ] Code compiles without warnings
- [ ] All existing tests pass
- [ ] New tests added for new functionality
- [ ] Code follows the project's style guidelines
- [ ] Documentation updated for public API changes
- [ ] Examples updated if needed
- [ ] Commit messages are clear and descriptive
- [ ] PR title clearly describes the change

## Breaking Changes
**If this is a breaking change, describe what breaks and how to migrate:**

### What breaks:
- API change 1
- API change 2

### Migration guide:
```cpp
// Old way:
OldAPI::method();

// New way:
NewAPI::method();
```

## Performance Impact
**If this affects performance, provide benchmarks or analysis:**
- Performance impact: [Better/Worse/No change]
- Benchmarks: [if applicable]

## Additional Notes
**Any additional information reviewers should know:**

## Related Issues
**Link related issues or discussions:**
- Closes #123
- Related to #456
- Follow-up to #789

---

**For Reviewers:**
- Focus areas for review: [specific areas that need attention]
- Known limitations: [if any]
- Future improvements: [if applicable]
