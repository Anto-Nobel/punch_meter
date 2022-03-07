#include <esp_now.h>
#include <WiFi.h> 

typedef struct readings
{
    float aX, aY, aZ, gX, gY, gZ;
} readings; 

readings right; 

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) { 
    memcpy(&right,incomingData,sizeof(right)); 
    Serial.print(right.aX); 
    Serial.print(" ");
    Serial.print(right.aY); 
    Serial.print(" ");
    Serial.print(right.aZ); 
    Serial.print(" ");
    Serial.print(right.gX); 
    Serial.print(" ");
    Serial.print(right.gY); 
    Serial.print(" ");
    Serial.print(right.gZ); 
    Serial.print("\n");
    delay(100);
}

void setup()
{
    Serial.begin(460800); 
    WiFi.mode(WIFI_STA); 
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
    
}
