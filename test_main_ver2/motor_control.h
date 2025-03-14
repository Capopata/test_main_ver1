#pragma once
#include "config.h"

enum MotorState { START, PUMPSUCTION, STOP };
extern MotorState motorState;
extern unsigned long motorRunningTime, runTime;

float calculateDelayTime(float rate, float target, int type, float stepsPerRevolution);

void NoticeSerial();