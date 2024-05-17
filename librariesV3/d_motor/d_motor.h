#ifndef D_MOTOR_H
#define D_MOTOR_H

#include <arduino.h>

void MotorRecalibrate(float inMin, float inStep, float inMax, float outMin, float outStep, float outMax);

void MotorSetup(int dirPin, int enPin);

void MotorTest(int dir, float effort, int dirPin, int dacPin, int enPin);

int MotorOn(int dir, float effort, int dirPin, int dacPin, int enPin);

void MotorOff(int dacPin, int enPin);

void MotorControlEffort(float userInput, int dirPin, int dacPin, int enPin, bool printVals);

int MotorDirection(float a);

float MotorMapping(float x, float y0, float y1, float y2, float x0, float x1, float x2);

void MotorCheckDir(int dir, int enPin);

#endif
