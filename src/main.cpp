/**
 * * Imports
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <SD_MMC.h>
#include <WiFi.h>
#include <esp_camera.h>

// Pins
#define FLASH_PIN 4

// Camera Variables
#define EEPROM_SIZE_IN_BYTES 1
#define MICROSECONDS_IN_SECONDS 1000000
#define SLEEP_TIME_IN_SECONDS 60

class CameraController {
public:
  CameraController() : photoNumber(0) {}

  void initialize() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE_IN_BYTES);
    loadPhotoNumberFromEEPROM();

    // Initialize SD card
    initializeSDCard();

    // Take and save photos
    takeAndSavePhotos();

    // Set sleep time
    setSleepTime();

    // Send the ESP32 into deep sleep
    esp_deep_sleep_start();
  }

private:
  int photoNumber;

  void initializeSDCard() {
    Serial.println("Initializing SD card");
    if (!SD_MMC.begin()) {
      Serial.println("Failed to initialize SD card!");
      return;
    }

    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("SD card slot appears to be empty!");
      return;
    }

    // If the SD card is empty then reset the EEPROM file counter back to 0
    resetPhotoNumbering();
  }

  void loadPhotoNumberFromEEPROM() {
    photoNumber = EEPROM.read(0);
    Serial.println("Next photo number loaded from preferences: " +
                   String(photoNumber));
  }

  void resetPhotoNumbering() {
    fs::FS &fs = SD_MMC;
    File sdCardRoot = fs.open("/");

    if (!sdCardRoot) {
      Serial.println("Failed to open SD card root folder!");
      return;
    }

    if (!sdCardRoot.isDirectory()) {
      Serial.println("SD card root folder cannot be read!");
      return;
    }

    File file = sdCardRoot.openNextFile();
    if (file.available() > 0) {
      Serial.println("SD card is not empty");
    } else {
      Serial.println("SD card is empty");
      photoNumber = 0;
      EEPROM.write(0, photoNumber);
      EEPROM.commit();
      Serial.println("Next photo number reset to 0");
    }
  }

  void takeAndSavePhotos() {
    // Take first picture (not saved, usually has a green tint)
    takePhoto(false);

    delay(1000);

    // Take second picture and save to SD card
    takePhoto(true);
  }

  void takePhoto(bool savePhoto) {
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    if (savePhoto) {
      String photoFileName = "/photo_" + String(photoNumber) + ".jpg";
      fs::FS &fs = SD_MMC;
      File file = fs.open(photoFileName.c_str(), FILE_WRITE);
      if (file) {
        file.write(fb->buf, fb->len);
        Serial.println("Saved file to path: " + String(photoFileName));
        ++photoNumber;
        if (photoNumber > 255) {
          photoNumber = 0;
        }

        EEPROM.write(0, photoNumber);
        EEPROM.commit();
        Serial.println("Next photo number saved to preferences: " +
                       String(photoNumber));
        file.close();
      } else {
        Serial.println("Failed to open file in writing mode");
      }
    }

    esp_camera_fb_return(fb);
  }

  void setSleepTime() {
    unsigned long sleepTime = MICROSECONDS_IN_SECONDS * SLEEP_TIME_IN_SECONDS;
    Serial.println("Going to sleep for " + String(SLEEP_TIME_IN_SECONDS) +
                   " seconds...");
    Serial.flush();

    esp_sleep_enable_timer_wakeup(sleepTime);
  }
};

class WiFiController {
public:
  WiFiController(const char *ssid, const char *password)
      : ssid(ssid), password(password) {}

  void connect() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to the WiFi...");
    }

    Serial.print("Connected to WiFi\nIP address: ");
    Serial.println(WiFi.localIP());
  }

private:
  const char *ssid;
  const char *password;
};

void setup() {
  CameraController cameraController;
  cameraController.initialize();
}

void loop() {
  WiFiController wifiController("IKEA-FREE", "pdzl7885");
  wifiController.connect();
}
