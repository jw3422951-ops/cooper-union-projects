#define interval 300
#define ledpin 9
#define ledpin2 10
int ledstate= LOW;
unsigned long previous =0;
long total = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("enter total lighting times");
  pinMode(ledpin,OUTPUT);
  pinMode(ledpin2,OUTPUT);
}

void loop(){
  if (total == 0) {
    if (Serial.available() == 0) return;
    total = Serial.parseInt();
    while (Serial.available() > 0) Serial.read();
    if (total <= 0) {
      total = 0;
      return;
    }
    previous = millis();
  }

  unsigned long time = millis();
  if (time - previous < interval) return;
  previous = time;
  if (ledstate == LOW) {
    ledstate = HIGH;
  } else {
    ledstate = LOW;
    total--;
  }
  digitalWrite(ledpin,ledstate);
  digitalWrite(ledpin2,ledstate);
}
