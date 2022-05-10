import serial 

with serial.Serial('/dev/ttyACM0', 115200, timeout=1) as ser: 
    while (True): 
        s = ser.readline() 
        print(s)
