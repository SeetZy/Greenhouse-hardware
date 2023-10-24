/**
 * * Imports
 */
#include "WiFi.h"
#include <Arduino.h>

// WiFi name
const char *ssid = "";
// WiFi password
const char *password = "";

// Declare functions
void connectWiFi();

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(921600);

  connectWiFi();
}

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);
  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
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
