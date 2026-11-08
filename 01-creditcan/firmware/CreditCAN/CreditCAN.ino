#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Stepper.h>

const int PIN_INDUCTIVE = 3;

Stepper pusher(2048, 10, 12, 11, 13);
Stepper chute(2048, 6, 8, 7, 9);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float TOF_BASELINE_MM = 132.0;

const int   PUSH_STEPS      = 3100;

const int   CHUTE_QUARTER = 1280;
const int   CHUTE_BOTTLE =  0;
const int   CHUTE_CAN    =  CHUTE_QUARTER;
const int   CHUTE_FOOD   =  CHUTE_QUARTER * 2;
const int   CHUTE_REJECT = -CHUTE_QUARTER;

const float ITEM_WEIGHT_MIN   = 6.0;
const float BOTTLE_WEIGHT_MAX = 30.0;

const float BOTTLE_HEIGHT_MIN = 40.0;

const float CAN_WEIGHT_MIN  = 5.0;
const float CAN_WEIGHT_MAX   = 25.0;
const float CAN_DENSITY_MAX  = 1.5;

const float DENSITY_LINE_1 = 1.5;
const float DENSITY_LINE_2 = 6.0;

const int CREDITS_BOTTLE        = 5;
const int CREDITS_CAN           = 8;
const int CREDITS_FOOD_PER_100G = 5;
const int CREDITS_REJECT        = 1;

const unsigned long METAL_WINDOW_MS = 6000;

const int STEP_RPM = 10;

enum Category { BOTTLE, CAN, FOOD, REJECT };
long totalCredits = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(PIN_INDUCTIVE, INPUT_PULLUP);
  pusher.setSpeed(STEP_RPM);
  chute.setSpeed(STEP_RPM);

  if (!lox.begin()) {
    Serial.println("VL53L0X not found - check wiring");
    while (1);
  }

  Serial.println("=== CreditCAN ready ===");
  Serial.println("Place item, then type weight (g) and press Enter");
}

void loop() {
  bool metalSeen = false;
  bool timerOn   = false;
  unsigned long windowStart = 0;

  while (true) {
    if (digitalRead(PIN_INDUCTIVE) == HIGH) {
      if (!metalSeen) {
        metalSeen = true;
        timerOn = true;
        windowStart = millis();
        Serial.println("[metal detected] you have 6s to enter weight...");
      }
    }

    if (Serial.available() > 0) break;

    if (timerOn && (millis() - windowStart >= METAL_WINDOW_MS)) {
      Serial.println("detection ended");
      return;
    }
  }

  float weight = Serial.parseFloat();
  while (Serial.available()) Serial.read();
  if (weight <= 0) return;

  Serial.println();
  Serial.println("------------------------");
  Serial.print("Metal: ");   Serial.println(metalSeen ? "METAL" : "none");
  Serial.print("Weight: ");  Serial.print(weight);  Serial.println(" g");

  float height  = readHeightMM();
  float density = (height > 1.0) ? (weight / height) : 999.0;
  Serial.print("Height: ");        Serial.print(height);  Serial.println(" mm");
  Serial.print("Density proxy: "); Serial.println(density);

  Category cat = classify(metalSeen, weight, height, density);
  handleCategory(cat, weight);

  Serial.print(">>> Total credits: ");
  Serial.println(totalCredits);
  Serial.println("Place next item, type weight (g) and press Enter");
}

float readHeightMM() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus == 4) return 0.0;
  float h = TOF_BASELINE_MM - measure.RangeMilliMeter;
  return (h < 0) ? 0 : h;
}

Category classify(bool isMetal, float weight, float height, float density) {
  if (isMetal) {
    if (weight >= CAN_WEIGHT_MIN && weight <= CAN_WEIGHT_MAX
        && density < CAN_DENSITY_MAX) return CAN;
    return REJECT;
  }
  if (weight < ITEM_WEIGHT_MIN) return REJECT;
  if (height >= BOTTLE_HEIGHT_MIN && weight <= BOTTLE_WEIGHT_MAX
      && density < DENSITY_LINE_1) return BOTTLE;
  if (density >= DENSITY_LINE_1 && density < DENSITY_LINE_2) return FOOD;
  return REJECT;
}

void handleCategory(Category cat, float weight) {
  int chuteSteps = 0, credits = 0;
  const char* name = "";

  switch (cat) {
    case BOTTLE: chuteSteps = CHUTE_BOTTLE; credits = CREDITS_BOTTLE; name = "Plastic bottle (0 deg)";  break;
    case CAN:    chuteSteps = CHUTE_CAN;    credits = CREDITS_CAN;    name = "Aluminum can (90 deg)";   break;
    case FOOD:   chuteSteps = CHUTE_FOOD;   credits = round(weight / 100.0 * CREDITS_FOOD_PER_100G); name = "Food waste (180 deg)"; break;
    case REJECT: chuteSteps = CHUTE_REJECT; credits = CREDITS_REJECT; name = "Reject (270 deg)";        break;
  }
  Serial.print("Category: "); Serial.println(name);

  if (chuteSteps != 0) chute.step(chuteSteps);
  delay(2000);
  pusher.step(PUSH_STEPS);
  pusher.step(-PUSH_STEPS);

  totalCredits += credits;
  Serial.print("Credits this round: +"); Serial.println(credits);

  if (chuteSteps != 0) chute.step(-chuteSteps);
}
