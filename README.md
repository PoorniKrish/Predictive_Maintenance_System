# 🚀 Predictive Maintenance System (ESP32 + IoT)

## 📌 Overview
This project implements a **Predictive Maintenance System** using an ESP32.
It monitors **temperature** and **vibration**, detects anomalies, and sends data via **MQTT** for real-time monitoring and alerts.

## 🎯 Features
* 🌡️ Temperature monitoring (DS18B20 / simulated)
* 📈 Vibration analysis (MPU6050 / simulated)
* ⚡ Edge anomaly detection
* ☁️ MQTT data transmission
* 📧 Alert-ready system (Version 3)
* 🔁 Real-time streaming every 2 seconds

## 🧠 System Logic
* Temperature increases gradually (thermal drift)
* Vibration simulates normal + fault conditions
* Anomaly detected if:

  * Temperature > threshold
  * OR vibration exceeds safe limit

## 🏗️ Versions
### 🔹 Version 1: Basic Simulation
* Serial output
* Edge anomaly detection

### 🔹 Version 2: MQTT Integration
* Publishes JSON data to broker
* Topic: `esp32/predictive`

### 🔹 Version 3: Alert Mode
* Lower thresholds for testing alerts
* Designed for email/notification integration

## 📡 MQTT Payload Example
```json
{
  "temperature": 45.23,
  "vibration": 2.10,
  "anomaly": 1
}
```

## 🔧 Hardware Used
* ESP32
* DS18B20 Temperature Sensor
* MPU6050 Accelerometer

## 💻 Simulation
Built and tested using Wokwi simulator.

## 🚀 Future Improvements
* Dashboard (Node-RED / Grafana)
* Email/SMS alerts
* Cloud integration (AWS / Azure)
