# Summer STEM 2026 · Arduino Exercises

These six early sketches document my practice with motors, sensors, and interactive electronics during my second summer at Cooper Union.

| Sketch | Focus |
| --- | --- |
| [Servo control](arduino16SERVO/arduino16SERVO.ino) | Maps a potentiometer reading to a servo angle from 0° to 180° using the Arduino `Servo` library. |
| [Ultrasonic distance and LEDs](arduino16/arduino16.ino) | Measures the ultrasonic echo pulse and lights different LEDs based on the estimated distance. |
| [LCD and joystick](LCDandjoystick/LCDandjoystick.ino) | Displays a welcome animation on a 16×2 I²C LCD and moves a blinking cursor using the joystick's X axis. |
| [Directional LEDs](summer_stem_July_FOURTEEN/summer_stem_July_FOURTEEN.ino) | Maps joystick directions to LEDs and fades the right LED using PWM. |
| [Blink and timing](Blink20267/Blink20267.ino) | Flashes two LEDs a set number of times based on input from the Serial Monitor. |
| [Button and buzzer](arduino_small_projact/arduino_small_projact.ino) | Plays a melody on a button press and alternates two LEDs. |

## Opening the sketches

Open each `.ino` file separately in the Arduino IDE and select Arduino Uno. The servo sketch uses `Servo.h`, and the LCD sketch uses `LiquidCrystal_I2C.h` and address `0x27`.

## Input and wiring

- **Blink.** Enter a positive flash count in the Serial Monitor at 9600 baud.
- **Directional LEDs.** Use a pull-down resistor on the button input so a press reads `HIGH`.
- **Button and buzzer.** The sketch uses the internal pull-up. Wire the button between its input pin and ground.

[Back to Cooper Union Summer Projects](../README.md#year-2--classroom-work)
