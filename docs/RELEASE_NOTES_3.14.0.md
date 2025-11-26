# Release Notes 3.14.0

**Release Date:** November 27, 2025  
**Version Type:** New Feature (Minor Release)

## Overview

Version 3.14.0 brings complete support for TFT display testing via the web interface, with a new dedicated UI allowing comprehensive testing of the ST7789 240x240 TFT screen.

## Key Features

### 1. Complete TFT Web Interface

A new **"TFT Display"** section has been added to the web interface, providing:

1. **Information Display**:
   - Screen resolution (240x240 pixels)
   - SPI pin configuration (MOSI, SCLK, CS, DC, RST)
   - Automatic detection status

2. **Full Test Button**:
   - Launches the complete sequence of 8 TFT tests
   - Real-time feedback via the interface

3. **"Boot Screen" Button**:
   - Restores the TFT boot screen at any time
   - Returns to initial state after tests

4. **8 Individual Tests**:
   - **Boot splash**: Displays boot screen with logo and system information
   - **Color test**: Shows 7 primary colors in sequence
   - **Shapes**: Draws rectangles, circles, triangles, both filled and outlined
   - **Text rendering**: Tests different font sizes and alignments
   - **Lines patterns**: Generates horizontal, vertical, and diagonal line patterns
   - **Animation**: Animated circle moving across the screen
   - **Progress bar**: Animated progress bar from 0% to 100%
   - **Final message**: Test completion message with visual feedback

### 2. OLED Interface Enhancement

The **"Boot Screen"** button has also been added to the OLED section to:
- Restore OLED boot display after tests
- Maintain consistency with TFT interface

### 3. New REST APIs

Four new endpoints have been added:

1. **`GET /api/tft-test`**:
   - Launches the complete sequence of 8 TFT tests
   - Returns: JSON with success status

2. **`GET /api/tft-step?step=<id>`**:
   - Executes an individual TFT test (boot, colors, shapes, text, lines, animation, progress, final)
   - Returns: JSON with test name and status

3. **`GET /api/tft-boot`**:
   - Restores the TFT boot screen
   - Returns: JSON with confirmation

4. **`GET /api/oled-boot`**:
   - Restores the OLED boot screen
   - Returns: JSON with confirmation

## Technical Improvements

### 1. Consistent Architecture

The TFT implementation follows the exact same architecture as OLED:
- Modular test functions in `src/main.cpp`
- HTTP handlers with standardized JSON responses
- Web interface with grid layout and individual buttons
- Integrated bilingual translation system

### 2. Internationalization

13 new bilingual translation keys (EN/FR) have been added:
- `tft_screen`, `tft_step_boot`, `tft_step_colors`, `tft_step_shapes`
- `tft_step_text`, `tft_step_lines`, `tft_step_animation`, `tft_step_progress`
- `tft_step_final`, `boot_screen`, `resolution`, `spi_pins`
- `tft_test_running`, `tft_step_running`, `restoring_boot_screen`

### 3. Adapted TFT Tests

TFT tests have been specifically adapted for a 240x240 color display:
- **Color test**: 7 colors (black, red, green, blue, yellow, cyan, magenta, white)
- **Shapes**: Use of filled and outlined shapes with varied colors
- **Animation**: Smooth movement with partial clearing
- **Progress bar**: Graphical visualization with percentage text

## Build Information

### Supported Environments

All three environments compile successfully:

1. **esp32s3_n16r8**:
   - RAM: 17.3% (56,764 bytes / 327,680 bytes)
   - Flash: 33.6% (1,056,817 bytes / 3,145,728 bytes)
   - Build time: 38.05s

2. **esp32s3_n8r8**:
   - RAM: 17.4% (57,072 bytes / 327,680 bytes)
   - Flash: 34.0% (1,070,061 bytes / 3,145,728 bytes)
   - Build time: 37.88s

3. **esp32devkitc**:
   - RAM: 17.3% (56,812 bytes / 327,680 bytes)
   - Flash: 85.0% (1,113,857 bytes / 1,310,720 bytes)
   - Build time: 37.44s

### Important Notes

- C++17 inline variable warnings are pre-existing and do not affect functionality
- All environments have sufficient RAM headroom
- The esp32devkitc environment uses 85% Flash but still has 196 KB available

## Migration from 3.13.1

No action required. This version is fully backward compatible:
- Existing features are not modified
- TFT feature addition does not affect OLED configurations
- Existing APIs remain unchanged

## Requirements

To use TFT tests, you need:
1. An ST7789 240x240 pixel TFT screen
2. SPI pin configuration in `include/tft_display.h`
3. Set `TFT_ENABLED` to `true` in configuration

If TFT is not detected, the section will not appear in the web interface.

## Updated Documentation

The following documents have been updated for v3.14.0:
- README (EN/FR)
- CHANGELOG (EN/FR)
- BUILD_AND_DEPLOY (EN/FR)
- RELEASE_NOTES (EN/FR) - new

## Acknowledgments

This version consolidates the project's modular architecture and demonstrates the system's flexibility to support different display types.

## Useful Links

- **GitHub Repository**: https://github.com/[your-username]/ESP32-Diagnostic
- **Complete Documentation**: `/docs/`
- **Bug Reports**: Use GitHub Issues

---

**Checksums (MD5)**
- esp32s3_n16r8: [calculate after upload]
- esp32s3_n8r8: [calculate after upload]
- esp32devkitc: [calculate after upload]
