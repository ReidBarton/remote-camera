import serial
import time

ser = serial.Serial('/dev/tty.usbserial-A50285BI', 115200)

time.sleep(2)
# begin photo send & wait for both radios to be in correct mode
ser.write(b'send-it!')
time.sleep(3)
# first msg never sends, needs fixing, send dummy msg
ser.write(b' ') 
time.sleep(3)

# message we want to send, or photo
ser.write(b'hello\n')
ser.write(b'this ')
ser.write(b'is ')
ser.write(b'a ')
ser.write(b'test ')
time.sleep(0.001)
ser.write(b'of ')
ser.write(b'the ')
ser.write(b'GEORGIA TECH ')
time.sleep(0.001)
ser.write(b'EMERGENCY NOTIFICATION SYSTEM \n')
ser.write(b'k thx bye \n')

# make sure the serial buffer is clear before we send a command to stop transmitting
time.sleep(3)
ser.write(b'done-boi')

# go back to wating for a reply
while(True):
  if(ser.in_waiting):
    data = ser.read(ser.in_waiting)
    print(data)
