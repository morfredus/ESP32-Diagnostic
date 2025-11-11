# Installation Guide (EN)

Follow the steps below to deploy the ESP32 Diagnostic Suite on your board.

> **Release 3.8.0:** This version has been migrated from Arduino IDE to **PlatformIO in Visual Studio Code**. The original [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic) repository is now **archived**. Bluetooth/BLE support has been **removed** as it is not properly supported under the PlatformIO platform configuration.

## 1. Prerequisites
- ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, or ESP32-H2 board (S3 recommended).
- USB cable with data capability.
- Optional: NeoPixel/WS2812B LED strip and 0.96" SSD1306 OLED screen for extended tests.
- **Visual Studio Code** with **PlatformIO IDE** extension installed.
- PlatformIO will automatically manage the ESP32 toolchain (espressif32 platform).

## 2. Library dependencies
All required libraries are declared in `platformio.ini` and will be installed automatically by PlatformIO:
- **Adafruit BusIO**
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **Adafruit NeoPixel**

The following libraries are bundled with the ESP32 platform: `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_chip_info`, `esp_flash`, `esp_heap_caps`, `esp_partition`.

## 3. Get the source code
1. Clone the repository from GitHub:
   ```bash
   git clone https://github.com/morfredus/ESP32-Diagnostic-PIO.git
   cd ESP32-Diagnostic-PIO
   ```
2. Open the project folder in Visual Studio Code with PlatformIO.

**Note:** The original [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic) repository (Arduino IDE version) is now archived.

## 4. Prepare Wi-Fi credentials
1. Duplicate `wifi-config-example.h` and rename the copy to `wifi-config.h`.
2. Open `wifi-config.h` and replace the placeholder SSID/PASS pairs with your own networks.
3. Multiple networks can be declared; the firmware will try them in order.

Detailed configuration notes are available in [CONFIG.md](CONFIG.md).

## 5. Compile and upload
1. In Visual Studio Code, open the PlatformIO sidebar.
2. Select your target environment from `platformio.ini` (e.g., `esp32-s3-devkit`).
3. Ensure **PSRAM** is enabled in your board configuration for ESP32-S3 boards to unlock advanced diagnostics.
4. Click **Build** to compile, then **Upload** to flash the firmware.
5. Open the **Serial Monitor** (115200 baud) to view the boot sequence.

Upon success, the serial monitor will display the firmware version and connection status. The OLED (if present) mirrors the Wi-Fi handshake phases (association, DHCP, completion).

**Note:** Bluetooth/BLE functionality is not available in this PlatformIO version.
