// Define pin connections & motor's steps per revolution
const int dirPin = 3;
const int stepPin = 2;
const int stepsPerRevolution = 200;
int d = 400;

// void button2(){
//   if(time-bbtime>=10){
//     bbtime = time;
//     if(!digitalRead(3)){
//       digitalWrite(dirPin, LOW);
//       motor();
//     }
//   }
// }

// void button(){
//   if(time-btime>=10){
//     btime = time;
//     if(!digitalRead(2)){
//       digitalWrite(dirPin, HIGH);
//       motor();
//     }
//   }
// }

void motor(){
    
    for(int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(d);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(d);
    }
    
}

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

// void val(){
//   if(time-t>=10 io){
//     t=time;
//     if(!digitalRead(22)){
//       Serial.println("BOLE GIYAAA!!");
//     }else{Serial.println("BOLE GIYE NAA!!");}
//   }
// }

void loop()
{
  // time = millis();
  // button();
  // button2();
  // val();
  // motor();
  digitalWrite(dirPin, HIGH);
  motor();
  motor();
  motor();
  motor();
  motor();
  motor();
  delay(500);
  digitalWrite(dirPin, LOW);
  motor();
  motor();
  motor();
  motor();
  motor();
  motor();
  delay(500);

	// Set motor direction clockwise
	
	// // delay(1000); // Wait a second
  // // Set motor direction counterclockwise
	// digitalWrite(dirPin, LOW);

	// // Spin motor slowly
	// for(int x = 0; x < stepsPerRevolution; x++)
	// {
	// 	digitalWrite(stepPin, HIGH);
	// 	delayMicroseconds(d);
	// 	digitalWrite(stepPin, LOW);
	// 	delayMicroseconds(d);
	// }
}