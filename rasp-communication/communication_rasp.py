import json
import serial
import time
import struct
import math

with open('/home/leonardo/2D-plotter-manipulator/Vectorization-bot/photo.json', 'r') as file:
    data = json.load(file)
ser = serial.Serial('/dev/serial0', baudrate=9600, timeout=0)
prev = [255 , 255]
prev_special = 0

n = 0
try:
    for sublist in data:
        for numbers in sublist:
            num1 = int(numbers[0])
            num2 = int(numbers[1])

            delay_num1 = 0.2
            delay_num2 = 0.4

            if math.sqrt((num1 - prev[0])**2 + (num2 - prev[1])**2) >= 4:
                prev = numbers

                message0 = struct.pack('>B', num1)
                ser.write(message0)
                print(f'Sent: {num1}')
                time.sleep(delay_num1)

                message1 = struct.pack('>B', num2)
                ser.write(message1)
                print(f'Sent: {num2}')
                time.sleep(delay_num2)
                if prev_special == 254:
                    prev_special = 1
                else: 
                    prev_special += 1

        num = 255
        if prev_special != num and prev_special != 0:
            n += 1
            special_message = struct.pack('>B', num)
            ser.write(special_message)
            time.sleep(delay_num2)
            print('special message : LIFT PEN')
            print(f'Percentage: {n * 100 / len(data)}')
            prev_special = 255
        elif prev_special == 0:
            n += 1
            special_message2 = struct.pack('>B', num-1)
            ser.write(special_message2)
            time.sleep(delay_num2)
            print(f'Percentage: {n * 100 / len(data)}')
            prev_special = 255
        else: 
            prev_special = 0
finally: 
    ser.close()
