#include <WiFi.h>
#include <esp_err.h>
#include <esp_wifi.h> 
#include "wifiUtils.h"

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  wifiutils_init();
  setupWifi();

  while (WiFi.status() != WL_CONNECTED) {
    wifiutils_loop();
    delay(100);
  }

  Serial.println("Connected");
}

void setupWifi() {
  // esp_wifi_internal_set_log_level(WIFI_LOG_VERBOSE);
  // esp_log_set_vprintf(&logToSerial);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);

  // WiFi.setSleep(false);
  // WiFi.enableLongRange(true);
  // // wifi_config_t wifi_config;
  // esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
  // wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  // esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  cmd_connect("");
}

void loop() {
  // put your main code here, to run repeatedly:
}

