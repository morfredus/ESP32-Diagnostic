## [Version 3.13.0] - 2025-11-26

### Changed
- Synchronized documentation for multi-board environments: `esp32s3_n16r8`, `esp32s3_n8r8`, and `esp32devkitc`.
- Removed references to unsupported boards (ESP32-S2/C3/C6/H2) across guides and feature matrices.
- Corrected I2C default documentation (SCL=20) and kept HC-SR04 defaults (TRIG=16, ECHO=17).
- Updated `PROJECT_VERSION` to 3.13.0 in `platformio.ini`.

### Notes
- Builds validated for esp32s3_n16r8 and esp32s3_n8r8. `esp32devkitc` compilation configuration present but not hardware-tested.

# Changelog

All notable changes to ESP32 Diagnostic Suite are documented here. This project follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Version 3.12.3] - 2025-11-26
### Changed
- Default HC‑SR04 pins set to `TRIG=16`, `ECHO=17` in `config.h` and aligned defaults in the web interface.

### Added
- New quick reference documents: `docs/PIN_MAPPING.md` and `docs/PIN_MAPPING_FR.md`.

### Technical Changes
- Bumped `PROJECT_VERSION` to 3.12.3 in `platformio.ini`.
- No other functional changes.

## [Version 3.12.2] - 2025-11-26
### Fixed
- **HC-SR04**: Hardened distance measurement by waiting for ECHO to settle LOW before triggering and using `pulseInLong()` with an extended timeout. Prevents false "No echo" results.

### Technical Changes
- No pin mapping changes. Only measurement sequence and timeout handling adjusted.

## [Version 3.12.1] - 2025-11-26
### Fixed
- **PSRAM Activation**: Ensured PSRAM is enabled on ESP32-S3 DevKitC-1 N16R8 in PlatformIO by adding `board_build.psram = enabled` and defining `BOARD_HAS_PSRAM`. This resolves memory tests failing to allocate in external PSRAM.

### Improved
- PlatformIO configuration refined for consistent PSRAM detection and usage across Arduino-ESP32 3.3.x.
- Updated FR/EN documentation and version references to 3.12.1.

### Technical Changes
- Bumped `PROJECT_VERSION` to 3.12.1 in `platformio.ini`.
- No pin-mapping changes (config.h remains unchanged).

## [Version 3.12.0] - 2025-11-26
### Fixed
- **CONFIG**: Validated and confirmed TFT backlight pin configuration (GPIO 15).
- Resolved duplicate TFT display declarations causing compilation warnings.
- Corrected pin mapping documentation in config.h for ESP32-S3 DevKitC-1 N16R8.

### Improved
- **DOCUMENTATION**: Removed obsolete development and debugging files for cleaner repository structure.
- Deleted temporary files: CORRECTIFS_APPLIQUES.md, CORRECTIF_v3.11.1.md, DEBUGGING_WEB_UI.md, PATCH_WEB_UI.cpp, RESUME_FINAL.md, RESUME_v3.11.1_FINAL.md.
- Consolidated pin mapping comments and organization in config.h.
- Enhanced PlatformIO configuration with improved PSRAM settings.

### Technical Changes
- Finalized TFT pin configuration for production use on ESP32-S3 N16R8.
- Cleaned repository of development artifacts and maintenance notes.
- Standardized configuration file structure and comments.
- Updated version to 3.12.0 in platformio.ini.

## [Version 3.11.4] - 2025-11-25
### Improved
- **MAINTENANCE**: Code quality improvements and cleanup.
- Removed obsolete development version history from source code headers.
- Removed unused `handleJavaScript()` function (dead code elimination).
- Simplified and standardized comment style throughout codebase.
- Fixed French typo: "defaut" → "défaut" in setup messages.

### Technical Changes
- Cleaned obsolete version comments (v3.8.x-dev through v3.10.3).
- Deleted `handleJavaScript()` function never referenced in routing.
- Normalized comment delimiters and removed redundant annotations.
- Updated version to 3.11.4 in platformio.ini.

## [Version 3.11.3] - 2025-11-25
### Fixed
- **CONFIG**: Corrected TFT backlight pin from GPIO 48 to GPIO 15 to resolve NeoPixel pin conflict.
- TFT display backlight now uses dedicated GPIO 15 instead of conflicting with GPIO 48 (NeoPixel).

### Technical Changes
- Updated `TFT_BL` definition in `config.h` from pin 48 to pin 15.
- Ensures proper TFT backlight operation without NeoPixel hardware conflicts.

## [Version 3.11.2] - 2025-11-25
### Fixed
- **BUILD**: Fixed FPSTR() type casting error preventing compilation.
- Corrected pointer type handling for PROGMEM strings in chunked transfer implementation.
- Changed `const char* staticJs = FPSTR(...)` to proper ESP32 PROGMEM access pattern.
- Resolved `cannot convert 'const __FlashStringHelper*' to 'const char*'` compilation error.

### Technical Changes
- Updated `handleJavaScriptRoute()` to use direct PROGMEM pointer access for verification.
- Maintained chunked transfer implementation while fixing type compatibility.
- Added comments explaining ESP32-specific PROGMEM memory mapping.

## [Version 3.11.1] - 2025-11-25
### Fixed
- **CRITICAL**: Fixed web interface JavaScript loading using chunked transfer encoding.
- Replaced monolithic JavaScript generation with memory-efficient chunked streaming.
- Enhanced debug logging to show JavaScript size breakdown (preamble, translations, static code).
- Fixed memory overflow issues when JavaScript exceeded allocated buffer size.

### Technical Changes
- Implemented chunked transfer encoding in `handleJavaScriptRoute()`.
- Separated JavaScript generation into three parts: preamble, translations, static code.
- Added PROGMEM verification to detect missing functions before sending.
- Reduced memory pressure by streaming JavaScript content instead of buffering.

## [Version 3.11.0] - 2025-11-25
### Added
- **NEW FEATURE**: TFT ST7789 display support with 240x240 resolution.
- Boot splash screen on TFT display showing system initialization.
- Real-time WiFi connection status visualization on TFT.
- IP address display on TFT once connected.
- Configurable TFT pins (MOSI, SCLK, CS, DC, RST, Backlight) in config.h.
- New tft_display.h header file for TFT display management.

### Fixed
- Web interface initialization and tab loading improvements.
- Enhanced JavaScript error handling for better UI responsiveness.

### Improved
- Better visual feedback during boot process with TFT display.
- Dual display support (OLED + TFT) for enhanced diagnostics.

## [Version 3.10.3] - 2025-11-25
### Added
- None.

### Fixed
- **CRITICAL**: Fixed `portGET_ARGUMENT_COUNT()` static assertion compilation error in FreeRTOS macros.
- Changed platform from unstable git version to stable `espressif32@6.5.0` release.
- Added `-DCONFIG_FREERTOS_ASSERT_ON_UNTESTED_FUNCTION=0` build flag to prevent FreeRTOS macro conflicts.

### Improved
- Enhanced build stability by using stable platform version instead of git repository.
- Improved compatibility with FreeRTOS and Arduino-ESP32 framework.

## [Version 3.10.2] - 2025-11-25
### Added
- None.

### Fixed
- **CRITICAL**: Fixed C++17 compilation flags causing runtime crash on startup.
- Added `build_unflags = -std=gnu++11` to properly override default C++ standard.
- Changed `-std=gnu++17` to `-std=c++17` for stricter C++17 compliance.
- Resolved inline variable initialization issues that prevented web interface and OLED display from functioning.

### Improved
- Cleaned up duplicate build flags in `platformio.ini`.
- Enhanced build system configuration for better C++17 support.

## [Version 3.9.0] - 2025-11-11
### Added
- Updated project documentation and repository references for PlatformIO-based deployment.
- Migrated from Arduino IDE to PlatformIO for improved build consistency and dependency management.

### Fixed
- None.

### Improved
- Updated all documentation to reflect PlatformIO toolchain and new repository URL.
- Standardized version references across entire project documentation.
- Enhanced repository structure for professional development workflow.

## [Version 3.8.14] - 2025-11-11
### Added
- None.

### Fixed
- **Critical**: Added missing `runtimeBLE` variable declaration that caused compilation errors on ESP32-S2/S3/C3/C6/H2 targets.
- Removed unused `DIAGNOSTIC_VERSION_HISTORY` array to reduce code clutter.

### Improved
- Cleaned up redundant `String` initializations (`String foo = ""` → `String foo`) across 7 instances.
- Standardized `for` loop spacing (`for(` → `for `) across 23 instances for better readability.
- Removed superfluous inline comments to improve code clarity.
- Updated version references to 3.8.14.

## [Version 3.8.0] - 2025-11-08
### Added
- Production release based on 3.7.30 with version bump for deployment readiness.

### Fixed
- None.

### Improved
- Standardized version references across all documentation and firmware constants.

## [Version 3.7.30] - 2025-11-08
### Added
- OLED-equipped boards now show Wi-Fi association, DHCP, and completion states during boot so the connection phase is visible without opening the serial monitor.

### Fixed
- Guarded optional NimBLE descriptors and aligned BLE callback signatures to eliminate missing header and `override` errors across ESP32 and ESP32-S3 targets.
- Prevented invalid NimBLE scan result conversions by validating the `start()` status before copying advertised device data.

### Improved
- Reused NimBLE scan buffers safely, resumed advertising after failures, and surfaced coherent HTTP errors whenever scans cannot start.
- Updated the FR/EN documentation set to capture the Wi-Fi splash workflow and the hardened BLE compatibility guidance for this release.

## [Version 3.5.1] - 2025-10-31
### Added
- Extended live translation bindings across diagnostics panels so wireless, pager, GPIO, and export widgets reuse the shared catalog without duplicates.

### Fixed
- Addressed untranslated board metadata (CPU cores, MAC addresses, reset reasons, memory metrics) to keep language toggles in sync without requiring a reload.

### Improved
- Refined the web client translation refresher to reapply prefixes, suffixes, and placeholders instantly when switching languages.

---

## [Version 3.4.0] - 2025-10-29
### Added
- Shared JSON helper prototypes to expose consistent utility signatures across diagnostics modules.

### Fixed
- Resolved build failures caused by mismatched JSON helper declarations during cross-module usage.
- Restored safe `String` conversions within the translation pipeline after the catalog optimisations.

### Improved
- Unified the HTTP JSON response builders and peripheral handlers to reduce duplication across exports.
- Harmonised the bilingual UI labels and retired the legacy touchpad diagnostic from the default run list.
- Updated the complete FR/EN documentation set to reflect the stabilised 3.3.x maintenance track.

---

## [Version 3.3.0] - 2025-10-27
### Added
- None.

### Fixed
- Removed redundant “NEW FEATURE” banner comments to avoid confusing maintenance efforts.

### Improved
- Consolidated the legacy "Version de dev" notes into a firmware constant so internal history remains accessible without banner duplication.
- Synced the firmware header comment and `DIAGNOSTIC_VERSION` macro at 3.3.0 and refreshed documentation to reflect the streamlined maintenance focus.

---

## [Version 3.2.0] - 2025-10-29
### Added
- Documented how to query the `/api/memory-details` endpoint and interpret fragmentation warnings from the latest diagnostics run.
- Captured the Bluetooth® and Wi-Fi debugging checklist from the post-release validation pass directly inside the usage and troubleshooting guides.

### Fixed
- Replaced remaining 3.1.19 references across READMEs and setup guides so the banner, guides, and bilingual changelog remain synchronized at 3.2.0.
- Clarified configuration guidance around language switching to avoid stale instructions when verifying `/api/set-language` responses.

### Improved
- Expanded installation, configuration, and usage manuals with concrete verification steps for the refreshed diagnostics (memory exports, BLE state widgets, Wi-Fi scan metadata).
- Strengthened the contributing guide with reminders to keep FR/EN documentation aligned during maintenance updates.

---

## [Version 3.1.19] - 2025-10-28
### Added
- None.

### Fixed
- Updated every guide to reference the bilingual changelog pair (`CHANGELOG.md` and `CHANGELOG_FR.md`) so both languages stay aligned.

### Improved
- Split the changelog into dedicated English and French files to make maintenance updates easier for each audience.
- Refreshed the version banner, `DIAGNOSTIC_VERSION`, and release highlights to reference 3.1.19 across the documentation set.

---

## [Version 3.1.18] - 2025-10-27
### Added
- None.

### Fixed
- Validated `/api/set-language` so French/English requests immediately return HTTP 200 while unsupported codes now receive a clear HTTP 400 response.
- Synchronized serial logs and exports with the new language-switch JSON responses.

### Improved
- Preallocated the `jsonEscape` buffer to reduce allocations while generating exports.
- Explicitly included `<cstring>` to guarantee C string helpers remain available on recent Arduino toolchains.
- Aligned the FR/EN documentation set (README, installation, configuration, usage, architecture, contributing, troubleshooting guides) with the 3.1.18 maintenance pass.

---

## [Version 3.1.18] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Validation de `/api/set-language` pour renvoyer immédiatement HTTP 200 sur `fr`/`en` et HTTP 400 sur les codes non pris en charge, supprimant les réponses ambiguës.
- Synchronisation des journaux série et exports avec les nouvelles réponses JSON du changement de langue.

### Améliorations
- Réservation anticipée du tampon de `jsonEscape` afin de réduire les allocations pendant la génération d'exports.
- Inclusion explicite de `<cstring>` pour assurer la compatibilité des fonctions de chaîne C sur les toolchains Arduino récentes.
- Documentation FR/EN alignée sur la maintenance 3.1.18 (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage).

---

## [Version 3.1.16] - 2025-10-27
### Added
- Unified sticky banner showing version, Wi-Fi/Bluetooth status, and a quick-access link from the web dashboard.
- Bluetooth® commands (enable, rename, reset) exposed both in the interface and through dedicated REST endpoints.

### Fixed
- Repositioned tab navigation event delegation to restore the selection after dynamic reloads.
- Refreshed translations on tabs and dynamic elements after a language switch to remove inconsistent labels.
- Restored the “Not tested” label for additional diagnostics across the interface, API, and exports.

### Improved
- Enriched Wi-Fi scan responses (BSSID, band, channel width, PHY mode) to simplify RF analysis.
- Extended `htmlEscape` preallocation to limit reallocations during HTML streaming and export generation.
- Synchronized the header comment and `DIAGNOSTIC_VERSION` at 3.1.16 to align logs and generated files.

---

## [Version 3.1.15-maint] - 2025-10-26
### Added
- None.

### Fixed
- Standardized the “Not tested” label for additional diagnostics (ADC, touch, PWM, stress) in the interface, exports, and API.
- Synchronized the header comment and `DIAGNOSTIC_VERSION` at 3.1.15-maint so logs and exported files show the correct number.

### Improved
- Extended the `htmlEscape` preallocation to reduce reallocations while escaping generated HTML strings.
- Updated the FR/EN documentation (README, installation, configuration, usage, architecture, contributing, troubleshooting guides) to reflect the 3.1.15-maint maintenance and checks.

---

## [Version 3.1.14-maint] - 2025-10-26
### Added
- None.

### Fixed
- Restored “Not tested” status labels for the additional diagnostics to keep the French interface consistent.
- Aligned the version banner comment and `DIAGNOSTIC_VERSION` with revision 3.1.14-maint to reflect the active maintenance.

### Improved
- Increased the memory reserved by `htmlEscape` to limit reallocations while escaping generated strings.

---

## [3.1.1] - 2025-10-26
### Fixed
- Removed obsolete comments and internal markers to clarify firmware maintenance status.

### Documentation
- Updated the FR/EN guides, exports, and constants to reflect the move to version 3.1.1.

---

## [3.1.0] - 2025-10-24
### Highlights
- Automatic enablement of the Bluetooth® Low Energy service with native advertising on compatible targets (ESP32, S3, C3, C6, H2).
- Web dashboard enriched with a BLE card showing status, device name, and recent pairing logs.
- Fully rewritten FR/EN document sets covering installation, configuration, usage, architecture, troubleshooting, and contribution.

### Features
- Default GATT service with dynamic device renaming via the web interface or serial console.
- New contextual Wi-Fi status messages (association, DHCP, authentication, captive portal) displayed in the top banner and REST API.
- Relaxed initial support for the latest ESP32 SoC revisions (S3/C3/C6/H2) within the BLE and Wi-Fi modules.

### Interface & API
- Hardened tab navigation through JavaScript event delegation to avoid lockups after partial refresh.
- Responsive top menu grouping status indicators on a single line for screens below 768 px.
- TXT/JSON/CSV exports and print preview synchronized with the new BLE and Wi-Fi states.

### Documentation
- Consolidated guides inside `docs/` with cross-links to English and French versions.
- Added a detailed contributing guide (Git workflow, validation, formatting) and BLE/Wi-Fi troubleshooting procedures.

### Fixed
- Harmonized BLE messages across the serial console, `/api/status`, and the web interface.
- Cleaned the dynamic translation JSON to remove orphan keys and casing inconsistencies.
- Clarified active tab detection in the JavaScript bundle, removing duplicate listeners.

---

## [2.6.0] - 2025-10-15
### Features
- Manual buttons on the web interface plus REST endpoints to control each OLED diagnostic animation individually.
- Ability to start and stop display sequences directly from the serial console.

### Improved
- Simplified the OLED I²C reconfiguration flow: select SDA/SCL pins and speed directly from the interface.
- Refreshed the translation pack (FR/EN) for all new OLED labels and runtime states.

### Fixed
- Fully removed TFT support (firmware, Arduino dependencies, UI fragments), shrinking the binary and eliminating compilation warnings.

---

## [2.5.1] - 2025-10-10
### Fixed
- Clarified PSRAM statuses on the dashboard, exports, and printable reports (detected value, frequency, octal mode).
- Adjusted PSRAM detection order to avoid false negatives on ESP32-WROVER boards.

### Improved
- Highlighted PSRAM-compatible boards with reminders to enable the option in the Arduino IDE and installation docs.

---

## [2.5.0] - 2025-10-08
### Features
- Fully translated TXT/JSON/CSV exports (FR/EN) including ISO 8601 timestamps and build numbers.
- Printable preview aligned with the selected language and the new diagnostic blocks.

### Improved
- Enriched export filenames with version number, date, and detected board.
- Revised the REST export workflow to shorten generation times and harmonize column titles.

### Fixed
- Harmonized date formats (FR/EN localization) in exports and the printable view.
- Added missing translations on download buttons and section labels.

---

## [2.4.0] - 2025-10-07
### Features
- Full FR/EN multilingual interface with hot switching without page reload.
- `/api/set-language` and `/api/get-translations` endpoints enabling external clients to control the language.
- `languages.h` file centralizing more than 150 translated strings for the interface, exports, and serial console.

### Improved
- All texts (web, API, exports, logs) now rely on the centralized translation catalog.
- Added a visual indicator for the active language and persisted the preference in the browser's local storage.

---

## [2.3.0] - 2025-10-06
### Features
- Suite of 10 OLED 0.96" I²C tests (contrast toggles, inversion, scroll, custom frames) with explanatory messages.
- Dynamic SDA/SCL pin reconfiguration through the web interface and API to simplify rewiring.

### Improved
- Automatically detects the display at address 0x3C and reruns tests after connection.
- Added a contrast calibration module to optimise OLEDs based on supply voltage.

---
