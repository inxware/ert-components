# Contributing

We welcome contributions from the community! Whether you're fixing bugs, adding features, porting to new platforms, or improving documentation, your help makes eRT better for everyone.

- [Quick contributing guide](#quick-contributing-guide)
- [Contribution areas](#contribution-areas)
- [Code review process](#code-review-process)
- [Development standards](#development-standards)
- [Commit guidelines](#commit-guidelines)
- [Getting help](#getting-help)
- [Recognition](#recognition)
- [Legal](#legal)

## Quick contributing guide

1. **Fork the repository**: e.g. use GitHub's [UI](https://github.com/inxware/ert-components/fork) for this.
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes**: and your changes and test them thoroughly.
4. **Add the changes**: `git add .`
5. **Commit**: `git commit -m 'feat(component): Added this amazing feature.'`
6. **Push**: `git push origin feature/amazing-feature`
7. **Open a Pull Request**: e.g. use GitHub's UI for this ("Contribute" -> "Open pull request").

## Contribution areas

### Bug reports

Found a bug? Help us fix it:
- Use the [bug report template](.github/ISSUE_TEMPLATE/ert-components-bug-report.md).
- Include system information, build logs, and reproduction steps.
- Test with the latest version before reporting.

### Feature requests

Have an idea for improvement?
- Use the [feature request template](.github/ISSUE_TEMPLATE/ert-components-feature-request.md).
- Describe the use case and expected behaviour.
- Consider contributing the implementation yourself!

### Platform ports

When adding support for new hardware, please keep the following in mind:
- Ensure all tests pass on your platform.
- Document hardware requirements and limitations.
- Include example applications.
- See [DEVELOPING.md](DEVELOPING.md) for more information.

### Component development

When creating new components, please try to adhere to the following guidelines:
- Include comprehensive unit and regression tests.
- Provide clear documentation and examples.
- Design for reusability across platforms.
- See [DEVELOPING.md](DEVELOPING.md) for more information.

### Documentation

You are encouraged to improving the documentation for inxware. Here are some suggestions:
- Fix typos, clarify instructions.
- Add examples and tutorials.
- Translate into other languages.
- Create video content and guides.

## Code review process

All contributions go through code review:

1. **Automated Checks**: CI runs tests on all supported platforms.
2. **Maintainer Review**: Core team reviews code quality and design.
3. **Community Review**: Other contributors may provide feedback.
4. **Testing**: Verify functionality on target platforms.
5. **Merge**: Approved changes are merged to main branch.

## Development standards

### Code quality

- **Follow existing style**: Use consistent naming and formatting.
- **Write tests**: Include unit tests for new functionality.
- **Document APIs**: All public functions need documentation.
- **Handle errors**: Proper error checking and reporting.

### Platform compatibility

- **Test on multiple platforms**: Verify cross-platform compatibility.
- **Use HAL abstractions**: Don't break platform abstraction.
- **Consider resource constraints**: MCUs have limited memory/storage.
- **Maintain backwards compatibility**: Don't break existing APIs.

### Security

- **No secrets in code**: Don't store passwords, certificates, API tokens, etc. in the repository. You must use configuration for credentials.
- **Validate inputs**: Check all external inputs.
- **Follow secure coding practices**: Prevent buffer overflows, accidental infinite loops, interrupt race conditions, etc.
- **Review dependencies**: Ensure third-party code is secure.

## Commit guidelines

- Write commit messages in English (you may include translations into other languages later in the message, if absolutely required).
- Ideally, try not to make multiple types of change in a single commit - it is a lot easier to review code where the changes are all aimed at achieving a single, specific outcome.
- Try to keep the line length in commit messages under 80 characters.

### Commit types
- `feat`: New feature.
- `fix`: Bug fix.
- `docs`: Documentation changes.
- `style`: Code style changes (formatting, etc.).
- `refactor`: Code refactoring.
- `test`: Adding or updating tests.
- `build`: Build system changes.

### Commit message format
```text
type(scope): mandatory one-line description.

[optional body]

[optional footer]
```

### Examples
```text
feat(components): add temperature sensor component.

Implements I2C temperature sensor component with calibration support.
Includes unit tests and documentation.

Closes #123
```

```text
fix(hal): resolve GPIO race condition on ESP32.

The GPIO write function had a race condition that could cause incorrect pin
states during rapid switching.
```

```text
test(build): add regression test for android build.

Ensures Android APK builds correctly with all required dependencies and
proper signing.
```

## Getting help

Check existing [issues](https://github.com/inxware/ert-components/issues) and [discussions](https://github.com/inxware/ert-components/discussions) and raise a new ticket, or create/join the discussions as required.

## Recognition

Contributors are recognised through:
- **Hall of Fame**: Featured on our website and documentation.
- **Contributor Badge**: Special recognition in the community.
- **Early Access**: Beta access to new features and tools.
- **Mentorship**: Direct support from core team members.
- **Conference Opportunities**: Speaking opportunities at events.

## Legal

By contributing, you agree that:
- Your contributions will be licensed under LGPLv3.
- You have the right to contribute the code.
- Your contributions may be used commercially under the inxware license.

## Attribution

*Copyright © 2008–2025 inx Limited. The community **eRT Components** release is open‑source under **LGPLv3**. The **EHS Kernel** is proprietary and licensed separately as described in [LICENSE.md](LICENSE.md).*
