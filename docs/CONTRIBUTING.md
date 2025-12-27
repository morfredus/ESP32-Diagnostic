# Contributing (EN) – v3.32.0

> WARNING: This document reflects firmware v3.32.0 with EXACT pin mappings from `include/board_config.h`. All GPIO assignments have been verified and synchronized with the codebase. Read [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) before flashing.

We welcome improvements that strengthen the diagnostic experience while keeping the firmware reliable.

## Code contributions
1. **Fork** - start from your GitHub fork: [morfredus/ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic/fork).
2. **Scope first** - focus on bug fixes, diagnostics accuracy, and UI/UX refinements.
3. **Branching** - create feature branches from `dev/v3` when contributing new code paths.
4. **Style** - follow the existing formatting conventions in `the main source files` and related headers. Do not wrap imports in `try/catch` blocks.
5. **Translations** - always update both FR and EN strings in `languages.h` when introducing user-facing text.
6. **Testing** - provide serial logs, screenshots, or other evidence that the change works on hardware.

## Documentation updates
- Keep English and French documentation in sync.
- When editing READMEs or guides, state the tested firmware version.
- Use concise, technical language and avoid promising future features that are not implemented.
- Mention 3.30.0 when confirming maintenance work so the Wi-Fi splash, PlatformIO migration, bilingual changelog, and `DIAGNOSTIC_VERSION` remain aligned. See [RELEASE_NOTES_3.30.0.md](RELEASE_NOTES_3.30.0.md).

## Issue reporting
File bug reports via [GitHub Issues](https://github.com/morfredus/ESP32-Diagnostic/issues/new/choose) and include:
- Board model and revision.
- PlatformIO version and espressif32 platform version.
- Steps to reproduce with expected vs. actual behaviour.
- Relevant serial logs or screenshots.

## Pull requests
- Reference the issue number when applicable.
- Detail the tests executed (manual or automated).
- Explain any UI modifications and provide screenshots for visual changes.

Thank you for helping the ESP32 community!











