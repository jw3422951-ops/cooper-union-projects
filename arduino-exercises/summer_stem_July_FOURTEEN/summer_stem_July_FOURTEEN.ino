#define X_pin  A1
#define Y_pin  A0
#define bottonpin  2 
#define upLEDpin 6
#define rightLEDpin 10
#define downLEDpin  9
#define leftLEDpin 11

void setup() {
  Serial.begin(9600);
  Serial.println("start to play");
  pinMode(upLEDpin, OUTPUT);
  pinMode(rightLEDpin,OUTPUT);
  pinMode(leftLEDpin,OUTPUT);
  pinMode(downLEDpin,OUTPUT);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin,INPUT);
  pinMode(bottonpin, OUTPUT);
  startanimation();
}
void loop(){
  int valuex= analogRead(X_pin); 
  int valuey= analogRead(Y_pin); 
  int brightness= 0;
  if (bottonpin== HIGH){
     digitalWrite(leftLEDpin,HIGH);
      digitalWrite(rightLEDpin,HIGH);
      digitalWrite(upLEDpin,HIGH);
      digitalWrite(downLEDpin,HIGH);
  }
  if(valuex<= 200){
    digitalWrite(leftLEDpin,HIGH);
    delay(100);
    digitalWrite(leftLEDpin,LOW);
    }
  if(valuex>=800){
    for(brightness=0;brightness<=220;brightness+=10)
    delay(10);
    analogWrite(rightLEDpin,brightness);
        if(brightness=220){
    for(brightness=0;brightness<=220;brightness-=10)
    delay(100);
    analogWrite(rightLEDpin,brightness);
    delay(100);}
  }
  if(valuey<=200){
    digitalWrite(upLEDpin,HIGH);
    delay(100);
    digitalWrite(upLEDpin,LOW);
  }
  if(valuey>=800){
    digitalWrite(downLEDpin,HIGH);
    delay(100);
    digitalWrite(downLEDpin,LOW);
  }
}
void startanimation(){
  digitalWrite(upLEDpin,HIGH);
  delay(200);
  digitalWrite(upLEDpin,LOW);
  digitalWrite(leftLEDpin,HIGH); 
  delay(200);
  digitalWrite(leftLEDpin,LOW);
  digitalWrite(downLEDpin,HIGH);
  delay(200);
  digitalWrite(downLEDpin,LOW);
  digitalWrite(rightLEDpin,HIGH);
  delay(200);
  digitalWrite(rightLEDpin,LOW);
}

