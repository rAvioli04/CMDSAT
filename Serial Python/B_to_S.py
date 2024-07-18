file = open("HOLA3.TXT", "rb")
cont = True
avg = 0
avg2 = 0
index = 0
index2 = 0
tot = 0
tat = 0
previous_time = 0
numset = set(())


while cont == True:
    #cont = False
    data = file.read(11)#4 bytes for the time, 2 for EACH axis and 1 for the new line
    if len(data) == 0:
        break
    print(data.hex())
    time = int(data[0] << 24) + int(data[1] << 16) + int(data[2] << 8) + int(data[3])
    temp = time - previous_time #delta time
    print(temp)

    numset.add(temp)
            
    if temp < 10000 and temp > 0:
        avg += temp
        index+=1
        tot = tot + 1


    if temp < 50000 and temp > 10000:
        avg2 += temp
        index2 += 1    
        tat = tat + 1
    
    previous_time = time;
    

    # while cont == False:
    #     x = input("hit space for next data point")
    #     if x == ' ':
    #         cont = True

file.close()
print(numset)
print(max(numset))
print(avg/index)
print(avg2/index2)
print(tot)
print(tat)