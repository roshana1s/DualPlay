// Define pin connections & motor's steps per revolution
const int dirPin = 3;
const int stepPin = 2;
const int stepsPerRevolution = 200;
int d = 425;
float step = 0;


void setup()
{
	// Declare pins as Outputs
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
  // pinMode(2, INPUT_PULLUP);
  // pinMode(3, INPUT_PULLUP);
 // pinMode(22,INPUT);
  Serial.begin(9600);
}

void loop()
{
  
	while(step!=1300)
  {
      digitalWrite(dirPin, HIGH);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(d);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(d);
      step++;
  }
  step=0;
  while(step!=1300){
    digitalWrite(dirPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(d);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(d);
      step++;
  }
  step=0;
}