# 📦 INSTALLATION GUIDE - ESP32 Diagnostic v3.0.0

[🇫🇷 Lire en Français](INSTALLATION.md) | [📖 Documentation](README.en.md) | [📋 Changelog](CHANGELOG.en.md)

---

## 🎯 Overview

This guide walks you through step-by-step installation of the ESP32 diagnostic system with dynamic, multilingual, and modern web interface.

---

## 📋 Prerequisites

### Required Hardware
- **ESP32** (ESP32-S2, ESP32-S3 or ESP32-C3 recommended)
- USB cable for programming
- Computer with Arduino IDE or PlatformIO
- **Optional:** WS2812B NeoPixel LED for visual indicator

### Required Software
- **Arduino IDE 2.x** or **PlatformIO**
- **ESP32 Board Package** version 3.1.3 or higher
- Modern web browser (Chrome, Firefox, Edge, Safari)

### Required Arduino Libraries
```
WiFi.h                    (included with ESP32 core)
WebServer.h               (included with ESP32 core)
SPIFFS.h                  (included with ESP32 core)
Wire.h                    (included with ESP32 core)
Adafruit_NeoPixel.h       (to install)
ArduinoJson.h v6.x        (to install - NOT v7)
```

---

## 📁 File Structure

Your project must contain **10 files** :

**Source Code Files :**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino          # Main file
├── api_handlers.h                # REST API handlers (18 endpoints)
├── web_interface.h               # HTML/CSS/JS interface (8 pages)
├── translations.h                # FR/EN translations
├── exemple-wifi-config.h         # ⭐ WiFi configuration template
├── exemple-config.h              # ⭐ Hardware configuration template
└── .gitignore                    # ⭐ Sensitive files protection
```

**Configuration Files (to create by you) :**
```
├── wifi-config.h                 # 🔒 YOUR WiFi credentials (NOT committed)
└── config.h                      # 🔒 YOUR hardware pins (NOT committed)
```

⚠️ **Important** : Files `wifi-config.h` and `config.h` contain sensitive information and must NEVER be shared or committed to Git.

---

## 🚀 Step-by-Step Installation

### Step 1: Prepare Arduino IDE Environment

#### 1.1 Install ESP32 Board Manager

1. Open Arduino IDE
2. Go to **File > Preferences**
3. Add to "Additional Boards Manager URLs":
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
4. Go to **Tools > Board > Boards Manager**
5. Search "ESP32" and install version **3.1.3 or higher**

#### 1.2 Install Required Libraries

1. Go to **Sketch > Include Library > Manage Libraries**
2. Install:
   - **Adafruit NeoPixel** (latest version)
   - **ArduinoJson** version **6.x** (⚠️ NOT version 7)

---

### Step 2: Create Project

1. **Create a new folder** named `ESP32-Diagnostic`
2. **Download the 6 project files**
3. **Place all files** in the `ESP32-Diagnostic` folder
4. **Open** the `ESP32-Diagnostic.ino` file with Arduino IDE

---

### Step 3: WiFi Configuration

#### Option A: Use Configuration File (recommended)

1. Copy `exemple-config.h` to `config.h`:
   ```bash
   cp exemple-config.h config.h
   ```

2. Edit `config.h` and modify the parameters:
   ```cpp
   #define WIFI_SSID "YourSSID"
   #define WIFI_PASSWORD "YourPassword"
   #define DEVICE_NAME "ESP32-Diagnostic"
   ```

#### Option B: Direct Modification in Main File

1. Open `ESP32-Diagnostic.ino`
2. Locate WiFi configuration lines (around line 50-51):
   ```cpp
   const char* ssid = "YourSSID";          // ← Modify here
   const char* password = "YourPassword";  // ← Modify here
   ```
3. Replace with your actual WiFi credentials

---

### Step 4: Pin Configuration (optional)

If using hardware components, adjust pins according to your wiring:

```cpp
// NeoPixel configuration (line ~40)
#define NEOPIXEL_PIN 48        // NeoPixel data pin
#define NEOPIXEL_COUNT 1       // Number of LEDs

// Other pins if needed
#define BUTTON_PIN 9           // Button pin (if used)
#define LED_PIN 2              // Internal LED pin
```

**Common pins by ESP32 model:**

| Model | Recommended NeoPixel Pin | PSRAM |
|-------|--------------------------|-------|
| ESP32-S3 | GPIO 48 | Yes (8MB) |
| ESP32-S2 | GPIO 18 | No |
| ESP32-C3 | GPIO 8 | No |
| ESP32 Classic | GPIO 16 | No |

---

### Step 5: Compilation and Upload

#### 5.1 Select Your Board

1. Go to **Tools > Board > ESP32 Arduino**
2. Choose your model:
   - `ESP32S3 Dev Module` for ESP32-S3
   - `ESP32S2 Dev Module` for ESP32-S2
   - `ESP32C3 Dev Module` for ESP32-C3
   - `ESP32 Dev Module` for Classic ESP32

#### 5.2 Configure Settings

**For ESP32-S3 (recommended):**
```
USB CDC On Boot: Enabled
Flash Size: 8MB (or according to your board)
Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
PSRAM: OPI PSRAM (if available, otherwise QSPI PSRAM)
Upload Speed: 921600
CPU Frequency: 240MHz
```

**For ESP32-S2:**
```
Flash Size: 4MB
Partition Scheme: Default 4MB with spiffs
Upload Speed: 921600
CPU Frequency: 240MHz
```

**For ESP32-C3:**
```
Flash Size: 4MB
Partition Scheme: Default 4MB with spiffs
Upload Speed: 921600
CPU Frequency: 160MHz
```

#### 5.3 Compile and Upload

1. **Compile**: Click ✓ (Verify)
2. **Connect ESP32** via USB
3. **Select port**: `Tools > Port > COMx` (Windows) or `/dev/ttyUSBx` (Linux)
4. **Upload**: Click → (Upload)

⚠️ **Note:** On some boards, hold the **BOOT** button during upload.

---

### Step 6: Startup Verification

#### 6.1 Open Serial Monitor

1. Open **Serial Monitor**: `Tools > Serial Monitor`
2. Set speed to **115200 baud**

#### 6.2 Expected Messages

You should see:

```
====================================
ESP32 Diagnostic System v3.2.0
====================================

[INIT] Initializing system...
[WIFI] Configuring WiFi Multi-SSID...
  - Added network: MyHomeWiFi
  - Added network: OfficeWiFi
  - Added network: iPhone-Hotspot
[WIFI] Connecting to WiFi...
..........
[WIFI] WiFi connected successfully!
  - SSID: MyHomeWiFi
  - IP Address: 192.168.1.xxx
  - Signal Strength: -45 dBm
  - MAC Address: XX:XX:XX:XX:XX:XX
[WEB] Web server started on port 80
[READY] System ready!
====================================
Access web interface at:
  - http://192.168.1.xxx
  - http://esp32-diag.local
====================================
```

✅ If you see these messages, installation is **successful**!

---

## 🌐 Accessing Web Interface

### Method 1: Via IP Address (always works)

1. Note the IP address displayed in serial monitor
2. Open a web browser
3. Navigate to: `http://192.168.1.xxx`

### Method 2: Via mDNS (more convenient)

- Navigate to: `http://esp32-diag.local`
- ⚠️ mDNS doesn't work on all networks (especially some corporate networks)

### Method 3: Via WiFi Access Point (fallback mode)

If WiFi connection fails, ESP32 will create an access point:
- SSID: `ESP32-Diagnostic-Setup`
- Password: `diagnostic123`
- Address: `http://192.168.4.1`

---

## 🔧 Advanced Configuration

### Adjust SPIFFS Memory

If you need more space for the application, modify partition scheme:

```
Tools > Partition Scheme > Huge APP (3MB No OTA/1MB SPIFFS)
```

### Enable Debug Logs

Add at the beginning of `.ino` file:

```cpp
#define DEBUG_MODE 1

#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

### Disable NeoPixel

If not using NeoPixel LED, comment this line:

```cpp
// #define USE_NEOPIXEL  // Disabled
```

---

## ⚠️ Troubleshooting

### Issue: Compilation Error

**Symptom:** Compilation errors related to ArduinoJson

**Solution:** Verify you're using ArduinoJson **version 6.x** and not v7
```
Manage Libraries > ArduinoJson > Install version 6.21.3
```

---

### Issue: ESP32 Won't Connect to WiFi

**Symptoms:**
```
[WIFI] Connecting to WiFi...
[WIFI] .........
[WIFI] Connection failed!
```

**Solutions:**
1. Check SSID and password (case sensitive)
2. Verify WiFi is 2.4GHz (ESP32 doesn't support 5GHz)
3. Move closer to WiFi access point
4. Check if WiFi uses MAC filtering

---

### Issue: Web Interface Doesn't Load

**Symptom:** Blank page or 404 error

**Solutions:**
1. Verify Partition Scheme includes SPIFFS
2. Recompile and upload complete code
3. Clear browser cache (Ctrl + F5)
4. Try another browser

---

### Issue: "Out of Memory" or Random Reboots

**Symptom:** ESP32 randomly reboots

**Solutions:**
1. Enable PSRAM if available: `Tools > PSRAM > OPI PSRAM`
2. Reduce buffer size in `web_interface.h`
3. Choose Partition Scheme with more APP memory

---

### Issue: Upload Failed

**Symptom:** "Failed to connect to ESP32" error

**Solutions:**
1. Hold **BOOT** button during upload
2. Change USB port
3. Reduce upload speed: `Tools > Upload Speed > 115200`
4. Install USB-Serial drivers:
   - Windows: [CP210x drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
   - macOS: Included in system
   - Linux: Add user to `dialout` group

---

## 📊 Post-Installation Tests

### Test 1: Interface Access

✅ Open `http://ESP32-IP` in browser
✅ Interface should display 8 tabs
✅ FR/EN language selector should work

### Test 2: Hardware Tests

✅ "Tests" tab should list all available tests
✅ Clicking "Run Test" should display real-time results
✅ NeoPixel should change color (if connected)

### Test 3: REST API

Test with curl or Postman:
```bash
curl http://ESP32-IP/api/system/info
```

Expected response:
```json
{
  "version": "3.2.0",
  "chipModel": "ESP32-S3",
  "cpuFreq": 240,
  "flashSize": 8388608,
  "freeHeap": 245760
}
```

---

## 🎓 Next Steps

Now that installation is complete:

1. 📖 Check [README.en.md](README.en.md) to understand features
2. 🧪 Explore "Tests" tab to diagnose your ESP32
3. 🌐 Test REST API for integration with other systems
4. 🎨 Customize interface according to your needs
5. 📊 Check [CHANGELOG.en.md](CHANGELOG.en.md) for updates

---

## 📞 Support

- **Documentation:** [README.en.md](README.en.md)
- **Project Structure:** [PROJECT_STRUCTURE.en.md](PROJECT_STRUCTURE.en.md)
- **Changelog:** [CHANGELOG.en.md](CHANGELOG.en.md)

---

## 📄 License

MIT License - See LICENSE file for details

---

**Developed with ❤️ for the ESP32 community**