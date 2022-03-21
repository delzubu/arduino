#include "Lpf2Hub.h"

// create a hub instance
Lpf2Hub myHub;
bool isInitialized = false;
byte port = (byte)PoweredUpHubPort::A;
bool h = false;
int speed = 30;

const int LED_PIN = 2;

void setup()
{
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  myHub.init(); // initalize the remote instance and try to connect
}

void connect() {
  // connect flow
  if (myHub.isConnecting())
  {
    digitalWrite(LED_PIN, h ? LOW : HIGH);
    h = !h;
    delay(200);

    if (myHub.getHubType() == HubType::POWERED_UP_HUB) {
      myHub.connectHub();
    }
    
    if (myHub.isConnected())
    {
      Serial.println("Connected to Remote");
    }
    else
    {
      Serial.println("Failed to connect to Remote");
    }
  }

  if (myHub.isConnected() && !isInitialized)
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("System is initialized");
    isInitialized = true;
    delay(200); //needed because otherwise the message is to fast after the connection procedure and the message will get lost
    // both activations are needed to get status updates
  }
}

void run() {
  myHub.setLedColor(RED);
  myHub.setBasicMotorSpeed(port, speed);
  speed = -speed;
  delay(1000);
  myHub.setLedColor(BLUE);
  myHub.stopBasicMotor(port);
  delay(1000);  
}

// main loop
void loop()
{
  connect();

  if (myHub.isConnected() && isInitialized) {
    run();
  }
} 
