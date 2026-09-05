<h1 style="text-align: center;">IoT Based Beehive Monitoring System 🐝📡</h1>
<br>
<div class="Intro">
    <h2>Introduction</h2>
    <p>This project is a real-time beehive health monitoring system built using ESP32. It captures bee acoustics using a digital microphone, measures hive weight using a load cell, and monitors temperature & humidity using a DHT22 sensor. All data is transmitted wirelessly to Adafruit IO via MQTT over Wi-Fi.

The system classifies hive status using FFT-based acoustic analysis and uses deep sleep between cycles to conserve battery power.</p>
</div>

<br>

<h2>System Architecture</h2>

<p>The ESP32-WROOM acts as the central controller, collecting data from multiple sensors and publishing it to the Adafruit IO dashboard via MQTT.</p>

<ul>
  <li>INMP441 microphone captures bee acoustic signals.</li>
  <li>FFT analysis classifies hive activity status.</li>
  <li>HX711 + Load Cell measures hive weight.</li>
  <li>DHT22 monitors internal temperature and humidity.</li>
  <li>ESP32 publishes all data to Adafruit IO via MQTT over Wi-Fi.</li>
  <li>ESP32 enters deep sleep between cycles to save power.</li>
</ul>

---

### INMP441 Microphone → ESP32

| INMP441 | ESP32 |
|---------|-------|
| VDD | 3.3V |
| GND | GND |
| SCK | GPIO 33 |
| WS | GPIO 25 |
| SD | GPIO 32 |
| SEL | GND |

---

### HX711 → ESP32

| HX711 | ESP32 |
|-------|-------|
| VCC | 3.3V |
| GND | GND |
| DOUT | GPIO 5 |
| SCK | GPIO 4 |

---

### DHT22 → ESP32

| DHT22 | ESP32 |
|-------|-------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 26 |

---

<h2>Working Principle</h2>

<p>The ESP32 wakes from deep sleep and sequentially reads all sensors. The INMP441 microphone records bee sounds for 60 seconds, and FFT is applied to identify the dominant frequency. Based on the frequency and amplitude, the hive is classified as Healthy, Agitated, Silent, or Inactive.</p>

<p>Once all sensor data is collected, the ESP32 connects to Wi-Fi and publishes the readings to four separate Adafruit IO feeds via MQTT. After publishing, it disconnects and enters deep sleep for the next cycle.</p>

---

<h2>Hive Status Classification</h2>

| Status | Frequency Range |
|--------|----------------|
| Healthy / Normal Hive Hum | 200 – 350 Hz |
| WARNING: Highly Agitated | 350 – 600 Hz |
| Empty / Silent Hive | Below noise floor |
| Ambient Noise / Inactive | Low amplitude |

---

<h2>Adafruit IO Feeds</h2>

| Feed | Data |
|------|------|
| `hive-status` | JSON: status, frequency, amplitude |
| `hive-weight` | Float (grams) |
| `hive-temp` | Float (°C) |
| `hive-humid` | Float (%) |

---

<h2>Libraries Required</h2>

<ul>
  <li><a href="https://github.com/kosme/arduinoFFT">arduinoFFT</a></li>
  <li><a href="https://github.com/bogde/HX711">HX711</a></li>
  <li><a href="https://github.com/adafruit/DHT-sensor-library">DHT sensor library</a> by Adafruit</li>
  <li><a href="https://github.com/adafruit/Adafruit_MQTT_Library">Adafruit MQTT Library</a></li>
  <li>WiFi — built into ESP32 Arduino core</li>
  <li>driver/i2s — built into ESP32 Arduino core</li>
</ul>

---

<h2>Future Improvements</h2>

<ul>
  <li>Integrate Random Forest ML model for smarter hive classification</li>
  <li>Add ESP32-CAM for visual hive monitoring</li>
  <li>Implement two-way communication for hive control</li>
  <li>Add LoRa support for long-range, off-grid monitoring</li>
  <li>Improve audio compression for faster FFT processing</li>
</ul>
