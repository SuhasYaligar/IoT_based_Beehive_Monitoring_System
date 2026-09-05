#include <WiFi.h>
#include <driver/i2s.h>
#include "arduinoFFT.h"
#include "HX711.h"
#include "DHT.h"              
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ======================== CONFIGURATION ========================
// Wi-Fi Credentials — replace with your own
const char* WLAN_SSID = "YOUR_WIFI_SSID";
const char* WLAN_PASS = "YOUR_WIFI_PASSWORD";

// Adafruit IO MQTT Settings — replace with your own
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"

// Separated Feed Target Paths
#define STATUS_FEED_PATH "/feeds/hive-status" 
#define WEIGHT_FEED_PATH "/feeds/hive-weight"
#define TEMP_FEED_PATH   "/feeds/hive-temp"    // Dedicated Temperature Feed
#define HUMID_FEED_PATH  "/feeds/hive-humid"   // Dedicated Humidity Feed

// INMP441 Microphone Pins
#define I2S_WS 25
#define I2S_SD 32
#define I2S_SCK 33
#define I2S_PORT I2S_NUM_0

// HX711 Load Cell Pins
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
const float CALIBRATION_FACTOR = -100; 
const long FIXED_TARE_OFFSET = 0; 

// DHT22 Climate Sensor Pins
#define DHTPIN 26          
#define DHTTYPE DHT22     

// Audio Parameters
#define SAMPLES 1024        
#define SAMPLING_FREQ 16000 

#define AMPLITUDE_THRESHOLD     3000.0  
#define DESERTED_THRESHOLD      800.0   
#define SMOOTHING_FACTOR        5      

#define DEEP_SLEEP_TIME_SEC     200   // Deep sleep interval in seconds  
#define SENSING_DURATION_MS     60000  // Hive sound monitoring for 60 seconds 
// ===============================================================

// Local Struct to return dual float elements cleanly
struct ClimateData {
  float temperature;
  float humidity;
};

double vReal[SAMPLES];
double vImag[SAMPLES];
int32_t raw_samples[SAMPLES];

float freq_history[SMOOTHING_FACTOR] = {0};
int history_index = 0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Registered Publisher Handles
Adafruit_MQTT_Publish hiveStatusPub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hive-status");
Adafruit_MQTT_Publish hiveWeightPub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hive-weight");
Adafruit_MQTT_Publish hiveTempPub   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hive-temp");
Adafruit_MQTT_Publish hiveHumidPub  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hive-humid");

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);
HX711 scale;
DHT dht(DHTPIN, DHTTYPE); 

void setup_i2s() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLING_FREQ,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, 
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1, 
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

// 1. DEDICATED METHOD: Get Hive Weight
float get_hive_weight() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.set_offset(FIXED_TARE_OFFSET); 
  
  float weight = scale.get_units(20); 
  if (weight > -2.0 && weight < 2.0) {
    weight = 0.0;
  }
  
  Serial.print("Measured Weight: ");
  Serial.print(weight, 1);
  Serial.println(" g");
  return weight;
}

// 2. DEDICATED METHOD: Get Hive Climate Data
ClimateData get_hive_humtemp() {
  dht.begin();
  delay(2000); // Required initialization stabilization window
  
  ClimateData data;
  data.humidity = dht.readHumidity();
  data.temperature = dht.readTemperature(); 

  if (isnan(data.humidity) || isnan(data.temperature)) {
    Serial.println("Error: Failed to read from DHT22 sensor!");
    data.temperature = 0.0;
    data.humidity = 0.0;
    return data;
  }

  Serial.print("Climate Status -> Temp: ");
  Serial.print(data.temperature, 1);
  Serial.print("C | Hum: ");
  Serial.print(data.humidity, 1);
  Serial.println("%");

  return data;
}

// 3. DEDICATED METHOD: Get Hive Audio Status (JSON format)
String get_hive_status() {
  setup_i2s();
  
  unsigned long sensing_start = millis();
  long healthyCount = 0;
  long agitatedCount = 0;
  long silentCount = 0;
  
  double runningFreqSum = 0;
  double runningAmpSum = 0;
  long validSampleCount = 0;

  Serial.println("Sampling hive acoustics for 60 seconds...");
  
  while (millis() - sensing_start < SENSING_DURATION_MS) {
    size_t bytes_read = 0;
    esp_err_t result = i2s_read(I2S_PORT, &raw_samples, sizeof(raw_samples), &bytes_read, portMAX_DELAY);
    
    if (result == ESP_OK && bytes_read > 0) {
      int samples_read = bytes_read / 4; 
      for (int i = 0; i < samples_read; i++) {
        vReal[i] = (double)(raw_samples[i] >> 14); 
        vImag[i] = 0.0;
      }

      FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.compute(FFT_FORWARD);
      FFT.complexToMagnitude();

      double peak = FFT.majorPeak();
      int peakIndex = round((peak * SAMPLES) / SAMPLING_FREQ);
      double peakAmplitude = vReal[peakIndex];

      // --- SENSING EVALUATION HIERARCHY ---
      if (peakAmplitude < AMPLITUDE_THRESHOLD) {
        // Sound below working noise floor — count as a silent moment
        if (peakAmplitude < DESERTED_THRESHOLD) {
          silentCount++;
        }
      } 
      else if (peak >= 200.0 && peak <= 600.0) {
        // Passes amplitude threshold and within bee frequency band — valid bee sound
        freq_history[history_index] = peak;
        history_index = (history_index + 1) % SMOOTHING_FACTOR;

        float smoothed_freq = 0;
        int valid_readings = 0;
        for (int i = 0; i < SMOOTHING_FACTOR; i++) {
          if (freq_history[i] > 0) {
            smoothed_freq += freq_history[i];
            valid_readings++;
          }
        }
        smoothed_freq /= valid_readings;

        runningFreqSum += smoothed_freq;
        runningAmpSum += peakAmplitude;
        validSampleCount++;

        if (smoothed_freq >= 200.0 && smoothed_freq <= 350.0) {
          healthyCount++;
        } else if (smoothed_freq > 350.0 && smoothed_freq <= 600.0) {
          agitatedCount++;
        }
      }
    }
    delay(50); 
  }

  double finalAverageFreq = 0.0;
  double finalAverageAmp = 0.0;
  String final_status = "Ambient Noise / Inactive";

  if (validSampleCount > 0) {
    finalAverageFreq = runningFreqSum / validSampleCount;
    finalAverageAmp = runningAmpSum / validSampleCount;
  }

  // --- VOTING DECISION ---
  if (validSampleCount > 0) {
    if (agitatedCount > healthyCount) {
      final_status = "WARNING: Highly Agitated";
    } else {
      final_status = "Healthy / Normal Hive Hum";
    }
  } else if (silentCount > 0) {
    final_status = "Empty / Silent Hive";
  }

  i2s_driver_uninstall(I2S_PORT);

  char jsonBuffer[256];
  snprintf(jsonBuffer, sizeof(jsonBuffer), 
           "{\"status\":\"%s\",\"frequency\":%.1f,\"amplitude\":%.0f}", 
           final_status.c_str(), finalAverageFreq, finalAverageAmp);

  return String(jsonBuffer);
}

void connect_wifi_and_mqtt(String jsonPayload, float currentWeight, ClimateData climate) {
  // Connect to Wi-Fi only after all sensor processing is complete
  Serial.print("\nConnecting to Wi-Fi: ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  unsigned long wifiTimeout = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - wifiTimeout > 15000) {
      Serial.println("\nWi-Fi connection failed. Skipping transmission.");
      return;
    }
  }
  Serial.println("\nWi-Fi Connected!");

  // Connect to MQTT Broker
  int8_t ret;
  uint8_t retries = 3;
  Serial.println("Connecting to Adafruit IO MQTT...");
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       mqtt.disconnect();
       delay(2000);
       retries--;
       if (retries == 0) {
         Serial.println("MQTT Connection Failed.");
         return;
       }
  }
  Serial.println("MQTT Broker Connected!");

  // Publish sequentially with 1-second rate-limiting delays
  if (hiveStatusPub.publish(jsonPayload.c_str())) {
    Serial.println("Published Status JSON successfully!");
  } else {
    Serial.println("Status JSON failed!");
  }
  delay(1000); 

  if (hiveWeightPub.publish(currentWeight, 1)) {
    Serial.println("Published Weight successfully!");
  } else {
    Serial.println("Weight failed!");
  }
  delay(1000); 

  if (hiveTempPub.publish(climate.temperature, 1)) {
    Serial.println("Published Temperature successfully!");
  } else {
    Serial.println("Temperature failed!");
  }
  delay(1000); 

  if (hiveHumidPub.publish(climate.humidity, 1)) {
    Serial.println("Published Humidity successfully!");
  } else {
    Serial.println("Humidity failed!");
  }
  delay(500); 
  
  Serial.println("All individual feeds published successfully.");
  mqtt.disconnect();
  WiFi.disconnect(true);
}

void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println("\n--- Hive Awake Cycle Triggered ---");
  
  // Step 1: Get weight
  float hive_weight = get_hive_weight();
  
  // Step 2: Get climate data
  ClimateData hive_climate = get_hive_humtemp();
  
  // Step 3: Run acoustic recording loop
  String hive_status_json = get_hive_status();

  // Step 4: All data collected — connect and publish
  connect_wifi_and_mqtt(hive_status_json, hive_weight, hive_climate);

  // Step 5: Enter deep sleep
  Serial.println("Entering deep sleep...");
  esp_sleep_enable_timer_wakeup((uint64_t)DEEP_SLEEP_TIME_SEC * 1000000);
  esp_deep_sleep_start();
}

void loop() {}
