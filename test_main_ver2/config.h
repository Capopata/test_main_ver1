#pragma once
#include <TFT_eSPI.h>
// Cấu hình chân GPIO
const int DIR_PIN = 27;
const int STEP_PIN = 26;
const int ENABLE_PIN = 25;
const int T_CS = 15;
// Định nghĩa màu sắc RGB
#define BACKGROUND_COLOR tft.color565(0, 0, 0)        // Đen
#define TEXT_COLOR tft.color565(255, 255, 255)        // Trắng
#define HIGHLIGHT_COLOR tft.color565(0, 255, 0)       // Xanh lá
#define WARNING_COLOR tft.color565(255, 0, 0)         // Đỏ
#define TARGET_BOX_COLOR tft.color565(0, 0, 255)      // Xanh dương
#define RATES_BOX_COLOR tft.color565(255, 165, 0)     // Cam
#define PROGRESS_COLOR tft.color565(128, 0, 128)      // Tím
#define BUTTON_COLOR_START tft.color565(0, 255, 255)  // Xanh ngọc
#define BUTTON_COLOR_RESET tft.color565(255, 255, 0)  // Vàng
#define STARTUP_COLOR tft.color565(0, 128, 255)       // Màu khởi động
#define STARTUP_TEXT_COLOR tft.color565(255, 255, 0)  // Vàng sáng

// Cấu hình động cơ và vít me
extern int screwType;
extern float stepsPerRevolution;

// Khai báo biến toàn cục
extern TFT_eSPI tft;
extern bool deviceConnected, stepState, modePre;
extern float targetDistance, rotationRate, targetDistancePre,rotationRatePre, curMillis, prevStepMillis;
extern int progress, changeValue, firstNotify, currentStep, checking, callback;
extern String Target, Rates;
extern unsigned long timeRun, timeRun1, countTime, delayTime;

