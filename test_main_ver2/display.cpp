#include "display.h"
#include "config.h"


// Triển khai các hàm vẽ màn hình ở đây (giữ nguyên code gốc)
void drawStartupScreen() {
    tft.fillScreen(STARTUP_COLOR);
    tft.setTextColor(STARTUP_TEXT_COLOR);
    tft.setFreeFont(&FreeSansBold9pt7b);  // Sử dụng font chữ mới
  
    // Hiệu ứng phóng to chữ "WELCOME!"
    for (int size = 1; size <= 3; size++) {
      tft.fillScreen(STARTUP_COLOR);  // Xóa màn hình
      tft.setTextSize(size);
      tft.setCursor(10, 100);
      tft.println("WELCOME!");
      delay(200);
    }
    // viết chữ Initializing
    tft.setTextSize(1);
    tft.setFreeFont(&FreeSansBold9pt7b);  // Sử dụng font nhỏ hơn
    tft.setCursor(20, 140);
    tft.println("Initializing...");
  }

  
// Hộp trượt
void drawSlidingBox(int targetX, int y, int w, int h, uint16_t fillColor, uint16_t borderColor, String text) {
    drawRoundedRect(targetX, y, w, h, fillColor, borderColor);
    tft.setCursor(targetX + 5, y + 20);
    tft.print(text);
  }
// Hình chữ nhật bo góc
void drawRoundedRect(int x, int y, int w, int h, uint16_t fillColor, uint16_t borderColor) {
    int r = 5;
    tft.fillRoundRect(x, y, w, h, r, fillColor);
    tft.drawRoundRect(x, y, w, h, r, borderColor);
  }
// Thanh tiến trình

void Timing(uint8_t hh, uint8_t mm, uint8_t ss) 
{
    tft.setTextColor(TEXT_COLOR, PROGRESS_COLOR);
    String Text = "Time left: ";
    int xtime = 20 + tft.textWidth(Text.c_str()), ypos = 140;
  
    auto drawWithLeadingZero = [&](uint8_t value) {
      if (value < 10) {
        xtime += tft.drawNumber(0, xtime, ypos - 13);
      }
      xtime += tft.drawNumber(value, xtime, ypos - 13);
    };
  
  
    // Draw hours
    drawWithLeadingZero(hh);
    xtime += tft.drawChar(':', xtime, ypos);
  
    // Draw minutes
    drawWithLeadingZero(mm);
    xtime += tft.drawChar(':', xtime, ypos);
  
    // Draw seconds
    drawWithLeadingZero(ss);
  
}

