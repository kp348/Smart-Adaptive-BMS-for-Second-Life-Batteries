IEEE Hack -Smart Adaptive BMS for Second-Life Batteries

This repository contains the schematic for the **Battery Monitoring System (BMS)** designed using **KiCad** as part of the IEEE Hack project.
Our idea is To monitor the real-time status of a battery (voltage, current, temperature, state of charge) and protect it from overcharging, 
deep discharging, overheating, or short circuits — thereby extending its life and ensuring safety.
A battery monitoring system is needed due to following reasons:
1. Safety
   Prevents hazards like fire or explosion by shutting off the battery during overcharge, over-discharge, or over-temperature.
2. Performance
  Ensures batteries operate within safe and optimal limits, maximizing their efficiency.
3. Longevity
  Avoids deep discharge or high-temperature stress, extending the battery’s lifespan.
4. Data Monitoring
  Enables users (or systems) to track battery health, usage cycles, and predict failures.

The battery monitoring system can be used in :
Electric Vehicle
Robots
Drones
Li-ion batteries appliactions
## 🧰 Project Overview

- **Tool Used:** KiCad 9
- **Target Hardware:** ESP32-WROOM-32,TP-4056 ,AK2112(Load Regulator),INA219(Current and voltage sensing),MicroSD Card, NTC(Negative Temperature Coeficient)
- **Main Features:**
  - Battery voltage and current sensing
  - Temperature monitoring with NTC
  - Safe charging using TP4056
 
  Steps to run the project:
  1. Follow the README instructions.
  2. Open IEEE Hack.pdf to get access to the schematic.
     Component	                        Purpose
      ESP32-WROOM-32	                  Main controller
      INA219	                        Measures battery voltage and current
      LM35 or NTC Thermistor	         Measures battery temperature
      TP4056	                        Lithium-ion charger (with status pins)
      IRLZ44N	                        Controls load via MOSFET switching
      MicroSD Card Module	            Logs data to SD card
      Battery	                        Second-life Lithium-ion cell
  3. Follow the code given in IEEE.ino and use it in Arduino IDE.
  4. Open Arduino IDE :
     Go to Tools > Board > ESP32 Dev Module
     Select the correct COM port under Tools > Port
  5.  Install Required Libraries
      Install these via Sketch > Include Library > Manage Libraries:
      Adafruit INA219
      WiFi
      SD
      SPI
      WebServer (comes with ESP32 package)
  6. Upload code in IEEE.ino to ESP32
  7. Open Serial Monitor to get the IP address.
  8.  Set Your Wi-Fi Credentials
      In the .ino file, update:
      const char* ssid = "YOUR_WIFI_NAME";
      const char* password = "YOUR_WIFI_PASSWORD";
  9.  Once ESP32 connects to Wi-Fi, it prints its IP and open that IP in ypur browser.
  10. The real time data from ESP32 will be seen in the web page linked to repo(index.html).
  11. For the representation of web page. Please visit the following link:
      https://kp348.github.io/Smart-Adaptive-BMS-for-Second-Life-Batteries/

