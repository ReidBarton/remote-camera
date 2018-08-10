import serial
import time
import os

start_time = time.time()

ser = serial.Serial('/dev/tty.usbserial-A50285BI', 57600)

time.sleep(2)


# clean garbage from previous run
while(ser.in_waiting):
  ser.readline(ser.in_waiting)


# # begin photo send & wait for both radios to be in correct mode
print("----- put into tx mode ----")
ser.write(b'send-it!00send-it!00send-it!00send-it!00send-it!00')
time.sleep(4)
print("conf:")


# clean garbage from previous run
while(ser.in_waiting):
  print(ser.readline(ser.in_waiting))


# print("sending image data...")

# progress = 0
# imageChunck =50
# image = "moon.jpg"
# incoming = 0

# filesize = os.path.getsize(image)

# f = open(image, 'rb')

# data = f.read(imageChunck)
# # print(data)
# while(data != b''):
#   print(data)
#   datalen = len(data)
#   while(50-len(data)>0):
#     data = data + b'0'

#   ser.write(data)

#   while(incoming<52): # <54
#     time.sleep(0.0001) # .025
#     # print(ser.in_waiting)
#     # print('wait')
#     if(ser.in_waiting):
#       incoming = incoming + ser.in_waiting
#       read = ser.read(ser.in_waiting)
#       print(read)
#   incoming =0

#   data = f.read(imageChunck)
#   progress+=imageChunck
#   print(str(100*progress/filesize) + ' %')
#   # time.sleep(0.01)

# f.close()



incoming = 0
j = 20
while(j>0):
  ser.write(b'43210432104321043210432104321043210432104321043210')
  print(j)
  while(incoming<52): # 66 with timing
    if(ser.in_waiting):
      incoming = incoming + ser.in_waiting
      data = ser.read(ser.in_waiting)
      print(data)
  incoming = 0
  j=j-1

# message we want to send, or photo
# ser.write(b'hello\n')
# time.sleep(0.5)
# ser.write(b'this ')
# time.sleep(0.5)
# ser.write(b'is ')
# time.sleep(0.5)
# ser.write(b'a ')
# time.sleep(0.5)
# ser.write(b'test ')
# time.sleep(0.5)
# ser.write(b'of ')
# ser.write(b'the ')
# time.sleep(0.5)
# ser.write(b'GEORGIA TECH ')
# time.sleep(0.5)
# ser.write(b'EMERGENCY NOTIFICATION')
# time.sleep(0.5)
# ser.write(b' SYSTEM \n')
# time.sleep(0.5)
# ser.write(b'k thx bye \n')
# time.sleep(0.5)

print("done")
# make sure the serial buffer is clear before we send a command to stop transmitting
time.sleep(3)
ser.write(b'done-boi00done-boi00done-boi00done-boi00done-boi00')
print("should be done now")

print("--- %s seconds ---" % (time.time() - start_time))

time.sleep(1)
# for debug
# go back to wating for a reply
while(ser.in_waiting):
  if(ser.in_waiting):
    data = ser.read(ser.in_waiting)
    print(data)

