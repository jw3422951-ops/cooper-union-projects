# Cooper Union Summer Projects

Across two summers at Cooper Union, I helped develop a recycling station concept with a team and then independently built a working waste sorter based on that idea.

| Year 1 · CreditCAN team model | Year 2 · Working sorter (2026) |
| --- | --- |
| <img src="assets/credit-can-model.jpg" alt="CreditCAN team model and its project poster" width="310"> | <img src="assets/year-2-sorter.jpg" alt="Working sorter with a transparent enclosure, rotating chute, wiring, and controller" width="310"> |

## Year 1 · CreditCAN team concept

**3D Fabrication · Honor Award**

We interviewed passersby about neighborhood conditions. Many said they were generally satisfied, yet we observed litter in the park and around nearby restaurants. That contrast led us to ask how we could encourage proper waste disposal in shared spaces.

We designed CreditCAN as a station that would award credit at nearby businesses for deposited items. Our team built and spray-painted a display model and designed the logo. A teammate and I proposed public-space sanitation as the theme. I suggested an automated collection bin, helped define its purpose and overall structure, and modeled the team prototype in Onshape.

## Year 2 · Working waste sorter (2026)

I designed and built this working sorter myself, including the mechanical assembly and the integration of the sensors and motors with an Arduino Mega 2560. During fabrication I revised parts of the original mechanism and developed a classification method using three low-cost sensors.

### Sensing and classification

| Sensor | Role |
| --- | --- |
| Inductive proximity sensor (NPN, normally closed) | Checks for metal. |
| Load cell and HX711 | Measures item mass. |
| VL53L0X time-of-flight sensor | Measures distance to the item, which the system uses to estimate its height. [Sensor documentation](https://www.st.com/en/imaging-and-photonics-solutions/vl53l0x.html) |

The sorter checks for metal first. For other items, the classifier combines mass and height information from the sensors. It has three sorting routes for plastic and paper recyclables, metal cans, and food waste. A fourth route rejects items outside the calibrated weight or classification limits.

### Mechanical layout and control

- **Top.** A fixed weighing platform holds the item while the sensors take readings.
- **Middle.** A ring gear and pinion turn the chute toward the selected route. A rack and pinion drive the pusher.
- **Bottom.** Fixed bins collect the sorted items.

I designed the finger-jointed enclosure in Onshape, laser-cut the panels, and generated the gears and racks with FeatureScript for 3D printing. A C++ state machine coordinates the sensor readings and motor movements. I calibrated thresholds and motor step counts on the physical build and recorded them as named constants.

<img src="assets/year-2-chute-detail.jpg" alt="Close-up of the printed rotating chute, pinion, and gear assembly" width="380">

### Liquid detection and build refinements

I tried continuous vibration to detect partially filled containers, but I could not get a useful signal that way. I switched to a short vibration pulse and then measured the variance of the load cell readings after the motor stopped to look for liquid sloshing.

I used flexible 30 AWG silicone wire with slack near the load cell to reduce pull on the weighing platform. I sequenced the motors to limit simultaneous current demand on the 5 V, 10 A supply.

[Back to my GitHub profile](https://github.com/jw3422951-ops)
