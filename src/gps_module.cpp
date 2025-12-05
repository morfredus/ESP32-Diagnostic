/*
 * GPS_MODULE.CPP - GPS NEO-6M/NEO-8M/NEO-M8 Implementation
 */

#include "gps_module.h"
#include "config.h"
#include <cmath>

// Global GPS variables
GPSData gpsData;
HardwareSerial& gpsSerial = Serial1;
String gpsTestResult = "Not tested";
bool gpsAvailable = false;

// GPS UART Configuration
void initGPS() {
  #if defined(PIN_GPS_RXD) && defined(PIN_GPS_TXD) && PIN_GPS_RXD >= 0 && PIN_GPS_TXD >= 0
    Serial.printf("Initializing GPS on RX=%d TX=%d\r\n", PIN_GPS_RXD, PIN_GPS_TXD);
    
    // Initialize UART1 for GPS
    gpsSerial.begin(9600, SERIAL_8N1, PIN_GPS_RXD, PIN_GPS_TXD);
    gpsSerial.setRxBufferSize(2048);  // Increased buffer for NMEA sentences
    
    // Setup PPS interrupt if available
    #if defined(PIN_GPS_PPS) && PIN_GPS_PPS >= 0
      pinMode(PIN_GPS_PPS, INPUT);
      Serial.printf("GPS PPS signal on GPIO %d\r\n", PIN_GPS_PPS);
    #endif
    
    gpsAvailable = true;
    delay(100);
  #else
    Serial.println("GPS: Pins not configured correctly");
    gpsAvailable = false;
  #endif
}

// Read and parse GPS NMEA sentences
void updateGPS() {
  if (!gpsAvailable || !gpsSerial) return;
  
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    
    // Very simple line buffer (no external library used)
    static String nmea_line;
    
    if (c == '\n') {
      nmea_line.trim();
      
      if (nmea_line.length() > 0 && nmea_line[0] == '$') {
        // Parse different NMEA sentence types
        if (nmea_line.startsWith("$GPRMC") || nmea_line.startsWith("$GNRMC")) {
          parseGPRMC(nmea_line);
        } else if (nmea_line.startsWith("$GPGGA") || nmea_line.startsWith("$GNGGA")) {
          parseGPGGA(nmea_line);
        } else if (nmea_line.startsWith("$GPGSA") || nmea_line.startsWith("$GNGSA")) {
          parseGPGSA(nmea_line);
        } else if (nmea_line.startsWith("$GPGSV") || nmea_line.startsWith("$GNGSV")) {
          parseGPGSV(nmea_line);
        }
      }
      
      nmea_line = "";
    } else if (c == '\r') {
      // Ignore carriage return
      continue;
    } else {
      nmea_line += c;
    }
  }
}

// Parse RMC sentence: $GPRMC,time,status,lat,N/S,lon,E/W,speed,course,date...
void parseGPRMC(const String& sentence) {
  int start = 0;
  int field = 0;
  String fields[13];
  
  // Split by comma
  for (int i = 0; i < sentence.length(); i++) {
    if (sentence[i] == ',' || sentence[i] == '*') {
      fields[field] = sentence.substring(start, i);
      start = i + 1;
      field++;
      if (field >= 13) break;
    }
  }
  
  if (field < 9) return;
  
  // Status: A=Active, V=Void
  bool active = (fields[2] == "A");
  gpsData.valid = active;
  
  if (!active) {
    gpsData.hasFix = false;
    gpsData.status_str = "No Fix";
    return;
  }
  
  gpsData.hasFix = true;
  gpsData.status_str = "Fix";
  
  // Parse time HHMMSS.SS
  if (fields[1].length() >= 6) {
    gpsData.hour = fields[1].substring(0, 2).toInt();
    gpsData.minute = fields[1].substring(2, 4).toInt();
    gpsData.second = fields[1].substring(4, 6).toInt();
    gpsData.hasTime = true;
  }
  
  // Parse latitude DDmm.mmmm
  if (fields[3].length() > 0) {
    float lat = fields[3].toFloat();
    gpsData.latitude = (int)(lat / 100) + (lat - (int)(lat / 100) * 100) / 60.0;
    if (fields[4] == "S") gpsData.latitude = -gpsData.latitude;
  }
  
  // Parse longitude DDDmm.mmmm
  if (fields[5].length() > 0) {
    float lon = fields[5].toFloat();
    gpsData.longitude = (int)(lon / 100) + (lon - (int)(lon / 100) * 100) / 60.0;
    if (fields[6] == "W") gpsData.longitude = -gpsData.longitude;
  }
  
  // Speed in knots
  if (fields[7].length() > 0) {
    gpsData.speed = fields[7].toFloat();
  }
  
  // Course
  if (fields[8].length() > 0) {
    gpsData.course = fields[8].toFloat();
  }
  
  // Parse date DDMMYY
  if (fields[9].length() >= 6) {
    gpsData.day = fields[9].substring(0, 2).toInt();
    gpsData.month = fields[9].substring(2, 4).toInt();
    gpsData.year = 2000 + fields[9].substring(4, 6).toInt();
    gpsData.hasDate = true;
  }
}

// Parse GGA sentence: $GPGGA,time,lat,N/S,lon,E/W,quality,satellites,hdop,altitude,M...
void parseGPGGA(const String& sentence) {
  int start = 0;
  int field = 0;
  String fields[15];
  
  // Split by comma
  for (int i = 0; i < sentence.length(); i++) {
    if (sentence[i] == ',' || sentence[i] == '*') {
      fields[field] = sentence.substring(start, i);
      start = i + 1;
      field++;
      if (field >= 15) break;
    }
  }
  
  if (field < 9) return;
  
  // Quality: 0=invalid, 1=GPS, 2=DGPS, 3=PPS, 4=RTK, 5=Float RTK, 6=Estimated, 7=Manual, 8=Simulation
  int quality = fields[6].toInt();
  gpsData.hasFix = (quality > 0);
  
  // Number of satellites
  if (fields[7].length() > 0) {
    gpsData.satellites = fields[7].toInt();
  }
  
  // HDOP (Horizontal Dilution of Precision)
  if (fields[8].length() > 0) {
    gpsData.hdop = fields[8].toFloat();
  }
  
  // Altitude above sea level
  if (fields[9].length() > 0) {
    gpsData.altitude = fields[9].toFloat();
  }
  
  // Update status based on quality
  if (quality == 0) {
    gpsData.status_str = "Invalid";
  } else if (quality == 1) {
    gpsData.status_str = "GPS Fix";
  } else if (quality == 2) {
    gpsData.status_str = "DGPS Fix";
  } else if (quality >= 3 && quality <= 5) {
    gpsData.status_str = "RTK Fix";
  } else {
    gpsData.status_str = "Fix";
  }
}

// Parse GSA sentence: $GPGSA,mode,fix_type,sat_ids...,pdop,hdop,vdop
void parseGPGSA(const String& sentence) {
  int start = 0;
  int field = 0;
  String fields[20];
  
  // Split by comma
  for (int i = 0; i < sentence.length(); i++) {
    if (sentence[i] == ',' || sentence[i] == '*') {
      fields[field] = sentence.substring(start, i);
      start = i + 1;
      field++;
      if (field >= 20) break;
    }
  }
  
  if (field < 3) return;
  
  // Fix type: 1=no fix, 2=2D, 3=3D
  int fix_type = fields[2].toInt();
  if (fix_type == 1) {
    gpsData.fix_type = "No Fix";
  } else if (fix_type == 2) {
    gpsData.fix_type = "2D";
  } else if (fix_type == 3) {
    gpsData.fix_type = "3D";
  }
  
  // Count satellites used (fields 3-14)
  gpsData.satellites_used = 0;
  for (int i = 3; i < 15 && i < field; i++) {
    if (fields[i].length() > 0) {
      gpsData.satellites_used++;
    }
  }
  
  // PDOP, HDOP, VDOP (usually last 3 fields)
  if (field >= 17) {
    if (fields[field - 3].length() > 0) gpsData.pdop = fields[field - 3].toFloat();
    if (fields[field - 2].length() > 0) gpsData.hdop = fields[field - 2].toFloat();
    if (fields[field - 1].length() > 0) gpsData.vdop = fields[field - 1].toFloat();
  }
}

// Parse GSV sentence: $GPGSV,total_msgs,msg_num,satellites_visible,sat_id,elevation,azimuth,snr...
void parseGPGSV(const String& sentence) {
  int start = 0;
  int field = 0;
  String fields[25];
  
  // Split by comma
  for (int i = 0; i < sentence.length(); i++) {
    if (sentence[i] == ',' || sentence[i] == '*') {
      fields[field] = sentence.substring(start, i);
      start = i + 1;
      field++;
      if (field >= 25) break;
    }
  }
  
  if (field < 4) return;
  
  // Satellites in view (field 3)
  if (fields[3].length() > 0) {
    // Store if this is the first GSV message (more accurate for visible satellites)
    if (fields[2] == "1") {
      // This is first message, we can use this count as total visible
    }
  }
}

// Test GPS module
void testGPS() {
  Serial.println("\r\n=== TEST GPS ===");
  
  if (!gpsAvailable) {
    gpsTestResult = "GPS not initialized";
    Serial.println("GPS: Not initialized");
    return;
  }
  
  Serial.println("GPS: Waiting for fix (10 seconds)...");
  gpsTestResult = "Waiting...";
  
  unsigned long start_time = millis();
  unsigned long timeout = 10000;  // 10 second timeout
  
  while (millis() - start_time < timeout) {
    updateGPS();
    delay(100);
    
    if (gpsData.hasFix && gpsData.satellites > 0) {
      gpsTestResult = "OK";
      Serial.printf("GPS: Fix obtained | Lat: %.6f, Lon: %.6f, Alt: %.1f m, Sats: %d, HDOP: %.1f\r\n",
        gpsData.latitude, gpsData.longitude, gpsData.altitude, gpsData.satellites, gpsData.hdop);
      return;
    }
  }
  
  // Timeout - report what we have
  if (gpsData.satellites > 0) {
    gpsTestResult = String(gpsData.satellites) + " satellites visible, waiting for fix...";
    Serial.printf("GPS: %s\r\n", gpsTestResult.c_str());
  } else {
    gpsTestResult = "Timeout - no data received";
    Serial.println("GPS: Timeout - no NMEA data received. Check connections and baud rate.");
  }
}

// Handle PPS interrupt (optional, for high precision timing)
void handlePPSInterrupt() {
  // This would be called via GPIO interrupt on PPS pin
  gpsData.hasPPS = true;
}
