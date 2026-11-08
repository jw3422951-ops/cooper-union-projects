#include<Servo.h>
Servo myServo;
#define potpin A0
#define ldrpin A1
#define Servo 9
void setup() {
myServo.attach(Servo);
Serial.begin(9600);
}

void loop() {
int potvalue = analogRead(potpin);
int ldrvalue = analogRead(ldrpin);
int angle= map(potvalue, 0,1023,0,180);
myServo.write(angle);
Serial.println(potvalue);
Serial.println(ldrvalue);
delay (200);
}