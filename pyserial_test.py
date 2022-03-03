import serial 

with serial.Serial('/dev/ttyUSB0',115200) as ser:
    line=ser.readline() 
    if len(line):
        print(line) 

