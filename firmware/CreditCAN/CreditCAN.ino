#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Stepper.h>

// ============ PINS ============
const int PIN_INDUCTIVE = 3;              // NPN black wire, HIGH = metal detected

// Gear 1 pusher — cross order IN1,IN3,IN2,IN4
Stepper pusher(2048, 10, 12, 11, 13);
// Gear 2 chute — cross order
Stepper chute(2048, 6, 8, 7, 9);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// ============ CALIBRATION CONSTANTS (seed values, edit here after real tests) ============
const float TOF_BASELINE_MM = 132.0;      // empty-plate ToF reading

const int   PUSH_STEPS      = 3100;       // Gear1 pusher travel (negative = reverse)

// Gear2: steps per 90 deg. Calculated from gear ratio 30:12 = 2.5
// (90/360) * 2.5 * 2048 = 1280. Verify on hardware, tweak this one value.
const int   CHUTE_QUARTER = 1280;
const int   CHUTE_BOTTLE =  0;                  // 0 deg   plastic bottle
const int   CHUTE_CAN    =  CHUTE_QUARTER;      // 90 deg  aluminum can
const int   CHUTE_FOOD   =  CHUTE_QUARTER * 2;  // 180 deg food waste
const int   CHUTE_REJECT = -CHUTE_QUARTER;      // 270 deg reject (reverse 90, shortest)

// Weight gates (g)
const float ITEM_WEIGHT_MIN   = 6.0;      // below this = paper/bag/foam -> reject
const float BOTTLE_WEIGHT_MAX = 30.0;     // bottle (empty or little water) always <= 30g

// Height gate (mm)
const float BOTTLE_HEIGHT_MIN = 40.0;     // real bottle stands at least 40mm tall

// Aluminum can: metal, light AND large-volume (low density)
const float CAN_WEIGHT_MIN  = 5.0;
const float CAN_WEIGHT_MAX   = 25.0;
const float CAN_DENSITY_MAX  = 1.5;       // small-heavy metal (battery) exceeds this -> reject

// density proxy = weight(g) / height(mm)
const float DENSITY_LINE_1 = 1.5;         // bottle / food boundary
const float DENSITY_LINE_2 = 6.0;         // food / reject boundary (raised: no rock sample, give kiwi room)

// credits
const int CREDITS_BOTTLE        = 5;      // per item
const int CREDITS_CAN           = 8;      // per item
const int CREDITS_FOOD_PER_100G = 5;      // per 100g
const int CREDITS_REJECT        = 1;      // all rejects still give 1

const unsigned long METAL_WINDOW_MS = 6000;  // after first metal hit, wait up to 6s for weight input

const int STEP_RPM = 10;

// ============ STATE ============
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
  bool metalSeen = false;              // has metal been detected this round (OR latch)
  bool timerOn   = false;              // metal window started
  unsigned long windowStart = 0;

  // Wait for weight input. Meanwhile watch the inductive sensor.
  while (true) {
    // read metal; first hit latches and starts the window
    if (digitalRead(PIN_INDUCTIVE) == HIGH) {
      if (!metalSeen) {
        metalSeen = true;
        timerOn = true;
        windowStart = millis();
        Serial.println("[metal detected] you have 6s to enter weight...");
      }
    }

    // weight typed -> stop watching immediately, go classify now (no need to wait out the window)
    if (Serial.available() > 0) break;

    // metal window timed out with no input -> abort this round, restart clean
    if (timerOn && (millis() - windowStart >= METAL_WINDOW_MS)) {
      Serial.println("detection ended");
      return;   // restart loop() fresh, metalSeen cleared
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
  if (measure.RangeStatus == 4) return 0.0;   // out of range → no height
  float h = TOF_BASELINE_MM - measure.RangeMilliMeter;
  return (h < 0) ? 0 : h;
}

Category classify(bool isMetal, float weight, float height, float density) {
  if (isMetal) {
    // can must be light AND large-volume (low density)
    if (weight >= CAN_WEIGHT_MIN && weight <= CAN_WEIGHT_MAX
        && density < CAN_DENSITY_MAX) return CAN;
    return REJECT;   // wrong weight OR small-heavy metal (e.g. battery)
  }
  // non-metal
  if (weight < ITEM_WEIGHT_MIN) return REJECT;             // too light: paper, bag, foam
  // bottle: tall AND light AND low density
  if (height >= BOTTLE_HEIGHT_MIN && weight <= BOTTLE_WEIGHT_MAX
      && density < DENSITY_LINE_1) return BOTTLE;
  // food: banana/kiwi-like density range (height not required)
  if (density >= DENSITY_LINE_1 && density < DENSITY_LINE_2) return FOOD;
  return REJECT;                                           // everything else
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

  if (chuteSteps != 0) chute.step(chuteSteps);   // 1) rotate chute to bin
  delay(2000);                                   // wait 2s at the bin
  pusher.step(PUSH_STEPS);                        // 2) push item down
  pusher.step(-PUSH_STEPS);                       // 3) retract same distance

  totalCredits += credits;                        // 4) award credits only after full retract
  Serial.print("Credits this round: +"); Serial.println(credits);

  if (chuteSteps != 0) chute.step(-chuteSteps);   // 5) rotate chute back to home
}