from asyncio import exceptions
import serial
import time
import pandas as pd
import numpy as np
from os import listdir
import math


ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(10)
for _ in range(100):
    line = ser.readline()
    if len(line):
        print(line)
    time.sleep(0.001)

df = pd.DataFrame(columns=['acc_X', 'acc_Y', 'acc_Z','gyro_X', 'gyro_Y', 'gyro_Z', 'acc'])
new_row = {}
for _ in range(300*5):
    line = ser.readline()
    data = line.decode()
    data.replace("`", " ")
    datapoints = list(map(float, data.split()))
    #datapoints=[int(x) for x in datapoints]
    if len(datapoints) == 6:
        # print(datapoints)
        new_row['acc_X'] = [datapoints[0]]
        new_row['acc_Y'] = [datapoints[1]]
        new_row['acc_Z'] = [datapoints[2]]
        new_row['gyro_X'] = [datapoints[3]]
        new_row['gyro_Y'] = [datapoints[4]]
        new_row['gyro_Z'] = [datapoints[5]]
        new_row['acc'] = [math.sqrt((datapoints[0]**2)+(datapoints[1]**2)+(datapoints[2]**2))]
        df1 = pd.DataFrame(new_row) 
        #df1.to_csv('test.csv')
        df=pd.concat([df,df1],ignore_index=True)
        print(new_row)
    time.sleep(0.001)
print(df.head())
df.to_csv('jabstop.csv')
ser.close()