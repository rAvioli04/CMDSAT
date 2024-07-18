import numpy as np
import serial
ser = serial.Serial("COM20",115200)
# ser.open()
lastTi = 0
while ser.is_open:
    line = ser.readline()
    # print(len(line))
    if len(line) == 10:
        accelerometer = 0
        # print(line[10])
        if line[8] == 49:
            accelerometer = 1
        data = np.array([int.from_bytes(line[0:2],"big"),int.from_bytes(line[2:4],"big"),int.from_bytes(line[4:6],"big"),int.from_bytes(line[6:8],"big"),accelerometer])
        if data[4] == 1:
            print(data[3]-lastTi)
            lastTi = data[3]
