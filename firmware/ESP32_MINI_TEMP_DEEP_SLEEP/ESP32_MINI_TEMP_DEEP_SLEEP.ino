#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "driver/temperature_sensor.h"
#include "esp_sleep.h"

#define LED_BUILTIN 8
#define uS_TO_S_FACTOR 1000000  // Conversion factor for microseconds to seconds
#define SLEEP_SECONDS 300       // Sleep for 5 minutes (300 seconds)

const char* ssid = "Get-7368B0";
const char* password = "qjzkld2mjy";
const char* serverUrl = "https://noyanov.com/Apps/Temp/api/update.php";
const char* token = "#tkn25!@";

RTC_DATA_ATTR int bootCount = 0;  // Store in RTC memory to survive deep sleep
temperature_sensor_handle_t temp_handle = NULL;

void blinkPattern(int times, int duration) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
  }
}

float readTemperature() {
  // Initialize temperature sensor if not already done
  if (temp_handle == NULL) {
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
    esp_err_t err = temperature_sensor_install(&temp_sensor_config, &temp_handle);
    if (err != ESP_OK) {
      Serial.printf("Failed to install temp sensor: 0x%x\n", err);
      return -127.0;
    }
  }
  
  // Enable sensor
  esp_err_t err = temperature_sensor_enable(temp_handle);
  if (err != ESP_OK) {
    Serial.printf("Failed to enable sensor: 0x%x\n", err);
    return -127.0;
  }
  
  // Critical stabilization delay
  delay(50);
  
  // Read temperature
  float tsens_out;
  err = temperature_sensor_get_celsius(temp_handle, &tsens_out);
  
  // Disable sensor immediately
  temperature_sensor_disable(temp_handle);
  
  if (err != ESP_OK) {
    Serial.printf("Temp read failed: 0x%x\n", err);
    return -127.0;
  }
  
  Serial.printf("Internal temp: %.2f°C\n", tsens_out);
  return tsens_out;
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("\nWiFi connection failed!");
  }
}

void sendTemperatureData(float temperature) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  // Create JSON payload
  DynamicJsonDocument doc(128);
  doc["token"] = token;
  doc["temperature"] = temperature;
  
  String payload;
  serializeJson(doc, payload);
  Serial.println("Payload: " + payload);
  
  // Create HTTP client
  WiFiClientSecure client;
  client.setInsecure(); // Bypass SSL verification (for low memory)
  
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.setTimeout(10000); // 10-second timeout
  
  // Send request
  Serial.println("Sending POST...");
  int httpCode = http.POST(payload);
  
  // Handle response
  if (httpCode > 0) {
    Serial.printf("HTTP code: %d\n", httpCode);
    String response = http.getString();
    Serial.println("Response: " + response);
    
    if (httpCode == 200) {
      Serial.println("Update successful!");
      blinkPattern(2, 100); // Success blink
    }
  } else {
    Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

void setup() {
  // Minimal serial setup
  Serial.begin(115200);
  delay(1000); // Wait for serial to stabilize
  
  // Configure LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Increment boot count
  bootCount++;
  Serial.println("\n==== ESP32C3-MINI CYBER THERMOMETER ====");
  Serial.printf("Boot count: %d\n", bootCount);

  // Read temperature
  float temperature = readTemperature();
  
  // Only process if we got a valid reading
  if (temperature > -100.0) {
    // Connect to WiFi and send data
    connectWiFi();
    sendTemperatureData(temperature);
    
    // Turn off WiFi to save power
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }
  
  // Cleanup before sleep
  if (temp_handle) {
    temperature_sensor_uninstall(temp_handle);
    temp_handle = NULL;
  }
  
  // Prepare for deep sleep
  Serial.println("Preparing for deep sleep...");
  esp_sleep_enable_timer_wakeup(SLEEP_SECONDS * uS_TO_S_FACTOR);
  
  // Add extra delay to allow serial to flush
  delay(200);
  
  // Enter deep sleep
  Serial.println("Entering deep sleep for " + String(SLEEP_SECONDS) + " seconds");
  esp_deep_sleep_start();
}

void loop() {
  // This will never be reached
}