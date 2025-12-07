# Installation Guide (EN)

Follow the steps below to deploy the ESP32 Diagnostic Suite on your board.

> **Release 3.8.0:** This version has been migrated from Arduino IDE to **PlatformIO in Visual Studio Code**. The original Arduino IDE version [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) repository is now **archived**. Bluetooth/BLE support has been **removed** as it is not properly supported under the PlatformIO platform configuration.

## 1. Prerequisites

### Hardware
- **Supported boards (v3.19.0):**
  - **ESP32-S3-DevKitC-1 N16R8** (16MB Flash, 8MB PSRAM) — Primary target, environment: `esp32s3_n16r8`
  - **ESP32-S3-DevKitC-1 N8R8** (8MB Flash, 8MB PSRAM) — Environment: `esp32s3_n8r8`
  - **ESP32-DevKitC Classic** (4MB Flash, no PSRAM) — Environment: `esp32devkitc`
- USB cable with data capability.
- Optional: NeoPixel/WS2812B LED strip, 0.96" OLED screen, TFT ST7789 display, sensors (DHT, HC-SR04, etc.).

### Software
- **Visual Studio Code** with **PlatformIO IDE** extension installed.
- PlatformIO will automatically manage the ESP32 toolchain (espressif32 platform ^6).
- Git for cloning the repository.

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
   git clone https://github.com/morfredus/ESP32-Diagnostic.git
   cd ESP32-Diagnostic
   ```
2. Open the project folder in Visual Studio Code with PlatformIO.

**Note:** The original Arduino IDE version [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) is now archived.

## 4. Prepare Wi-Fi credentials
1. Duplicate `secrets-example.h` and rename the copy to `secrets.h`.
2. Open `secrets.h` and replace the placeholder SSID/PASS pairs with your own networks.
3. Multiple networks can be declared; the firmware will try them in order.

Detailed configuration notes are available in [CONFIG.md](CONFIG.md).

## 5. Compile and upload
1. In Visual Studio Code, open the PlatformIO sidebar.
2. Select your target environment from `platformio.ini`:
   - **esp32s3_n16r8** (default) — ESP32-S3 with 16MB Flash, 8MB PSRAM
   - **esp32s3_n8r8** — ESP32-S3 with 8MB Flash, 8MB PSRAM
   - **esp32devkitc** — Classic ESP32 with 4MB Flash, no PSRAM
3. Click **Build** to compile, then **Upload** to flash the firmware.
4. Open the **Serial Monitor** (115200 baud) to view the boot sequence.

Upon success, the serial monitor will display the firmware version and connection status. The OLED (if present) mirrors the Wi-Fi handshake phases (association, DHCP, completion).

**Note:** Bluetooth/BLE functionality is not available in this PlatformIO version.
