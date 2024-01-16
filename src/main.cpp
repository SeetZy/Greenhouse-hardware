/**
 * * Library imports
 */
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <WiFi.h>

// Sensor Pins
#define TEMP_PIN 4
#define SOIL_PIN 35

// WiFi credentials
const char *ssid = "IKEA-FREE";
const char *password = "pdzl7885";

// Post method URL
const char *post_url = "http://localhost:3000";

struct {
  float tempC = 0;
  int soilHum = 0;
  int airHum = 0;
} monVar;

class EnvironmentMonitor {
public:
  EnvironmentMonitor()
      // SCL 22; SDA 21
      : lcd(0x27, 16, 2), oneWire(TEMP_PIN), DS18B20(&oneWire) {}

  void begin() {
    Serial.begin(921600);
    DS18B20.begin();
    lcd.init();
    lcd.clear();
    pinMode(BUILTIN_LED, OUTPUT);
  }

  // Connects to the WiFi
  void connectWiFi() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to the WiFi...");
    }

    Serial.print("Connected to WiFi\nIP address: ");
    Serial.println(WiFi.localIP());
  }

  // Sends data to cloud server
  void sendData() {
    StaticJsonDocument<200> jsonDocument;

    jsonDocument["tempC"] = monVar.tempC;
    jsonDocument["soilHum"] = monVar.soilHum;

    String jsonData;
    serializeJson(jsonDocument, jsonData);

    http.begin(post_url);

    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

private:
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;
  HTTPClient http;
  WiFiClient client;
};

class SensorMonitor {
public:
  SensorMonitor() : lcd(0x27, 16, 2), oneWire(TEMP_PIN), DS18B20(&oneWire) {}

  void checkTemp() {
    DS18B20.requestTemperatures();
    monVar.tempC = DS18B20.getTempCByIndex(0);

    Serial.print("Temperature: ");
    Serial.print(monVar.tempC);
    Serial.println("°C");
  }

  void checkSoilLevels() {
    monVar.soilHum = analogRead(SOIL_PIN);

    if (monVar.soilHum < 1000) {
      Serial.print("Soil is Dry: ");
    } else if (monVar.soilHum > 1000) {
      Serial.print("Soil is Wet: ");
    }

    Serial.println(monVar.soilHum);
  }

  void showLcd() {
    lcd.backlight();
    lcd.setCursor(1, 0);
    lcd.print("Temp: " + String(monVar.tempC) + (char)223 + "C");
    lcd.setCursor(2, 1);
    lcd.print("  Hum: " + String(monVar.soilHum) + "%");
  }

private:
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;
};

EnvironmentMonitor environmentMonitor;
SensorMonitor sensorMonitor;

void setup() { environmentMonitor.begin(); }

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    environmentMonitor.connectWiFi();
  } else {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    sensorMonitor.checkTemp();
    sensorMonitor.checkSoilLevels();
    sensorMonitor.showLcd();
    environmentMonitor.sendData();
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
  }
}