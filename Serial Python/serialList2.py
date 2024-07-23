import numpy as np
import serial
import sys
from datetime import datetime
import time

ser = serial.Serial("COM8",115200,timeout=0.1)

header0 = f"test{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}0.txt"
header1 = f"test{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}1.txt"
source = f"raw{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}1.txt"

accel0 = open(header0, "a")
accel1 = open(header1, "a")
raw = open(source, "ab")
# ser.open()

lastTi0 = 0
lastTi1 = 0

lastActualTi = time.time_ns()/1000
choice = sys.argv[1]

tim = datetime.now()
count = 0
while ser.is_open:
    line = ser.read_until(b"1\n",36)
    # print(len(line))
    if len(line) == 36:
        raw.write(line)
        actualTi = time.time_ns()/1000
        data0 = np.array([int.from_bytes(line[0:2],"big",signed=True),int.from_bytes(line[2:4],"big",signed=True),int.from_bytes(line[4:6],"big",signed=True),int.from_bytes(line[6:8],"big",signed=False),int.from_bytes(line[8:10],"big",signed=True),int.from_bytes(line[10:12],"big",signed=True),int.from_bytes(line[12:14],"big",signed=True),int.from_bytes(line[14:16],"big"),line[16]])
        data1 = np.array([int.from_bytes(line[18:20],"big",signed=True),int.from_bytes(line[20:22],"big",signed=True),int.from_bytes(line[22:24],"big",signed=True),int.from_bytes(line[24:26],"big",signed=False),int.from_bytes(line[26:28],"big",signed=True),int.from_bytes(line[28:30],"big",signed=True),int.from_bytes(line[30:32],"big",signed=True),int.from_bytes(line[32:34],"big"),line[34]])
        # print(f"{'1' if data[4] else '0'}: {data[3]-(lastTi1 if data[4] else lastTi0)}")
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
            accel0 = open(header0, "a")
            accel1 = open(header1, "a")
            raw = open(source, "ab")
            count = 0

    # if (count > 50000):
    #     break
# print(actualTi-lastActualTi)
    # lastActualTi = time.time_ns()/1000
print(count/(datetime.now()-tim).total_seconds())
accel0.close()
accel1.close()