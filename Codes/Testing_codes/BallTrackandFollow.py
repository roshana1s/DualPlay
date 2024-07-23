import cv2
import threading
import time
import numpy as np

color = [27, 129, 189]  # Color in BGR colorspace

img = cv2.imread("background.jpg")
cap = cv2.VideoCapture(0)
#cap.set(3, 640)  
#cap.set(4, 480)
center_x , center_y, position,n = 2,2,2,0

def get_limits(color):
    c = np.uint8([[color]])  # BGR values
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)
    
    hue = hsvC[0][0][0]  # Get the hue value

    # Handle red hue wrap-around
    if hue >= 165:  # Upper limit for divided red hue
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([180, 255, 255], dtype=np.uint8)
    elif hue <= 15:  # Lower limit for divided red hue
        lowerLimit = np.array([0, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)
    else:
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)

    return lowerLimit, upperLimit

lowerLimit, upperLimit = get_limits(color) #Get the range of the color that need to identify

def tracking():
    global cap,img,center_x, center_y,n
    while True:
        ret, frame = cap.read() #Get frames

        hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #Convert into HSV

        mask = cv2.inRange(hsvImage, lowerLimit, upperLimit) #Create a mask (If the color is identified - white, other locations black)

        mask_ = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)

        contours, gg = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) #Find those white signs
        
        for contour in contours:
            area = cv2.contourArea(contour) #Find the area of each contour
            if area > 100:  # Adjust the area threshold as needed
                x, y, w, h = cv2.boundingRect(contour) #Get the location as a rectangle
                #cv2.drawContours(frame, contour, -1, (0,255,0), 1)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 5) 
                center_x = x + w // 2
                center_y = y + h // 2
                print(f"Center Location: ({center_x}, {center_y})")
                img = cv2.imread("background.jpg")
                cv2.circle(img, (center_x, center_y), 50, (79, 255, 223), -1)

        cv2.imshow('frame', frame)
        cv2.imshow('sketch', img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            n=100
            cap.release()
            cv2.destroyAllWindows()
            break

def following():
    global position, center_x, n
    while True:
        if position > center_x:
            while position > center_x:
                position -= 1
                print(f"position - {position}, target - {center_x}")
                time.sleep(0.001)
                if n == 100:
                    break
        elif position < center_x:
            while position < center_x:
                position += 1
                print(f"position - {position}, target - {center_x}")
                time.sleep(0.001)
                if n == 100:
                    break
        if n == 100:
            break

def modeSelect():
    mode = input()
    if mode=='1':
        test1 = threading.Thread(target=tracking, args=())
        test2 = threading.Thread(target=following, args=())

        test1.start()
        test2.start()
modeSelect()
        


