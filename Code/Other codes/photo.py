import picamera

with picamera.PiCamera() as c:
	c.resolution= (1280,720)
	c.capture("/home/pi/Desktop/img.jpg") 
