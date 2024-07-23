#include <LiquidCrystal_I2C.h>

const int ir_blue = 5, ir_green = 6;
int score, blueScore = 0, greenScore = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int breakbeam() {
    if (!digitalRead(ir_blue)) {
        Serial.println("IR Beam is Broken - Blue");
        return 1;
    } //else if (!digitalRead(ir_green)) {
       // Serial.println("IR Beam is Broken - Green");
        //return 2;
   else {
        Serial.println("IR Beam is Intact");
        return 0;
    }
}

void scoreDisplay(int blue, int green){
    lcd.setCursor(0, 1);
    lcd.print("Blue: ");
    lcd.print(blue);
    lcd.print(" Green: ");
    lcd.print(green);
}

void score_update(int score) {
    if (score == 1) {
        B += 1;3
    } else if (score == 2) {
        G += 1;
    }
    scoreDisplay(B, G);

}

void setup() {
    Serial.begin(9600);
    pinMode(ir_blue, INPUT);
    pinMode(ir_green, INPUT);
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Score Update:");
    scoreDisplay(0,0);
}

void loop() {
    score = breakbeam();
    if(score){
      score_update(score);
      delay(1000);
    }
    delay(50); // Add a delay to avoid too rapid updates
}
