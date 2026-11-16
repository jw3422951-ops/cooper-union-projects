#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Stepper.h>

Adafruit_VL53L0X tof = Adafruit_VL53L0X();
Stepper pusher(2048, 10, 12, 11, 13);
Stepper chute(2048, 6, 8, 7, 9); //chute motor
int metalpin = 3;
int total = 0;
float baseline = 132; //empty plate
int pushSteps = 3000;
int quarter = 1280;

void setup() {
  Serial.begin(115200);
  pinMode(metalpin, INPUT_PULLUP);
  pusher.setSpeed(10);
  chute.setSpeed(10);

  if (!tof.begin()) {
    Serial.println("tof not working");
    while (1);
  }
  Serial.println("put item in and type weight");
}

void loop() {
  bool metal = false;
  unsigned long t = 0;

  // wait for weight, check metal at the same time
  while (Serial.available() == 0) {
    if (digitalRead(metalpin) == HIGH && metal == false) {
      metal = true;
      t = millis();
      Serial.println("metal! type weight in 6 sec");
    }
    if (metal == true && millis() - t > 6000) {
      Serial.println("too slow, try again");
      return;
    }
  }

  String s = Serial.readStringUntil('\n');
  float weight = s.toFloat();
  if (weight <= 0) return;

  // height from tof
  VL53L0X_RangingMeasurementData_t measure;
  tof.rangingTest(&measure, false);
  float height = baseline - measure.RangeMilliMeter;
  if (measure.RangeStatus == 4) height = 0;
  if (height < 0) height = 0;

  float density = 100;   // if no height just make it big
  if (height > 1) density = weight / height;

  Serial.println("-----");
  Serial.print("metal: ");
  Serial.println(metal);
  Serial.print("weight: ");
  Serial.println(weight);
  Serial.print("height: ");
  Serial.println(height);
  Serial.print("density: ");
  Serial.println(density);

  int steps = 0;
  int credits = 0;

  if (metal) {
    if (weight >= 5 && weight <= 25 && density < 1.5) {
      Serial.println("can");
      steps = quarter;
      credits = 8;
    } else {
      Serial.println("reject");
      steps = -quarter;
      credits = 1;
    }
  } else if (weight < 6) {
    Serial.println("reject");
    steps = -quarter;
    credits = 1;
  } else if (height >= 60 && weight <= 30 && density < 1.5) {
    Serial.println("bottle");
    steps = 0;
    credits = 5;
  } else if (density >= 1.5 && density < 6) {
    Serial.println("food");
    steps = quarter * 2;
    credits = round(weight / 100 * 5);
  } else {
    Serial.println("reject");
    steps = -quarter;
    credits = 1;
  }

  // turn chute, push, come back
  chute.step(steps);
  delay(2000);
  pusher.step(pushSteps);
  pusher.step(-pushSteps);
  total = total + credits;
  chute.step(-steps);

  Serial.print("credits: +");
  Serial.println(credits);
  Serial.print("total: ");
  Serial.println(total);
  Serial.println("next item");
}
