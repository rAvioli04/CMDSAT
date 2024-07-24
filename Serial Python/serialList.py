import numpy as np
import serial
import sys
from datetime import datetime
import time

#########################################################
# IMPORTANT VARIABLES TO CHANGE
#########################################################
adaloggerName = "B"   # Set to the adalogger's letter
comPort = "COM4"
accel0character = bytearray('1\n',"ascii") # Change this to whatever data[16] is set to, KEEP THE NEWLINE
accel1character = bytearray('2\n',"ascii") # Change this to whatever (data+18)[16] is set to, KEEP THE NEWLINE
#########################################################

ser = serial.Serial(comPort,115200,timeout=0.1)

# accel0FileName = f"Adalogger{adaloggerName}_{comPort}{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}0.txt"
# accel1FileName = f"Adalogger{adaloggerName}_{comPort}{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}1.txt"
rawFileName = f"raw_Adalogger{adaloggerName}_{comPort}_{datetime.now().strftime('%Y_%m_%dT%H_%M_%S')}.txt"

# accel0 = open(accel0FileName, "a")
# accel1 = open(accel1FileName, "a")
raw = open(rawFileName, "ab")

choice = "1" if len(sys.argv) <= 1 else sys.argv[1]

count = 0
while ser.is_open:
    bytes = ser.read_all()
    # line = ser.read_until(accel1character)
    # print(len(line))
    raw.write(bytes)

    if (count % 200000 == 0):
        print("adalogger " + adaloggerName + " on " + comPort + " good")

    count = count + 1

# accel0.close()
# accel1.close()
raw.close()