# 🪑 Intellichair
<img width="1097" height="543" alt="image" src="https://github.com/user-attachments/assets/ab02113c-0a97-4d13-b0fd-c7df51466992" />

## 🧩 Hardware Used
- Arduino Uno / Nano (microcontroller)

- 4 × Full Bridge Load Cells (weight sensors)

- 4 × HX711 Amplifier Modules (to read load cell signals)

- Buzzer Module (posture alert system)

## 🔧 Sensor Module (Load Cell + HX711)
The sensor module consists of a load cell paired with an HX711 amplifier.
- **Load Cell:** A strain gauge-based sensor that measures the force (weight) applied to it. Each corner of the chair has one load cell, allowing the system to calculate weight distribution.

- **HX711 Amplifier:** A precision 24-bit ADC (Analog-to-Digital Converter) specifically designed for weight scales. It converts the tiny electrical signals from the load cells into readable digital values for the Arduino.
