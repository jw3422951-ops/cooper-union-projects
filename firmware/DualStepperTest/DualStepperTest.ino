#include <Stepper.h>

const int stepsPerRevolution = 2048;

Stepper stepper1(stepsPerRevolution, 8, 9, 10, 11);
Stepper stepper2(stepsPerRevolution, 4, 6, 5, 7);

const float stepsPerMM = stepsPerRevolution / (3.1416 * 12.0);
const float oneRevMM   = 3.1416 * 12.0;

float fullStroke = 80;
float strokeMM   = fullStroke - oneRevMM;
int steps = round(stepsPerMM * strokeMM);

void setup() {
  stepper1.setSpeed(15);
  stepper2.setSpeed(15);
}

void loop() {
  for (int i = 0; i < steps; i++) {
    stepper1.step(1);
    stepper2.step(1);
  }
  delay(1000);

  for (int i = 0; i < steps; i++) {
    stepper1.step(-1);
    stepper2.step(-1);
  }
  delay(1000);
}
