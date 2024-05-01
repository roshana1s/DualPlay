// Define pin connections & motor's steps per revolution
const int dirPin = 3;
const int stepPin = 2;
const int stepsPerRevolution = 200;
int d = 400;
unsigned long time,b2time=0,b1time=0,stime=0;

void button2(){         //left
  if((time-b2time>=10)){
    b2time = time;
    if(!digitalRead(13) && digitalRead(10)){
      digitalWrite(dirPin, LOW);
      motor();
    }
  }
}

void button1(){        //right
  if((time-b1time>=10)){
    b1time = time;
    if(!digitalRead(12) && digitalRead(11)){
      digitalWrite(dirPin, HIGH);
      motor();
    }
  }
}

void motor(){
    
    for(int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(d);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(d);
    }
    
}

void solenoid(){
   if((time-stime>=10)){
    stime = time;
    if(digitalRead(8)){
      digitalWrite(9, HIGH);
    }else{
      digitalWrite(9, LOW);
    }
   }
}

void setup()
{
	// Declare pins as Outputs
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
  pinMode(9, OUTPUT); //RELAY
  pinMode(13, INPUT_PULLUP);//left move
  pinMode(12, INPUT_PULLUP);//right move
  pinMode(11, INPUT_PULLUP);//right stop
  pinMode(10, INPUT_PULLUP);//left stop
  pinMode(8, INPUT_PULLUP);//solenoid
}

void loop()
{
  time = millis();
  button1();
  button2();
  solenoid();
}