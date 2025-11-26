# ESP32 Diagnostic Suite — Release Notes 3.13.1 (2025-11-26)

Maintenance release. No functional firmware changes. Dependency and documentation improvements.

## Summary
1. PlatformIO dependencies: switched Adafruit libraries to caret ranges (`^`) to allow safe minor/patch updates without breaking changes.
2. Build: verified clean compilation on all three environments (`esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`). Upload validated on `esp32s3_n16r8`.
3. Documentation: READMEs and BUILD guides updated (EN/FR) with per-environment `pio` commands and build status.

## Details
1. `platformio.ini` — Dependencies
   - `adafruit/Adafruit NeoPixel` → `@^1.12.0`
   - `adafruit/DHT sensor library` → `@^1.4.6`
   - `adafruit/Adafruit BusIO` → `@^1.17.4`
   - `adafruit/Adafruit GFX Library` → `@^1.12.4`
   - `adafruit/Adafruit ST7735 and ST7789 Library` → `@^1.11.0`
   - Benefit: receive SemVer-compliant bugfixes/improvements (minor/patch) automatically.

2. Documentation — Updates
   - `README.md` / `README_FR.md`: added a maintenance block listing the changes (caret versions + verified builds).
   - `docs/BUILD_AND_DEPLOY.md` / `docs/BUILD_AND_DEPLOY_FR.md`: added `pio run -e ...` examples per environment and a "Build status / Statut de build" section.

3. Build quality — Verified state
   - `esp32s3_n16r8`: Build OK, Upload OK.
   - `esp32s3_n8r8`: Build OK.
   - `esp32devkitc`: Build OK.

## Compatibility
- No compatibility changes. Supported environments remain: `esp32s3_n16r8` (primary), `esp32s3_n8r8`, `esp32devkitc`.

## Upgrade
1. No configuration changes required. A regular rebuild is sufficient.
2. To verify:
   - `pio run -e esp32s3_n16r8`
   - `pio run -e esp32s3_n8r8`
   - `pio run -e esp32devkitc`

## Notes
- These changes are strictly maintenance (deps/docs). The firmware stays on the 3.13.x line.
