# Release Notes — 3.17.1 (2025-12-05)

- Refreshed ESP32-S3 pin mapping: GPS RX=18/TX=17/PPS=8; TFT MOSI=11/SCLK=12/CS=10/DC=9/RST=13/BL=7; RGB R=47/G=48/B=45; sensors updated (PWM/Buzzer=14, DHT=5, Motion=4, Light=19, HC-SR04 TRIG=3/ECHO=6); buttons unchanged (BTN1=1, BTN2=2).
- Updated ESP32 Classic pin mapping: GPS RX=16/TX=17/PPS=4; TFT MOSI=23/SCLK=18/CS=19/DC=27/RST=26/BL=13; RGB R=12/G=14/B=15; sensors updated (PWM/Buzzer=5, DHT=25, Motion=36, Light=2, HC-SR04 TRIG=32/ECHO=33); buttons BTN1=34/BTN2=35.
- Documentation aligned (README EN/FR, pin mapping guides, feature matrix, usage, build notes) and `PROJECT_VERSION` bumped to `3.17.1` in `platformio.ini`.
- Release type: Patch (3.17.0 ? 3.17.1); functionality unchanged aside from new default pin assignments.








