// MPU 6050 Accelerometer and Gyroscope Exame
// Written by Taylor Schweizer

// We will be using the I2C communication protocol to communicate witht he MPU 6050
// The I2C protocol is setup in the Wire library - we need to include this library
#include <Wire.h>;

// The default address of the MPU is 0x68
const int MPU_addr = 0x68;

void setup()
{
  // Here we will begin the I2C communication
  Serial.begin(115200);
  Serial.println("Hit a key to start"); // signal initalization done
  while (Serial.available() == 0)
    ;
  Wire.begin(4, 5, 100000);

  // Begin transmission with the MPU
  Wire.beginTransmission(MPU_addr);

  // According to the MPU 6050 Register Map, the PWR_MGMT_1 register allows us to configure the power mode of the MPU.
  // Looking at the register map page 40, PWR_MGMT_1 is located at 0x6B (107 in decimal, 01101011 in binary)
  // Think of each register in the MPU of having 8 bits that can be changed to change the settings.
  // For register 0x6B, the first three bits - Bit0 through Bit2 - are for selecting the clock source. A value of 0 sets the clock source to the internal 8MHZ oscillator, therefore the first three bits are all 0.
  // If we decided to use the PLL with external 19.2MHz reference, we would send 5 (decimal), so Bit0 would be 1, Bit1 would be 0, Bit2 would be 1, making 101 in binary which is 5 in decimal
  // Bit3 is labelled TEMP_DIS, and allows us to turn the temperature sensor on or off. A value of 1 turns if off, we might as well leave it on so Bit3 will be 0.
  // Bit4 is reserved, so we will just send 0 - the default
  // Bit5 is labelled CYCLE, which lets us specificy an automatic sleep cycle. We will set it to 0 and avoid the hassle of a sleep mode
  // Bit6 is labelled SLEEP, which lets us put it into sleep mode. Since we want the device on, we will set Bit6 to 0.
  // Bit7 is DEVICE_RESET, which resets all internal registers to their default values. We won't be doing that, so Bit7 is 0.

  // Based on the above, the 8 bits we will send to the MPU are all 0, so we can send any value of zero we want - hex 0x00, dec 0
  // This will power on the device and specify the initial power settings.
  // First, we tell the MPU which register we are writing to
  Wire.write(0x6B);

  // Second, we send the values that we discussed previously
  Wire.write(0x00);
  // Now, I'm not sure if you can continue sending data by sending a new register address and then the data, or if you need to end the transmission first. Just to be on the safe side, we will end the transmission and restart it.
  Wire.endTransmission(true);
  delay(50);
  Wire.beginTransmission(MPU_addr);
  // Now that we are done with that, let's go on and configure the accelerometer and gyroscope
  // Note: none of this is really essential: I'm basically just doing this to help you understand how to work with these register maps and stuff. This device is extremely powerful if it is used correctly, and practice makes perfect.

  // First, let's configure the Gyroscope. According to page 14 of the register map, the register holding the GYRO_CONFIG settings is located at 0x1B
  // Here is a breakdown of the bits:
  // Bit0 - Reserved, set to 0
  // Bit1 - Reserved, set to 0
  // Bit2 - Reserved, set to 0
  // Bit3 - FS_SEL
  // Bit4 - FS_SEL  -  This sets the full scale range of the gyroscope. Considering we will be running the wheel at a max of 100 RPM, that is 600°/s. Therefore we need to set FS_SEL to the +-1000°/s setting, which is decimal 2, hex 0x02, binary 10
  // Bit5 - ZG_ST - Activates self test of gyroscope Z axis if set to 1. We will set it to 0
  // Bit6 - YG_ST - same as ZG_ST but with Y axis
  // Bit7 - XG_ST - same as ZG_ST but with X axis

  // According to those values, we want to send the value 0 0 0 1 0 0 0 0, which is dec 16 hex 0x10
  // And remember, we want to send it to 0x1B
  Wire.write(0x1B);
  Wire.write(0x10);

  // End and restart transmission
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);

  // Next, let's do the accelerometer. I'll skip going through figuring out each bit, and just say I want to send 0x00 - there's almost no way we will experience more than 2g of acceleration on the wheelchair, so the full scale range can be +-2g. The address is 0x1C
  Wire.write(0x1C);
  Wire.write(0x00);

  // Once we send the values, we end the transmission because we don't have any further data to send for now
  Wire.endTransmission(true);

  // Begin a serial connection so we can read the data
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
  Serial.print("X Axis = ");
  Serial.print(XAxisFinal);
  Serial.println("g");
  Serial.print("Y Axis = ");
  Serial.print(YAxisFinal);
  Serial.println("g");
  Serial.print("Z Axis = ");
  Serial.print(ZAxisFinal);
  Serial.println("g");
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
  Serial.print("X Axis = ");
  Serial.print(XGyroFinal);
  Serial.println(" deg/s");
  Serial.print("Y Axis = ");
  Serial.print(YGyroFinal);
  Serial.println(" deg/s");
  Serial.print("Z Axis = ");
  Serial.print(ZGyroFinal);
  Serial.println(" deg/s");
  // Hell yeah!
  delay(500);
}