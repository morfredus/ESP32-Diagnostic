# Release Notes — 3.19.0 (2025-12-07)

## Changed
- **Pin mapping extracted to `board_config.h`:** The board-specific GPIO map now lives in a dedicated header, keeping `config.h` focused on feature toggles and common settings. All pin values stay the same; ESP32-S3 buttons remain on GPIO 38/39 to avoid upload/reset conflicts.
- **Secrets file rename:** `wifi-config.h` ? `secrets.h` (with `secrets-example.h`). The firmware now includes `secrets.h`; legacy headers emit a build-time error to prevent accidental use.
- **Git ignore hardening:** `.gitignore` highlights `include/secrets.h` (and keeps the legacy name ignored) to avoid publishing credentials.
- **Version bump:** `PROJECT_VERSION` set to `3.19.0` in `platformio.ini`.

## Documentation
- Updated README (EN/FR), CONFIG guides, pin mapping references, install/build checklists, FAQ, troubleshooting, security, architecture map, and usage notes to reflect `board_config.h`, the new button pins, and `secrets.h`.

## Upgrade notes
- Copy `include/secrets-example.h` to `include/secrets.h` and fill in your SSIDs/passwords (do not commit this file).
- If you maintained local pin changes, move them into `include/board_config.h`; other runtime options stay in `config.h`.







