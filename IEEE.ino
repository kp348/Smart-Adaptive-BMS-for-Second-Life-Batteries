#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// SD card chip select
#define SD_CS 5

// Simulated battery data (replace with real sensor readings)
float voltage = 3.7;
float current = 1.25;
float temperature = 35.0;
int soc = 82;
int cycles = 127;
String overheat = "2 days ago";
String overdischarge = "5 days ago";
int healthHistory[10] = {100, 98, 96, 94, 93, 91, 90, 88, 87, 85};

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
  Serial.println("Open in browser: http://" + WiFi.localIP().toString() + "/data");
  Serial.println("View log file:   http://" + WiFi.localIP().toString() + "/log");

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }

  // Define /data endpoint
  server.on("/data", HTTP_GET, []() {
    StaticJsonDocument<512> doc;
    doc["voltage"] = voltage;
    doc["current"] = current;
    doc["temperature"] = temperature;
    doc["soc"] = soc;
    doc["cycles"] = cycles;
    doc["overheat"] = overheat;
    doc["overdischarge"] = overdischarge;

    JsonArray hist = doc.createNestedArray("healthHistory");
    for (int i = 0; i < 10; i++) {
      hist.add(healthHistory[i]);
    }

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);

    // Save to SD card
    File file = SD.open("/log.txt", FILE_APPEND);
    if (file) {
      file.println(json);
      file.close();
    }
  });

  // Define /log endpoint to read log.txt
  server.on("/log", HTTP_GET, []() {
    if (SD.exists("/log.txt")) {
      File file = SD.open("/log.txt", FILE_READ);
      if (file) {
        String logContent;
        while (file.available()) {
          logContent += (char)file.read();
        }
        file.close();
        server.send(200, "text/plain", logContent);
      } else {
        server.send(500, "text/plain", "Failed to open log file.");
      }
    } else {
      server.send(404, "text/plain", "No log file found.");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
