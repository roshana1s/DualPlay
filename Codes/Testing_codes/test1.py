import cv2
import time
import numpy as np

color = [27, 129, 189]  # Color in BGR colorspace

img = cv2.imread("background.jpg")
cap = cv2.VideoCapture(0)
center_x, center_y, position, n = 2, 2, 2, 0

def get_limits(color):
    c = np.uint8([[color]])  # BGR values
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)
    
    hue = hsvC[0][0][0]  # Get the hue value

    if hue >= 165:
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([180, 255, 255], dtype=np.uint8)
    elif hue <= 15:
        lowerLimit = np.array([0, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)
    else:
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)

    return lowerLimit, upperLimit

while True:
    ret, frame = cap.read()

    hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lowerLimit, upperLimit = get_limits(color)
    mask = cv2.inRange(hsvImage, lowerLimit, upperLimit)

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for contour in contours:
        area = cv2.contourArea(contour)
        if area > 100:
            x, y, w, h = cv2.boundingRect(contour)
            center_x = x + w // 2
            center_y = y + h // 2
            print(f"Center Location: ({center_x}, {center_y})")
            cv2.circle(img, (center_x, center_y), 50, (79, 255, 223), -1)

    cv2.imshow('frame', frame)
    cv2.imshow('sketch', img)

    # Adjust paddle position
    if position > center_x:
        position -= 1
        print(f"Move left - position: {position}, target: {center_x}")
    elif position < center_x:
        position += 1
        print(f"Move right - position: {position}, target: {center_x}")

    time.sleep(0.1)  # Adjust the sleep time based on your preference

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
