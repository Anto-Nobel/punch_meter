#include <Wire.h>;
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "right"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <MQUnifiedsensor.h>

#define WIFI_SSID "nobel"
#define WIFI_PASSWORD "sarobert"

//InfluxDB
#define INFLUXDB_URL "https://europe-west1-1.gcp.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "KlmcztU1dEWZ04JzoiK3eg2eBtZxH-LbbYauBEH0UGpdmfjqxGvwIpaAIXv8EqTwIiXD-Hs0O3Konu3NjyrQfA=="
#define INFLUXDB_ORG "nobel2002@gmail.com"
#define INFLUXDB_BUCKET "nobel2002's Bucket"
#define TZ_INFO "IST"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point right_accX("right");

const int MPU_addr = 0x68;

void setup()
{
    Serial.begin(115200);
    //Serial.println("Press any key");
    //while(Serial.available()==0);
    Wire.begin(4,5,115200);
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
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  int16_t XGyroFull = Wire.read() << 8 | Wire.read();
  int16_t YGyroFull = Wire.read() << 8 | Wire.read();
  int16_t ZGyroFull = Wire.read() << 8 | Wire.read();
  float XGyroFinal = (float)XGyroFull / 32.8;
  float YGyroFinal = (float)YGyroFull / 32.8;
  float ZGyroFinal = (float)ZGyroFull / 32.8;
  //Serial.print("X_Acc:");
  Serial.print(XAxisFinal);
  Serial.print(",");
  //Serial.print("Y_Acc:");
  Serial.print(YAxisFinal);
  Serial.print(",");
  //Serial.print("Z_Acc:");
  Serial.print(ZAxisFinal);
  Serial.print(",");
  //Serial.print("X_Gyro:");
  Serial.print(XGyroFinal);
  Serial.print(",");
  //Serial.print("Y_Gyro:");
  Serial.print(YGyroFinal);
  Serial.print(",");
  //Serial.print("Z_Gyro:");
  Serial.println(ZGyroFinal);
}
