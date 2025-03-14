#pragma once
#include <TFT_eSPI.h>
#include "config.h"

void drawStartupScreen();
void drawSlidingBox(int targetX, int y, int w, int h, uint16_t fillColor, uint16_t borderColor, String text);
void drawRoundedRect(int x, int y, int w, int h, uint16_t fillColor, uint16_t borderColor);
void Timing(uint8_t hh, uint8_t mm, uint8_t ss);
