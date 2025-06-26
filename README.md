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
