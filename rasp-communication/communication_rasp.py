# importing libraries
import json
import serial
import time

# opening file json
with open('photo.json', 'r') as file:
    data = json.load(file)  # data è una lista di subliste

# serial initialization
ser = serial.Serial('/dev/serial0',9600) # rasp pi 0

# sending data 
for sublist in data:                    # per ogni sublista di data 
    for numbers in sublist:             # per ogni numero di sublista
        x, y = numbers                  # [x = num1 , y = num2]
        message = f'[{x},{y}]'          # messagio che invio in seriale 
        ser.write(message.encode())     # codifica automaticamente il messaggio e lo scrive in seriale 
        print(f'Inviato: {message}')    # debug
        time.sleep(0.5)                 # pausa 

    # if sublist is finished -> lift pen
    special_message = '[6666, 6666]'                # cambiare con valore riconoscibile che non sta dentro il workspace
    ser.write(special_message.encode())             # scrive in seriale 
    print(f'messaggio speciale: {special_message}') #debug

ser.close()     # close serial communication 


