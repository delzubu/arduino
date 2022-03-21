#ifndef DELZUBU_WIFIUTILS_H_
#define DELZUBU_WIFIUTILS_H_ 
#include <esp_wifi_internal.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include "utils.h"

void wifiutils_init();
void wifiutils_init(String, String);
void wifiutils_init(String, String, uint8_t*);
void wifiutils_loop();

void cmd_status(String);
void cmd_reset(String);
void cmd_flasherase(String);
void cmd_connect(String);
void cmd_authmode(String);
void cmd_scanmethod(String);
void cmd_sortmethod(String);
void cmd_bssid(String);
void cmd_ssid(String);
void cmd_pwd(String);
void cmd_trace(String);
void cmd_connect(String);
void cmd_scan(String);

#endif /* DELZUBU_WIFI_H_ */ 
