import sys

from matplotlib import pyplot as plt
import numpy as np
#########################################################
# IMPORTANT VARIABLES TO CHANGE FOR TESTING
#########################################################
filename = "raw_AdaloggerB_COM4_2024_07_24T08_09_59.txt"
accelCharacter = '2' # CHANGE THIS TO LOOK AT THE OTHER ACCELEROMETER

file = open(filename,'rb')
lines = file.readlines()
# print(lines)
properlines = [line for line in lines if (len(line) == 20 and line[-2] == bytearray(accelCharacter,"ascii")[0])]
accelx = np.array([int.from_bytes(line[0:2],"big",signed=True) for line in properlines])
accely = np.array([int.from_bytes(line[2:4],"big",signed=True) for line in properlines])
accelz = np.array([int.from_bytes(line[4:6],"big",signed=True) for line in properlines])
temp = np.array([int.from_bytes(line[6:8],"big",signed=False) for line in properlines])
gyrox = np.array([int.from_bytes(line[8:10],"big",signed=True) for line in properlines])
gyroy = np.array([int.from_bytes(line[10:12],"big",signed=True) for line in properlines])
gyroz = np.array([int.from_bytes(line[12:14],"big",signed=True) for line in properlines])
micros = np.array([int.from_bytes(line[14:18],"big",signed=False) for line in properlines])
offset = 0x10000
count = 0
# for i in range(len(micros) - 1):
#     delta = micros[i+1] - micros[i]
#     micros[i] = micros[i] + offset * count
#     if delta < -50000:
#         count = count + 1
    # if i >1:
        # print(micros[i] - micros[i -1 ])

bytesToGs = 8/0x7FFF # +-8 gs per half the range of bytes
bytesToAngularFreq = 2000/0x7FFF # +-2000 rad/s per half the range of bytes
plt.close()
plt.figure()
plt.subplot(231)
plt.scatter(micros*1e-6,accelx*bytesToGs)
plt.title("accelx")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(232)
plt.scatter(micros*1e-6,accely*bytesToGs)
plt.title("accely")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(233)
plt.scatter(micros*1e-6,accelz*bytesToGs)
plt.title("accelz")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(234)
plt.scatter(micros*1e-6,gyrox*bytesToAngularFreq)
plt.title("gyrox")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.subplot(235)
plt.scatter(micros*1e-6,gyroy*bytesToAngularFreq)
plt.title("gyroy")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.subplot(236)
plt.scatter(micros*1e-6,gyroz*bytesToAngularFreq)
plt.title("gyroz")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.show()