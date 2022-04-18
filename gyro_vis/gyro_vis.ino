 #include <Wire.h>;

const int MPU_addr = 0x68;

void setup()
{
    Serial.begin(115200);
    //Serial.println("Press any key");
    //while(Serial.available()==0);
    Wire.begin(4,5,100000);
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
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x41);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 2, true);
    int16_t tempFull = Wire.read() << 8 | Wire.read();
    float tempFinal = (float) tempFull / 340.0 + 36.53;
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6, true);
    int16_t XGyroFull = Wire.read() << 8 | Wire.read();
    int16_t YGyroFull = Wire.read() << 8 | Wire.read();
    int16_t ZGyroFull = Wire.read() << 8 | Wire.read();
    float XGyroFinal = (float)XGyroFull/32.8;
    float YGyroFinal = (float)YGyroFull/32.8;
    float ZGyroFinal = (float)ZGyroFull/32.8;
    Serial.print(XGyroFinal);
    Serial.print(" ");
    Serial.print(YGyroFinal);
    Serial.print(" ");
    Serial.print(ZGyroFinal); 
    Serial.println("");
    delay(100);
}
