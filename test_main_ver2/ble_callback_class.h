#ifndef BLE_CALLBACK_CLASS_H
#define BLE_CALLBACK_CLASS_H

#include "BLEDevice.h"
#include "ble_uuids.h"
#include <stdint.h>

bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected!");
    BLEDevice::stopAdvertising();
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    //reset after disconnect
    targetChar->setValue("0");
    rateChar->setValue("0");
    typeChar->setValue("0");
    modeChar->setValue("0");
    isPumpingChar->setValue("0");
    Serial.println("Device disconnected");
    BLEDevice::startAdvertising();
  }
};


#endif //BLE_CALLBACK_CLASS_H