import serial
ser = serial.Serial('/dev/tty.usbserial-A50285BI', 57600)
ser.write(b'hello')

while(True):
  if(ser.in_waiting):
    data = ser.read(ser.in_waiting)
    print(data)
