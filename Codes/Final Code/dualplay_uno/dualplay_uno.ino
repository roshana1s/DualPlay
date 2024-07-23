// DualPlay 
// Code for arduino Uno

// A4988 motor driver direction and step pins
const int dirPin = 3, stepPin = 2;

const int stepsPerRevolution = 70;
const int debounceDelay = 50; // Debounce delay in milliseconds

// Pins for the remote
const int leftMove = 11, rightMove = 10, solenoid = 8;

// Pins for the relay and two limit switches
const int relay = 9, leftStop = 5, rightStop = 6;

// Variables to messure time
unsigned long currentTime, solenoidTime = 0;

// delay between two steps of stepper motor
int del = 400;

// variables to catch signals from raspberry pi
String com, signal1, signal2;


// Function to identify Button press for the paddle
void handleButtons() {
  static unsigned long lastDebounceTime1 = 0;
  static unsigned long lastDebounceTime2 = 0;
  static int lastButtonState1 = HIGH;
  static int lastButtonState2 = HIGH;

  int buttonState1 = digitalRead(rightMove);
  int buttonState2 = digitalRead(leftMove);

  if (buttonState1 != lastButtonState1) {
    lastDebounceTime1 = currentTime;
  }

  if (buttonState2 != lastButtonState2) {
    lastDebounceTime2 = currentTime;
  }

  if ((currentTime - lastDebounceTime1) > debounceDelay) {
    if (buttonState1 == LOW && !digitalRead(rightStop)) {
      digitalWrite(dirPin, HIGH);
      runMotor();
    }
  }

  if ((currentTime - lastDebounceTime2) > debounceDelay) {
    if (buttonState2 == LOW && digitalRead(leftStop)) {
      digitalWrite(dirPin, LOW);
      runMotor();
    }
  }

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
}

// Function to run paddle
void runMotor() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
}

// Function to identify button input to solenoid and strike the ball
void handleSolenoid() {
  if (currentTime - solenoidTime >= 10) {
    solenoidTime = currentTime;
    if (digitalRead(solenoid) == LOW) {
      digitalWrite(relay, LOW);
    } else {
      digitalWrite(relay, HIGH);
    }
  }
}

void aiMove(String signal) {
  static unsigned long lastDebounceTime1 = 0;
  static unsigned long lastDebounceTime2 = 0;
  static int lastState1 = HIGH;
  static int lastState2 = HIGH;

  // change state according to raspberry signal (-1,0,1,2) - Like button pressing
  int state1, state2;
  if (signal == "1") {
    state1 = LOW;
    state2 = HIGH;
  } else if (signal == "0") {
    state1 = HIGH;
    state2 = LOW;
  } else if (signal == "-1" || signal == "2") {
    state1 = HIGH;
    state2 = HIGH;
  }

  if (state1 != lastState1) {
    lastDebounceTime1 = currentTime;
  }

  if (state2 != lastState2) {
    lastDebounceTime2 = currentTime;
  }

  if ((currentTime - lastDebounceTime1) > debounceDelay) {
    if (state1 == LOW && !digitalRead(rightStop)) {
      digitalWrite(dirPin, HIGH);
      runMotor();
    }
  }

  if ((currentTime - lastDebounceTime2) > debounceDelay) {
    if (state2 == LOW && digitalRead(leftStop)) {
      digitalWrite(dirPin, LOW);
      runMotor();
    }
  }

  lastState1 = state1;
  lastState2 = state2;

  // Old aiMove function (direction stucks sometimes)

  // if (currentTime - lastDebounceTime >= debounceDelay) {
  //   lastDebounceTime = currentTime;
  //   if(signal1==-1){
  //     digitalWrite(stepPin, LOW);
  //     return;
  //   }
  //   else{
  //     if ((signal1 == "0") && (currentTime - leftTime >= 10)) {
  //       leftTime = currentTime;
  //       digitalWrite(dirPin, LOW);
  //       runMotor();
  //     }else if ((signal1 == "1") && (currentTime - rightTime >= 10)) {
  //       rightTime = currentTime;
  //       digitalWrite(dirPin, HIGH);
  //       runMotor();
  //     }
  //   }
    
  //}
}


// Function for strike - AI player
void aiSolenoid(String signal1) {
    if (signal1 == "2") {
      digitalWrite(relay, LOW);
      delay(100);
      digitalWrite(relay, HIGH);
      delay(100);
    }
    else{
      digitalWrite(relay, HIGH);
    }
  
}

// Function to clear serial buffer
void ClearSerial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(relay, OUTPUT); // RELAY
  pinMode(leftMove, INPUT_PULLUP); // left move
  pinMode(rightMove, INPUT_PULLUP); // right move
  pinMode(rightStop, INPUT_PULLUP); // right stop
  pinMode(leftStop, INPUT_PULLUP); // left stop
  pinMode(solenoid, INPUT_PULLUP); // solenoid

  digitalWrite(relay, HIGH); // switch off relay at first
}

void loop() {

  // wait for the signal from raspberry pi to get the starting position of the paddle
  while (true) {
    if (Serial.available() > 0) {
      com = Serial.readStringUntil('\n');
      if (com == "-2") {
        digitalWrite(dirPin, HIGH);
        while(!digitalRead(rightStop)) runMotor();
        break;
      }else if(com == "-3"){
        digitalWrite(dirPin, LOW);
        while(digitalRead(leftStop)) runMotor();
        break;
      }
    }
  }

  // wait for the signal from raspberry pi
  while (true) {
    if (Serial.available() > 0) {
      com = Serial.readStringUntil('\n');
      if (com == "5" || com == "6") {
        break;
      }
    }
  }

  // if com = 5 means it is human vs human - ready the remote for this
  if (com == "5") {
    while (true) {
      currentTime = millis();
      handleButtons();
      handleSolenoid();
      
      if (Serial.available() > 0) {
        signal2 = Serial.readStringUntil('\n');
        if (signal2 == "10") {
          break;
        }
      }
    }

  // if com = 6 means it is Human vs System - get ready to play as the AI player
  } else if (com == "6") {
    while (true) {
      currentTime = millis();
      
      // get the signal from raspberry pi which send signals according to ball movement
      signal1 = Serial.readStringUntil('\n');
      ClearSerial();
      aiSolenoid(signal1);
      aiMove(signal1);
      if(signal1=="10"){
        break;
      }
    }
  }
}
