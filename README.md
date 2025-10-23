# Undervoltage and Overvoltage Protection System ⚡️

This project implements a **voltage monitoring and protection system** using an **Arduino Uno** to safeguard electrical appliances from damaging voltage fluctuations, specifically **undervoltage** (brownouts) and **overvoltage** (surges).

It utilizes an LCD to display the real-time voltage and system status and a relay to automatically disconnect the load (e.g., a light bulb) when the voltage is outside the safe operating range.

---

## 📸 System Overview

The image below shows the assembled hardware, including the Arduino, voltage sensor, relay module, and LCD display.

![Simulation Circuit.jpg](https://github.com/harshvardhan-sharma-X/Undervoltage_and_Overvoltage_protection_system/blob/main/Simulation%20Circuit.png?raw=true)
![Normal Image.jpg](https://github.com/harshvardhan-sharma-X/Undervoltage_and_Overvoltage_protection_system/blob/main/Simulation%20Circuit.png?raw=true)
*(Note: Replace the placeholder image URL with the actual link to your hosted image.)*

---

## 💡 Key Features

* **Real-time Voltage Monitoring:** Continuously measures the AC voltage.
* **Automatic Protection:** Uses a relay to disconnect the load when the voltage falls below the **180V** undervoltage threshold or exceeds the **240V** overvoltage threshold.
* **Visual Feedback:** An **LCD display** shows the current voltage (Vrms) and the system status (**Normal**, **Under Volt**, or **Over Volt**).
* **Serial Monitoring:** Outputs voltage data to the Serial Monitor for debugging and data logging.

---

## 🛠️ Hardware Requirements

| Component | Description | Quantity |
| :--- | :--- | :--- |
| **Arduino Uno** | Microcontroller | 1 |
| **Voltage Sensor** | To measure AC voltage (e.g., ZMPT101B module or similar) | 1 |
| **Relay Module** | Single-channel module to switch the AC load | 1 |
| **LCD Display** | 16x2 or 20x4 I2C-enabled (e.g., with PCF8574 adapter) | 1 |
| **Breadboard** | For prototyping and connections | 1 |
| **Connecting Wires** | Jumper wires | Assorted |
| **Load** | An AC appliance, such as a light bulb (for demonstration) | 1 |

---

## 🔌 Wiring and Connections

| Component | Connection | Notes |
| :--- | :--- | :--- |
| **Voltage Sensor** | Analog Pin **A0** | Voltage sensor output to Arduino analog input. |
| **Relay Module (Input)** | Digital Pin **10** | Connects to the control pin of the relay module. |
| **LCD (SDA)** | Analog Pin **A4** (or corresponding I2C pin) | I2C Data Line |
| **LCD (SCL)** | Analog Pin **A5** (or corresponding I2C pin) | I2C Clock Line |
| **Relay (Output)** | In series with the AC Load | Connects to the AC line to switch power on/off. |

---

## ⚙️ Software and Libraries

This project requires the following libraries to be installed in your Arduino IDE:

1.  **`LiquidCrystal_I2C`**: For communicating with the I2C LCD display.
2.  **`EmonLib`**: The OpenEnergyMonitor Library for robust and accurate AC voltage measurement.

You can install these libraries directly through the Arduino IDE's Library Manager (**Sketch -> Include Library -> Manage Libraries...**).

---

## 👨‍💻 Code Explanation

### 1. The Code

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EmonLib.h>

const int relay = 10;  

EnergyMonitor emon;

float current_Volts;
unsigned long printPeriod = 500;
unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4); 

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(relay, OUTPUT);

  // Calibration: A0 pin, AC RMS value, Phase shift.
  // 234.0 is the default calibration constant, which you may need to fine-tune.
  emon.voltage(A0, 234.0, 1.7); 
  Serial.begin(9600);
}

void loop() {
  // Calculate voltage over 20 cycles and 2000 samples
  emon.calcVI(20, 2000);      
  current_Volts = emon.Vrms;   

  if (millis() - previousMillis >= printPeriod) {
    previousMillis = millis();

    // Display Voltage
    lcd.setCursor(0, 0);
    lcd.print("Voltage: ");
    lcd.print(current_Volts, 1);
    lcd.print(" V     ");

    // --- PROTECTION LOGIC ---
    if (current_Volts < 180) {
      // Undervoltage detected: Disconnect Load (Relay HIGH)
      lcd.setCursor(0, 1);
      lcd.print("Status: Under Volt ");
      digitalWrite(relay, HIGH);
    } 
    else if (current_Volts > 240) {
      // Overvoltage detected: Disconnect Load (Relay HIGH)
      lcd.setCursor(0, 1);
      lcd.print("Status: Over Volt  ");
      digitalWrite(relay, HIGH);
    } 
    else {
      // Normal voltage: Connect Load (Relay LOW)
      lcd.setCursor(0, 1);
      lcd.print("Status: Normal     ");
      digitalWrite(relay, LOW);
    }

    // Output to Serial Monitor for debugging
    Serial.print(current_Volts);  
    Serial.print(" ");
    Serial.print(180);            
    Serial.print(" ");
    Serial.print(240);            
    Serial.println(" ");
  }
}
