#include"config.h"

TFT_eSPI tft = TFT_eSPI();  // Khởi tạo màn hình



// Cấu hình của động cơ và vít me
int screwType = 2;                 // Loại vít me 2mm
float stepsPerRevolution = 200.0;  // Số bước đầy đủ để động cơ quay 1 vòng( không tính vi bước)

// Khai báo biến để quản lý trạng thái động cơ
enum MotorState { START,
                  PUMPSUCTION,
                  STOP };
MotorState motorState = START;
unsigned long motorRunningTime;  // Using when motor run
unsigned long runTime = 0;       // Thời gian chạy (s)
unsigned long timeRun, timeRun1, delayTime; //timeRun = Rate/Target; timeRun1 : Duplicate of timeRun
unsigned long countTime;  // Count time run
int currentStep = 0;      // Bước hiện tại trong một vòng quay
bool stepState = LOW;     // Trạng thái xung STEP (HIGH/LOW)
bool modePre;
int checking ;// if device disconect, screen change the 

//Khai báo biến vị trí hiển thị trên màn hình
int xPos = tft.width(), xBlue = 180, progress = 0, firstNotify =0, changeValue = 0;  //set poisition x-axis for time pump
float targetDistancePre, rotationRatePre, prevStepMillis=0, curMillis = 0;
String Rates, Target;