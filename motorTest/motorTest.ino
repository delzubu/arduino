const int PIN_MOTOR = 14;

void setup() {
  Serial.begin(115200);

  ledcSetup(0, 50, 10);
  ledcAttachPin(PIN_MOTOR,0);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("26 (25.6) - 0 degrees");
  ledcWrite(0, 26);
  delay(2000);

  Serial.println("51 - 45 degrees");
  ledcWrite(0, 51);
  delay(2000);

  Serial.println("77 (76.8) - 90 degrees");
  ledcWrite(0, 77);
  delay(2000);

  Serial.println("128 - 180 degrees");
  ledcWrite(0, 128);
  delay(2000);
}
