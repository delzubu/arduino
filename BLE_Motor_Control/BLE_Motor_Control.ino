// Check BLE_Motor_Control_bb.png for schematics


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

const int PIN_INPUT = 2; // A0 / Analog IO 02 port as indicated on board
const int INPUT_RANGE[] = {1161, 4095}; // Measured min and max input values
const int OUTPUT_RANGE[] = {180, 0}; // 0-180

#define SERVICE_UUID        "96c27a9e-a4bf-443b-b65c-fb68dce1d99a"
#define CHARACTERISTIC_UUID "f38effdc-6191-4668-96b7-f0c2f47f32fb"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
int value = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  pinMode(PIN_INPUT, INPUT);
  Serial.begin(115200);

  BLEDevice::init("Motor Controller");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0); 
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

}

void loop() {

      // notify changed value
    if (deviceConnected) {
        int analogValue = analogRead(PIN_INPUT);
        int mappedValue = map(analogValue, INPUT_RANGE[0], INPUT_RANGE[1], OUTPUT_RANGE[0], OUTPUT_RANGE[1]);
        Serial.print("Value:"); Serial.println(value);
        pCharacteristic->setValue((uint8_t*)&value, sizeof(value));
        pCharacteristic->notify();
        value++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    
  // put your main code here, to run repeatedly:

}
