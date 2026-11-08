# Summer STEM 2026 · Arduino Exercises

These three early sketches document my practice with motors, sensors, and interactive electronics during my second summer at Cooper Union.

| Sketch | Focus |
| --- | --- |
| [Servo control](arduino16SERVO/arduino16SERVO.ino) | Maps a potentiometer reading to a servo angle from 0° to 180° using the Arduino `Servo` library. |
| [Ultrasonic distance and LEDs](arduino16/arduino16.ino) | Measures the ultrasonic echo pulse and lights different LEDs based on the estimated distance. |
| [LCD and joystick](LCDandjoystick/LCDandjoystick.ino) | Displays a welcome animation on a 16×2 I²C LCD and moves a blinking cursor using the joystick's X axis. |

## Opening the sketches

Open each `.ino` file separately in the Arduino IDE. The servo sketch uses `Servo.h`, and the LCD sketch uses `LiquidCrystal_I2C.h` and address `0x27`.

[Back to Cooper Union Summer Projects](../README.md#arduino-exercises)
