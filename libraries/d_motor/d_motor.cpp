#include "d_motor.h"

#define DAC_RES 1023
#define MOTOR_ON_V 3.4
#define MAX_ARD_V 3.4

//This code you cn put in the void setup area of your code. It sets up the pins.
void MotorSetup(int dirPin, int enPin){
  pinMode(dirPin, OUTPUT); 
  //pinMode(dacPin, OUTPUT); dont need to initialize dac, changes function, cant use ny pther pin
  
  pinMode(enPin, OUTPUT);
}

void MotorTest(int dir, float effort, int dirPin, int dacPin, int enPin){
  // dir is the direction pin, 1 for CW and 0 foir CCW
  // effort is the "speed or torque" of the motor. 75 is the max! 
  // Serial.println(effort);
  //MotorOn(dir, effort, dirPin, dacPin, enPin);
  Serial.println(MotorOn(dir, effort, dirPin, dacPin, enPin));
}

 int MotorOn(int dir, float effort, int dirPin, int dacPin, int enPin ) {
  //effort = constrain(effort, 0, 75) / 100;  //constrain effort in case of out of bounds input
  int dac = (1 - (effort/100)) * DAC_RES * MOTOR_ON_V / MAX_ARD_V;
  digitalWrite(dirPin, dir);  //set motor direction to arg(motorDir)
  analogWrite(dacPin, dac);   // set motor power to arg(motorPwr)
  
  digitalWrite(enPin, 1); // set motor enable to on
  return dac;
}

void MotorOff(int dacPin, int enPin) {
  analogWrite(dacPin, DAC_RES);  // set motor power to 0
  digitalWrite(enPin, 0); // set motor enable to off
}
