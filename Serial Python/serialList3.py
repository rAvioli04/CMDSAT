import numpy as np
import serial
import sys
from datetime import datetime
import time

#########################################################
# IMPORTANT VARIABLES TO CHANGE
#########################################################
adaloggerName = "A"   # Set to the adalogger's letter
comPort = "COM16"
accel0character = b'0' # Change this to whatever data[16] is set to
accel1character = b'1' # Change this to whatever (data+18)[16] is set to
#########################################################

ser = serial.Serial(comPort,115200,timeout=0.1)

accel0FileName = f"Adalogger{adaloggerName}_{comPort}{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}0.txt"
accel1FileName = f"Adalogger{adaloggerName}_{comPort}{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}1.txt"
rawFileName = f"raw_Adalogger{adaloggerName}_{comPort}{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}1.txt"

accel0 = open(accel0FileName, "a")
accel1 = open(accel1FileName, "a")
raw = open(rawFileName, "ab")

choice = sys.argv[1]

count = 0
while ser.is_open:
    line = ser.read_until([accel1character, b'\n'])
    # print(len(line))
    raw.write(line)
    if len(line) == 36:
        data0 = np.array([int.from_bytes(line[0:2],"big",signed=True),int.from_bytes(line[2:4],"big",signed=True),int.from_bytes(line[4:6],"big",signed=True),int.from_bytes(line[6:8],"big",signed=False),int.from_bytes(line[8:10],"big",signed=True),int.from_bytes(line[10:12],"big",signed=True),int.from_bytes(line[12:14],"big",signed=True),int.from_bytes(line[14:16],"big"),line[16]])
        data1 = np.array([int.from_bytes(line[18:20],"big",signed=True),int.from_bytes(line[20:22],"big",signed=True),int.from_bytes(line[22:24],"big",signed=True),int.from_bytes(line[24:26],"big",signed=False),int.from_bytes(line[26:28],"big",signed=True),int.from_bytes(line[28:30],"big",signed=True),int.from_bytes(line[30:32],"big",signed=True),int.from_bytes(line[32:34],"big"),line[34]])
        if choice == '1':
            print(f"({data0[0]:>5}, {data0[1]:>5}, {data0[2]:>5}, {data0[3]:>5},{data0[4]:>5},{data0[5]:>5},{data0[6]:>5},{data0[7]:>5}, {data0[8]})", end="")
            print(f"({data1[0]:>5}, {data1[1]:>5}, {data1[2]:>5}, {data1[3]:>5},{data1[4]:>5},{data1[5]:>5},{data1[6]:>5},{data1[7]:>5}, {data1[8]})", end="\n")

        accel0.write(f"{data0[0]},{data0[1]},{data0[2]},{data0[3]},{data0[4]},{data0[5]},{data0[6]},{data0[7]},{data0[8]}" + "\n")
        accel1.write(f"{data1[0]},{data1[1]},{data1[2]},{data1[3]},{data1[4]},{data1[5]},{data1[6]},{data1[7]},{data1[8]}" + "\n")
        count = count + 1
        if (count > 2000):
            accel0.flush()
            accel1.flush()
            raw.flush()
            raw.close()
            accel0.close()
            accel1.close()
            accel0 = open(accel0FileName, "a")
            accel1 = open(accel1FileName, "a")
            raw = open(rawFileName, "ab")
            count = 0

accel0.close()
accel1.close()
raw.close()