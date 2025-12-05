/*
 * ENVIRONMENTAL_SENSORS.H - AHT20 (Temp/Humidity) + BMP280 (Pressure) Handler
 * Uses I2C with pins configured in config.h
 * AHT20 I2C Address: 0x38
 * BMP280 I2C Address: 0x76 or 0x77
 */

#ifndef ENVIRONMENTAL_SENSORS_H
#define ENVIRONMENTAL_SENSORS_H

#include <Arduino.h>
#include <Wire.h>

// Environmental Data Structure
struct EnvironmentalData {
  bool aht20_available = false;
  bool bmp280_available = false;
  
  // AHT20 Data
  float temperature_aht20 = -999.0;
  float humidity = -999.0;
  String aht20_status = "Not detected";
  
  // BMP280 Data
  float temperature_bmp280 = -999.0;
  float pressure = -999.0;      // hPa
  float altitude = -999.0;      // meters (calculated)
  String bmp280_status = "Not detected";
  
  // Combined
  float temperature_avg = -999.0;  // Average of both sensors
  String combined_status = "No sensors detected";
};

extern EnvironmentalData envData;
extern String envSensorTestResult;
extern bool envSensorAvailable;

// Function declarations
void initEnvironmentalSensors();
void updateEnvironmentalSensors();
void testEnvironmentalSensors();
void testAHT20();
void testBMP280();
bool readAHT20Data();
bool readBMP280Data();
void calculateAltitude(float pressure_sea_level_hpa = 1013.25);

#endif // ENVIRONMENTAL_SENSORS_H
