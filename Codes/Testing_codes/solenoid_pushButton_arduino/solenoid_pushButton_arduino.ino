void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(3)){
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(2,LOW);
  }
}