/*
 * GPS_MODULE.H - GPS NEO-6M/NEO-8M/NEO-M8 Module Handler
 * Utilise UART1 avec les pins référencées UNIQUEMENT via board_config.h (GPS_RXD, GPS_TXD, GPS_PPS)
 * Parses NMEA sentences for location, altitude, satellites, HDOP
 */

#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <Arduino.h>
#include <HardwareSerial.h>

// GPS Data Structure
struct GPSData {
  bool valid = false;
  bool hasTime = false;
  bool hasDate = false;
  bool hasFix = false;
  bool hasPPS = false;  // Pulse Per Second signal detection
  
  float latitude = 0.0;
  float longitude = 0.0;
  float altitude = 0.0;
  float speed = 0.0;      // knots
  float course = 0.0;
  float hdop = 999.0;     // Horizontal Dilution of Precision
  float vdop = 999.0;     // Vertical Dilution of Precision
  float pdop = 999.0;     // Position Dilution of Precision
  
  uint8_t satellites = 0;
  uint8_t satellites_used = 0;
  
  uint16_t year = 0;
  uint8_t month = 0;
  uint8_t day = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
  
  String status_str = "No Fix";
  String fix_type = "";  // 2D or 3D
};

extern GPSData gpsData;
extern HardwareSerial& gpsSerial;
extern String gpsTestResult;
extern bool gpsAvailable;

// Function declarations
void initGPS();
void updateGPS();
void testGPS();
void parseGPRMC(const String& sentence);
void parseGPGGA(const String& sentence);
void parseGPGSA(const String& sentence);
void parseGPGSV(const String& sentence);
void handlePPSInterrupt();

#endif // GPS_MODULE_H
