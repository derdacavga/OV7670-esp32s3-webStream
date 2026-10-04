#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"

extern "C" int SCCB_Write(uint8_t slv_addr, uint8_t reg, uint8_t data);
#define OV7670_I2C_ADDR 0x21

#define CAPTURE_FRAMESIZE    FRAMESIZE_QVGA
#define CAPTURE_JPEG_QUALITY 35 

const char *ssid     = "Change Your SSDI";
const char *password = "Wifi Password";

#define PWDN_GPIO_NUM   -1 // connect to GND
#define RESET_GPIO_NUM  -1 // connect to 3.3v
#define XCLK_GPIO_NUM   6
#define SIOD_GPIO_NUM   2
#define SIOC_GPIO_NUM   1

#define Y9_GPIO_NUM     7   // D7
#define Y8_GPIO_NUM     8   // D6
#define Y7_GPIO_NUM     9   // D5
#define Y6_GPIO_NUM     10  // D4
#define Y5_GPIO_NUM     11  // D3
#define Y4_GPIO_NUM     12  // D2
#define Y3_GPIO_NUM     13  // D1
#define Y2_GPIO_NUM     14  // D0
#define VSYNC_GPIO_NUM  3
#define HREF_GPIO_NUM   4
#define PCLK_GPIO_NUM   5

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  char part_buf[64];

  uint32_t last_time = millis();
  uint32_t frame_count = 0;

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      vTaskDelay(pdMS_TO_TICKS(1));
      continue;
    }

    uint8_t *jpg_buf = NULL;
    size_t jpg_buf_len = 0;

    bool converted = frame2jpg(fb, CAPTURE_JPEG_QUALITY, &jpg_buf, &jpg_buf_len);
    esp_camera_fb_return(fb);
    fb = NULL;

    if (!converted || !jpg_buf) {
      if (jpg_buf) free(jpg_buf);
      continue;
    }

    size_t hlen = snprintf(part_buf, sizeof(part_buf), _STREAM_PART, jpg_buf_len);
    res = httpd_resp_send_chunk(req, part_buf, hlen);

    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }

    free(jpg_buf);
    jpg_buf = NULL;

    if (res != ESP_OK) break;

    frame_count++;
    if (millis() - last_time >= 1000) {
      Serial.printf("Current FPS: %u\n", frame_count);
      frame_count = 0;
      last_time = millis();
    }
  }
  return res;
}

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  config.send_wait_timeout = 2;

  httpd_uri_t stream_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = stream_handler,
    .user_ctx = NULL
  };

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
    Serial.println("Stream server online.");
  }
}

void setup() {
  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  delay(500);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;

  config.xclk_freq_hz = 16500000;
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size   = CAPTURE_FRAMESIZE;
  config.jpeg_quality = 25;

  config.fb_count     = 1;
  config.grab_mode    = CAMERA_GRAB_LATEST;
  config.fb_location  = CAMERA_FB_IN_DRAM;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    while (1) delay(1000);
  }

  SCCB_Write(OV7670_I2C_ADDR, 0x11, 0x40);
  SCCB_Write(OV7670_I2C_ADDR, 0x1E, 0x30);

  Serial.println("OV7670 configured with high-speed prescaler.");

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("Stream URL: http://");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

void loop() {
  delay(10000);
}