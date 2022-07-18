#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFC,0xF5,0xC4,0x0F,0x84,0x2C};

const int MPU_addr = 0x68;

typedef struct readings
{
    float aX, aY, aZ, gX, gY, gZ;
} readings;

readings right;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    Serial.begin(57600);
    Wire.begin(4, 5, 57600);
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

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing esp now");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
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
    /*Serial.print("X Axis = ");
    Serial.print(XAxisFinal);
    Serial.println("g");
    Serial.print("Y Axis = ");
    Serial.print(YAxisFinal);
    Serial.println("g");
    Serial.print("Z Axis = ");
    Serial.print(ZAxisFinal);
    Serial.println("g");*/
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
    /*Serial.print("X Axis = ");
    Serial.print(XGyroFinal);
    Serial.println(" deg/s");
    Serial.print("Y Axis = ");
    Serial.print(YGyroFinal);
    Serial.println(" deg/s");
    Serial.print("Z Axis = ");
    Serial.print(ZGyroFinal);
    Serial.println(" deg/s");*/ 
    right.aX=XAxisFinal;
    right.aY=YAxisFinal;
    right.aZ=ZAxisFinal;
    right.gX=XGyroFinal;
    right.gY=YGyroFinal;
    right.gZ=ZGyroFinal; 
    esp_err_t result = esp_now_send(broadcastAddress,(uint8_t *)&right,sizeof(right));
    if(result==ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else{
        Serial.println("Error sending the data");
    }
    //delay(2000);
}
