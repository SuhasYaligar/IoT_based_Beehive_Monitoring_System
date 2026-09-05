# 🐝 IoT Based Beehive Monitoring System

An ESP32-based IoT system for real-time beehive health monitoring. It continuously collects acoustic, weight, and climate data from the hive, classifies hive status using FFT analysis, and publishes all data to **Adafruit IO** via MQTT — then enters deep sleep to conserve battery.

---

## 📋 Features

- **Acoustic Analysis** — INMP441 I2S microphone + FFT classifies hive status:
  - `Healthy / Normal Hive Hum` (200–350 Hz)
  - `WARNING: Highly Agitated` (350–600 Hz)
  - `Empty / Silent Hive`
  - `Ambient Noise / Inactive`
- **Weight Monitoring** — HX711 + load cell tracks hive mass over time
- **Climate Sensing** — DHT22 captures temperature and humidity
- **Remote Dashboard** — Live readings, graphs, and alerts via Adafruit IO
- **Deep Sleep** — ESP32 sleeps between cycles to extend battery life

---

## 🛠 Hardware Required

| Component | Purpose |
|-----------|---------|
| ESP32-WROOM Dev Board | Main microcontroller |
| INMP441 I2S Microphone | Acoustic monitoring |
| 20 kg Load Cell + HX711 | Hive weight measurement |
| DHT22 | Temperature & humidity sensing |
| ESP32-CAM *(optional)* | Visual hive monitoring |

### Pin Configuration

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
- `WiFi` — built into ESP32 Arduino core
- `driver/i2s` — built into ESP32 Arduino core

---

## ⚙️ Configuration

Open `IoT_based_Beehive_Monitoring_System.ino` and fill in your credentials at the top:

```cpp
// Wi-Fi
const char* WLAN_SSID = "YOUR_WIFI_SSID";
const char* WLAN_PASS = "YOUR_WIFI_PASSWORD";

// Adafruit IO
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"
```

Get your Adafruit IO key at [io.adafruit.com](https://io.adafruit.com) → My Key.

### Adafruit IO Feeds

Create these four feeds in your Adafruit IO dashboard:

| Feed Name | Data Type | Example |
|-----------|-----------|---------|
| `hive-status` | JSON String | `{"status":"Healthy","frequency":280.5,"amplitude":4120}` |
| `hive-weight` | Float (grams) | `2340.5` |
| `hive-temp` | Float (°C) | `34.2` |
| `hive-humid` | Float (%) | `62.1` |

### Tuning Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `CALIBRATION_FACTOR` | `-100` | Adjust until weight reads correctly |
| `AMPLITUDE_THRESHOLD` | `3000.0` | Minimum amplitude to count as bee sound |
| `DESERTED_THRESHOLD` | `800.0` | Below this → hive considered silent |
| `SENSING_DURATION_MS` | `60000` | Acoustic sampling window (60 seconds) |
| `DEEP_SLEEP_TIME_SEC` | `200` | Sleep duration between cycles (~3.3 min) |

---

## 🔄 Operation Cycle

```
Wake from deep sleep
    │
    ├─ 1. Read hive weight        (HX711 + Load Cell)
    ├─ 2. Read temperature & humidity  (DHT22)
    ├─ 3. Record & analyse acoustics for 60s  (INMP441 + FFT)
    ├─ 4. Connect Wi-Fi → MQTT → Publish all feeds to Adafruit IO
    └─ 5. Enter deep sleep (~3.3 min) → repeat
```

> Wi-Fi is intentionally kept **off** during sensor sampling to avoid RF interference and save power.

---

## 📡 MQTT Payload Example

**hive-status feed:**
```json
{"status":"Healthy / Normal Hive Hum","frequency":280.5,"amplitude":4120}
```

---

## ⚠️ Security Notice

Never commit real credentials to a public repository. This repo uses placeholder strings. For added security, move credentials to a separate `secrets.h` file (already listed in `.gitignore`).

---

## 📄 License

MIT License — free to use, modify, and share.
