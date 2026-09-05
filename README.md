# 🐝 Beehive Monitoring System

An ESP32-based IoT system for real-time beehive monitoring. It collects acoustic, weight, and climate data from the hive and publishes it to **Adafruit IO** via MQTT, then enters deep sleep to conserve battery.

---

## 📋 Features

- **Acoustic Analysis** — INMP441 I2S microphone + FFT to classify hive status:
  - `Healthy / Normal Hive Hum` (200–350 Hz)
  - `WARNING: Highly Agitated` (350–600 Hz)
  - `Empty / Silent Hive`
  - `Ambient Noise / Inactive`
- **Weight Monitoring** — HX711 load cell amplifier measures hive mass
- **Climate Sensing** — DHT22 captures internal temperature and humidity
- **MQTT Publishing** — Sends all data to Adafruit IO on four separate feeds
- **Deep Sleep** — ESP32 sleeps between cycles to extend battery life

---

## 🛠 Hardware Required

| Component | Purpose |
|-----------|---------|
| ESP32 Dev Board | Main microcontroller |
| INMP441 I2S Microphone | Acoustic monitoring |
| HX711 + Load Cell | Hive weight measurement |
| DHT22 | Temperature & humidity |

### Pin Configuration

| Signal | ESP32 GPIO |
|--------|------------|
| I2S WS (Word Select) | 25 |
| I2S SD (Data) | 32 |
| I2S SCK (Clock) | 33 |
| HX711 DOUT | 5 |
| HX711 SCK | 4 |
| DHT22 Data | 26 |

---

## 📦 Libraries Required

Install these via Arduino Library Manager or PlatformIO:

- [`arduinoFFT`](https://github.com/kosme/arduinoFFT)
- [`HX711`](https://github.com/bogde/HX711)
- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library) by Adafruit
- [`Adafruit MQTT Library`](https://github.com/adafruit/Adafruit_MQTT_Library)
- `WiFi` (built into ESP32 Arduino core)
- `driver/i2s` (built into ESP32 Arduino core)

---

## ⚙️ Configuration

Open `BHMS.ino` and update the configuration block at the top of the file:

```cpp
// Wi-Fi
const char* WLAN_SSID = "YOUR_WIFI_SSID";
const char* WLAN_PASS = "YOUR_WIFI_PASSWORD";

// Adafruit IO
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"
```

You can find your Adafruit IO key at [io.adafruit.com](https://io.adafruit.com) → My Key.

### Adafruit IO Feeds Required

Create the following feeds in your Adafruit IO dashboard:

| Feed Name | Data |
|-----------|------|
| `hive-status` | JSON: `{"status":"...", "frequency":..., "amplitude":...}` |
| `hive-weight` | Float (grams) |
| `hive-temp` | Float (°C) |
| `hive-humid` | Float (%) |

---

## 🔧 Tuning Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `CALIBRATION_FACTOR` | `-100` | Adjust until weight reads accurately |
| `AMPLITUDE_THRESHOLD` | `3000.0` | Minimum amplitude to count as bee sound |
| `DESERTED_THRESHOLD` | `800.0` | Below this → hive is silent |
| `SENSING_DURATION_MS` | `60000` | Acoustic sampling window (ms) |
| `DEEP_SLEEP_TIME_SEC` | `200` | Sleep between cycles (seconds) |

---

## 🔄 Operation Cycle

```
Wake from deep sleep
    │
    ├─ 1. Read hive weight (HX711)
    ├─ 2. Read temperature & humidity (DHT22)
    ├─ 3. Record & analyse hive acoustics for 60s (INMP441 + FFT)
    ├─ 4. Connect to Wi-Fi → Connect to MQTT → Publish all feeds
    └─ 5. Deep sleep for ~3.3 minutes → repeat
```

Wi-Fi is intentionally kept **off** during sensor sampling to avoid interference and save power.

---

## 📡 MQTT Payload Example

**hive-status feed:**
```json
{"status":"Healthy / Normal Hive Hum","frequency":280.5,"amplitude":4120}
```

---

## ⚠️ Security Notice

Never commit real credentials to a public repository. This file uses placeholder strings. Store secrets in environment variables or a private config file if needed.

---

## 📄 License

MIT License — feel free to use, modify, and share.
