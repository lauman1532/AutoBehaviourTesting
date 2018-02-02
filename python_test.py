import serial
import time

serialPort = "COM5"
baudRate = 9600
ser0 = serial.Serial(serialPort, baudRate)

time.sleep(1)

while (ser0.isOpen()):
	if (ser0.inWaiting() > 0):
		print(ser0.readline().decode('ascii'))

ser0.close