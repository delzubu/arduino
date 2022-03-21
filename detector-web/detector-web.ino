#include <dummy.h>
#include <WiFi.h>
#include <WiFiServer>

const int SENSOR_PORT = 2;

const int BUFFER_LENGTH = 20;
const int TRIGGER_THRESHOLD = 25;
const int CHANGE_CONFIDENCE = 3;

int last_values[BUFFER_LENGTH];
int last_position = 0;
int moving_average;

int change_indicator = 0;

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER_PORT, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("esp32_1");
  WiFi.connect("TaliganetMB 2", "Miu321Miu");
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}

int movingAverage(int newValue) {
  last_position++;
  if (last_position == BUFFER_LENGTH) last_position = 0;
  moving_average = moving_average - (last_values[last_position] / BUFFER_LENGTH) + (last_values[last_position] = newValue) / BUFFER_LENGTH;

  return moving_average;
}

void loop() {
  // put your main code here, to run repeatedly:
  measureLight();
  
  delay(10);
}

void measureLight() {
  int analogValue = analogRead(SENSOR_PORT);
  if (abs(analogValue - movingAverage(analogValue)) > TRIGGER_THRESHOLD) {

    if (change_indicator == CHANGE_CONFIDENCE) {
      Serial.println("TRIGGER onChange");
      if (onChange) onChange(change_indicator);
    }
    change_indicator ++;
  } else {
    if (change_indicator > 0) {
      change_indicator--;

      if (change_indicator == 0) {
        Serial.println("TRIGGER offChange");
        if (offChange) offChange(change_indicator);
      }
    }
  }
}

