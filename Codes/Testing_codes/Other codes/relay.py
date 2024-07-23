import RPi.GPIO as GPIO
from time import sleep
from gpiozero import Button

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

b = Button(3)

#set gpio pin 18 for output
GPIO.setup(4,GPIO.OUT)

while(True):
	b.wait_for_press()
	GPIO.output(4,GPIO.HIGH)
	sleep(0.1)
	GPIO.output(4,GPIO.LOW)
		
		
