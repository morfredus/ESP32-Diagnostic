# Release Notes - Version 3.20.3

**Release Date**: December 8, 2025  
**Type**: Patch Release - Code Optimization

## Overview

Version 3.20.3 is a code optimization release focusing exclusively on memory efficiency improvements. This release applies 9 systematic optimizations that eliminate over 90 string allocations throughout the codebase without changing any functionality, pins, or hardware behavior.

## What's New

### Code Optimization ([OPT-001] through [OPT-009])

**Memory Efficiency Improvements:**
- **90+ string allocations eliminated** through unified buffer-based approaches
- **13 debug/status messages** converted from String concatenation to snprintf formatting
- **30+ test result assignments** now use pre-allocated constants
- **O(1) GPIO list building** replacing O(n) allocation pattern

**Specific Optimizations:**

1. **[OPT-001] Version String Formatting**
   - Changed from 11 allocations to 1 using snprintf buffer
   - Location: `getArduinoCoreVersionString()`

2. **[OPT-002] Uptime Formatting**
   - Changed from 4-6 allocations to 1 using buffer-based approach
   - Location: `formatUptime()`

3. **[OPT-003] Extern Declarations**
   - Reorganized with inline documentation for clarity
   - Location: `include/web_interface.h`

4. **[OPT-004] Test Result Constant**
   - Created `DEFAULT_TEST_RESULT_STR` used in 10+ initialization locations
   - Eliminates repeated `String(Texts::not_tested)` allocations

5. **[OPT-005] GPIO List Building**
   - Changed from O(n) String += loop to single snprintf buffer
   - Location: `getGPIOList()`

6. **[OPT-006] Chip Features**
   - Eliminated substring operations through buffer-based approach
   - Location: `getChipFeatures()`

7. **[OPT-007] Message Formatting**
   - Converted 13 debug/status messages to snprintf (2-9 allocations → 1 each)
   - Locations: config messages, WiFi status, HTML title, OLED steps

8. **[OPT-008] TextField Usage in formatUptime**
   - Direct `.str().c_str()` calls avoid unnecessary String allocations (3 instances)
   - Location: `formatUptime()` days/hours/minutes formatting

9. **[OPT-009] Test Status Constants**
   - Created `OK_STR` and `FAIL_STR` constants
   - Applied to 30+ locations: GPIO tests, sensor results, LED demos, config handlers

## Technical Details

### Impact Assessment
- **No functional changes**: All pins, tests, and features remain identical
- **No API changes**: External behavior is unchanged
- **Backward compatible**: Drop-in replacement for v3.20.2
- **Runtime efficiency**: Reduced heap allocations improve memory stability

### Testing
- ✅ Successfully compiled on **ESP32-S3** (esp32s3_n16r8) - Build time: 52s
- ✅ Successfully compiled on **ESP32 CLASSIC** (esp32devkitc) - Build time: 25s
- ✅ All optimizations marked with inline `[OPT-###]` comments for traceability

### Files Modified
- `src/main.cpp` - 224 insertions, 97 deletions (primary optimization target)
- All documentation updated to v3.20.3

## Upgrade Instructions

### From v3.20.2 to v3.20.3
This is a seamless upgrade with no configuration changes required:

1. Pull the latest code from `dev/maint` or `main` branch
2. Compile and upload as normal
3. No configuration file changes needed
4. No web interface changes required

### Verification
After upload, the firmware will function identically to v3.20.2 but with improved memory efficiency. You can verify the version by:
- Checking the web interface header
- Looking at Serial output during boot
- Accessing `/api/version` endpoint

## Breaking Changes
**None** - This is a fully backward-compatible patch release.

## Known Issues
None introduced in this release. All issues from v3.20.2 remain unchanged.

## Contributors
- Code optimization and implementation: GitHub Copilot
- Testing and validation: morfredus

## Next Steps
Version 3.20.3 completes the memory optimization initiative. Future releases will focus on:
- Feature enhancements
- Additional sensor support
- Web interface improvements

---

**Full Changelog**: [CHANGELOG.md](../CHANGELOG.md)  
**Documentation**: [docs/](../docs/)  
**GitHub**: https://github.com/morfredus/ESP32-Diagnostic
