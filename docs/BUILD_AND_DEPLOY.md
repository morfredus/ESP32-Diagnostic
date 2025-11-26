# ESP32 Diagnostic Suite – Build & Deploy (v3.12.0)

This document describes the supported toolchains and the recommended release checklist for firmware version 3.12.0.

> **Important:** Version 3.12.0 has been migrated from Arduino IDE to **PlatformIO**. The original Arduino IDE version [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) repository is now **archived**. Bluetooth/BLE support has been **removed**.

## Toolchains
The project uses PlatformIO for build management. All dependencies are declared in `platformio.ini`:

1. Open the project in **Visual Studio Code** with the **PlatformIO IDE** extension.
2. Select your target environment (e.g., `esp32-s3-devkit`).
3. Run **Build** to compile, then **Upload** to flash.
4. Open **Serial Monitor** at 115200 baud to verify.

Alternatively, use the PlatformIO CLI:
```bash
pio run --target upload
pio device monitor --baud 115200
```

**Note:** Arduino IDE and Arduino CLI are **no longer supported** for this version. Use the archived [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) repository for Arduino IDE compatibility.

## Pre-deployment checklist
- [ ] Update `wifi-config.h` with production SSID/passwords and optional enterprise credentials.
- [ ] Confirm `DIAGNOSTIC_VERSION` matches `3.8.0` in both firmware and documentation.
- [ ] Verify multilingual assets compile without warnings (`languages.h`).
- [ ] Run a full diagnostic cycle on a reference board and export JSON/CSV reports.
- [ ] Capture screenshots or printouts of the dashboard for release notes if required.

## Acceptance tests
| Test | Procedure | Expected result |
|------|-----------|-----------------|
| Connectivity smoke test | Connect to lab Wi-Fi, run quick diagnostics. | PASS with RSSI above -70 dBm, ping < 40 ms. |
| Memory soak | Run full diagnostics 5 times consecutively. | No heap exhaustion, PSRAM stable. |
| Peripheral validation | Attach OLED + NeoPixel chain, enable modules. | PASS with consistent frame rate and LED animation. |
| REST API sanity | Trigger `/api/run` then `/api/status`. | Receives queued status and JSON payload. |

## Release packaging
1. Tag the repository with `v3.9.0` (Git annotated tag).
2. Attach compiled binaries (`.bin`) for each target board if distributing via GitHub Releases.
3. Publish release notes referencing [CHANGELOG.md](../CHANGELOG.md).
4. Share links to the updated wiki (`docs/home.md`) for support teams.

## OTA considerations
- The firmware is primarily distributed via USB flashing. For OTA, integrate with your preferred bootloader and ensure the
  diagnostic binary remains under the OTA partition size.
- Always validate signature or checksum after OTA transfer to prevent corrupted diagnostics.

## Rollback strategy
- Keep a known-good image (e.g., v3.2.21-maint) ready for emergency downgrades.
- Document the rollback steps in the internal SOP, referencing this guide for re-flashing instructions.

## Related resources
- [Installation guide](INSTALL.md)
- [Configuration guide](CONFIG.md)
- [Release notes](../CHANGELOG.md)
