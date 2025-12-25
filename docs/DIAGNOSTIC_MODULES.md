# ESP32 Diagnostic Suite – Diagnostic Modules (v3.30.0)

> WARNING: This document reflects firmware v3.30.0 with EXACT pin mappings from `include/board_config.h`. All GPIO assignments have been verified and synchronized with the codebase. Read [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) before flashing.

This guide dives into each automated diagnostic module shipped with version 3.15.0. Use it to understand the execution order,
metrics collected, and interpretation guidelines when reviewing reports.

**New in v3.15.0:** Multi-environment support with hardware-specific configurations for ESP32-S3 (N16R8/N8R8) and ESP32 Classic targets. Pin mappings adapt automatically based on the selected build environment.

## Execution pipeline
The scheduler processes modules in the order shown below. Each module publishes status events consumed by the web dashboard,
REST API, and serial logger.

1. **Environment preflight** - verifies firmware version, build flags, and free heap.
2. **Connectivity** – runs Wi-Fi scans and mDNS advertisement checks (BLE removed in PlatformIO version v3.30.0).
3. **Network throughput** - executes ping and HTTP fetch benchmarks when Wi-Fi credentials are available.
4. **Memory & storage** - measures PSRAM, heap fragmentation, and flash read/write integrity.
5. **GPIO sweep** - toggles digital pins, analog inputs, and PWM outputs (legacy touchpad routine remains removed).
6. **Peripheral buses** - validates I2C (primary/secondary), SPI loopback, UART loopback, and OneWire sensors.
7. **Display & LEDs** - runs OLED sequences, NeoPixel animations, and ensures proper refresh timings.
8. **Reporting** - packages results, updates the activity log, and exposes data for export or API retrieval.

## Module reference
| Module | Description | Key metrics | Export keys |
|--------|-------------|-------------|-------------|
| Environment preflight | Confirms build metadata, uptime, reset reason, CPU frequency. | Firmware version, reset reason, heap free, PSRAM detected. | `env.version`, `env.reset_reason`, `env.heap_free`. |
| Connectivity | Surveys access points, drives the OLED Wi-Fi splash, checks association. **Note:** BLE advertising removed in PlatformIO version. | AP count, RSSI per AP, association time. | `wifi.networks[]`, `wifi.association_ms`. |
| Network throughput | Measures latency and HTTP response size against configured targets. | Ping min/avg/max, HTTP status, throughput (kB/s). | `net.ping`, `net.http.status`, `net.http.kbps`. |
| Memory & storage | Performs heap stress tests and flash CRC validation. | Heap high-water mark, PSRAM size, flash CRC status. | `memory.heap_max`, `memory.psram`, `storage.flash_crc`. |
| GPIO sweep | Pulses digital outputs, samples analog inputs, validates pull-ups (legacy touchpad routine removed). | Pin pass/fail list, ADC mean/variance, PWM summary exposed via hardware tests. | `gpio.list`, `hardware_tests.pwm`. |
| Peripheral buses | Confirms bus initialisation and optional device detection (OLED, sensors, INA219, etc.). | I2C device IDs, SPI loopback status, UART checksum, OneWire presence. | `bus.i2c.devices[]`, `bus.spi.status`, `bus.uart.loopback`. |
| Display & LEDs | Exercises graphical output and LED timing routines. | OLED frame checksum, FPS, NeoPixel saturation & gamma results. | `display.oled.frames`, `display.oled.fps`, `led.neopixel.status`. |
| Reporting | Serialises results to JSON, CSV, TXT, and handles REST payload caching. | Export size per format, timestamp, operator notes. | `report.generated_at`, `report.formats`, `report.operator`. |

## Status levels
- **PASS** - module executed without warnings and all metrics are within expected thresholds.
- **WARN** - module executed but found inconsistent values (e.g., high latency). Review metrics before accepting hardware.
- **FAIL** - module aborted or produced critical errors. Hardware should be quarantined for further investigation.

## Custom modules
Custom diagnostics can be added by registering callbacks in `registerCustomDiagnostics()` within `the main source file`. Each
callback receives a context object that exposes:

- `logInfo`, `logWarn`, `logError` helpers for serial and web logging.
- Access to the JSON report builder for injecting structured payloads.
- Timestamps and board metadata for traceability.

Ensure custom modules follow the PASS/WARN/FAIL contract and publish unique export keys to prevent name collisions.

## Troubleshooting tips
- If a module reports **FAIL** without serial logs, enable verbose logging via `#define DIAGNOSTIC_DEBUG 1` in `config.h`.
- Modules relying on optional hardware (OLED, NeoPixel, INA219) only activate when the device is detected during preflight.
- Use the REST endpoint `/api/diagnostics/{module}` to retrieve isolated results for a specific module.
- For reproducible issues, export the JSON report and attach it to the GitHub issue template.

## Related resources
- [Feature matrix](FEATURE_MATRIX.md) – board compatibility and optional hardware notes.
- [REST API reference](API_REFERENCE.md) – payload schemas when automating module execution.
- [Troubleshooting](TROUBLESHOOTING.md) – recovery steps when a module is stuck or unavailable.
- [New in v3.30.0](RELEASE_NOTES_3.30.0.md)











