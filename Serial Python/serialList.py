import numpy as np
import serial
import sys

ser = serial.Serial("COM21" , 115200)
accel0 = open("test0.txt", "w")
accel1 = open("test1.txt", "w")
# ser.open()

lastTi0 = 0
lastTi1 = 0
from datetime import datetime
import time
lastActualTi = time.time_ns()/1000
choice = sys.argv[1]

tim = datetime.now()
count = 0
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
        if choice == '1':
            print(f"({data[0]:>5}, {data[1]:>5}, {data[2]:>5}, {data[3]:>5},{data[4]:>5},{data[5]:>5},{data[6]:>5},{data[7]:>5}, {data[8]})", end=("" if data[8] else '\n'))
        if data[8]:
            accel1.write(f"{data[0]},{data[1]},{data[2]},{data[3]},{data[4]},{data[5]},{data[6]},{data[7]},{data[8]}" + "\n")
            lastTi1 = data[7]
        else:
            accel0.write(f"{data[0]},{data[1]},{data[2]},{data[3]},{data[4]},{data[5]},{data[6]},{data[7]},{data[8]}" + "\n")
            lastTi0 = data[7]
        count = count + 1
    else:
        accel0.flush()
        accel1.flush()

    if (count > 10000):
        break
# print(actualTi-lastActualTi)
    # lastActualTi = time.time_ns()/1000
print(count/(datetime.now()-tim).total_seconds())
accel0.close()
accel1.close()