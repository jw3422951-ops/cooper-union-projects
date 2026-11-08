#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_C5  523
#define NOTE_D5  587

#define buzzer 8
#define ledpin10 10
#define ledpin11 11
#define buttonpin 9

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4
};
int duration[] = {
  8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2
};
int songlenth = sizeof melody / sizeof melody[0];
int lastbuttonstate = HIGH;

void setup() {
  pinMode(ledpin10,OUTPUT);
  pinMode(ledpin11,OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonpin,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("start to sing");
}

void loop() {
  int buttonstate = digitalRead(buttonpin);
  if (buttonstate == LOW && lastbuttonstate == HIGH){
    playsong();
  }
  lastbuttonstate = buttonstate;
}

void playsong(){
  for (int i=0; i<songlenth; i++){
    int notetime = 1000 / duration[i];
    int led = i % 2 == 0 ? ledpin10 : ledpin11;
    digitalWrite(led,HIGH);
    tone(buzzer,melody[i],notetime);
    delay(notetime);
    digitalWrite(led,LOW);
    delay(notetime / 4);
  }
}
