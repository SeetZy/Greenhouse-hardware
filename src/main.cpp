#include "FS.h"
#include "SD_MMC.h"
#include "driver/rtc_io.h"
#include "esp_camera.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
#include "time.h"
#include <Arduino.h>
#include <WiFi.h>

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
    config.pixel_format = PIXFORMAT_JPEG; // YUV422,GRAYSCALE,RGB565,JPEG
    config.grab_mode = CAMERA_GRAB_LATEST;

    // Select lower framesize if the camera doesn't support PSRAM
    if (psramFound()) {
      config.frame_size =
          FRAMESIZE_UXGA;       // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
      config.jpeg_quality = 10; // 0-63 lower number means higher quality
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

  void initWiFi() {
    WiFi.begin(ssid, password);
    Serial.println("Connecting Wifi");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
  }

  void setTimezone(String timezone) {
    Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
    setenv("TZ", timezone.c_str(), 1);
    tzset();
  }

  void initTime(String timezone) {
    struct tm timeinfo;
    Serial.println("Setting up time");
    configTime(0, 0, "pool.ntp.org");
    if (!getLocalTime(&timeinfo)) {
      Serial.println(" Failed to obtain time");
      return;
    }
    Serial.println("Got the time from NTP");
    setTimezone(timezone);
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
    initWiFi();
    initTime(myTimezone);
    Serial.print("Initializing the camera module...");
    configInitCamera();
    Serial.println("Ok!");
    Serial.print("Initializing the MicroSD card module... ");
    initMicroSDCard();
  }

  void captureAndSavePhoto() {
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed");
      delay(1000);
      ESP.restart();
    }

    String path = getPictureFilename();
    Serial.printf("Picture file name: %s\n", path.c_str());

    fs::FS &fs = SD_MMC;
    File file = fs.open(path.c_str(), FILE_WRITE);
    if (!file) {
      Serial.printf("Failed to open file in writing mode");
    } else {
      file.write(fb->buf, fb->len);
      Serial.printf("Saved: %s\n", path.c_str());
    }
    file.close();
    esp_camera_fb_return(fb);
  }
};

CameraSystem cameraSystem("IKEA-FREE", "pdzl7885", "CST6CDT,M3.2.0,M11.1.0", 32,
                          -1, 0, 26, 27, 35, 34, 39, 36, 21, 19, 18, 5, 25, 23,
                          22);

void setup() { cameraSystem.setup(); }

void loop() {
  cameraSystem.captureAndSavePhoto();
  delay(10000);
}
