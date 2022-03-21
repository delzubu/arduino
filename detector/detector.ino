#include <dummy.h>

const int SENSOR_PORT = 2;
const int BUZZER_PORT = 13;

const int BUFFER_LENGTH = 20;
const int TRIGGER_THRESHOLD = 25;
const int CHANGE_CONFIDENCE = 3;

int last_values[BUFFER_LENGTH];
int last_position = 0;
int moving_average;

int change_indicator = 0;

unsigned long buzzer_off = 0;

void (*onChange)(int) = NULL;
void (*offChange)(int) = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER_PORT, OUTPUT);

  onChange = on_change;
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
  buzzerOff();
  
  delay(10);
}

void measureLight() {
  int analogValue = analogRead(SENSOR_PORT);
  if (abs(analogValue - movingAverage(analogValue)) > TRIGGER_THRESHOLD) {
//    Serial.print(analogValue);
//    Serial.print(" (");
//    Serial.print(moving_average);
//    Serial.println(")");

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

void buzzerOff() {
  if(buzzer_off > 0 && micros() > buzzer_off) {
    Serial.println("BUZZER OFF");
    buzzer_off = 0;
    noTone();
  }
}

void on_change(int changeLevel) {
  Serial.println("BUZZER ON");
  tone(BUZZER_PORT, 1000);
  buzzer_off = micros() + 1000000;
}


int playing = 0;
void tone(byte pin, int freq) {
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(pin, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
  playing = pin; // store pin
}
void noTone() {
  tone(playing, 0);
}
