const int dirPin = 3;
const int stepPin = 2;
const int stepsPerRevolution = 200;
const int debounceDelay = 20; // Debounce delay in milliseconds
const int relay = 9, leftMove = 10, rightMove = 11, rightStop = 22, leftStop = 24, solenoid = 8;
unsigned long currentTime, b2Time = 0, b1Time = 0, solenoidTime = 0;
int del = 400;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(relay, OUTPUT); // RELAY
  pinMode(leftMove, INPUT_PULLUP); // left move
  pinMode(rightMove, INPUT_PULLUP); // right move
  pinMode(rightStop, INPUT_PULLUP); // right stop
  pinMode(leftStop, INPUT_PULLUP); // left stop
  pinMode(solenoid, INPUT_PULLUP); // solenoid
}

void loop()
{
  currentTime = millis();
  handleButtons();
  handleSolenoid();
}

void handleButtons()
{
  static unsigned long lastDebounceTime = 0;
  static int lastButtonState1 = HIGH;
  static int lastButtonState2 = HIGH;

  int buttonState1 = digitalRead(rightMove);
  int buttonState2 = digitalRead(leftMove);

  if (buttonState1 != lastButtonState1) {
    lastDebounceTime = currentTime;
  }

  if (buttonState2 != lastButtonState2) {
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) > debounceDelay) {
    if (buttonState1 == LOW && digitalRead(rightStop)) {
      digitalWrite(dirPin, HIGH);
      runMotor();
    }
    if (buttonState2 == LOW && digitalRead(leftStop)) {
      digitalWrite(dirPin, LOW);
      runMotor();
    }
  }

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
}

void runMotor()
{
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
}

void handleSolenoid()
{
  if (currentTime - solenoidTime >= 10) {
    solenoidTime = currentTime;
    if (digitalRead(solenoid) == HIGH) {
      digitalWrite(relay, HIGH);
    } else {
      digitalWrite(relay, LOW);
    }
  }
}
