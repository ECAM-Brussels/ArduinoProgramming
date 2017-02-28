// Accelerometer.ino
// Author: Sébastien Combéfis
// Version: February 28, 2017

#include <Wire.h>

#define KEYES_345_ADDRESS      (0x53)

#define KEYES_REG_DEVID        (0x00)
#define KEYES_REG_POWER_CTL    (0x2D)
#define KEYES_REG_DATA_FORMAT  (0x31)
#define KEYES_REG_OFSX         (0x1E)
#define KEYES_REG_OFSY         (0x1F)
#define KEYES_REG_OFSZ         (0x20)
#define KEYES_REG_DATAX0       (0x32)
#define KEYES_REG_DATAY0       (0x34)
#define KEYES_REG_DATAZ0       (0x36)

// Write a 1-byte value into a register
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(KEYES_345_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Read a 1-byte value from a register
int8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(KEYES_345_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(KEYES_345_ADDRESS, 1);
  return Wire.read();
}

// Read a 2-byte value from a register
int16_t readRegister16(uint8_t reg) {
  Wire.beginTransmission(KEYES_345_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(KEYES_345_ADDRESS, 2);
  return (uint16_t) (Wire.read() | (Wire.read() << 8));
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Detect the accelerometer
  uint8_t devid = readRegister(KEYES_REG_DEVID);
  if (devid != 0xe5) {
    Serial.write("Failed to find the device!\n");
    exit(-1);
  }
  
  // Set the sensitivity to +/-2g
  Serial.write("Device detected, setting sensitivity to +/-2g...\n");
  writeRegister(KEYES_REG_DATA_FORMAT, 0);
  
  // Calibrate the accelerometer
  Serial.write("Calibrating the device...\n");
  writeRegister(KEYES_REG_OFSX, 0);
  writeRegister(KEYES_REG_OFSY, 0);
  writeRegister(KEYES_REG_OFSZ, 0);

  int avx = 0, avy = 0, avz = 0;
  int N = 10;
  for (int i = 0; i < N; i++) {
    avx += readRegister16(KEYES_REG_DATAX0);
    avy += readRegister16(KEYES_REG_DATAY0);
    avz += readRegister16(KEYES_REG_DATAZ0);
    delay(1000);
  }
  
  writeRegister(KEYES_REG_OFSX, -(avx / N) / 4);
  writeRegister(KEYES_REG_OFSY, -(avy / N) / 4);
  writeRegister(KEYES_REG_OFSZ, -(avz / N) / 4);
  
  // Activate measurement mode
  writeRegister(KEYES_REG_POWER_CTL, 0x08);
  Serial.write("Starting to measure!\n");
}

void loop() {
  char output[512];
  int x, y, z;
  
  // Read x, y and z-axis values
  x = readRegister16(KEYES_REG_DATAX0);
  y = readRegister16(KEYES_REG_DATAY0);
  z = readRegister16(KEYES_REG_DATAZ0);
  
  sprintf(output, "(x, y, z) = (%d, %d, %d)\n", x, y, z);
  Serial.write(output);
  delay(1000);
}

