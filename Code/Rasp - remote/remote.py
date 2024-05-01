import RPi.GPIO as GPIO
import time
from RpiMotorLib import RpiMotorLib

left_time = 0
right_time = 0
sd_time = 0

rs = 4
ls = 27
right = 18
left = 22

dirPin = 24
stepPin = 23

solenoid_relay = 16
solenoid_btn = 25

GPIO.setmode(GPIO.BCM)
GPIO.setup(rs, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(ls, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(right, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(left, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(solenoid_btn, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(solenoid_relay, GPIO.OUT)

motor = RpiMotorLib.A4988Nema(dirPin, stepPin, (21, 21, 21), "DRV8825")

def left_button():
    global left_time  # Declare left_time as global
    if (time.time() * 1000 - left_time >= 10):
        left_time = time.time() * 1000
        if (GPIO.input(left) == GPIO.LOW and GPIO.input(rs) == GPIO.HIGH):
            motor.motor_go(True, "Full", 200, 0.0004, False, 0)

def right_button():
    global right_time  # Declare right_time as global
    if (time.time() * 1000 - right_time >= 10):
        right_time = time.time() * 1000
        if (GPIO.input(right) == GPIO.LOW and GPIO.input(ls) == GPIO.HIGH):
            motor.motor_go(False, "Full", 200, 0.0004, False, 0)

def solenoid():
	global sd_time
	if (time.time() * 1000 - sd_time >= 10):
		sd_time = time.time() * 1000
		if (GPIO.input(solenoid_btn) == GPIO.LOW):
			GPIO.output(solenoid_relay,GPIO.HIGH)
		else:
			GPIO.output(solenoid_relay,GPIO.LOW)
			

while True:
    left_button()
    right_button()
    solenoid()
    
