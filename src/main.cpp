#include <Arduino.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <WiFi.h>

#define TEMP_PIN 4
#define SOIL_PIN 35
#define LIGHT_PIN 19

#define RELAY1 15 // Fan
#define RELAY2 18 // Water pump
#define RELAY3 5  // Heater

const char *ssid = "IKEA-FREE";
const char *password = "pdzl7885";

const char *post_url = "https://greenhouse-hardware-e6811b87b8a0.herokuapp.com/"
                       "post-greenhouse-info";

struct {
  float tempC = 0;
  int soilHum = 0;
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
    pinMode(LIGHT_PIN, OUTPUT);

    digitalWrite(LIGHT_PIN, HIGH);
  }

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

  void sendData() {
    connectWiFi();

    // Create a JSON object
    DynamicJsonDocument jsonDoc(256);
    jsonDoc["soilHum"] = monVar.soilHum;
    jsonDoc["tempC"] = monVar.tempC;

    // Serialize JSON to a string
    String jsonData;
    serializeJson(jsonDoc, jsonData);

    // Initialize HTTP client
    http.begin(post_url);
    http.addHeader("Content-Type", "application/json");

    // Send HTTP POST request with serialized JSON data
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

    // End the HTTP client
    http.end();
  }

  void checkTemp() {
    DS18B20.requestTemperatures();
    monVar.tempC = DS18B20.getTempCByIndex(0);
    monVar.tempC = ceil(monVar.tempC);

    Serial.print("Temperature: ");
    Serial.print(monVar.tempC);
    Serial.println("°C");
  }

  void checkSoilLevels() {
    monVar.soilHum = analogRead(SOIL_PIN);

    if (monVar.soilHum < 1000) {
      Serial.print("Soil is Dry: ");
    } else {
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

  void fanControl() { digitalWrite(RELAY1, (monVar.tempC >= 24) ? LOW : HIGH); }

  void heaterControl() {
    digitalWrite(RELAY3, (monVar.tempC <= 24) ? LOW : HIGH);
  }

  void waterPumpControl() {
    digitalWrite(RELAY2, (monVar.soilHum < 1000) ? LOW : HIGH);
  }

  void update() {
    checkTemp();
    checkSoilLevels();
    showLcd();
    fanControl();
    heaterControl();
    waterPumpControl();
  }

private:
  LiquidCrystal_I2C lcd;
  OneWire oneWire;
  DallasTemperature DS18B20;
  HTTPClient http;
  WiFiClient client;
};

GreenhouseMonitor greenhouseMonitor;

void setup() { greenhouseMonitor.begin(); }

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);

  greenhouseMonitor.update();

  static unsigned long lastSendDataTime = 0;
  if (millis() - lastSendDataTime >= 30000) {
    greenhouseMonitor.sendData();
    lastSendDataTime = millis();
  }

  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
}