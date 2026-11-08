#include <Stepper.h>

const int stepsPerRevolution = 2048;

// 电机 1 —— 用你调通的引脚顺序
Stepper stepper1(stepsPerRevolution, 8, 9, 10, 11);
// 电机 2 —— 第二块 ULN2003，另外 4 个脚
Stepper stepper2(stepsPerRevolution, 4, 6, 5, 7);

const float stepsPerMM = stepsPerRevolution / (3.1416 * 12.0);  // ≈54.3
const float oneRevMM   = 3.1416 * 12.0;                         // 一圈 ≈37.7mm

float fullStroke = 80;                       // 完整行程
float strokeMM   = fullStroke - oneRevMM;    // 减一圈 → 实际走 ≈42.3mm
int steps = round(stepsPerMM * strokeMM);

void setup() {
  stepper1.setSpeed(15);
  stepper2.setSpeed(15);
}

void loop() {
  // 两个一起往前推 —— 各走 1 步、轮流走，才能"同时"动
  for (int i = 0; i < steps; i++) {
    stepper1.step(1);
    stepper2.step(1);
  }
  delay(1000);

  // 两个一起往回退回原位
  for (int i = 0; i < steps; i++) {
    stepper1.step(-1);
    stepper2.step(-1);
  }
  delay(1000);
}
