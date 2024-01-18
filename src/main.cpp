/**
 * * Library imports
 */
#include <Arduino.h>
#include <FS.h>
#include <HTTPClient.h>
#include <SD_MMC.h>
#include <WiFi.h>
#include <driver/rtc_io.h>
#include <esp_camera.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/soc.h>
#include <time.h>

class CameraSystem {
private:
  const char *ssid;
  const char *password;
  String myTimezone;
  int PWDN_GPIO_NUM;
  int RESET_GPIO_NUM;
  int XCLK_GPIO_NUM;
  int SIOD_GPIO_NUM;
  int SIOC_GPIO_NUM;
  int Y9_GPIO_NUM;
  int Y8_GPIO_NUM;
  int Y7_GPIO_NUM;
  int Y6_GPIO_NUM;
  int Y5_GPIO_NUM;
  int Y4_GPIO_NUM;
  int Y3_GPIO_NUM;
  int Y2_GPIO_NUM;
  int VSYNC_GPIO_NUM;
  int HREF_GPIO_NUM;
  int PCLK_GPIO_NUM;
  camera_config_t config;

  void configInitCamera() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;

    if (psramFound()) {
      config.frame_size = FRAMESIZE_UXGA;
      // 0-63 lower number means higher quality
      config.jpeg_quality = 10;
      config.fb_count = 1;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.jpeg_quality = 12;
      config.fb_count = 1;
    }

    // Initialize the Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
    }
  }

  void initMicroSDCard() {
    Serial.println("Starting SD Card");
    if (!SD_MMC.begin()) {
      Serial.println("SD Card Mount Failed");
      return;
    }
    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("No SD Card attached");
      return;
    }
  }

  String getPictureFilename() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return "";
    }
    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
    Serial.println(timeString);
    String filename = "/picture_" + String(timeString) + ".jpg";
    return filename;
  }

public:
  CameraSystem(const char *ssid, const char *password, String myTimezone,
               int PWDN_GPIO_NUM, int RESET_GPIO_NUM, int XCLK_GPIO_NUM,
               int SIOD_GPIO_NUM, int SIOC_GPIO_NUM, int Y9_GPIO_NUM,
               int Y8_GPIO_NUM, int Y7_GPIO_NUM, int Y6_GPIO_NUM,
               int Y5_GPIO_NUM, int Y4_GPIO_NUM, int Y3_GPIO_NUM,
               int Y2_GPIO_NUM, int VSYNC_GPIO_NUM, int HREF_GPIO_NUM,
               int PCLK_GPIO_NUM)
      : ssid(ssid), password(password), myTimezone(myTimezone),
        PWDN_GPIO_NUM(PWDN_GPIO_NUM), RESET_GPIO_NUM(RESET_GPIO_NUM),
        XCLK_GPIO_NUM(XCLK_GPIO_NUM), SIOD_GPIO_NUM(SIOD_GPIO_NUM),
        SIOC_GPIO_NUM(SIOC_GPIO_NUM), Y9_GPIO_NUM(Y9_GPIO_NUM),
        Y8_GPIO_NUM(Y8_GPIO_NUM), Y7_GPIO_NUM(Y7_GPIO_NUM),
        Y6_GPIO_NUM(Y6_GPIO_NUM), Y5_GPIO_NUM(Y5_GPIO_NUM),
        Y4_GPIO_NUM(Y4_GPIO_NUM), Y3_GPIO_NUM(Y3_GPIO_NUM),
        Y2_GPIO_NUM(Y2_GPIO_NUM), VSYNC_GPIO_NUM(VSYNC_GPIO_NUM),
        HREF_GPIO_NUM(HREF_GPIO_NUM), PCLK_GPIO_NUM(PCLK_GPIO_NUM) {}

  void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);
    delay(2000);
    Serial.print("Initializing the camera module...");
    configInitCamera();
    Serial.println("Ok!");
    Serial.print("Initializing the MicroSD card module... ");
    initMicroSDCard();
  }

  // Connects to the WiFi
  void initWiFi() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to the WiFi...");
    }

    Serial.print("Connected to WiFi\nIP address: ");
    Serial.println(WiFi.localIP());

    initTime(myTimezone);
  }

  // Gets the timezone from the internet
  void setTimezone(String timezone) {
    Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
    setenv("TZ", timezone.c_str(), 1);
    tzset();
  }

  // Sets the time based on the timezone
  void initTime(String timezone) {
    struct tm timeinfo;
    Serial.println("Setting up time");
    configTime(0, 0, "pool.ntp.org");
    if (!getLocalTime(&timeinfo)) {
      Serial.println(" Failed to obtain time");
      return;
    }
    setTimezone(timezone);
    Serial.println("Got the time from NTP");
    Serial.println(timezone);
  }

  // Captures and uploads the photo to the server
  void captureAndUploadPhoto() {
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed");
      delay(1000);
      ESP.restart();
    }

    // Create a buffer to hold the JPEG photo
    uint8_t *jpegBuffer = (uint8_t *)malloc(fb->len);
    if (!jpegBuffer) {
      Serial.println("Failed to allocate memory for the JPEG buffer");
      esp_camera_fb_return(fb);
      return;
    }

    // Copy the photo data to the buffer
    memcpy(jpegBuffer, fb->buf, fb->len);

    String path = getPictureFilename();
    Serial.printf("Picture file name: %s\n", path.c_str());

    fs::FS &fs = SD_MMC;
    File file = fs.open(path.c_str(), FILE_WRITE);
    if (!file) {
      Serial.printf("Failed to open file in writing mode");
    } else {
      file.write(jpegBuffer, fb->len);
      Serial.printf("Saved: %s\n", path.c_str());
      file.close();
    }

    free(jpegBuffer);
    esp_camera_fb_return(fb);
  }
};

CameraSystem cameraSystem("IKEA-FREE", "pdzl7885",
                          "EET-2EEST,M3.5.0/3,M10.5.0/4", 32, -1, 0, 26, 27, 35,
                          34, 39, 36, 21, 19, 18, 5, 25, 23, 22);

void setup() { cameraSystem.setup(); }

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    cameraSystem.initWiFi();
  } else {
    cameraSystem.captureAndUploadPhoto();
    delay(10000);
  }
}