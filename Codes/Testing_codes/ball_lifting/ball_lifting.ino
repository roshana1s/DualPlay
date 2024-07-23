#include <Servo.h>
int unsigned long time,breakbeamTime=0,servoTime=0;
Servo myservo;
int pos;

void motor(){
  delay(5000);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);             
    delay(15);                       
  }
  delay(5000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);       
    delay(15);                   
  }
}

void breakbeam(){
  if(time-breakbeamTime>=10){
    if (digitalRead(5)){
      Serial.println("IR Beam is Intact"); 
    }else{
      Serial.println("IR Beam is Broken");
      motor();
    }
  }
}

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  pinMode(5, INPUT);
}

void loop() {
  time=millis();
  breakbeam();
}