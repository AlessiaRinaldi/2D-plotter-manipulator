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
                print(f'Inviato: {num1}')
                time.sleep(delay_num1)

                message1 = struct.pack('>B', num2)
                ser.write(message1)
                print(f'Inviato: {num2}')
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
            print(f'special message : {special_message}')
            print(f'Percentage: {n * 100 / len(data)}')
            prev_special = 255
        elif prev_special == 0:
            n += 1
            special_message2 = struct.pack('>B', num-1)
            ser.write(special_message2)
            time.sleep(delay_num2)
            print(f'special message : {special_message2}')
            print(f'Percentage: {n * 100 / len(data)}')
            prev_special = 255
        else: 
            prev_special = 0
finally: 
    ser.close()

# import json
# import serial

# import time
# import struct

# # Apertura del file JSON
# with open('~/2D-plotter-manipulator/Vectorization-bot/photo.json', 'r') as file:
#     data = json.load(file)  # data è una lista di subliste

# # Inizializzazione della porta seriale
# ser = serial.Serial('/dev/serial0', baudrate=9600, timeout=0)  # rasp pi 0
# prev = [255 , 255]
# try:

#     n_vectors = len(data)

#     num3, remainder = divmod(n_vectors, 10000)
#     num2, remainder = divmod(remainder, 100)
#     num1 = remainder

#     msg = struct.pack('>B', num1)
#     ser.write(msg)
#     time.sleep(0.05)
#     msg = struct.pack('>B', num2)
#     ser.write(msg)
#     time.sleep(0.05)
#     msg = struct.pack('>B', num3)
#     ser.write(msg)
#     time.sleep(0.05)

#     for sublist in data:  # per ogni sublista di data
#         for numbers in sublist:  # per ogni lista di numeri in sublista
#             # Converte i numeri nella lista in interi, se necessario
#             num1 = int(numbers[0])
#             num2 = int(numbers[1])
#             # Controllo
#             if num1 == prev[0] and num2 == prev[1]:
#                 continue
#             prev = numbers
#             # Definisci i ritardi per num1 e num2
#             delay_num1 = 0.05
#             delay_num2 = 0.5

#             message0 = struct.pack('>B', num1)
#             ser.write(message0)
#             print(f'Inviato: {num1}')
#             time.sleep(delay_num1)

#             message1 = struct.pack('>B', num2)
#             ser.write(message1)
#             print(f'Inviato: {num2}')
#             time.sleep(delay_num2)
#         # if sublist is finished -> lift pen
#         num = 255                                     # cambiare con valore riconoscibile che non sta dentro il workspace
#         special_message = struct.pack('>B', num)      # 1 byte 
#         ser.write(special_message)                    # scrive in seriale 
#         print(f'special message: {special_message}')  # debug
# finally:
#     # Chiude la connessione seriale alla fine
#     ser.close()