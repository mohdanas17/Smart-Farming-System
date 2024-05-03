#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#include "SoilMoistureSensor.h"

// Replace with your network credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Telegram Bot credentials and chat ID
String BOTtoken = "YourBotToken";
String chatId = "YourChatID";

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// Soil Moisture Sensor
SoilMoistureSensor soilSensor(A0);

// BME280 Sensor
BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize BME280 sensor
  Wire.begin();
  if (!bme.begin()) {
    Serial.println("Could not find BME280 sensor, check wiring!");
    while (1);
  }

  // Initialize Telegram bot
  bot.sendMessage(chatId, "Smart Farming System started!", "");

  // Initialize Soil Moisture Sensor
  soilSensor.begin();
}

void loop() {
  // Check soil moisture level
  int soilMoisture = soilSensor.getMoisture();
  if (soilMoisture < 30) { // Adjust threshold based on your needs
    // Soil moisture is low, trigger irrigation system
    // Code to control irrigation system goes here
    bot.sendMessage(chatId, "Soil moisture level is low, irrigating...", "");
    delay(5000); // Simulate irrigation delay
  }

  // Read environmental data from BME280 sensor
  float temperature = bme.readTempC();
  float humidity = bme.readFloatHumidity();
  float pressure = bme.readFloatPressure();

  // Send environmental data to Telegram
  String message = "Temperature: " + String(temperature) + " °C\n";
  message += "Humidity: " + String(humidity) + " %\n";
  message += "Pressure: " + String(pressure / 100.0) + " hPa";
  bot.sendMessage(chatId, message, "");

  // Wait for some time before next iteration
  delay(60000); // Adjust delay based on your requirements
}
