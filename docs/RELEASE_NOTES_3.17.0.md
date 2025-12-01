# Release Notes — 3.17.0 (2025-12-01)

## Added
- Hardware buttons support (BTN1/BTN2) enabled via `ENABLE_BUTTONS` without changing pin mapping.
  - BTN1: short press plays a brief buzzer tone for haptic feedback.
  - BTN2: short press cycles RGB LED colors (red → green → blue → white).

## Docs
- Updated top-level README and French README with version `v3.17.0` and button behavior.
- Updated `docs/CONFIG*.md` to document `ENABLE_BUTTONS` and button notes.
- Updated `docs/FEATURE_MATRIX*.md` headings to `v3.17.0`; added Buttons row (EN), fixed TFT pin mapping.

## Build
- Bumped `PROJECT_VERSION` to `3.17.0` in `platformio.ini`.

## Notes
- No pin mapping was changed; `PIN_BUTTON_1`/`PIN_BUTTON_2` remain defined in `include/config.h` per target.
- On ESP32 Classic, GPIO 34–39 have no internal pull-ups; use external pull-ups if needed when BTN2=35.
