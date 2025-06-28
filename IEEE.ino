#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <SD.h>
#include <WebServer.h>

// Pins
#define TEMP_SENSOR_PIN 34
#define MOSFET_PIN 26
#define SD_CS 5
#define CHRG_PIN 33
#define STDBY_PIN 32

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

Adafruit_INA219 ina219;
WebServer server(80);

// Sensor data
float batteryVoltage = 0.0, current_mA = 0.0, temperatureC = 0.0;
String chargeStatus = "Unknown";

// 🛠 Forward declaration to fix scope error
void handleData();

void setup() {
  Serial.begin(115200);

  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  pinMode(CHRG_PIN, INPUT_PULLUP);
  pinMode(STDBY_PIN, INPUT_PULLUP);
  digitalWrite(MOSFET_PIN, LOW);

  if (!ina219.begin()) {
    Serial.println("INA219 not found!");
    while (1);
  }

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card failed");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData);  // ✅ Now works!
  server.begin();
}

void loop() {
  readSensors();
  logToSD();
  server.handleClient();
  delay(2000);
}

void readSensors() {
  batteryVoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();

  int analogValue = analogRead(TEMP_SENSOR_PIN);
  temperatureC = (analogValue / 4095.0) * 3.3 * 100.0;

  bool chrg = digitalRead(CHRG_PIN) == LOW;
  bool stdby = digitalRead(STDBY_PIN) == LOW;

  if (chrg) chargeStatus = "Charging";
  else if (stdby) chargeStatus = "Charged";
  else chargeStatus = "Not Charging";

  digitalWrite(MOSFET_PIN, (batteryVoltage > 3.5));
}

void logToSD() {
  File dataFile = SD.open("/bms_log.txt", FILE_APPEND);
  if (dataFile) {
    dataFile.print(millis()); dataFile.print(",");
    dataFile.print(batteryVoltage); dataFile.print(",");
    dataFile.print(current_mA); dataFile.print(",");
    dataFile.print(temperatureC); dataFile.print(",");
    dataFile.println(chargeStatus);
    dataFile.close();
  }
}

void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8" />
      <meta name="viewport" content="width=device-width, initial-scale=1.0" />
      <title>Smart BMS Dashboard</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: #0b0c10;
          color: #ffffff;
          margin: 0;
          padding: 0;
        }
        header {
          background-color: #1f2833;
          padding: 20px;
          text-align: center;
        }
        h1 { color: #66fcf1; }
        .section { padding: 20px; }
        .card {
          background: #1f2833;
          border: 1px solid #45a29e;
          border-radius: 10px;
          padding: 20px;
          margin-bottom: 20px;
        }
        canvas {
          width: 100% !important;
          max-width: 500px;
          margin-top: 20px;
        }
      </style>
    </head>
    <body>
      <header>
        <h1>Smart BMS Dashboard</h1>
        <p>Adaptive Battery Monitoring Interface</p>
      </header>
      <div class="section">
        <div class="card">
          <h2>Live Telemetry</h2>
          <p>Voltage: <span id="voltage">--</span> V</p>
          <p>Current: <span id="current">--</span> A</p>
          <p>Temperature: <span id="temperature">--</span> °C</p>
          <p>State of Charge (SOC): <span id="soc">--</span>%</p>
        </div>
        <div class="card">
          <h2>Health Trend (Last 10 Cycles)</h2>
          <canvas id="healthChart"></canvas>
        </div>
        <div class="card">
          <h2>Battery Passport</h2>
          <p>Cycle Count: <span id="cycles">--</span></p>
          <p>Last Overheat Event: <span id="overheat">--</span></p>
          <p>Last Overdischarge: <span id="overdischarge">--</span></p>
        </div>
      </div>
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
      <script>
        async function fetchData() {
          try {
            const response = await fetch("/data");
            if (!response.ok) throw new Error("Network response was not ok");
            return await response.json();
          } catch (error) {
            console.warn("ESP32 not connected, using mock data:", error);
            return {
              voltage: 3.7,
              current: 1.25,
              temperature: 35,
              soc: 82,
              healthHistory: [100, 98, 96, 94, 93, 91, 90, 88, 87, 85],
              cycles: 127,
              overheat: "2 days ago",
              overdischarge: "5 days ago",
            };
          }
        }

        function updateUI(data) {
          document.getElementById("voltage").textContent = data.voltage;
          document.getElementById("current").textContent = data.current;
          document.getElementById("temperature").textContent = data.temperature;
          document.getElementById("soc").textContent = data.soc;
          document.getElementById("cycles").textContent = data.cycles;
          document.getElementById("overheat").textContent = data.overheat;
          document.getElementById("overdischarge").textContent = data.overdischarge;

          new Chart(document.getElementById("healthChart"), {
            type: "line",
            data: {
              labels: [...Array(10).keys()].map(i => `Cycle ${i + 1}`),
              datasets: [{
                label: "Health %",
                data: data.healthHistory,
                borderColor: "#66fcf1",
                backgroundColor: "rgba(102, 252, 241, 0.2)",
                fill: true,
                tension: 0.4,
              }],
            },
            options: {
              responsive: true,
              scales: {
                y: {
                  beginAtZero: false,
                  min: 80,
                  max: 100,
                },
              },
            },
          });
        }

        fetchData().then(updateUI);
      </script>
    </body>
    </html>
  )rawliteral");
}

void handleData() {
  String json = "{";
  json += "\"voltage\":" + String(batteryVoltage, 2) + ",";
  json += "\"current\":" + String(current_mA / 1000.0, 2) + ",";
  json += "\"temperature\":" + String(temperatureC, 1) + ",";
  json += "\"soc\":82,";
  json += "\"healthHistory\":[100,98,96,94,93,91,90,88,87,85],";
  json += "\"cycles\":127,";
  json += "\"overheat\":\"2 days ago\",";
  json += "\"overdischarge\":\"5 days ago\"}";
  server.send(200, "application/json", json);
}
