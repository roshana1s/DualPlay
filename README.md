# DualPlay
### The Ultimate Two-Side Air Hockey Experience with an Intelligent Opponent

![dualplay_intro](https://github.com/user-attachments/assets/6c3324e9-0288-424b-be45-fd128047f998)


## Introduction 📢
These days, as technology affects every part of our lives, hardware and entertainment combine to create a world of creative possibilities. The main attempt presented by this project is the creation  of an independent physical game that Blends traditional gaming with modern technology.

## Problem in Brief 🚩
1. No human interaction in traditional computer gaming.
   
2. Issues on social dynamics and physical health.
   
3. Traditional physical games require the presence of another player.
   


## Aim 🎯
Create an interactive multiplayer table game that combines the excitement of air hockey with innovative features.


## Objectives 🎯
1. Design and build the game with two game modes including an autonomous gameplay mode.
   
3. Ensure an enjoyable experience regardless of player availability.


## Game Modes 🎮
### 1. Human vs Human
Two players control paddles using two remotes.

### 2. Human vs AI Player
Human player controls the paddle using a remote and the paddle of the AI player autonomously controlled according to the ball movement by the system.

## Intelligent Features 🧠
1. When the game mode is “Human vs AI”, AI player’s paddle controlled autonomously by tracking the ball and the AI paddle real-time.

2. When a goal is scored, it will detect autonomously, and scores will be updated according to it.

3. After a goal is scored, ball will be redeployed back to the play area autonomously without human intervention.


## Technologies Used 🖥️

### 1. Computer Vision 👁️‍🗨️
  
We implemented a real-time computer vision algorithm using OpenCV in Python to track the colors of the ball and the AI player's paddle. We used HSV color space for this. This allowed us to accurately identify and monitor their positions throughout the game. This computer vision algorithm is executed on Raspberry pi 4 board.

![image](https://github.com/user-attachments/assets/e6a92ed7-3887-4b82-a950-5380193db8e4)

### 2. Arduino Mega and Uno

Arduino Mega controls Green Player side (Human player) and Uno controls Red player side (Human/AI). Game setup LCD, Keypad, Score update LCDs, IR modules, Redeploy servo motors are controlled by Mega. Both boards programmed using C++ and Arduino IDE.

### 3. Serial Communication

We used Serial Communication to communicate between Raspberry pi and two Arduino boards. Arduino boards plugged into raspberry pi through USB ports. 

## Resources Required - Hardware ⚙️

Raspberry pi 4 model B

Arduino Mega

Arduino Uno

Nema 17 Stepper motor *2

180-degree SG90 Servo motor *2

360-degree SG90 continuous Servo motor *2

A4988 Stepper Motor Driver *2

12V 30A power supply

Obstacle Avoidance IR module *2

LCD screen with I2C module *3

Keypad

Limit switch *4

Buck converter *2

Solenoid with Relay module *2


## Resources Required - Software 👨🏽‍💻

Arduino IDE

Geany editor

Tinkercad

SolidWorks

fritzing



## Block Diagram

![image](https://github.com/user-attachments/assets/cfef04e0-22c0-432f-b0e4-662bcd458a6e)

## Our Team
1. Epasinghe S K
2. Helapalla K O R I
3. Dhananjana R A
4. Nimraka T M N
5. Randinu L L







