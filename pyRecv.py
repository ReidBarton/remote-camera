import serial
import time

# ser = serial.Serial('/dev/tty.usbserial-A50285BI', 115200)
ser = serial.Serial('COM6', 57600)

f = open("moon-out4.jpg", 'wb')

# go back to wating for a reply
var = True
while(var):
  if(ser.in_waiting):
    data = ser.readline(ser.in_waiting)
    print(data)
    if(data == b'done-boi'):
      var = False
    else:
      f.write(data)
    
f.close() 