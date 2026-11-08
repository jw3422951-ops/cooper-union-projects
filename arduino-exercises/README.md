# Summer STEM 2026 · Arduino Exercises

These three early sketches document my practice with LEDs, joystick input, and an I²C LCD during my second summer at Cooper Union.

| Sketch | Focus | Status |
| --- | --- | --- |
| [LCD and joystick](LCDandjoystick/LCDandjoystick.ino) | Displays a welcome animation on a 16×2 I²C LCD and moves a blinking cursor using the joystick's X axis. | Complete sketch structure. Build and hardware behavior are unverified. |
| [Directional LEDs](summer_stem_July_FOURTEEN/summer_stem_July_FOURTEEN.ino) | Maps joystick directions to LEDs and explores PWM brightness. | Draft with the logic issues described below. |
| [Blink and timing](Blink20267/Blink20267.ino) | Explores LED blink patterns and timing. | Unfinished draft with syntax errors. |

## Opening the sketches

Open each `.ino` file separately in the Arduino IDE. Each sketch has its own folder. The LCD sketch uses `LiquidCrystal_I2C.h` and address `0x27`. Check the library, board, and pin definitions against your hardware.

## Debugging notes

In the directional LED sketch, the button pin is configured as an output, and the program compares the pin number with `HIGH`. The test should read the pin configured as an input. The expression `if (brightness = 220)` assigns a value. A comparison would use `==`. The PWM writes also sit outside the unbraced loops, and the decreasing loop needs a starting value and stop condition that produce the intended fade.

[Back to Cooper Union Summer Projects](../README.md#year-2--classroom-work)
