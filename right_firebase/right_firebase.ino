#include <Wire.h>
#include <esp_now.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "time.h"

#define WIFI_SSID "nobel"
#define WIFI_PASSWORD "sarobert"

#define API_KEY "AIzaSyDLHqlCiDTFUgyF32n1Ejcg3XjQWfEf29c"
#define DATABASE_URL "https://punchmeter-15476-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int MPU_addr = 0x68;

void initWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }

    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}

void initMPU()
{
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

void setup()
{
    Serial.begin(460800);
    initWiFi();
    initMPU();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    config.api_key = API_KEY;
    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;
    // Assign the user sign in credentials
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    // Assign the callback function for the long running token generation task
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Assign the maximum retry of token generation
    config.max_token_generation_retry = 5;

    // Initialize the library with the Firebase authen and config
    Firebase.begin(&config, &auth);

    // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    while ((auth.token.uid) == "")
    {
        Serial.print('.');
        delay(1000);
    }
}

String tellDate()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    char a[22];
    strftime(a, 22, "%d-%m-%Y", &timeinfo);
    String t = convertToString(a, 22);
    return t;
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
    /*Serial.print(XAxisFinal);
    Serial.print(",");
    Serial.print(YAxisFinal);
    Serial.print(",");
    Serial.print(ZAxisFinal);
    Serial.print(",");
    Serial.print(XGyroFinal);
    Serial.print(",");
    Serial.print(YGyroFinal);
    Serial.print(",");
    Serial.print(ZGyroFinal);
    Serial.print("\n");*/
    delay(1);
}
