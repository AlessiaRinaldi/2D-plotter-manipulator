import json
import serial
import time
import struct

# Apertura del file JSON
with open('~/2D-plotter-manipulator/Vectorization-bot/photo.json', 'r') as file:
    data = json.load(file)  # data è una lista di subliste

# Inizializzazione della porta seriale
ser = serial.Serial('/dev/serial0', baudrate=9600, timeout=0)  # rasp pi 0
try:
    for sublist in data:  # per ogni sublista di data
        for numbers in sublist:  # per ogni lista di numeri in sublista
            # Converte i numeri nella lista in interi, se necessario
            num1 = int(numbers[0])
            num2 = int(numbers[1])
            # Definisci i ritardi per num1 e num2
            delay_num1 = 0.5
            delay_num2 = 1.0

            if  num1 == 0:
                num1 = 1
            message0 = struct.pack('>B', num1)
            ser.write(message0)
            print(f'Inviato: {num1}')
            time.sleep(delay_num1)

            if num2 == 0:
                num2 = 1
            message1 = struct.pack('>B', num2)
            ser.write(message1)
            print(f'Inviato: {num2}')
            time.sleep(delay_num2)
        # if sublist is finished -> lift pen
        num = 0                                       # cambiare con valore riconoscibile che non sta dentro il workspace
        special_message = struct.pack('>B', num)      # 1 byte 
        ser.write(special_message)                    # scrive in seriale 
        print(f'special message: {special_message}')  # debug
finally:
    # Chiude la connessione seriale alla fine
    ser.close()
