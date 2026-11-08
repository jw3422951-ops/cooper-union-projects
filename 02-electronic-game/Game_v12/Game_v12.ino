const int BTN = 3;
const int LED_P1 = 4;
const int LED_P2 = 5;
const int NUM_SCORE_LEDS = 6;
const int SCORE_LEDS[NUM_SCORE_LEDS] = {6, 7, 8, 9, 10, 13};
const int BUZZER_START   = 11;
const int BUZZER_VICTORY = 12;

const int TOTAL_ROUNDS = 5;
const unsigned long TIMEOUT_MS = 5000;

const unsigned long TIME_THRESHOLDS[5] = {125, 150, 200, 300, 500};

int scoreP1 = 0;
int scoreP2 = 0;

void setup() {
  pinMode(BTN, INPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_P2, OUTPUT);
  for (int i = 0; i < NUM_SCORE_LEDS; i++) pinMode(SCORE_LEDS[i], OUTPUT);
  pinMode(BUZZER_START, OUTPUT);
  pinMode(BUZZER_VICTORY, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println(F("=== Reaction Time Game 反应速度对战 ==="));
  startupAnimation();
}

void loop() {
  scoreP1 = 0;
  scoreP2 = 0;

  for (int round = 1; round <= TOTAL_ROUNDS; round++) {
    Serial.print(F("--- Round 第 "));
    Serial.print(round);
    Serial.println(F(" 轮 ---"));

    unsigned long timeP1 = playTurn(1);
    int pointsP1 = (timeP1 < TIMEOUT_MS) ? timeToPoints(timeP1) : 0;
    scoreP1 += pointsP1;
    reportPoints(1, timeP1, pointsP1);
    delay(1000);

    unsigned long timeP2 = playTurn(2);
    int pointsP2 = (timeP2 < TIMEOUT_MS) ? timeToPoints(timeP2) : 0;
    scoreP2 += pointsP2;
    reportPoints(2, timeP2, pointsP2);

    if (timeP1 < timeP2)      flashPlayerLED(1, 2);
    else if (timeP2 < timeP1) flashPlayerLED(2, 2);

    Serial.print(F("Total 总分  P1: ")); Serial.print(scoreP1);
    Serial.print(F("  P2: ")); Serial.println(scoreP2);
    delay(1500);
  }

  announceWinner();

  Serial.println(F("Press the button to play again 按按钮再来一局"));
  waitForPress();
  delay(500);
}

unsigned long playTurn(int player) {
  int led = (player == 1) ? LED_P1 : LED_P2;

  Serial.print(F("P")); Serial.print(player);
  Serial.println(F("'s turn 的回合"));

  for (int i = 0; i < 3; i++) {
    digitalWrite(led, HIGH);
    tone(BUZZER_START, 600, 80);
    delay(180);
    digitalWrite(led, LOW);
    delay(180);
  }

  while (digitalRead(BTN) == HIGH) delay(10);

  unsigned long waitTime = random(2000, 5001);
  unsigned long waitStart = millis();
  while (millis() - waitStart < waitTime) {
    if (digitalRead(BTN) == HIGH) {
      Serial.println(F("False start! 0 points. 抢跑!0分"));
      errorBeep();
      for (int i = 0; i < 5; i++) {
        digitalWrite(led, HIGH); delay(100);
        digitalWrite(led, LOW);  delay(100);
      }
      while (digitalRead(BTN) == HIGH) delay(10);
      return TIMEOUT_MS;
    }
  }

  tone(BUZZER_START, 2500, 400);
  unsigned long goTime = millis();

  while (millis() - goTime < TIMEOUT_MS) {
    if (digitalRead(BTN) == HIGH) {
      unsigned long t = millis() - goTime;
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      while (digitalRead(BTN) == HIGH) delay(10);
      return t;
    }
  }

  Serial.println(F("Too slow! 0 points. 超时!0分"));
  errorBeep();
  return TIMEOUT_MS;
}

void reportPoints(int player, unsigned long t, int points) {
  Serial.print(F("P")); Serial.print(player);
  if (t < TIMEOUT_MS) {
    Serial.print(F(" time 用时: ")); Serial.print(t);
    Serial.print(F(" ms | Points 得分: ")); Serial.println(points);
  } else {
    Serial.println(F(" scored 0 得0分"));
  }
  if (points > 0) showScoreLEDs(points);
}

int timeToPoints(unsigned long t) {
  if (t <= TIME_THRESHOLDS[0]) return 6;
  if (t <= TIME_THRESHOLDS[1]) return 5;
  if (t <= TIME_THRESHOLDS[2]) return 4;
  if (t <= TIME_THRESHOLDS[3]) return 3;
  if (t <= TIME_THRESHOLDS[4]) return 2;
  return 1;
}

void showScoreLEDs(int points) {
  int fadeCount = (points > 5) ? 5 : points;
  for (int i = 0; i < fadeCount; i++) {
    tone(BUZZER_START, 800 + i * 200, 100);
    fadeInLED(SCORE_LEDS[i], 350);
  }
  if (points >= 6) {
    delay(200);
    godlikeFlash();
  }
  delay(1000);
  fadeOutAll((points >= 6) ? 6 : fadeCount, 500);
}

void godlikeFlash() {
  int notes[] = {1047, 1319, 1568};
  for (int i = 0; i < 3; i++) {
    digitalWrite(SCORE_LEDS[5], HIGH);
    tone(BUZZER_VICTORY, notes[i], 100);
    delay(150);
    digitalWrite(SCORE_LEDS[5], LOW);
    delay(120);
  }
  digitalWrite(SCORE_LEDS[5], HIGH);
  tone(BUZZER_VICTORY, 1568, 300);
  delay(300);
}

void fadeInLED(int pin, int durationMs) {
  const int steps = 40;
  const long periodUs = 2000;
  for (int s = 1; s <= steps; s++) {
    long onUs = periodUs * s * s / ((long)steps * steps);
    unsigned long stepEnd = millis() + durationMs / steps;
    while (millis() < stepEnd) {
      if (onUs > 0) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(onUs);
      }
      digitalWrite(pin, LOW);
      delayMicroseconds(periodUs - onUs);
    }
  }
  digitalWrite(pin, HIGH);
}

void fadeOutAll(int count, int durationMs) {
  const int steps = 40;
  const long periodUs = 2000;
  for (int s = steps; s >= 0; s--) {
    long onUs = periodUs * s * s / ((long)steps * steps);
    unsigned long stepEnd = millis() + durationMs / steps;
    while (millis() < stepEnd) {
      if (onUs > 0) {
        for (int i = 0; i < count; i++) digitalWrite(SCORE_LEDS[i], HIGH);
        delayMicroseconds(onUs);
      }
      for (int i = 0; i < count; i++) digitalWrite(SCORE_LEDS[i], LOW);
      delayMicroseconds(periodUs - onUs);
    }
  }
  for (int i = 0; i < NUM_SCORE_LEDS; i++) digitalWrite(SCORE_LEDS[i], LOW);
}

void flashPlayerLED(int player, int times) {
  int led = (player == 1) ? LED_P1 : LED_P2;
  for (int i = 0; i < times; i++) {
    digitalWrite(led, HIGH); delay(200);
    digitalWrite(led, LOW);  delay(200);
  }
}

void announceWinner() {
  Serial.println(F("=== FINAL 终局 ==="));
  Serial.print(F("P1: ")); Serial.print(scoreP1);
  Serial.print(F("  P2: ")); Serial.println(scoreP2);

  if (scoreP1 == scoreP2) {
    Serial.println(F("It's a tie! 平局!"));
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_P1, HIGH); digitalWrite(LED_P2, HIGH);
      setAllScoreLEDs(HIGH); delay(250);
      digitalWrite(LED_P1, LOW);  digitalWrite(LED_P2, LOW);
      setAllScoreLEDs(LOW);  delay(250);
    }
    return;
  }

  int winner = (scoreP1 > scoreP2) ? 1 : 2;
  Serial.print(F("WINNER 获胜者: P")); Serial.println(winner);

  int winnerLED = (winner == 1) ? LED_P1 : LED_P2;

  int melody[]   = {523, 659, 784, 1047, 784, 1047};
  int duration[] = {150, 150, 150, 300, 150, 500};

  for (int n = 0; n < 6; n++) {
    digitalWrite(winnerLED, HIGH);
    setAllScoreLEDs(HIGH);
    tone(BUZZER_VICTORY, melody[n], duration[n]);
    delay(duration[n]);
    digitalWrite(winnerLED, LOW);
    setAllScoreLEDs(LOW);
    delay(50);
  }

  for (int i = 0; i < 6; i++) {
    digitalWrite(winnerLED, HIGH); setAllScoreLEDs(HIGH); delay(150);
    digitalWrite(winnerLED, LOW);  setAllScoreLEDs(LOW);  delay(150);
  }
}

void setAllScoreLEDs(int state) {
  for (int i = 0; i < NUM_SCORE_LEDS; i++) digitalWrite(SCORE_LEDS[i], state);
}

void errorBeep() {
  tone(BUZZER_START, 200, 400);
  delay(400);
}

void waitForPress() {
  while (digitalRead(BTN) == LOW) delay(10);
}

void startupAnimation() {
  for (int i = 0; i < NUM_SCORE_LEDS; i++) {
    digitalWrite(SCORE_LEDS[i], HIGH); delay(100);
  }
  digitalWrite(LED_P1, HIGH); digitalWrite(LED_P2, HIGH);
  tone(BUZZER_START, 880, 200);
  delay(400);
  digitalWrite(LED_P1, LOW); digitalWrite(LED_P2, LOW);
  setAllScoreLEDs(LOW);
}
