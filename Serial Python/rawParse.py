import sys

from matplotlib import pyplot as plt
import numpy as np

file = open("raw2024_07_23T16_23_401.txt",'rb')

lines = file.readlines()
# print(lines)
properlines = [line for line in lines if (len(line) == 18 and line[-2] == 52)]
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
    # if i >1:
        # print(micros[i] - micros[i -1 ])
plt.close()
plt.figure()
limit = 10000
plt.scatter(micros[:],gyroy[:])
plt.show()