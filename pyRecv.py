import serial
import time

# ser = serial.Serial('/dev/tty.usbserial-A50285BI', 115200)
ser = serial.Serial('COM7', 57600)

picNum = 4
while(True):

  time.sleep(4)
  while(ser.in_waiting):
    print(ser.read(ser.in_waiting))

  print('okay')

  imageName = "another" + str(picNum) + ".jpg"
  f = open(imageName, 'wb')

  first = True  
  # go back to wating for a reply
  doneFlag = True
  while(doneFlag):
    if(ser.in_waiting>65 or not first):
      if(ser.in_waiting):
        print(ser.in_waiting)
        data = ser.read(ser.in_waiting)
        if(not data== b''):
          print(data)
        if(not data.find(b'done')== -1):
          doneFlag = False
          print("end of file detected")
        else:
          if(first):
            if(data.find(b'started')):
              first = False
              print("start of file")
          else:
            f.write(data)
  f.close() 
  picNum = picNum +1  