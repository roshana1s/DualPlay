#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int d,Nrounds,gm,com;
 
char keys[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
byte rowPins[4] = {2,3,4,5}; 
byte colPins[4] = {6,7,8,9}; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

LiquidCrystal_I2C lcd(0x27, 16, 2);


void wc_msg(){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Welcome to ");
    lcd.setCursor(4, 1);
    lcd.print("dualPlay"); 
}

int game_mode(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter GameMode");
    lcd.setCursor(2, 1);
    lcd.print("1.HvH  2.SvH");
    char key;
    while(1){
      key = kpd.getKey();
      if(key=='1' || key=='2') break;
    }
    lcd.clear();
    if(key == '1'){
      lcd.setCursor(1, 0);
      lcd.print("You Selected");
      lcd.setCursor(1, 1);
      lcd.print("Human V Human");
  }
  else if(key == '2'){
    lcd.setCursor(1, 0);
    lcd.print("You Selected");
    lcd.setCursor(1, 1);
    lcd.print("System V Human");
  }
  delay(2000);
  return key-'0';
}

int diff(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Difficulty");
    lcd.setCursor(0, 1);
    lcd.print("1.Easy  2.Medium");
    char key;
    while(1){
      key = kpd.getKey();
      if(key=='1' || key=='2') break;
    }
    lcd.clear();
    if(key == '1'){
      lcd.setCursor(2, 0);
      lcd.print("You Selected");
      lcd.setCursor(6, 1);
      lcd.print("Easy");
  }
  else if(key == '2'){
    lcd.setCursor(2, 0);
    lcd.print("You Selected");
    lcd.setCursor(5, 1);
    lcd.print("Medium");
  }
  delay(2000);
  return key-'0';
}
int rounds(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No. of rounds?");
    lcd.setCursor(0, 1);
    lcd.print("1)3  2)5  3)10");
    char key;
    while(1){
      key = kpd.getKey();
      if(key=='1' || key=='2' || key=='3') break;
    }
    lcd.clear();
    if(key == '1'){
      lcd.setCursor(1, 0);
      lcd.print("NO of rounds:");
      lcd.setCursor(14, 0);
      lcd.print(3);
  }
  else if(key == '2'){
    lcd.setCursor(1, 0);
    lcd.print("NO of rounds:");
    lcd.setCursor(14, 0);
    lcd.print(5);
  }
  else if(key == '3'){
     lcd.setCursor(1, 0);
     lcd.print("NO of rounds:");
     lcd.setCursor(14, 0);
     lcd.print(10);
  }
  else{
    lcd.setCursor(2, 0);
    lcd.print("Try again");
  }
  return key-'0';
}

void gameSetup(){
  wc_msg();
  delay(3000);
  gm = game_mode();
  if(gm==1){
      d = 0;
      Nrounds = rounds();
   }
   else if(gm==2){
      d = diff();
      Nrounds = rounds();
   }
  com = (gm*100)+(d*10)+(Nrounds);
}

void setup()
{
	Serial.begin(9600);
	lcd.begin();
	lcd.backlight();
  gameSetup();
  Serial.println(com);
  delay(3000);
}

void loop()
{
	
}
