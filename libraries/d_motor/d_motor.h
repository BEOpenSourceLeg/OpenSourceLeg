#ifndef D_MOTOR_H
#define D_MOTOR_H

#include <arduino.h>

void MotorSetup(int dirPin, int enPin);

void MotorTest(int dir, float effort, int dirPin, int dacPin, int enPin);

int MotorOn(int dir, float effort, int dirPin, int dacPin, int enPin);

void MotorOff(int dacPin, int enPin);

#endif
