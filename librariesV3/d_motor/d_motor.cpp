#include "d_motor.h"

#define DAC_RES 1023
#define MOTOR_ON_V 3.4
#define MAX_ARD_V 3.4

float d_inMin = 0.0;
float d_inStep = 32.10;
float d_inMax = 100.0;
float d_outMin = 0.0;
float d_outStep = .001;
float d_outMax = 100.0;

int d_dirOld = 0;

void MotorRecalibrate(float inMin, float inStep, float inMax, float outMin, float outStep, float outMax){
  d_inMin = inMin;
  d_inStep = inStep;
  d_inMax = inMax;
  d_outMin = outMin;
  d_outStep = outStep;
  d_outMax = outMax;
} 

//This code you cn put in the void setup area of your code. It sets up the pins.
void MotorSetup(int dirPin, int enPin) {
  pinMode(dirPin, OUTPUT);
  //pinMode(dacPin, OUTPUT); dont need to initialize dac, changes function, cant use ny pther pin
  pinMode(enPin, OUTPUT);
}

void MotorTest(int motorDir, float effort, int dirPin, int dacPin, int enPin) {
  // motorDir is the direction pin, 1 for CW and 0 foir CCW
  // effort is the "speed or torque" of the motor. 75 is the max!
  // Serial.println(effort);
  //MotorOn(motorDir, effort, dirPin, dacPin, enPin);
  Serial.println(MotorOn(motorDir, effort, dirPin, dacPin, enPin));
}

int MotorOn(int motorDir, float effort, int dirPin, int dacPin, int enPin) {
  //effort = constrain(effort, 0, 75) / 100;  //constrain effort in case of out of bounds input
  int dac = (1 - (effort / 100)) * DAC_RES * MOTOR_ON_V / MAX_ARD_V;
  digitalWrite(dirPin, motorDir);  //set motor direction to arg(motorDir)
  analogWrite(dacPin, dac);   // set motor power to arg(motorPwr)

  digitalWrite(enPin, 1); // set motor enable to on

  //Serial.print(dac); Serial.print('\t');
  return dac;
}

void MotorOff(int dacPin, int enPin) {
  analogWrite(dacPin, DAC_RES);  // set motor power to 0
  digitalWrite(enPin, 0); // set motor enable to off
}


void MotorControlEffort(float userInput, int dirPin, int dacPin, int enPin, bool printVals) {
  // determine if the motor changes direction
  int motorDir = MotorDirection(userInput);
  MotorCheckDir(motorDir, enPin);
  d_dirOld = motorDir;
  // map control effort to motor space
  float effort = MotorMapping(userInput, d_inMin, d_inStep, d_inMax, d_outMin, d_outStep, d_outMax);
  // run motorOn
  int dac = MotorOn(motorDir, effort, dirPin, dacPin, enPin);
  // print values if needed
  if (printVals == 1) {
    Serial.print("User entered ");
    Serial.println(userInput);
    Serial.print("Effort value is ");
    Serial.println(effort);
    Serial.print("DAC value is ");
    Serial.println(dac);
    Serial.print("Direction is ");
    Serial.println(motorDir);
  }

  //Serial.print(motorDir);
  //Serial.print('\t');
}

int MotorDirection(float a) {
  if (a > 0) {
    return 1;
  } 
  else {
    return 0;
  }
}

float MotorMapping(float x, float y0, float y1, float y2, float x0, float x1, float x2) {
  x = abs(x);
  x = constrain(x, x0, x2);
  if (x < x1) {
    return (x - x0) * ((y1 - y0) / (x1 - x0)) + y0 ;
  }
  else {
    return (x - x1) * ((y2 - y1) / (x2 - x1)) + y1;
  }
}

void MotorCheckDir(int motorDir, int enPin) {
  if (motorDir != d_dirOld) {
    //digitalWrite(enPin, LOW);
    MotorOn(motorDir, 0, 12, A0, 13);

    delay(11);
  }
}
