# Web Interface Source Files

This directory contains the **readable, maintainable source code** for the ESP32 Diagnostic web interface.

## 📁 Directory Structure

```
web_src/
├── README.md           # This file
├── styles.css          # Readable CSS (13 KB)
├── app.js              # Readable JavaScript - Full version (115 KB)
└── app-lite.js         # Readable JavaScript - Lite version for ESP32 Classic (3.8 KB)
```

## 🎯 Purpose

The web interface code (HTML, CSS, JavaScript) is embedded directly into the ESP32 firmware. To minimize memory usage and maximize performance, this code **must be minified** before compilation.

However, minified code is extremely difficult to read and maintain. This directory solves that problem by providing:

1. **Readable source files** - Easy to edit and understand
2. **Automatic minification** - Scripts to convert readable code to minified code
3. **Version control friendly** - Track changes in human-readable format

## 🔄 Workflow

### Initial Setup

1. **Install Python dependencies** (one-time):
   ```bash
   pip install rcssmin rjsmin jsbeautifier cssbeautifier
   ```

### Making Changes to the Web Interface

**IMPORTANT:** Never edit `include/web_interface.h` directly! Always edit the source files in `web_src/`.

#### Step 1: Extract Current Code (optional)

If you need to extract the latest minified code from `web_interface.h`:

```bash
python tools/extract_web_sources.py
```

This will:
- Extract minified CSS and JavaScript from `include/web_interface.h`
- Beautify the code
- Save readable versions to `web_src/`

#### Step 2: Edit Source Files

Edit the readable source files in `web_src/`:

- `styles.css` - Modify CSS styles
- `app.js` - Modify JavaScript (full version)
- `app-lite.js` - Modify JavaScript (lite version for ESP32 Classic)

#### Step 3: Minify and Update Header

After making changes, run:

```bash
python tools/minify_web.py
```

This will:
- Read source files from `web_src/`
- Minify CSS and JavaScript
- Update `include/web_interface.h` with minified content
- Show compression statistics

Example output:
```
======================================================================
ESP32 Diagnostic - Web Assets Minifier
======================================================================

1. Reading source files...
   - CSS: web_src/styles.css
   - JS (Full): web_src/app.js
   - JS (Lite): web_src/app-lite.js

2. Minifying CSS...
    Original: 13144 bytes
    Minified: 9715 bytes
    Saved: 3429 bytes (26.1%)

3. Injecting CSS into web_interface.h...

4. Processing JavaScript (Full)...
    Original: 115331 bytes
    Minified: 94279 bytes
    Saved: 21052 bytes (18.3%)

5. Processing JavaScript (Lite)...
    Original: 3819 bytes
    Minified: 2768 bytes
    Saved: 1051 bytes (27.5%)

6. Writing updated web_interface.h...

======================================================================
✅ Web interface header updated successfully!
======================================================================
```

#### Step 4: Compile and Upload

```bash
# Compile the project
pio run

# Upload to ESP32
pio run --target upload
```

#### Step 5: Test

1. Open the web interface at `http://ESP32-Diagnostic.local` or the ESP32's IP address
2. Test all functionality
3. Verify that changes are visible and working correctly

## 📊 File Sizes

| File | Readable Size | Minified Size | Savings |
|------|---------------|---------------|---------|
| CSS | ~13 KB | ~10 KB | ~26% |
| JavaScript (Full) | ~115 KB | ~94 KB | ~18% |
| JavaScript (Lite) | ~3.8 KB | ~2.8 KB | ~28% |

## 🛠️ Tools

### extract_web_sources.py

Extracts and beautifies code from `web_interface.h`.

**Usage:**
```bash
python tools/extract_web_sources.py
```

**Features:**
- Extracts minified CSS and JavaScript
- Beautifies code for readability
- Preserves comments and structure
- Creates backup of original files

### minify_web.py

Minifies source files and updates `web_interface.h`.

**Usage:**
```bash
python tools/minify_web.py
```

**Features:**
- Minifies CSS using rcssmin
- Minifies JavaScript using rjsmin
- Preserves web_interface.h structure
- Shows compression statistics

## 📝 Best Practices

1. **Always edit source files** - Never edit `web_interface.h` directly
2. **Test after minifying** - Always test the web interface after running minification
3. **Version control** - Commit both source files and generated `web_interface.h`
4. **Document changes** - Add comments to source files explaining complex logic
5. **Keep it clean** - Remove unused CSS/JavaScript to minimize firmware size

## 🚨 Important Notes

- The HTML structure is generated dynamically in C++ code (`generateHTML()` function)
- CSS is embedded in the `<style>` tag
- JavaScript is stored in `PROGMEM` constants
- Two JavaScript versions:
  - **Full** (`DIAGNOSTIC_JS_STATIC`) - Complete interface for ESP32-S3
  - **Lite** (`DIAGNOSTIC_JS_STATIC_LITE`) - Simplified interface for ESP32 Classic

## 🔍 Troubleshooting

### "Module not found" errors

Install Python dependencies:
```bash
pip install rcssmin rjsmin jsbeautifier cssbeautifier
```

### Minification produces broken code

1. Check for syntax errors in source files
2. Ensure JavaScript is valid (no ES6+ features not supported by minifier)
3. Test source files in a browser before minifying

### Web interface doesn't update after compilation

1. Clear browser cache (Ctrl+Shift+R or Cmd+Shift+R)
2. Verify `web_interface.h` was updated (check file modification time)
3. Ensure compilation completed successfully

## 📚 Additional Resources

- [Project README](../README.md)
- [Architecture Documentation](../docs/ARCHITECTURE.md)
- [Web Interface Documentation](../docs/WEB_INTERFACE.md)
- [API Reference](../docs/API_REFERENCE.md)

## 💡 Tips

- **Use comments** - Add comments in source files; they'll be removed during minification
- **Format code** - Use proper indentation; it helps readability and doesn't affect minified size
- **Test incrementally** - Test changes frequently rather than making many changes at once
- **Check diff** - Review the diff of `web_interface.h` before committing

---

**Version:** 3.31.0
**Last Updated:** 2025-12-27
