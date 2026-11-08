# Cooper Union Summer Projects

Over two summers at Cooper Union, I went from modeling a recycling station with a team to independently designing and building a working waste sorter.

## Independent waste sorter

**Year 2 · 2026**

I revised the mechanism during fabrication and integrated the sensors and motors with an Arduino Mega 2560.

<img src="assets/year-2-sorter.jpg" alt="Working sorter with a transparent enclosure, rotating chute, wiring, and controller" width="480">

### Sensing and classification

| Input | Role |
| --- | --- |
| Inductive proximity sensor (NPN, normally closed) | Checks for metal. |
| VL53L0X time-of-flight sensor | Measures distance to the item, which the system uses to estimate its height. [Sensor documentation](https://www.st.com/en/imaging-and-photonics-solutions/vl53l0x.html) |
| Serial | I enter the item's mass manually in the working prototype. |

The sorter checks for metal first and uses thresholds for mass, height, and mass divided by height. The four routes are plastic bottles, aluminum cans, food waste, and rejects.

### Mechanical layout and control

- **Top.** A fixed platform holds the item near the sensors.
- **Middle.** A ring gear and pinion turn the chute toward the selected route. A rack and pinion drive the pusher.
- **Bottom.** Fixed bins collect the sorted items.

I designed the finger-jointed enclosure in Onshape, laser-cut the panels, and generated the gears and racks with FeatureScript for 3D printing.

<img src="assets/year-2-chute-detail.jpg" alt="Close-up of the printed rotating chute, pinion, and gear assembly" width="380">

#### CAD drawings

These DXF exports document the enclosure, pusher, and drive components during development.

- [Enclosure](models/creditcan/enclosure.dxf?raw=true) · [Board](models/creditcan/board.dxf?raw=true) · [Side panel](models/creditcan/side-panel.dxf?raw=true)
- [Pusher](models/creditcan/pusher.dxf?raw=true) · [Large gear](models/creditcan/large-gear.dxf?raw=true) · [Small gear](models/creditcan/small-gear.dxf?raw=true)

### Firmware and motor sequence

I wrote the initial Arduino C++ sketch myself and used AI to help identify and correct errors. The sketch waits for a mass entry in the Serial Monitor, classifies the item, and runs the motor sequence. Step counts and classification thresholds are stored as named constants. [Open the Arduino Mega 2560 firmware](firmware/CreditCAN/CreditCAN.ino)

The chute rotates to the selected bin when needed. The program waits two seconds, extends and retracts the pusher, and returns the chute to its starting position. I designed the sequence around the blocking `step()` calls in Arduino's standard `Stepper` library so that each move finishes before the next begins. Reducing simultaneous motor demand was a secondary consideration. [Arduino Stepper library source](https://github.com/arduino-libraries/Stepper/blob/master/src/Stepper.cpp#L175-L218)

An earlier bench test alternates individual steps between two motors to move them forward and back. This copy comes from the Arduino IDE build cache. [Open the dual-stepper bench test](firmware/DualStepperTest/DualStepperTest.ino)

### Future work

Integrate the load cell and HX711 to measure mass automatically. Then test whether the variance in load cell readings after a short vibration pulse can help identify partially filled containers.

## Year 1 · CreditCAN team concept

**3D Fabrication · Honor Award**

We interviewed passersby about neighborhood conditions. Many said they were generally satisfied, yet we observed litter in the park and around nearby restaurants. That contrast led us to ask how we could encourage proper waste disposal in shared spaces.

We designed CreditCAN as a station that would award credit at nearby businesses for deposited items. Our team built and spray-painted a display model and designed the logo. A teammate and I proposed public-space sanitation as the theme. I suggested an automated collection bin, helped define its purpose and overall structure, and modeled the team prototype in Onshape.

<img src="assets/credit-can-model.jpg" alt="CreditCAN team model and its project poster" width="380">

## Year 2 · Classroom work

### What I learned in class

I learned the basics of ESP32 Wi-Fi and Bluetooth and practiced working with motors, ultrasonic distance sensors, LEDs, and push buttons. I used classroom explanations and AI to understand how the components worked, then tried writing code for the exercises.

The class also covered assembly design, dimensional tolerances, and shaft and hole fits. I applied what I learned about clearance and fit when designing parts for later 3D modeling and laser-cut fabrication projects.

<img src="assets/year-2-assembly-and-tolerance-class.jpg" alt="Classroom slide on assembly design, tolerances, and clearance, transition, and interference fits" width="560">

### Arduino exercises

The sketches include a potentiometer-controlled servo, ultrasonic distance measurement with LED feedback, and exercises with joysticks, LEDs, and an I²C LCD. [Browse the Arduino exercises](arduino-exercises)

### Team mini-project · Electronic game

My team built a small electronic game with two push buttons, colored LEDs, and an LCD that displayed a replay prompt. I was responsible for the 3D modeling and helped debug the game.

<img src="assets/electronic-game.jpg" alt="Breadboard game with buttons, LEDs, and an LCD showing the replay prompt" width="560">

**Game demonstration (2:41)**

https://github.com/user-attachments/assets/599749b5-15be-4854-b69c-446686e29c17

[Original recording (MP4)](assets/electronic-game-demo.mp4)

[Early reaction game sketch (Arduino Uno)](game/Game_v12/Game_v12.ino) uses a shared button for two players. The demonstration above shows the later build with two buttons and an LCD.

#### CAD assembly

<img src="assets/electronic-game-cad.png" alt="CAD assembly of the game enclosure with an LCD opening, button cutouts, and a separate base panel" width="460">

[Download the assembly file (STL)](https://github.com/jw3422951-ops/credit-can/raw/refs/heads/main/models/electronic-game-assembly.stl)

[Back to my GitHub profile](https://github.com/jw3422951-ops)
