/*
 * environmental_sensors.cpp - AHT20 + BMP280 Implementation
 */

#include "environmental_sensors.h"
#include "config.h"
#include <cmath>

// Global environmental variables
EnvironmentalData envData;
String envSensorTestResult = "Not tested";
bool envSensorAvailable = false;

// AHT20 I2C Address and commands
#define AHT20_ADDRESS 0x38
#define AHT20_CMD_INIT 0xBE
#define AHT20_CMD_TRIGGER 0xAC
#define AHT20_CMD_SOFTRESET 0xBA

// BMP280 I2C Addresses (try both)
#define BMP280_ADDRESS_PRIMARY 0x76
#define BMP280_ADDRESS_SECONDARY 0x77

// BMP280 Registers
#define BMP280_REG_ID 0xD0
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_CALIB_T1 0x88
#define BMP280_REG_CALIB_P1 0x8E
#define BMP280_REG_PRESSURE 0xF7
#define BMP280_REG_TEMP 0xFA

// BMP280 Calibration data
struct BMP280_Cal {
  uint16_t T1 = 0;
  int16_t T2 = 0;
  int16_t T3 = 0;
  uint16_t P1 = 0;
  int16_t P2 = 0;
  int16_t P3 = 0;
  int16_t P4 = 0;
  int16_t P5 = 0;
  int16_t P6 = 0;
  int16_t P7 = 0;
  int16_t P8 = 0;
  int16_t P9 = 0;
  int32_t t_fine = 0;
} bmp280_cal;

uint8_t bmp280_addr = 0;  // Will be set during init

// Initialize environmental sensors
void initEnvironmentalSensors() {
  Serial.printf("Initializing environmental sensors on I2C (SDA=%d SCL=%d)\r\n", I2C_SDA, I2C_SCL);
  
  envSensorAvailable = false;
  
  // Try to detect and initialize AHT20
  Wire.beginTransmission(AHT20_ADDRESS);
  uint8_t err = Wire.endTransmission();
  
  if (err == 0) {
    // Send initialization sequence
    Wire.beginTransmission(AHT20_ADDRESS);
    Wire.write(AHT20_CMD_INIT);
    Wire.write(0x08);  // Init param 1
    Wire.write(0x00);  // Init param 2
    Wire.endTransmission();
    
    delay(100);
    
    envData.aht20_available = true;
    envData.aht20_status = "Initializing...";
    Serial.println("AHT20: Detected at 0x38");
  } else {
    envData.aht20_available = false;
    envData.aht20_status = "Not detected";
  }
  
  // Try to detect and initialize BMP280 (try both addresses)
  uint8_t bmp280_id = 0;
  
  // Try primary address
  Wire.beginTransmission(BMP280_ADDRESS_PRIMARY);
  err = Wire.endTransmission();
  
  if (err == 0) {
    // Check chip ID
    Wire.beginTransmission(BMP280_ADDRESS_PRIMARY);
    Wire.write(BMP280_REG_ID);
    if (Wire.endTransmission() == 0) {
      Wire.requestFrom(BMP280_ADDRESS_PRIMARY, 1);
      if (Wire.available()) {
        bmp280_id = Wire.read();
        if (bmp280_id == 0x58) {
          bmp280_addr = BMP280_ADDRESS_PRIMARY;
          envData.bmp280_available = true;
          Serial.printf("BMP280: Detected at 0x%02X (ID: 0x%02X)\r\n", bmp280_addr, bmp280_id);
        }
      }
    }
  }
  
  // Try secondary address if not found
  if (!envData.bmp280_available) {
    Wire.beginTransmission(BMP280_ADDRESS_SECONDARY);
    err = Wire.endTransmission();
    
    if (err == 0) {
      Wire.beginTransmission(BMP280_ADDRESS_SECONDARY);
      Wire.write(BMP280_REG_ID);
      if (Wire.endTransmission() == 0) {
        Wire.requestFrom(BMP280_ADDRESS_SECONDARY, 1);
        if (Wire.available()) {
          bmp280_id = Wire.read();
          if (bmp280_id == 0x58) {
            bmp280_addr = BMP280_ADDRESS_SECONDARY;
            envData.bmp280_available = true;
            Serial.printf("BMP280: Detected at 0x%02X (ID: 0x%02X)\r\n", bmp280_addr, bmp280_id);
          }
        }
      }
    }
  }
  
  if (envData.bmp280_available) {
    // Configure BMP280
    // Set normal mode with oversampling x16 for all measurements
    Wire.beginTransmission(bmp280_addr);
    Wire.write(BMP280_REG_CTRL_MEAS);
    Wire.write(0xFF);  // Osrs_t=101 (x16), Osrs_p=101 (x16), Mode=11 (Normal)
    Wire.endTransmission();
    
    // Read calibration data
    Wire.beginTransmission(bmp280_addr);
    Wire.write(BMP280_REG_CALIB_T1);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)bmp280_addr, (uint8_t)26);
    
    if (Wire.available() >= 26) {
      bmp280_cal.T1 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.T2 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.T3 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P1 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P2 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P3 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P4 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P5 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P6 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P7 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P8 = Wire.read() | (Wire.read() << 8);
      bmp280_cal.P9 = Wire.read() | (Wire.read() << 8);
    }
    
    envData.bmp280_status = "Ready";
  } else {
    envData.bmp280_status = "Not detected";
  }
  
  envSensorAvailable = (envData.aht20_available || envData.bmp280_available);
}

// Read AHT20 temperature and humidity
bool readAHT20Data() {
  if (!envData.aht20_available) return false;
  
  // Trigger measurement
  Wire.beginTransmission(AHT20_ADDRESS);
  Wire.write(AHT20_CMD_TRIGGER);
  Wire.write(0x33);  // Param 1
  Wire.write(0x00);  // Param 2
  Wire.endTransmission();
  
  // Wait for measurement to complete
  delay(80);
  
  // Read 6 bytes: status + humidity (2.5 bytes) + temperature (2.5 bytes)
  Wire.requestFrom(AHT20_ADDRESS, 6);
  
  if (Wire.available() >= 6) {
    uint8_t status = Wire.read();
    
    // Check if measurement is ready (bit 7 = 0)
    if (status & 0x80) {
      return false;  // Still measuring
    }
    
    // Read raw data bytes
    uint8_t data[5];
    for (int i = 0; i < 5; i++) {
      data[i] = Wire.read();
    }
    
    // Extract humidity (20 bits from bytes 0-2, high bits first)
    uint32_t humidity_raw = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
    
    // Extract temperature (20 bits from bytes 2-4, low bits first)
    uint32_t temp_raw = (((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | (uint32_t)data[4];
    
    // Convert to percentage and Celsius
    envData.humidity = ((float)humidity_raw / 1048576.0) * 100.0;  // 2^20 = 1048576
    envData.temperature_aht20 = ((float)temp_raw / 1048576.0) * 200.0 - 50.0;
    
    return true;
  }
  
  return false;
}

// Read BMP280 temperature and pressure
bool readBMP280Data() {
  if (!envData.bmp280_available || bmp280_addr == 0) return false;
  
  // Read pressure and temperature data
  Wire.beginTransmission(bmp280_addr);
  Wire.write(BMP280_REG_PRESSURE);
  Wire.endTransmission();
  
  Wire.requestFrom((uint8_t)bmp280_addr, (uint8_t)6);
  
  if (Wire.available() >= 6) {
    uint32_t adc_P = ((uint32_t)Wire.read() << 12) | ((uint32_t)Wire.read() << 4);
    adc_P |= Wire.read() >> 4;
    
    uint32_t adc_T = ((uint32_t)Wire.read() << 12) | ((uint32_t)Wire.read() << 4);
    adc_T |= Wire.read() >> 4;
    
    // Temperature compensation (simplified)
    int32_t var1 = (((adc_T >> 3) - ((int32_t)bmp280_cal.T1 << 1))) * ((int32_t)bmp280_cal.T2) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)bmp280_cal.T1)) * ((adc_T >> 4) - ((int32_t)bmp280_cal.T1))) >> 12) * ((int32_t)bmp280_cal.T3)) >> 14;
    bmp280_cal.t_fine = var1 + var2;
    
    envData.temperature_bmp280 = (bmp280_cal.t_fine * 5 + 128) >> 8;
    envData.temperature_bmp280 /= 100.0;
    
    // Pressure compensation (simplified)
    int64_t var1_p = ((int64_t)bmp280_cal.t_fine) - 128000;
    int64_t var2_p = var1_p * var1_p * (int64_t)bmp280_cal.P6;
    var2_p = var2_p + ((var1_p * (int64_t)bmp280_cal.P5) << 17);
    var2_p = var2_p + (((int64_t)bmp280_cal.P4) << 35);
    var1_p = ((var1_p * var1_p * (int64_t)bmp280_cal.P3) >> 8) + ((var1_p * (int64_t)bmp280_cal.P2) << 12);
    var1_p = (((1LL << 47) + var1_p) * ((int64_t)bmp280_cal.P1)) >> 33;
    
    if (var1_p != 0) {
      int64_t pressure = 1048576 - adc_P;
      pressure = (((pressure << 31) - var2_p) * 3125) / var1_p;
      var1_p = (((int64_t)bmp280_cal.P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
      var2_p = (((int64_t)bmp280_cal.P8) * pressure) >> 19;
      pressure = ((pressure + var1_p + var2_p) >> 8) + (((int64_t)bmp280_cal.P7) << 4);
      envData.pressure = pressure / 256.0 / 100.0;  // Convert to hPa
    }
    
    return true;
  }
  
  return false;
}

// Calculate altitude from pressure
void calculateAltitude(float pressure_sea_level_hpa) {
  if (envData.pressure > 0) {
    // Using barometric formula
    envData.altitude = 44330.0 * (1.0 - pow(envData.pressure / pressure_sea_level_hpa, 1.0 / 5.255));
  }
}

// Update both sensors
void updateEnvironmentalSensors() {
  bool aht20_ok = false;
  bool bmp280_ok = false;
  
  // Try to read each sensor independently
  if (envData.aht20_available) {
    aht20_ok = readAHT20Data();
    if (aht20_ok) {
      envData.aht20_status = "OK";
    } else {
      envData.aht20_status = "Read error";
    }
  }
  
  if (envData.bmp280_available) {
    bmp280_ok = readBMP280Data();
    if (bmp280_ok) {
      envData.bmp280_status = "OK";
    } else {
      envData.bmp280_status = "Read error";
    }
  }
  
  // Calculate average temperature if both sensors are available
  if (aht20_ok && bmp280_ok) {
    envData.temperature_avg = (envData.temperature_aht20 + envData.temperature_bmp280) / 2.0;
    calculateAltitude();
    envData.combined_status = "Both sensors OK";
  } else if (aht20_ok) {
    envData.temperature_avg = envData.temperature_aht20;
    envData.combined_status = "AHT20 OK";
  } else if (bmp280_ok) {
    envData.temperature_avg = envData.temperature_bmp280;
    calculateAltitude();
    envData.combined_status = "BMP280 OK";
  } else {
    envData.combined_status = "No data available";
  }
}

// Test environmental sensors
void testEnvironmentalSensors() {
  Serial.println("\r\n=== TEST ENVIRONMENTAL SENSORS ===");
  
  if (!envSensorAvailable) {
    envSensorTestResult = "No sensors detected";
    Serial.println("Environmental Sensors: No sensors detected");
    return;
  }
  
  testAHT20();
  testBMP280();
  
  // Try to read data
  bool success = false;
  
  for (int i = 0; i < 5; i++) {
    updateEnvironmentalSensors();
    delay(100);
    
    if (envData.temperature_avg > -999.0) {
      success = true;
      break;
    }
  }
  
  if (success) {
    envSensorTestResult = "OK";
    Serial.printf("Environmental Sensors: OK\r\n");
    Serial.printf("  Temperature: %.1f°C (AHT20: %.1f°C, BMP280: %.1f°C)\r\n",
      envData.temperature_avg, envData.temperature_aht20, envData.temperature_bmp280);
    Serial.printf("  Humidity: %.1f%%\r\n", envData.humidity);
    Serial.printf("  Pressure: %.2f hPa\r\n", envData.pressure);
    Serial.printf("  Altitude: %.1f m\r\n", envData.altitude);
  } else {
    envSensorTestResult = "Error reading data";
    Serial.println("Environmental Sensors: Error reading data");
  }
}

// Test AHT20 specifically
void testAHT20() {
  if (!envData.aht20_available) return;
  
  Serial.println("Testing AHT20...");
  
  if (readAHT20Data()) {
    envData.aht20_status = "OK";
    Serial.printf("  AHT20: T=%.1f°C, H=%.1f%%\r\n", envData.temperature_aht20, envData.humidity);
  } else {
    envData.aht20_status = "Read error";
    Serial.println("  AHT20: Failed to read");
  }
}

// Test BMP280 specifically
void testBMP280() {
  if (!envData.bmp280_available) return;
  
  Serial.println("Testing BMP280...");
  
  if (readBMP280Data()) {
    envData.bmp280_status = "OK";
    Serial.printf("  BMP280: T=%.1f°C, P=%.2f hPa\r\n", envData.temperature_bmp280, envData.pressure);
  } else {
    envData.bmp280_status = "Read error";
    Serial.println("  BMP280: Failed to read");
  }
}
