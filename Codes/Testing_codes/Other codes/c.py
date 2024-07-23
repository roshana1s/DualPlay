from picamera import PiCamera
from time import sleep


c = PiCamera()
c._check_camera_open()

c.start_preview()
sleep(20)
c.stop_preview()
