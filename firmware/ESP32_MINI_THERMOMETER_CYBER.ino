#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#define LED_BUILTIN 8

const char* ssid = "Get-7368B0";
const char* password = "qjzkld2mjy";
const char* serverUrl = "https://noyanov.com/Apps/Temp/api/update.php";
const char* token = "#tkn25!@";  // MUST MATCH PHP TOKEN

WiFiClientSecure client;

float readTemperature() {
  return 22.6; // Your sensor reading
}

void blinkPattern(int times, int duration) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n==== ESP32C3-MINI CYBER THERMOMETER ====");
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Configure WiFiClientSecure
  client.setInsecure(); // Bypass SSL certificate validation
  
  // Connect to WiFi
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
    while(true) {
      blinkPattern(5, 200); // Error pattern
      delay(2000);
    }
  }
}

void loop() {
  Serial.println("\n--- Starting Update Cycle ---");
  
  // 1. Ensure WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Reconnecting...");
    WiFi.begin(ssid, password);
    delay(5000);
    return;
  }
  
  // 2. Read temperature
  float temperature = readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temperature);
  
  // 3. Create JSON payload
  DynamicJsonDocument doc(128);
  doc["token"] = token;
  doc["temperature"] = temperature;
  
  String payload;
  serializeJson(doc, payload);
  Serial.println("Payload: " + payload);
  
  // 4. Create HTTP client
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  
  // 5. Send request
  Serial.println("Sending POST request...");
  int httpCode = http.POST(payload);
  
  // 6. Handle response
  if (httpCode > 0) {
    Serial.printf("HTTP code: %d\n", httpCode);
    String response = http.getString();
    Serial.println("Response: " + response);
    
    // Parse JSON response
    DynamicJsonDocument resDoc(256);
    DeserializationError error = deserializeJson(resDoc, response);
    
    if (!error) {
      const char* status = resDoc["status"];
      const char* message = resDoc["message"];
      
      Serial.print("Status: ");
      Serial.println(status);
      Serial.print("Message: ");
      Serial.println(message);
      
      if (httpCode == 200 && strcmp(status, "success") == 0) {
        Serial.println("Update successful!");
        blinkPattern(2, 100); // Success blink
      } else {
        // Show token mismatch details
        if (resDoc.containsKey("expected") && resDoc.containsKey("received")) {
          Serial.print("Token mismatch! Expected: ");
          Serial.print(resDoc["expected"].as<const char*>());
          Serial.print(", Received: ");
          Serial.println(resDoc["received"].as<const char*>());
        }
        blinkPattern(4, 100); // Error blink
      }
    } else {
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
      blinkPattern(5, 100); // Error blink
    }
  } else {
    Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
    blinkPattern(3, 200); // Error blink
  }
  
  // 7. Cleanup
  http.end();
  
  delay(30000); // Wait 30 seconds
}