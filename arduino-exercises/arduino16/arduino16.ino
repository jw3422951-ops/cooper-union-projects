#define ledpin 2
#define ledpin2 3
#define ledpin3 4
#define ledpin4 5
#define trig 9
#define echo 10

void setup() {
  Serial.begin(9600);
  Serial.print("start to measure\n");
  int pins[]={ledpin,ledpin2,ledpin3,ledpin4};
  for (int i=0; i<4;i++){
  (pinMode(pins[i],OUTPUT));
  }
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  startanimation();
}

void loop() {
  float distance = getdistance();

  if (distance < 40){
    digitalWrite(ledpin3,HIGH);
    delay(200);
    digitalWrite(ledpin3,LOW);
  }
  else if (distance >= 40 && distance < 60){
    digitalWrite(ledpin2,HIGH);
    delay(350);
    digitalWrite(ledpin2,LOW);
  }
  else if (distance >= 60 && distance <= 80){
    digitalWrite(ledpin,HIGH);
    delay(500);
    digitalWrite(ledpin,LOW);
  }
  else if (distance > 80 && distance <= 100){
    digitalWrite(ledpin4,HIGH);
  }
}

void startanimation(){
  int pins[]={ledpin,ledpin2,ledpin3,ledpin4};
  for (int i=0; i<4;i++){
    digitalWrite(pins[i],LOW);
  }
}

float getdistance(){
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  long duration=pulseIn(echo,HIGH);
  float distance = duration/2 * 0.034;
  Serial.println(distance);
  return distance;
}