/**
 * * Library imports
 */
#include <Arduino.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <WiFi.h>

// Sensor Pins
#define TEMP_PIN 4
#define SOIL_PIN 35

// WiFi credentials
const char *ssid = "IKEA-FREE";
const char *password = "pdzl7885";

class EnvironmentMonitor {
public:
  EnvironmentMonitor()
      : lcd(0x27, 16, 2), oneWire(TEMP_PIN), DS18B20(&oneWire) {}

  void begin() {
    Serial.begin(921600);
    DS18B20.begin();
    lcd.init();
    lcd.clear();
    lcd.backlight();
    pinMode(BUILTIN_LED, OUTPUT);
  }

  void loop() {
    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    } else {
      digitalWrite(BUILTIN_LED, HIGH);
      delay(1000);
      digitalWrite(BUILTIN_LED, LOW);
      delay(1000);
    }
  }

private:
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;

  void connectWiFi() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to the WiFi...");
    }

    Serial.print("Connected to WiFi\nIP address: ");
    Serial.println(WiFi.localIP());
  }

  void connectDb() {}
};

class SensorMonitor {
public:
  SensorMonitor(int soilThreshold)
      : soilThreshold(soilThreshold), lcd(0x27, 16, 2), oneWire(TEMP_PIN),
        DS18B20(&oneWire) {}

  void checkTemp() {
    DS18B20.requestTemperatures();
    float tempC = DS18B20.getTempCByIndex(0);

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println("°C");
  }

  void checkSoilLevels() {
    int value = analogRead(SOIL_PIN);

    if (value < soilThreshold) {
      Serial.print("Soil is Dry: ");
    } else if (value > soilThreshold) {
      Serial.print("Soil is Wet: ");
    }

    Serial.println(value);
  }

  void showLcd() {
    lcd.setCursor(2, 0);
    lcd.print("Hello, world");
  }

private:
  int soilThreshold;
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;
};

EnvironmentMonitor environmentMonitor;
SensorMonitor sensorMonitor(1000);

void setup() { environmentMonitor.begin(); }

void loop() {
  environmentMonitor.loop();
  sensorMonitor.checkTemp();
  sensorMonitor.checkSoilLevels();
  sensorMonitor.showLcd();
}
