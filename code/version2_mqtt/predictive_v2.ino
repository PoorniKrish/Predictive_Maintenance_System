#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

// ================= PIN DEFINITIONS =================
#define ONE_WIRE_BUS 4   // DS18B20 DATA pin

// ================= WIFI CONFIG (WOKWI) =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ================= MQTT CONFIG =================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "esp32/predictive";

// ================= OBJECTS =================
WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
MPU6050 mpu;

// ================= VARIABLES =================
float temperature = 0.0;
float vibration = 0.0;
float vibrationFiltered = 1.0;
bool anomaly = false;

// Simulated temperature
float simulatedTemp = 25.0;

// ================= MQTT RECONNECT =================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_Predictive_Maintenance")) {
      Serial.println("Connected");
    } else {
      Serial.println("Retrying...");
      delay(2000);
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // ---- WiFi ----
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");

  // ---- MQTT ----
  client.setServer(mqttServer, mqttPort);

  // ---- Sensors ----
  tempSensor.begin();
  Wire.begin(21, 22);   // SDA, SCL
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully");
  } else {
    Serial.println("MPU6050 connection failed!");
  }

  randomSeed(analogRead(0));
}

// ================= LOOP =================
void loop() {

  // ---- MQTT maintain ----
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // =================================================
  // TEMPERATURE SIMULATION (THERMAL DRIFT)
  // =================================================
  simulatedTemp += random(-5, 15) / 10.0;   // -0.5 to +1.5 °C

  if (simulatedTemp < 25.0) simulatedTemp = 25.0;
  if (simulatedTemp > 90.0) simulatedTemp = 90.0;

  temperature = simulatedTemp;

  // =================================================
  // VIBRATION SIMULATION (NORMAL + FAULT)
  // =================================================
  float baseVibration = 1.0 + random(-15, 15) / 100.0; // 0.85 – 1.15 g

  // Fault spike (10% chance)
  if (random(0, 100) > 90) {
    baseVibration = random(40, 70) / 10.0;  // 4.0 – 7.0 g
  }

  // Low-pass filter
  vibrationFiltered = (0.7 * vibrationFiltered) + (0.3 * baseVibration);
  vibration = vibrationFiltered;

  // =================================================
  // EDGE ANOMALY DETECTION (DEMO MODE)
  // =================================================
  anomaly = (temperature > 45.0 || vibration > 1.8);

  // =================================================
  // MQTT PAYLOAD
  // =================================================
  String payload = "{";
  payload += "\"temperature\":" + String(temperature, 2) + ",";
  payload += "\"vibration\":" + String(vibration, 2) + ",";
  payload += "\"anomaly\":" + String(anomaly);
  payload += "}";

  client.publish(mqttTopic, payload.c_str());

  // ---- Serial Output ----
  Serial.println(payload);

  delay(2000);
}
