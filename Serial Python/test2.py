import sys

from matplotlib import pyplot as plt
import numpy as np
#########################################################
# OLD DO NOT USE!!!!!!!!!!!
#########################################################
filename = "raw2024_07_23T16_23_401.txt"
accelCharacter = '4' # CHANGE THIS TO LOOK AT THE OTHER ACCELEROMETER

file = open(filename,'rb')
lines = file.readlines()
# print(lines)
properlines = [line for line in lines if (len(line) == 18 and line[-2] == bytearray(accelCharacter,"ascii")[0])]
accelx = np.array([int.from_bytes(line[0:2],"big",signed=True) for line in properlines])
accely = np.array([int.from_bytes(line[2:4],"big",signed=True) for line in properlines])
accelz = np.array([int.from_bytes(line[4:6],"big",signed=True) for line in properlines])
temp = np.array([int.from_bytes(line[6:8],"big",signed=False) for line in properlines])
gyrox = np.array([int.from_bytes(line[8:10],"big",signed=True) for line in properlines])
gyroy = np.array([int.from_bytes(line[10:12],"big",signed=True) for line in properlines])
gyroz = np.array([int.from_bytes(line[12:14],"big",signed=True) for line in properlines])
micros = np.array([int.from_bytes(line[14:16],"big",signed=False) for line in properlines])
offset = 0x10000
count = 0
for i in range(len(micros) - 1):
    delta = micros[i+1] - micros[i]
    micros[i] = micros[i] + offset * count
    if delta < -50000:
        count = count + 1
    if i >1:
        print(micros[i] - micros[i -1 ])

bytesToGs = 8/0x7FFF # +-8 gs per half the range of bytes
bytesToAngularFreq = 2000/0x7FFF # +-2000 rad/s per half the range of bytes
time = np.arange(len(micros))
plt.close()
plt.figure()
plt.subplot(231)
plt.scatter(time,accelx*bytesToGs)
plt.title("accelx")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(232)
plt.scatter(time,accely*bytesToGs)
plt.title("accely")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(233)
plt.scatter(time,accelz*bytesToGs)
plt.title("accelz")
plt.xlabel("Time [s]")
plt.ylabel("Acceleration [g]")
plt.subplot(234)
plt.scatter(time,gyrox*bytesToAngularFreq)
plt.title("gyrox")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.subplot(235)
plt.scatter(time,gyroy*bytesToAngularFreq)
plt.title("gyroy")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.subplot(236)
plt.scatter(time,gyroz*bytesToAngularFreq)
plt.title("gyroz")
plt.xlabel("Time [s]")
plt.ylabel("Angular Velocity [rad/s]")
plt.show()