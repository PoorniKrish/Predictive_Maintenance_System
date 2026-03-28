#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

#define ONE_WIRE_BUS 4   // DS18B20 DATA pin

// -------- Sensor Objects --------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
MPU6050 mpu;

// -------- Variables --------
float temperature = 0.0;
float vibration = 0.0;
float vibrationFiltered = 1.0;
bool anomaly = false;

// Simulated temperature (machine heating)
float simulatedTemp = 25.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

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

void loop() {

  // =================================================
  // TEMPERATURE SIMULATION (Thermal Drift)
  // =================================================
  simulatedTemp += random(-5, 15) / 10.0;   // -0.5 to +1.5 °C

  if (simulatedTemp < 25) simulatedTemp = 25;
  if (simulatedTemp > 90) simulatedTemp = 90;

  temperature = simulatedTemp;

  // =================================================
  // VIBRATION SIMULATION (NORMAL + FAULT)
  // =================================================

  // --- Normal vibration noise around 1g ---
  float baseVibration = 1.0 + random(-15, 15) / 100.0; // 0.85g – 1.15g

  // --- Fault vibration spike (10% chance) ---
  if (random(0, 100) > 90) {
    baseVibration = random(30, 50) / 10.0;  // 3.0g – 5.0g
  }

  // --- Simple low-pass filter (smooth output) ---
  vibrationFiltered = (0.7 * vibrationFiltered) + (0.3 * baseVibration);
  vibration = vibrationFiltered;

  // =================================================
  // EDGE ANOMALY DETECTION
  // =================================================
  anomaly = (temperature > 40.0 || vibration > 1.8);

  // =================================================
  // OUTPUT
  // =================================================
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.print(" °C | Vibration: ");
  Serial.print(vibration, 2);
  Serial.print(" g | Anomaly: ");
  Serial.println(anomaly ? "YES" : "NO");

  delay(2000);
}
