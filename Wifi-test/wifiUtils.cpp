#include "wifiUtils.h"

static wifi_config_t wifi_config;

static String wifiutils_ssid = "TaliganetMB 2";
static String wifiutils_password = "Miu321Miu";
static uint8_t wifiutils_bssid[6] = {0,0,0,0,0,0};

void setupCallbackMap();


void wifiutils_init(String ssid, String password, uint8_t* bssid) {
    wifiutils_ssid = ssid;
    wifiutils_password = password;
    memcpy(wifiutils_bssid, bssid, sizeof(wifiutils_bssid));

    wifiutils_init();
}

void wifiutils_init(String ssid, String password) {
    wifiutils_ssid = ssid;
    wifiutils_password = password;

    wifiutils_init();
}

void wifiutils_init() {
    setupCallbackMap();
}

void wifiutils_loop() {
    consoleHandler();
}


String bssidStr(uint8_t *bssid) {
  char mac[18] = { 0 }; 
  sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]); 
  return String(mac);
}

void parseBssidStr(String s, uint8_t *bssid) {
  sscanf(s.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X", &bssid[0], &bssid[1], &bssid[2], &bssid[3], &bssid[4], &bssid[5]); 
}

String encryptionTypeStr(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (0):
      return "Open";
    case (1):
      return "WEP";
    case (2):
      return "WPA_PSK";
    case (3):
      return "WPA2_PSK";
    case (4):
      return "WPA_WPA2_PSK";
    case (5):
      return "WPA2_ENTERPRISE";
    default:
      return "UNKOWN";
    }
  }


void setupCallbackMap() {
  registerCommandCallback("status", &cmd_status);
  registerCommandCallback("reset", &cmd_reset);
  registerCommandCallback("flasherase", &cmd_flasherase);
  registerCommandCallback("connect", &cmd_connect);
  registerCommandCallback("authmode", &cmd_authmode);
  registerCommandCallback("scanmethod", &cmd_scanmethod);
  registerCommandCallback("sortmethod", &cmd_sortmethod);
  registerCommandCallback("ssid", &cmd_ssid);
  registerCommandCallback("bssid", &cmd_bssid);
  registerCommandCallback("scan", &cmd_scan);
  registerCommandCallback("pwd", &cmd_pwd);
  registerCommandCallback("trace", &cmd_trace);
}

void cmd_status(String args) {
  Serial.print("WiFi.status: "); 
  switch(WiFi.status()) {
    case WL_IDLE_STATUS: Serial.println("0 IDLE"); break;
    case WL_NO_SSID_AVAIL: Serial.println("1 NO_SSID_AVAIL"); break; 
    case WL_SCAN_COMPLETED: Serial.println("2 SCAN_COMPLETED"); break;
    case WL_CONNECTED: Serial.println("3 CONNECTED"); break;
    case WL_CONNECT_FAILED: Serial.println("4 CONNECT_FAILED"); break;
    case WL_CONNECTION_LOST: Serial.println("5 CONNECTION_LOST"); break;
    case WL_DISCONNECTED: Serial.println("6 DISCONNECTED"); break;
  }
  
  if(args != "") {
    // wifi_config_t wifi_config;
    esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
    const char* ssid = reinterpret_cast<const char*>(wifi_config.sta.ssid); 
    const char* pwd = reinterpret_cast<const char*>(wifi_config.sta.password); 

    Serial.print("ssid: "); Serial.println(ssid);
    Serial.print("pwd: "); Serial.println(pwd);
    Serial.print("scan_method: "); 
    switch(wifi_config.sta.scan_method) {
      case WIFI_FAST_SCAN: Serial.println("0 WIFI_FAST_SCAN");break;
      case WIFI_ALL_CHANNEL_SCAN: Serial.println("1 WIFI_ALL_CHANNEL_SCAN");break;
    }
    Serial.print("listen_interval: "); Serial.println(wifi_config.sta.listen_interval);
    Serial.print("sort_method: "); 
    switch(wifi_config.sta.sort_method) {
      case WIFI_CONNECT_AP_BY_SIGNAL: Serial.println("0 CONNECT_AP_BY_SIGNAL");break;
      case WIFI_CONNECT_AP_BY_SECURITY: Serial.println("1 CONNECT_AP_BY_SECURITY");break;
    }
    Serial.print("threshold.authmode: "); 
    switch(wifi_config.sta.threshold.authmode) {
      WIFI_AUTH_OPEN: Serial.println("0 OPEN");break;
      WIFI_AUTH_WEP: Serial.println("1 WEP");break;
      WIFI_AUTH_WPA_PSK: Serial.println("2 WPA");break;
      WIFI_AUTH_WPA2_PSK: Serial.println("3 WPA2");break;
      WIFI_AUTH_WPA_WPA2_PSK: Serial.println("4 WPA/WPA2");break;
      WIFI_AUTH_WPA2_ENTERPRISE: Serial.println("5 WPA2 Enterprise");break;
      WIFI_AUTH_MAX: Serial.println("6 MAX");break;
      default: Serial.println(wifi_config.sta.threshold.authmode); break;
    }
  }
}

void cmd_reset(String args) {
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);

  // esp_wifi_disconnect ();
  // esp_wifi_stop ();
  // esp_wifi_deinit ();
  
  WiFi.mode(WIFI_STA);
  Serial.println("Connection reset");
}

void cmd_flasherase(String args)
{
  ESP_ERROR_CHECK(nvs_flash_erase());
  nvs_flash_init();
}

void cmd_connect(String args) {
  Serial.print("Connecting to "); Serial.print(wifiutils_ssid);

  if(args=="bssid") {
    Serial.print(" "); Serial.print(bssidStr(wifiutils_bssid));
    Serial.println();
    WiFi.begin(wifiutils_ssid.c_str(), wifiutils_password.c_str(), 0, wifiutils_bssid);
  } else {
    Serial.println();
    WiFi.begin(wifiutils_ssid.c_str(), wifiutils_password.c_str());
  }
}

void cmd_authmode(String args) {
  // wifi_config_t wifi_config;
  esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
  if(args == "on") {
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  } else if(args == "off") {
    wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  }
  Serial.print("Threshold.authmode: "); 
  switch(wifi_config.sta.threshold.authmode) {
    WIFI_AUTH_OPEN: Serial.println("0 OPEN");break;
    WIFI_AUTH_WEP: Serial.println("1 WEP");break;
      WIFI_AUTH_WPA_PSK: Serial.println("2 WPA");break;
      WIFI_AUTH_WPA2_PSK: Serial.println("3 WPA2");break;
      WIFI_AUTH_WPA_WPA2_PSK: Serial.println("4 WPA/WPA2");break;
      WIFI_AUTH_WPA2_ENTERPRISE: Serial.println("5 WPA2 Enterprise");break;
      WIFI_AUTH_MAX: Serial.println("6 MAX");break;
      default: Serial.println(wifi_config.sta.threshold.authmode); break;
    }
}

void cmd_scanmethod(String args) {
  esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
  if(args == "on") {
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN; 
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  } else if(args == "off") {
    wifi_config.sta.scan_method = WIFI_FAST_SCAN; 
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  }
  Serial.print("scan_method: "); 
  switch(wifi_config.sta.scan_method) {
    case WIFI_FAST_SCAN: Serial.println("0 WIFI_FAST_SCAN");break;
    case WIFI_ALL_CHANNEL_SCAN: Serial.println("1 WIFI_ALL_CHANNEL_SCAN");break;
  }
}

void cmd_sortmethod(String args) {
  esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
  if(args == "on") {
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL; 
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  } else if(args == "off") {
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SECURITY; 
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  }
  Serial.print("sort_method: "); 
  switch(wifi_config.sta.sort_method) {
    case WIFI_CONNECT_AP_BY_SIGNAL: Serial.println("0 CONNECT_AP_BY_SIGNAL");break;
    case WIFI_CONNECT_AP_BY_SECURITY: Serial.println("1 CONNECT_AP_BY_SECURITY");break;
  }
}

void cmd_ssid(String args) {
  Serial.print("SSID:");
  if(args != "") wifiutils_ssid = args;
  Serial.println(wifiutils_ssid);
}

void cmd_bssid(String args) {
  Serial.print("BSSID:");
  if(args != "") {
    parseBssidStr(args, wifiutils_bssid);
  }
  Serial.println(bssidStr(wifiutils_bssid));
}


void cmd_pwd(String args) {
  Serial.print("Password:");
  if(args != "") wifiutils_password = args;
  Serial.println(wifiutils_password);
}

void cmd_trace(String args) 
{
  if(args == "on") {
      esp_log_level_set("*", ESP_LOG_VERBOSE);
    Serial.println("Trace is on");
  } else if(args == "off") {
      esp_log_level_set("*", ESP_LOG_ERROR);
    Serial.println("Trace is off");
  } else {
    Serial.println("trace:on|off");
  }
}

void cmd_scan(String args) {
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(") [");
            Serial.print(WiFi.channel(i));
            Serial.print("] ");
            Serial.print(WiFi.BSSIDstr(i));
            Serial.print(" ");
            Serial.println(encryptionTypeStr(WiFi.encryptionType(i)));
            delay(10);
        }
    }
    Serial.println("");
}  
