/*#define ledpin 9
#define ledpin2 10
#define ledpin3 11
void blink2 (int a, int b, int c);
void blink3(int a, int b, int c);

void setup(){
    pinMode (ledpin, OUTPUT);
    pinMode (ledpin2, OUTPUT);
    pinMode (ledpin3,OUTPUT);

    Serial.begin(9600);
    Serial.println("start of blink");
    }

void loop (){
      blink1 (1, 1000, ledpin);
      blink2(1,600,ledpin3);
      blink3(1,1000,ledpin2);
    }

void blink1(int rep, int wait, int pin)
{
   for( int i =0; i<rep; i++){
   digitalWrite(pin,HIGH);
   delay(300);
   analogWrite(pin, 50);}
   
  }
void blink2(int rep, int wait, int pin)
{
  for (int i= 0; i<rep;i++){
    digitalWrite(pin, HIGH);
    delay(300);
    analogWrite(pin, 50);
  }
}
void blink3 (int rep, int wait, int pin)
{
  for (int i=0; i<rep; i++){
    digitalWrite(pin, HIGH);
    delay(100);
    analogWrite(pin, 20);
  }
}
*/

#define interval 300
#define ledpin 9
#define ledpin2 10
int ledstate= HIGH;
int ledstate= LOW;
unsigned long time = millis();
unsigned long previous =0;
//serial.begin(9600);
//serial.println("start of blink")

void setup(){
  pinMode(ledpin,OUTPUT);
  pinMode(ledpin2,OUTPUT);
}

void loop(){
int i=0;
int total lighning times= 0;
scanf_s("please type your number,%d",total lighting times);
  while (i<){

  }















  /* time = millis();
  if (time-previous>= interval){
    previous = time ;
    if(ledstate == LOW){
      ledstate = HIGH;
    }
    else 
    ledstate =LOW;
  }
  digitalWrite(ledpin,ledstate);
  digitalWrite(ledpin,
  






