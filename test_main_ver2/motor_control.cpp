#include "motor_control.h"
#include "display.h"
#include "config.h"

#define pi 3.1415926535897
/*
 Thông báo với Serial về các thông số nhận được khi người 
 dùng nhập từ điện thoại
*/
void CompNotifi() {
    drawSlidingBox(10, 160, 200, 30, WARNING_COLOR, TEXT_COLOR, "Progress:        %");
    delay(1000);
    progress = 0;
    Serial.print("Noitice: Completed !");  // noitification pumped
    //Đưa vitme về vị trí ban đầu
  }
  /*
   * Tính toán thời gian delay giữa các bước của động cơ dựa trên thông số kỹ thuật.
   * @param rate Tốc độ bơm (ml/s).
   * @param target Lượng chất lỏng cần bơm (ml).
   * @param type Loại vít (1 hoặc 2), ảnh hưởng đến hệ số bước/vòng.
   * @param stepsPerRevolution Số bước/vòng của động cơ.
   * @return Thời gian delay (microseconds) giữa các bước.
   */
float calculateDelayTime(float rate, float target, int type, float stepsPerRevolution) {
  // Kiểm tra giá trị đầu vào
  if (rate <= 0 || target <= 0) {
    Serial.println("Error: Invalid input in calculateDelayTime()");
    return 0;  // Trả về 0 để ngăn động cơ chạy
  }
  float Lead = 0.2;  // Bước vít (cm), khoảng cách di chuyển khi vít me quay 1 vòng
  float microsteps = 16.0;     // Số vi bước trên mỗi bước đấy đủ của động cơ
  // Công thức tính delayTime:
  // delayTime = (pipCrossSection*Lead)/(StepsPerRevolution*microsteps*Rates)
  //pipCrossSection = pi* r^2(cm^2)
  // Lead: bước của vitme (cm)
  // Rates: Lưu lượng bơm (ml/s)
  // StepsPerRevolution: số bước động cơ (bước/ vòng)
  // Đơn vị: Milisecond ( 1e3 chuyển từ giây sang mili giây)
  float delayTime = (pi*0.55*0.55*Lead*1e3)/(stepsPerRevolution*microsteps*rate);
  return delayTime;
}
  