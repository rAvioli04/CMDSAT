import numpy as np
import serial
ser = serial.Serial("COM21",115200)
# ser.open()
lastTi0 = 0
lastTi1 = 0
from datetime import datetime
import time
lastActualTi = time.time_ns()/1000
while ser.is_open:
    line = ser.readline()
    # print(len(line))
    if len(line) == 18:
        actualTi = time.time_ns()/1000
        accelerometer = 0
        # print(line[10])
        if line[16] == 49:
            accelerometer = 1
        data = np.array([int.from_bytes(line[0:2],"big",signed=True),int.from_bytes(line[2:4],"big",signed=True),int.from_bytes(line[4:6],"big",signed=True),int.from_bytes(line[6:8],"big",signed=False),int.from_bytes(line[8:10],"big",signed=True),int.from_bytes(line[10:12],"big",signed=True),int.from_bytes(line[12:14],"big",signed=True),int.from_bytes(line[14:16],"big"),accelerometer])
        # print(f"{'1' if data[4] else '0'}: {data[3]-(lastTi1 if data[4] else lastTi0)}")
        print(f"({data[0]:>5}, {data[1]:>5}, {data[2]:>5}, {data[3]:>5},{data[4]:>5},{data[5]:>5},{data[6]:>5},{data[7]:>5}, {data[8]})", end=("" if data[8] else '\n'))
        if data[4]:
            lastTi1 = data[3]
        else:
            lastTi0 = data[3]
        # print(actualTi-lastActualTi)
        lastActualTi = time.time_ns()/1000