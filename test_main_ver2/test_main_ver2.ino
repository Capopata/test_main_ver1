#include <Arduino.h>
#include <BLEDevice.h>
#include "config.h"
#include "display.h"
#include "motor_control.h"
#include "ble_callback_class.h"
#include "ble_uuids.h"

float StepInterval;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  drawStartupScreen();
  delay(1000);                                            // Dừng màn hình khởi động
  BluetoothSetup();                                       // Thiết lập bluetooth
  drawUI(targetDistance, rotationRate, deviceConnected);  // Vẽ giao diện chính
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(T_CS , OUTPUT);
  digitalWrite(T_CS, HIGH);
}

void loop() {
  // While user connect with esp32, device will start
  //isPumping =0;
  while (deviceConnected)
  {
    checking += 1;
    if(checking == 1)
      {
        drawUI(0, 0, deviceConnected);
        tft.setCursor(30, 30);
        tft.println("Bluetooth connected");
      }
    switch (motorState)
      {
        case START:
          // Read value when isPumping =0
          if( isPumping == 0)
            {
              targetDistance = String(targetChar->getValue().c_str()).toFloat();
              rotationRate = String(rateChar->getValue().c_str()).toFloat();
              type = String(typeChar->getValue().c_str()).toInt();
              mode = String(modeChar->getValue().c_str()).toInt();// mode = 0 - WITHDRAW; mode = 1 - INFUSE
              isPumping = String(isPumpingChar->getValue().c_str()).toInt(); //isPumping = 0 - STOP
              NoticeSerial();  // Serial thông báo các giá trị nhận được từ bluetooth
              delay(100);
            }
          if(isPumping == 1)
            {
              drawUI(targetDistance, rotationRate, deviceConnected );
              changeValue = 1;
              timeRun = targetDistance * 1e3 / rotationRate +100;  // Thời gian để bơm hết
              timeRun1 = timeRun;            
              
              StepInterval = calculateDelayTime(rotationRate, targetDistance, screwType, stepsPerRevolution);
              digitalWrite(ENABLE_PIN, LOW);
              digitalWrite(DIR_PIN, mode);

              //set đồ họa
              tft.setFreeFont(&FreeSansBold9pt7b);
              drawSlidingBox(10, 120, 200, 30, PROGRESS_COLOR, TEXT_COLOR, "Time left: ");
              Serial.println(" - Set up done! " );
              motorState = PUMPSUCTION;
              prevStepMillis = curMillis;
            }
          break;
        case PUMPSUCTION:
          // Step the motor
          curMillis = millis();

          if (curMillis - prevStepMillis >= StepInterval) 
            {
                prevStepMillis = curMillis;
                runTime+= StepInterval;
                countTime+=StepInterval;
                digitalWrite(STEP_PIN, HIGH);
                digitalWrite(STEP_PIN, LOW);
            }else
            {
              isPumping = String(isPumpingChar->getValue().c_str()).toInt();
              if (isPumping == 0 )
                {
                  isPumping = 2;
                  digitalWrite(ENABLE_PIN, HIGH);
                  motorState = STOP;
                  break;
                }
              mode = String(modeChar->getValue().c_str()).toInt();
              if( modePre != mode)
                {
                  modePre = mode;
                  digitalWrite(DIR_PIN, mode);
                }
              if (runTime>0 && countTime >timeRun)
                {
                  runTime = 0;
                }
              if (countTime> timeRun && runTime ==0 ) 
                {
                  Serial.print("complete!");
                  digitalWrite(ENABLE_PIN, HIGH);
                  motorState = STOP;

                  delay(1000);
                  progress = 0;
                  drawUI(0, 0, deviceConnected);
                  checking =0;
                }
              if(runTime >= 1e3 && timeRun1 > runTime) 
                {
                  //timeRun1: thời gian còn lại; timeRun: thời gian chạy; runTime: thời gian để đưa ra màn hình sau 1s
                  uint64_t timeRun2 = timeRun1;
                  uint8_t hh = timeRun2 / (1000 * 60 * 60);  // Chia để lấy giờ
                  timeRun2 -= hh * 3600 * 1000;              // Trừ số giờ đã tính
                  uint8_t mm = timeRun2 / (1000 * 60);       // Chia để lấy phút
                  timeRun2 -= mm * 1000 * 60;                // Trừ số phút đã tính
                  uint8_t ss = timeRun2 / 1000;              // Chia để lấy giây
                  // Đồ họa thời gian
                  Timing(hh, mm, ss);                                         // Đưa thời gian ra màn hình
                  timeRun1 = timeRun1 - 1e3;
                  runTime -= 1e3;
                }
            }
          break;
        case STOP:
          isPumping = String(isPumpingChar->getValue().c_str()).toInt();
          mode = String(modeChar->getValue().c_str()).toInt();
          if(isPumping == 1 )
            {
              timeRun1 = 0;
              countTime = 0;
              motorState = PUMPSUCTION;
              digitalWrite(ENABLE_PIN, LOW);
              digitalWrite(DIR_PIN, mode);
              modePre = mode;
            }
          if (isPumping == 0)
            {
              motorState = START;
              checking = 1;
            }
          break;
      }
  }
  if (!deviceConnected)
    {
      if(checking > 0)
        {
          progress = 0;
          drawUI(0, 0, deviceConnected);
          checking =0;
        }
      tft.setCursor(30, 30);
      tft.println("Bluetooth disconnected");
      changeValue = 0;
      motorState = START;
      firstNotify =0;
      countTime = 0;
    }
}
void BluetoothSetup() {
  Serial.println("Starting BLE work!");
  BLEDevice::init("MMMMMMMicropump 2024");         //make a server with the name in "___"
  BLEServer *pServer = BLEDevice::createServer();  // create the server
  pServer->setCallbacks(new MyServerCallbacks());  //set a call back

  BLEService *pService = pServer->createService(SERVICE_UUID);  //make a service for the characteristics

  // 5 data channels
  targetChar = pService->createCharacteristic(TARGET_CHAR, BLECharacteristic::PROPERTY_WRITE);
  rateChar = pService->createCharacteristic(RATE_CHAR, BLECharacteristic::PROPERTY_WRITE);
  typeChar = pService->createCharacteristic(TYPE_CHAR, BLECharacteristic::PROPERTY_WRITE);
  modeChar = pService->createCharacteristic(MODE_CHAR, BLECharacteristic::PROPERTY_WRITE);
  isPumpingChar = pService->createCharacteristic(IS_PUMPING_CHAR, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);


  pService->start();
  Serial.println("service started!");
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  //make the bluetooth visable to the things
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x00);
  BLEDevice::startAdvertising();
}
void NoticeSerial() {
  Serial.printf("Rate:  %f \n", rotationRate);
  Serial.printf("Target:  %f \n", targetDistance);
  Serial.printf("Type: %d \n", type);
  Serial.printf("Mode: %d\n", mode);
  Serial.printf("Running: %d\n", isPumping);
  Serial.println("-----------------");
}
// Cài đặt ban đầu
void drawUI(float targetDistance, float rotationRate, bool deviceConnected) {

  tft.fillScreen(BACKGROUND_COLOR);

  // Tiêu đề
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(30, 30);
  if (!deviceConnected) {
    tft.println("Bluetooth disconnected");
  } else {
    tft.println("Bluetooth connected");
  }

  // Hộp Target
  Target = String(targetDistance);
  Target = "Target: " + Target + " ml";
  drawSlidingBox(10, 40, 200, 30, TARGET_BOX_COLOR, TEXT_COLOR, Target);

  // Hộp Rates
  Rates = String(rotationRate, 5);
  Rates = "Rates: " + Rates + " ml/s";
  drawSlidingBox(10, 80, 200, 30, RATES_BOX_COLOR, TEXT_COLOR, Rates);

  // Hộp Progress (căn giữa)
  if (targetDistance == 0 || rotationRate == 0) {
    drawSlidingBox(10, 120, 200, 30, PROGRESS_COLOR, TEXT_COLOR, "Time left: __:__:__");
  }


}


