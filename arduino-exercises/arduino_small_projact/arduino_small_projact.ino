#define NOTE_C4  262   // Do
#define NOTE_D4  294   // Re
#define NOTE_E4  330   // Mi
#define NOTE_F4  349   // Fa
#define NOTE_G4  392   // Sol
#define NOTE_A4  440   // La
#define NOTE_C5  523   // 高音Do
#define NOTE_D5  587

#define buzzer 8
#define ledpin 10
#define ledpin 11
#define buttonpin 9 
void setup() {
  // put your setup code here, to run once:
pinMode(ledpin10,OUTPUT);
pinMode(ledpin11,OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(buttonpin,INPUT_PULLUP);
//digitalRead(buttonpin);
Serial.began(9600);
Serialprintln("start to sing");
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonstate = digitalRead(buttonpin);
if (buttonstate == LOW){
playsong();
}
}
void playsong(int)
melody[NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4];
int duration []={8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2};
int songlenth = sizeof melody[]/ sizeof melody[0];
