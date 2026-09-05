# 🐝 IoT Based Beehive Monitoring System

**Synopsis Report — "IoT Based Beehive Monitoring System"**  
Submitted in partial fulfilment of the requirement for completion of **PROJECT WORK 2 [23EC7PWPJ2]**

| Name | USN |
|------|-----|
| Suhas V Y | 1BM23EC268 |
| Sumukh Shetty J S | 1BM23EC269 |
| Tejas H B | 1BM23EC279 |
| Sampatkumar B H | 1BM23EC226 |

**Under the guidance of:** Mrs. Sowmya Sunkara, Assistant Professor, Dept. ECE  
**BMS College of Engineering, Bengaluru**

---

## 📋 Abstract

Beekeeping is important for agriculture and the environment, but regularly checking the health of a bee colony manually can be difficult and may disturb the bees. To overcome this, this project proposes a **Smart Beehive Health Monitoring System** that continuously monitors the hive without the need to open it frequently.

The system uses an **ESP32-WROOM** as the main controller. Two **DHT22 sensors** measure the temperature and humidity inside and outside the hive, while a **20 kg load cell with HX711** monitors changes in hive weight. An **INMP441 microphone** records bee sounds, and **FFT analysis** is used to study changes in their acoustic activity. An **ESP32-CAM** is also used to visually monitor the hive.

The collected data is processed to obtain features such as **temperature difference, humidity difference, weight change, and dominant sound frequency**. These features are given to a **Random Forest machine-learning model** to estimate the hive's health or activity status. The system uses **Wi-Fi, MQTT, and Adafruit IO** to display live readings, graphs, and alerts remotely.

Overall, the system helps in **early detection of abnormal hive conditions, reduces manual inspection, supports better decision-making, and promotes efficient and sustainable beekeeping**.

---

## 🌐 Area of Application / Domain

This project falls under the domains of **Embedded Systems, Smart Agriculture (Agri-Tech), IoT, Environmental Monitoring, Machine Learning, and Digital Signal Processing**.

It combines:
- Real-time embedded sensing and monitoring using ESP32-WROOM
- Temperature and humidity monitoring using DHT22 sensors
- Hive weight monitoring using a 20 kg load cell and HX711
- Bee acoustic signal processing using an INMP441 microphone and FFT analysis
- Machine-learning-based hive activity/health classification using Random Forest
- Visual hive monitoring using ESP32-CAM
- Wireless communication and remote monitoring using Wi-Fi, MQTT, and Adafruit IO
- Remote alerts and intelligent monitoring for early detection of abnormal hive conditions

**Application Areas:**
- Commercial beekeeping and smart apiculture
- Smart agriculture and precision beekeeping
- Agricultural pollination management
- Remote monitoring of bee colonies
- Environmental and ecological research
- Early detection of abnormal hive conditions
- Honey production and hive productivity analysis
- IoT and machine-learning-based agricultural automation

---

## 📖 Introduction

Modern beekeeping requires continuous monitoring of hive conditions to maintain colony health and productivity. Traditional manual inspection is time-consuming, may disturb the bees, and can miss abnormal conditions at an early stage.

This project proposes a **Smart Beehive Health Monitoring System** using an **ESP32-WROOM** as the main controller. The system combines multiple sensing and analysis techniques:

- **DHT22:** Monitors temperature and humidity inside and outside the hive
- **Load Cell + HX711:** Measures hive weight and detects weight variations
- **INMP441 + FFT:** Captures and analyzes bee sound patterns
- **ESP32-CAM:** Provides visual monitoring of hive activity
- **Random Forest:** Uses extracted features to estimate hive health/activity status
- **MQTT + Adafruit IO:** Provides remote data visualization and alerts

The system collects sensor data continuously and derives useful parameters such as **temperature difference, humidity difference, weight variation, and dominant sound frequency**. These parameters are combined to provide a better understanding of changes occurring inside and around the hive.

By combining **embedded sensing, signal processing, machine learning, visual monitoring, and IoT connectivity**, the system provides a more complete approach to hive monitoring. The main aim is to enable **early detection of abnormal hive activity, reduce frequent manual inspection, support better decision-making, and promote efficient and sustainable beekeeping**.

---

## 📚 Literature Survey

1. *"A Smart Sensor-Based Measurement System for Advanced Bee Hive Monitoring"* — **A. Zacepins, E. Stalidzans, and J. Meitalovs** — Use of multiple sensors to monitor internal hive parameters such as temperature, humidity, and environmental conditions for early detection of abnormal hive conditions.

2. *"IoT-Based Beehive Monitoring System for Real-Time Monitoring of Bee Colonies"* — **S. Zacepins, A. Kviesis, and E. Stalidzans** — IoT technology for remote hive monitoring using temperature and humidity sensors; demonstrates that real-time monitoring reduces manual inspection.

3. *"Real-Time Monitoring System of a Beehive Using Wireless Sensor Networks"* — **V. Ferrari, D. Striani, and M. Paci** — Wireless collection and transmission of hive environmental data to highlight the importance of automation in modern apiculture.

4. *"Automated Beehive Acoustics Monitoring: A Comprehensive Review"* — **J. A. Cecchi, M. Terenzi, and G. Spinsante** — Acoustic signal analysis to detect bee behavior; changes in buzzing frequency can indicate stress, queen absence, or swarming activity.

5. *"IoT-Based Smart Beehive Monitoring System Using Weight Measurement"* — **A. Kviesis and E. Stalidzans** — Load cell-based hive weight monitoring for analyzing nectar intake and predicting honey production.

6. *"Monitoring Carbon Dioxide Concentration in Honey Bee Hives for Colony Health Assessment"* — **J. Van Nerum and D. B. Berckmans** — CO₂ levels inside the hive indicate ventilation quality and colony density.

7. *"IoT Integrated Multi-Sensor Beehive Monitoring for Real-Time Bee Colony Health Analysis"* — **M. T. Al-Ghadi, H. Alshamrani, and A. Al-Ghamdi** — Multi-parameter monitoring combining environmental sensing and intelligent analysis.

8. *"Smart Apiculture: Precision Beekeeping Using Sensor Technology"* — **M. Meikle and N. Holst** — Modern smart beekeeping solutions integrating sensing with control mechanisms to improve hive productivity.

---

## ❗ Problem Definition & Problem Statement

### Problem Definition

Maintaining a healthy bee colony requires regular monitoring, but traditional manual inspection is time-consuming, may disturb the bees, and cannot provide continuous information about hive conditions. Changes in **temperature, humidity, weight, and bee activity** may therefore go unnoticed.

### Problem Statement

Bee colonies are sensitive to changes in temperature, humidity, weight, and bee activity, which can affect their health and productivity. Existing systems often provide basic sensor readings without combining **acoustic, visual, and intelligent analysis**, limiting early detection of abnormal activity.

There is a need for a **low-cost and intelligent beehive monitoring system** that continuously collects hive parameters, analyzes bee sounds using **FFT**, provides visual monitoring through **ESP32-CAM**, and estimates hive activity using a **Random Forest model**, with remote monitoring via **Wi-Fi, MQTT, and Adafruit IO**.

---

## 💡 Proposed Solution

| # | Identified Problem | Proposed System Solution |
|---|-------------------|--------------------------|
| 1 | Manual hive inspection disturbs bees and may miss early problems | Continuous real-time monitoring using multiple sensors without frequently opening the hive |
| 2 | Changes in temperature and humidity affect colony health | DHT22 sensors monitor inside and outside temperature and humidity continuously |
| 3 | Difficulty in detecting changes in bee activity | INMP441 microphone with FFT analysis identifies changes in bee acoustic activity |
| 4 | Lack of real-time information about hive weight | 20 kg load cell with HX711 monitors hive weight and detects weight variations |
| 5 | Difficulty in visually observing hive activity remotely | ESP32-CAM provides remote visual monitoring of the hive |
| 6 | Multiple sensor readings are difficult to analyze manually | Random Forest ML model analyzes extracted features to estimate hive activity or health status |

---

## 🔧 Project Execution Steps

1. **Requirement Analysis & System Planning** — Define project objectives and finalize system architecture and components.
2. **Hardware Setup** — Configure ESP32-WROOM as main controller; connect DHT22, load cell + HX711, INMP441, and ESP32-CAM.
3. **Sensor Data Acquisition** — Collect temperature, humidity, hive weight, and bee acoustic signals; configure ESP32-CAM.
4. **Signal Processing & Feature Extraction** — Apply FFT to bee sound signals; calculate temperature difference, humidity difference, and weight variation.
5. **Machine Learning Implementation** — Prepare extracted feature data; train and implement a Random Forest model to estimate hive activity/health status.
6. **IoT Integration** — Connect to Wi-Fi; use MQTT with Adafruit IO to display sensor values, graphs, predicted status, and alerts remotely.
7. **System Integration & Testing** — Integrate all hardware, software, ML, camera, and IoT modules; test accuracy, communication, and reliability.
8. **Final Prototype & Demonstration** — Assemble and demonstrate real-time monitoring, FFT analysis, ML prediction, camera monitoring, and IoT dashboard.

---

## 🛒 Estimated Bill of Materials

| SI.No | Component | Qty | Approx Cost (₹) | Description |
|-------|-----------|-----|-----------------|-------------|
| 1 | ESP32 Development Board | 1 | 500 | Main controller with Wi-Fi |
| 2 | Beehive Box (Wooden) | 1 | 2500 | Physical hive structure |
| 3 | DHT22 Temperature & Humidity Sensor | 1 | 400 | Hive temp & humidity |
| 4 | Load Cell (20 kg) | 1 | 300 | Hive weight measurement |
| 5 | HX711 Amplifier Module | 1 | 150 | Load cell interface |
| 6 | MQ135 Gas/Air Quality Sensor | 1 | 200 | Internal air quality |
| 7 | INMP441 Microphone Module | 1 | 150 | Bee sound for FFT |
| 8 | OLED Display (optional) | 1 | 400 | Local data display |
| 9 | Mini DC Fan (optional) | 1 | 150 | Assisted ventilation |
| 10 | Power Supply/Battery + Charger | 1 | 400 | Power management |
| 11 | Breadboard/PCB + Jumper Wires | 1 | 200 | Prototype wiring |
| 12 | Enclosure / Hive Mount | 1 | 300 | Electronics protection |
| 13 | LoRa Module | 1 | 700 | Long-range wireless communication |
| 14 | ESP32-CAM Module | 1 | 900 | Visual hive/bee activity monitoring |

**Total Estimated Cost: ₹7,500 – ₹13,000 (Prototype Level)**

---

## 🔌 Hardware Pin Configuration

| Signal | ESP32 GPIO |
|--------|------------|
| I2S WS (Word Select) | 25 |
| I2S SD (Data In) | 32 |
| I2S SCK (Clock) | 33 |
| HX711 DOUT | 5 |
| HX711 SCK | 4 |
| DHT22 Data | 26 |

---

## 📦 Libraries Required

Install via Arduino Library Manager:
- [`arduinoFFT`](https://github.com/kosme/arduinoFFT)
- [`HX711`](https://github.com/bogde/HX711)
- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library) by Adafruit
- [`Adafruit MQTT Library`](https://github.com/adafruit/Adafruit_MQTT_Library)
- `WiFi` and `driver/i2s` (built into ESP32 Arduino core)

---

## ⚙️ Configuration

Open `IoT_based_Beehive_Monitoring_System.ino` and update the configuration block:

```cpp
const char* WLAN_SSID = "YOUR_WIFI_SSID";
const char* WLAN_PASS = "YOUR_WIFI_PASSWORD";

#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"
```

Create these feeds in your [Adafruit IO](https://io.adafruit.com) dashboard:

| Feed | Data |
|------|------|
| `hive-status` | JSON: `{"status":"...", "frequency":..., "amplitude":...}` |
| `hive-weight` | Float (grams) |
| `hive-temp` | Float (°C) |
| `hive-humid` | Float (%) |

---

## ⚠️ Security Notice

Never commit real credentials to a public repository. This file uses placeholder strings. Store secrets in a private config file or environment variables.

---

## 📄 License

MIT License — free to use, modify, and share.
