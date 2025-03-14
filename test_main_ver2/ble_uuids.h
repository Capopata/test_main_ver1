#ifndef BLE_UUIDS_H
#define BLE_UUIDS_H

#include <BLEServer.h>

#define SERVICE_UUID        "c61cf2de-b11a-4c64-9eba-f3c44757f8a5"
#define CHARACTERISTIC_UUID "5a1f4286-8367-458f-b65d-08cf01671c7f"

#define TARGET_CHAR       "56ff2f09-bac7-4c72-9808-fe83a47a2e0e" //Target
#define RATE_CHAR         "f7ffb23d-3e50-474e-a583-3dea7e4f87fb" //Rate
#define TYPE_CHAR         "64c88a24-32d5-42ec-8039-2113f09459ce" //Syringe type
#define MODE_CHAR         "5b56d760-138a-4039-96f4-fbdcb9c870c1" //mode
#define IS_PUMPING_CHAR   "5cfa85d7-76d0-4ef7-9c91-d146d8313d01" //isPumping

BLECharacteristic *targetChar;
BLECharacteristic *rateChar;
BLECharacteristic *typeChar;
BLECharacteristic *modeChar;
BLECharacteristic *isPumpingChar;

float targetDistance; //  ml
float rotationRate; // ml/s
unsigned short  type;
unsigned short  mode;
unsigned short  isPumping;


#endif //BLE_UUIDS_H