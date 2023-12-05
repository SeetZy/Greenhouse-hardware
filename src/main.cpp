/**
 * * Library imports
 */
#include <Arduino.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <WiFi.h>

// Pins
#define TEMP_PIN 4
#define SOIL_PIN 35

// WiFi name
const char *ssid = "IKEA-FREE";
// WiFi password
const char *password = "pdzl7885";

// SCL 22; SDA 21
LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire(TEMP_PIN);
DallasTemperature DS18B20(&oneWire);

// Values
float tempC;
int soilThreshold = 1000;

// Declare functions
void connectWiFi();
void checkTemp();
void checkSoilLevels();
void showLcd();

void setup() {
  Serial.begin(921600);
  DS18B20.begin();
  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Pin modes
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  } else if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to the WiFi...");
  }

  Serial.print("Connected to WiFi\nIP address: ");
  Serial.println(WiFi.localIP());
}

void checkTemp() {
  DS18B20.requestTemperatures();
  tempC = DS18B20.getTempCByIndex(0);

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
