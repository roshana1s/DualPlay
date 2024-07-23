// DualPlay 

// Code for arduino Mega

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Obstacle Avoidance Sensor IR Infrared Module pins 
const int ir_red = 5, ir_green = 6;

// initial values and declare variables
int d, Nrounds, round_input, gm, com, gp, score, redScore = 0, greenScore = 0;

// A4988 motor driver direction and step pins
const int dirPin = 46, stepPin = 3;

const int stepsPerRevolution = 70;
const int debounceDelay = 50; // Debounce delay in milliseconds

// Pins for the remote
const int leftMove = 10, rightMove = 11, solenoid = 8;

// Pins for the relay and two limit switches
const int relay = 9, leftStop = 22, rightStop = 24;

// Pin for the active buzzer
const int buzzer = 2;

// Variables to messure time
unsigned long currentTime, b2Time = 0, b1Time = 0, solenoidTime = 0;

// delay between two steps of stepper motor
int del = 400, ir_value;

// 2D array for the 4x3keypad matrix
char keys[4][3] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};

// define row pins and column pins of the keypad
byte rowPins[4] = {28,30,32,34}; 
byte colPins[3] = {38,40,44}; 

//46 
// map keys
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 3 );


// define three lcd screens
LiquidCrystal_I2C lcd_gameSetup(0x27, 16, 2);
LiquidCrystal_I2C lcd_green(0x25 ,16, 2);
LiquidCrystal_I2C lcd_red(0x26, 16, 2);

// define four servo motors
Servo lift_green;
Servo push_green;
Servo lift_red;
Servo push_red;

/////////////////////////////////////////////////////////////////

// Function for Active buzzer
void sound_buzzer(){
    digitalWrite(buzzer, HIGH);
    delay(300);           
    digitalWrite(buzzer, LOW);
}

// welcome message
void wc_msg(){
    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(3, 0);
    lcd_gameSetup.print("Welcome to ");
    lcd_gameSetup.setCursor(4, 1);
    lcd_gameSetup.print("dualPlay"); 
}

// Select Play or Not
int gameplay(){
  lcd_gameSetup.clear();
  lcd_gameSetup.setCursor(0, 0);
  lcd_gameSetup.print("1)start");
  lcd_gameSetup.setCursor(0, 1);
  lcd_gameSetup.print("2)switch off");
  char key;
  while(1){
    key = kpd.getKey();
    if(key=='1' || key=='2'){
         sound_buzzer();
         break;
    }
  }
  return key-'0';
}

// Select the game mode
int game_mode(){
    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(2, 0);
    lcd_gameSetup.print("Select the");
    lcd_gameSetup.setCursor(3, 1);
    lcd_gameSetup.print("Game Mode");
    delay(1000);
    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(0, 0);
    lcd_gameSetup.print("1.Human vs Human");
    lcd_gameSetup.setCursor(0, 1);
    lcd_gameSetup.print("2.Human vs AI");
    char key;
    while(1){
      key = kpd.getKey();
      if(key=='1' || key=='2') {
        sound_buzzer();
        break;
      }
    }
    lcd_gameSetup.clear();
    if(key == '1'){
      lcd_gameSetup.setCursor(1, 0);
      lcd_gameSetup.print("You Selected");
      lcd_gameSetup.setCursor(1, 1);
      lcd_gameSetup.print("Human vs Human");
  }
  else if(key == '2'){
    lcd_gameSetup.setCursor(1, 0);
    lcd_gameSetup.print("You Selected");
    lcd_gameSetup.setCursor(1, 1);
    lcd_gameSetup.print("Human vs AI");
  }
  delay(1000);
  return key-'0';
}

// Select Number of Rounds
int rounds(){
  lcd_gameSetup.clear();
  lcd_gameSetup.setCursor(0, 0);
  lcd_gameSetup.print("{No of rounds}#");
  lcd_gameSetup.setCursor(0, 1);
  lcd_gameSetup.print("Enter(max 99): ");
  lcd_gameSetup.setCursor(14, 1);

  String number = "";
  while(1){
    char key;
    key =  kpd.getKey();
    if(isdigit(key) && number.length()<2){
      sound_buzzer();
      if(key == '0' && number.length()==0){
        continue;
      }else{
        lcd_gameSetup.print(key);
        number += String(key);
      }
    }
    else if(key=='#' && number.length()>=1){
      sound_buzzer();
      lcd_gameSetup.clear();
      lcd_gameSetup.setCursor(0, 0);
      lcd_gameSetup.print("No of rounds:");
      lcd_gameSetup.setCursor(13, 0);
      lcd_gameSetup.print(number);
      lcd_gameSetup.setCursor(0, 1);
      lcd_gameSetup.print("Game is ongoing");
      return number.toInt();
    }
    else if(key=='*' && number.length()>=1){
      sound_buzzer();
      number = number.substring(0, number.length() - 1);
      lcd_gameSetup.clear();
      lcd_gameSetup.setCursor(0, 0);
      lcd_gameSetup.print("{No of rounds}#");
      lcd_gameSetup.setCursor(0, 1);
      lcd_gameSetup.print("Enter(max 99): ");
      lcd_gameSetup.setCursor(14, 1);
      lcd_gameSetup.print(number);
    }
  }
}

// game setup function
void gameSetup(){
  gp = gameplay();
  if(gp==1){
    wc_msg();
    delay(1000);
    gm = game_mode();
    Nrounds = rounds();
    com = gm;
  }
  else{
    com = 0;
    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(1, 0);
    lcd_gameSetup.print("Switching Off!!!!");
    delay(4000);

    lcd_gameSetup.clear();
    lcd_green.clear();
    lcd_red.clear();

    lcd_gameSetup.setCursor(1, 0);
    lcd_gameSetup.print("GoodBye !!");
    lcd_green.setCursor(1, 0);
    lcd_green.print("GoodBye !!");
    lcd_red.setCursor(1, 0);
    lcd_red.print("GoodBye !!");
  }
}

// Function to clear serial buffer
void ClearSerial(){
  while (Serial.available() > 0) {
    Serial.read();
  }
}

// Function to check whther ir beam is intact or not
int breakbeam() { 
  // Small delay to avoid flooding the Serial Monitor
    if (!digitalRead(ir_red)) {
        // Serial.println("IR Beam is Broken - red");
        return 1;
    }else if (!digitalRead(ir_green)) {
        // Serial.println("IR Beam is Broken - Green");
        return 2;
   }else {
        // Serial.println("IR Beam is Intact");
        return 0;
    }
}

// Function to update score
void scoreDisplay(int red, int green){
    lcd_green.setCursor(0, 1);
    lcd_green.print("red: ");
    lcd_green.print(red);
    lcd_green.print(" Green: ");
    lcd_green.print(green);

    lcd_red.setCursor(0, 1);
    lcd_red.print("red: ");
    lcd_red.print(red);
    lcd_red.print(" Green: ");
    lcd_red.print(green);
}

void score_update(int score) {
    if (score == 1) {
        greenScore += 1;
    } else if (score == 2) {
        redScore += 1;
    }
    scoreDisplay(redScore, greenScore);

}

// Function to identify Button press for the paddle
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

// Function to run paddle
void runMotor()
{
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
}

// Function to identify button input to solenoid and strike the ball
void handleSolenoid()
{
  if (currentTime - solenoidTime >= 10) {
    solenoidTime = currentTime;
    if (digitalRead(solenoid) == LOW) {
      digitalWrite(relay, LOW);
    } else {
      digitalWrite(relay, HIGH);
    }
  }
}

// Function for the redeploy mechanism in the green side
void greenDeploy() {
  for (int pos = 90; pos <= 180; pos += 2) {
    push_green.write(pos);
    delay(5);  // Wait for 5 milliseconds for the servo to move
  }
  
  lift_green.write(75);  // Rotate counterclockwise
  delay(1600);     // Rotate for 2 seconds
  
  lift_green.write(90);  // Stop the servo
  delay(500);     // Wait for 1 second
  
  // Move the push_green servo back to 10 degrees
  push_green.write(90);
  delay(500);
  lift_green.write(110); // Rotate clockwise
  delay(1200);     // Rotate for 2 seconds
  
  lift_green.write(90);  // Stop the servo
}

// Function for the redeply mechanism in the red side
void redDeploy(){
  for (int pos = 85; pos <= 180; pos = pos + 1) {
    push_red.write(pos); // Sets 'push_red' servo to current 'pos' position
    delay(5);        // Waits for 5 milliseconds
  }

  lift_red.write(80);  // Rotate counterclockwise at full speed
  delay(1500);       // Rotate for 2 seconds
  lift_red.write(90); // Stop the servo
  push_red.write(85);
  delay(1000);
  lift_red.write(98); // Rotate clockwise at full speed
  delay(900);        

  lift_red.write(90);
}

// Function for run redeploy
void redeploy(int value){
  if(value==1){
    redDeploy();
  }
  else if(value==2){
    greenDeploy();
  }
}

// Function to get starting positions
void startPaddle(int value){
  if(value==1){
    digitalWrite(dirPin, LOW);
    while(digitalRead(leftStop)) runMotor();
    Serial.println("left");
  }else if(value==2){
    digitalWrite(dirPin, HIGH);
    while(digitalRead(rightStop)) runMotor();
    Serial.println("right");
  }
}

///////////////////////////////////////////////////////////////////

void setup()
{
	Serial.begin(115200);
  while(!Serial);

  pinMode(ir_red, INPUT);
  pinMode(ir_green, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(leftMove, INPUT_PULLUP); 
  pinMode(rightMove, INPUT_PULLUP); 
  pinMode(rightStop, INPUT_PULLUP); 
  pinMode(leftStop, INPUT_PULLUP); 
  pinMode(solenoid, INPUT_PULLUP); 
  pinMode(buzzer, OUTPUT);

  digitalWrite(relay, HIGH); // switch off relay at first

  lift_green.attach(14);  // Attaches the servo object 'lift_green' to pin 10
  push_green.attach(15);   // Attaches the servo object 'push_green' to pin 9
  push_green.write(10);   // Initialize push_green servo to starting position
  lift_green.write(90);

  lift_red.attach(50);  // Attaches the servo object 'lift_red' to pin 10
  push_red.attach(52);   // Attaches the servo object 'push_red' to pin 9
  lift_red.write(90);

  lcd_gameSetup.begin();
	lcd_gameSetup.backlight();
  lcd_green.begin();
	lcd_green.backlight();
  lcd_red.begin();
	lcd_red.backlight();

  lcd_green.setCursor(4,0);
  lcd_green.print("DualPlay");
  lcd_red.setCursor(4,0);
  lcd_red.print("DualPlay");

  // wait until for the signal from raspberry pi (to identify the correct port)
  String s;
  while(!(Serial.available()>0)){
    if(Serial.available()>0){
      s = Serial.readStringUntil('\n');
      if(s=='port'){
        break;
      }
    }
    lcd_gameSetup.setCursor(0,0);
    lcd_gameSetup.print("Loading....");
  }

  ClearSerial();

  // send a signal to raspberry pi to get and distiguish the correct port of mega and uno
  delay(500);
  Serial.println("1");
}

void loop()
{

  lcd_green.setCursor(4,0);
  lcd_green.print("DualPlay");
  lcd_red.setCursor(4,0);
  lcd_red.print("DualPlay");

	// Wait until for the signal from the Raspberry pi
  String start;
  while (true){
    if(Serial.available() > 0){
        start = Serial.readStringUntil('\n');
        if(start=="start"){
          break;
        }
    }
  }
  ClearSerial();
  gameSetup();
  delay(1000);

  Serial.println(com);
  delay(1000);

  if(com==0){
    // stop executing code if they selected switch off
    while(true);
  }

  lcd_green.clear();
  lcd_red.clear();

  // check the ball is in a hole or not to start the game
  while(true){
    ir_value = breakbeam();
    if(ir_value){
      break;
    }
    lcd_green.setCursor(0,0);
    lcd_green.print("Insert the ball");
    lcd_green.setCursor(0,1);
    lcd_green.print("to a hole!!");
    lcd_red.setCursor(0,0);
    lcd_red.print("Insert the ball");
    lcd_red.setCursor(0,1);
    lcd_red.print("to a hole!!");
  }

  delay(1500);

  lcd_green.clear();
  lcd_red.clear();

  delay(400);

  lcd_green.setCursor(0, 0);
  lcd_green.print("Game is starting");
  lcd_red.setCursor(0, 0);
  lcd_red.print("Game is starting");
  scoreDisplay(0,0);
  
  // get the starting position of two paddles
  startPaddle(ir_value);

  delay(1500);

  lcd_green.clear();
  lcd_red.clear();

  delay(500);

  // initial score
  lcd_green.setCursor(0, 0);
  lcd_green.print("Score Update:");
  lcd_red.setCursor(0, 0);
  lcd_red.print("Score Update:");
  scoreDisplay(0,0);

  // code to lift the ball and strike to the play area
  redeploy(ir_value);

  delay(500);
  
  // send a signal to raspberry pi to start the game
  Serial.println("100");

  ir_value = 0;

  // loop to run until one player wins
  while(true){
    // code to run paddle according to the user input until a beam breaks(until ball go to a hole)
    while(!ir_value){
      ir_value = breakbeam();
      currentTime = millis();
      handleButtons();
      handleSolenoid();
    }
    sound_buzzer();
    ClearSerial();
    delay(100);
    // send a message to tell raspberry pi to stop the remote/AI functions
    Serial.print("6");
    delay(1000);

    // update scores
    score_update(ir_value);

    // get the starting positions
    startPaddle(ir_value);

    delay(2000);

    // end the game if one player wins
    if(greenScore== Nrounds || redScore==Nrounds){
      Serial.print("8");
      break;
    }
  
    delay(1000);

    // code to lift the ball and strike to the play area
    redeploy(ir_value);
    Serial.print("7");

    ir_value = 0;
  }

  if(greenScore == Nrounds){
    lcd_green.setCursor(0,0);
    lcd_green.print("Woo-hoo-You Won!");
    lcd_green.setCursor(0,1);
    lcd_green.print("easy right? :)");
    lcd_red.setCursor(0,0);
    lcd_red.print("Game over :(");
    lcd_red.setCursor(0,1);
    lcd_red.print("Until Next Time");

    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(0,0);
    lcd_gameSetup.print("Player Green");
    lcd_gameSetup.setCursor(0,1);
    lcd_gameSetup.print("Won The Match !!");
  }
  else if(redScore == Nrounds){
    lcd_red.setCursor(0,0);
    lcd_red.print("Woo-hoo-You Won!");
    lcd_red.setCursor(0,1);
    lcd_red.print("easy right? :)");
    lcd_green.setCursor(0,0);
    lcd_green.print("Game over :(");
    lcd_green.setCursor(0,1);
    lcd_green.print("Until Next Time");

    lcd_gameSetup.clear();
    lcd_gameSetup.setCursor(0,0);
    lcd_gameSetup.print("Player red");
    lcd_gameSetup.setCursor(0,1);
    lcd_gameSetup.print("Won The Match !!");
  }

  delay(15000);
  lcd_green.clear();
  lcd_red.clear();
  lcd_gameSetup.clear();
  greenScore = 0;
  redScore = 0;
}

