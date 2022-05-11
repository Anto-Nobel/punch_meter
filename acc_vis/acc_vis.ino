#include <Wire.h>;

const int MPU_addr = 0x68;

void setup()
{
  Serial.begin(115200);
  //Serial.println("Hit a key to start"); 
  //while (Serial.available() == 0);
  Wire.begin(4, 5, 100000);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  delay(50);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B);
  Wire.write(0x10);
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop()
{
  byte XAxis = 0;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 1, true);
  XAxis = Wire.read();
  byte XAxisL = 0;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3C);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 1, true);
  XAxisL = Wire.read();
  int16_t XAxisFull = XAxis << 8 | XAxisL;
  float XAxisFinal = (float)XAxisFull / 16384.0;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3D);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 4, true);
  int16_t YAxisFull = (Wire.read() << 8 | Wire.read());
  int16_t ZAxisFull = (Wire.read() << 8 | Wire.read());
  float YAxisFinal = (float)YAxisFull / 16384.0;
  float ZAxisFinal = (float)ZAxisFull / 16384.0;
  Serial.print(XAxisFinal);
  Serial.print(" ");
  Serial.print(YAxisFinal);
  Serial.print(" ");
  Serial.print(ZAxisFinal);
  Serial.println("");
  delay(100);
}