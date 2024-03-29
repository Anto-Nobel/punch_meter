#include <esp_now.h>
#include <WiFi.h>

unsigned long timer = 0;
long loopTime = 5000;

typedef struct readings
{
    float aX, aY, aZ, gX, gY, gZ;
} readings;

readings right;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&right, incomingData, sizeof(right));
    /*Serial.print(right.aX);
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
    Serial.println(" ");*/
    sendToPC(&right.aX, &right.aY, &right.aZ, &right.gX, &right.gY, &right.gZ);
    // delay(100);
}

void sendToPC(float *aX, float *aY, float *aZ, float *gX, float *gY, float *gZ)
{
    byte *baX = (byte *)(aX);
    byte *baY = (byte *)(aY);
    byte *baZ = (byte *)(aZ);
    byte *bgX = (byte *)(gX);
    byte *bgY = (byte *)(gY);
    byte *bgZ = (byte *)(gZ);
    byte buf[24] = {baX[0], baX[1], baX[2], baX[3], baY[0], baY[1], baY[2], baY[3], baZ[0], baZ[1], baZ[2], baZ[3], bgX[0], bgX[1], bgX[2], bgX[3], bgY[0], bgY[1], bgY[2], bgY[3], bgZ[0], bgZ[1], bgZ[2], bgZ[3]};
    Serial.write(buf, 24);
}

void timeSync(unsigned long deltaT)
{
    unsigned long currTime = deltaT - (currTime - timer);
    long timeToDelay = deltaT - (currTime - timer);
    if (timeToDelay > 5000)
    {
        delay(timeToDelay / 1000);
        delayMicroseconds(timeToDelay % 1000);
    }
    else if (timeToDelay > 0)
    {
        delayMicroseconds(timeToDelay);
    }
    else
    {
        // timeToDelay is negative so we start immediately
    }
    timer = currTime + timeToDelay;
}

void setup()
{
    Serial.begin(57600);
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);
}

// https://thepoorengineer.com/en/arduino-python-plot/#multiple

void loop()
{
    //timeSync(loopTime);
}
