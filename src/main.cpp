/**
 * * Imports
 */
#include <Arduino.h>
#include <WiFi.h>

// WiFi name
const char *ssid = "";
// WiFi password
const char *password = "";

// Pins
#define FLASH_PIN 4

// Declare functions
void connectWiFi();

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_PIN, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  } else if (WiFi.status() == WL_CONNECTED) {
    Serial.println("serial monitor is working");
    digitalWrite(FLASH_PIN, HIGH);
    delay(1000);
    digitalWrite(FLASH_PIN, LOW);
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
