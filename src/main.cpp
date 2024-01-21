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

// Relay Pins
#define RELAY1 15 // Fan
#define RELAY2 5  // Heater
#define RELAY3 18 // Water pump
#define RELAY4 19 // Lights

// WiFi Credentials
const char *ssid = "IKEA-FREE";
const char *password = "pdzl7885";

// Post Method URL
const char *post_url = "https://greenhouse-hardware-e6811b87b8a0.herokuapp.com/"
                       "post-greenhouse-info";

struct {
  float tempC = 0;
  int soilHum = 0;
  String time = "20/01/2024 14:15";
} monVar;

class GreenhouseMonitor {
public:
  GreenhouseMonitor()
      : lcd(0x27, 16, 2), oneWire(TEMP_PIN), DS18B20(&oneWire) {}

  void begin() {
    Serial.begin(921600);
    DS18B20.begin();
    lcd.init();
    lcd.clear();
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode(RELAY3, OUTPUT);
    pinMode(RELAY4, OUTPUT);

    digitalWrite(RELAY4, HIGH);
    checkTemp();
    checkSoilLevels();
    showLcd();
  }

  // Connects to the WiFi
  void connectWiFi() {

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 15) {
      WiFi.begin(ssid, password);
      delay(500);

      Serial.print("Connecting to the WiFi... Attempt ");
      Serial.println(attempts + 1);

      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected to WiFi\nIP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Failed to connect to WiFi");
    }
  }

  void initTime() {}

  // Sends data to cloud server
  void sendData() {
    connectWiFi();
    initTime();

    StaticJsonDocument<200> jsonDocument;

    jsonDocument["tempC"] = monVar.tempC;
    jsonDocument["soilHum"] = monVar.soilHum;
    jsonDocument["time"] = monVar.time;

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
    lcd.print("  Hum: " + String(monVar.soilHum));
  }

  void fanControl() {}

  void heaterControl() {}

  void waterPumpControl() {}

  void lightControl() {
    unsigned long currentMillis = millis();
    unsigned long lastToggleTime;

    // Toggle RELAY4 every 16 hours
    if (currentMillis - lastToggleTime >= TOGGLE_INTERVAL) {
      lastToggleTime = currentMillis;

      // Toggle RELAY4 state
      digitalWrite(RELAY4, !digitalRead(RELAY4));
    }
  }

  void update() {
    unsigned long currentMillis = millis();
    unsigned long lastUpdateTime;

    if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL) {
      lastUpdateTime = currentMillis;

      checkTemp();
      checkSoilLevels();
      showLcd();
      sendData();
    }
  }

private:
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;
  HTTPClient http;
  WiFiClient client;
  static const unsigned long UPDATE_INTERVAL = 60000;
  static const unsigned long TOGGLE_INTERVAL = 16 * 60 * 60 * 1000;
};

GreenhouseMonitor greenhouseMonitor;

void setup() { greenhouseMonitor.begin(); }

void loop() {
  // Indicates that there is function
  digitalWrite(BUILTIN_LED, HIGH);

  greenhouseMonitor.update();
  greenhouseMonitor.lightControl();

  // Indicates that there is function
  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
}