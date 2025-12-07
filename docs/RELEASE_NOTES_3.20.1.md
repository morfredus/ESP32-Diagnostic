# Release Notes — 3.20.1 (2025-12-07)

## Fixed
- **USB/OTG stability (ESP32-S3):** Freed USB D-/D+ (GPIO19/20) by moving defaults to safe pins.
  - I2C defaults: SDA=15, SCL=16
  - RGB LED: Red=21, Green=45, Blue=47 (Red leaves GPIO19)
  - GPIO45 is a strapping pin; LED stays off at boot to avoid boot mode issues.

## Notes
- OTG instability was caused by I2C traffic and the RGB red channel on GPIO19/20, which are the native USB data lines. With the new mapping, keep GPIO19/20 unused by peripherals.
- `PROJECT_VERSION` is set to `3.20.1` in `platformio.ini`.

## Upgrade
- Rebuild and flash with the updated defaults (`pio run -e esp32s3_n16r8 --target upload`).
- If you had custom pin overrides, align them with the new mapping to keep USB stable.
