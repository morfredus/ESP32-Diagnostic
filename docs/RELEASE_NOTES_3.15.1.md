# Release Notes v3.15.1 - Critical Memory Fix

**Release Date:** November 27, 2025  
**Type:** Patch Release (Bug Fix)  
**Severity:** Critical for ESP32 Classic users

---

## Overview

Version 3.15.1 addresses a critical memory issue that prevented the web interface from loading on ESP32 Classic boards (`esp32devkitc` environment) without PSRAM. This patch implements chunked streaming of JavaScript content from PROGMEM, eliminating large heap allocations that caused crashes on memory-constrained devices.

---

## Critical Bug Fix

### Web Interface Loading Failure on ESP32 Classic

**Problem:**
- Web pages failed to load or displayed blank content on ESP32 Classic (4MB Flash, no PSRAM)
- Root cause: Large JavaScript file (`DIAGNOSTIC_JS_STATIC`) was being converted from PROGMEM to a single String object, causing heap exhaustion
- Symptom: HTTP 500 errors, connection timeouts, or incomplete page rendering

**Solution:**
- Implemented chunked transfer encoding for JavaScript content
- Modified `handleJavaScriptRoute()` to stream PROGMEM data in 1KB blocks using `memcpy_P()` and `server.sendContent()`
- Eliminated single large String allocation (previously ~50KB+)

**Impact:**
- ✅ ESP32 Classic (`esp32devkitc`): Web UI now loads reliably
- ✅ ESP32-S3 N16R8: Improved memory efficiency, no regressions
- ✅ ESP32-S3 N8R8: Improved memory efficiency, no regressions

---

## Technical Details

### Changed Files
- `src/main.cpp`: Modified `handleJavaScriptRoute()` function

### Before (v3.15.0)
```cpp
// Large allocation - fails on ESP32 Classic
const char* staticJsPtr = DIAGNOSTIC_JS_STATIC;
size_t staticJsLen = strlen(staticJsPtr);
server.sendContent(String(FPSTR(DIAGNOSTIC_JS_STATIC)));
```

### After (v3.15.1)
```cpp
// Chunked streaming - memory-efficient
const char* staticJsPtr = DIAGNOSTIC_JS_STATIC;
size_t staticJsLen = strlen_P(staticJsPtr);
const size_t CHUNK_SIZE = 1024;
char chunkBuf[CHUNK_SIZE + 1];
size_t sent = 0;
while (sent < staticJsLen) {
  size_t n = (staticJsLen - sent) > CHUNK_SIZE ? CHUNK_SIZE : (staticJsLen - sent);
  memcpy_P(chunkBuf, staticJsPtr + sent, n);
  chunkBuf[n] = '\0';
  server.sendContent(chunkBuf);
  sent += n;
}
```

### Memory Savings
- **Peak heap allocation reduced by ~50KB** during web page serving
- Chunked transfer uses only 1KB stack buffer per iteration
- No functional changes to UI or user experience

---

## Testing & Validation

### Tested Environments
| Environment | Board | Flash | PSRAM | Status |
|-------------|-------|-------|-------|--------|
| `esp32s3_n16r8` | ESP32-S3 DevKitC-1 N16R8 | 16MB | 8MB OPI | ✅ Validated |
| `esp32s3_n8r8` | ESP32-S3 DevKitC-1 N8R8 | 8MB | 8MB | ✅ Validated |
| `esp32devkitc` | ESP32 Classic DevKitC | 4MB | None | ✅ **Fixed** |

### Validation Checklist
- [x] Web interface loads on ESP32 Classic
- [x] All tabs accessible (Overview, Display & Signal, Sensors, Hardware Tests, Wireless, Benchmark, Export)
- [x] Language switching (FR/EN) works
- [x] REST API endpoints respond correctly
- [x] No regressions on ESP32-S3 variants
- [x] Memory fragmentation reduced across all boards

---

## Upgrade Instructions

### From v3.15.0
1. Pull latest code from repository
2. Verify `platformio.ini` shows `PROJECT_VERSION="3.15.1"`
3. Clean build environment:
   ```bash
   pio run -t clean
   ```
4. Build for your target:
   ```bash
   # ESP32 Classic (critical fix applies here)
   pio run -e esp32devkitc
   
   # Or ESP32-S3 variants (benefits from optimization)
   pio run -e esp32s3_n16r8
   pio run -e esp32s3_n8r8
   ```
5. Upload firmware:
   ```bash
   pio run --target upload -e <your_environment>
   ```

### From Earlier Versions
Follow standard upgrade procedure as documented in [BUILD_AND_DEPLOY.md](BUILD_AND_DEPLOY.md).

---

## Breaking Changes
**None.** This is a backward-compatible patch release.

---

## Known Issues
None introduced by this release. See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for general known issues.

---

## Recommendations

### For ESP32 Classic Users
- **Upgrade immediately** if experiencing web interface loading issues
- This patch is critical for reliable operation on boards without PSRAM

### For ESP32-S3 Users
- Upgrade recommended for improved memory efficiency
- No urgent issues addressed for S3 variants, but optimization benefits apply

---

## Future Work

While this patch resolves the immediate memory crisis, future enhancements may include:
- Optional gzip compression for JavaScript assets
- Filesystem-based asset serving (LittleFS/SPIFFS)
- Progressive web app (PWA) capabilities for offline caching
- Further CSS/HTML minification

---

## Related Documentation
- [CHANGELOG.md](../CHANGELOG.md) - Full version history
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Diagnostic guides
- [ARCHITECTURE.md](ARCHITECTURE.md) - Firmware internals

---

## Support
If you encounter issues after upgrading:
1. Check serial monitor output during boot
2. Verify free heap using `/debug/status` endpoint
3. Report issues at: https://github.com/morfredus/ESP32-Diagnostic/issues

---

**Version:** 3.15.1  
**Git Tag:** `v3.15.1`  
**Commit:** (to be tagged after merge to main)
