#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define pin_x A0
#define pin_y A1
#define bottonpin 2

char message[] = "hello";
int cursorPos = 0;   // 全局变量,记录光标位置

void setup() {
  Serial.begin(9600);
  pinMode(pin_x, INPUT);
  pinMode(pin_y, INPUT);
  pinMode(bottonpin, INPUT);
  startanimation();
  lcd.setCursor(cursorPos, 0);
  lcd.blink();
}

void loop() {
  int valuex = analogRead(pin_x);
  int valuey = analogRead(pin_y);

  if (valuex <= 300 && cursorPos > 0) {
    cursorPos--;
  }
  if (valuex >= 800 && cursorPos < 15) {
    cursorPos++;
  }

  lcd.setCursor(cursorPos, 0);
}

void startanimation() {
  byte smile[8] = {0b00000, 0b00000, 0b00000, 0b001010, 0b10001, 0b01010, 0b00100, 0b00000};
  lcd.init();
  lcd.createChar(0, smile);
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.print("welcome");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  delay(1000);
  lcd.clear();
}