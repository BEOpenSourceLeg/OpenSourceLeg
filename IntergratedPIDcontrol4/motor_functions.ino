
// if the motor angle surpasses these two angles it should turn the motor off the prevent it from slamming into the end stops
void eStop() {
  if ((encVal) > 80) {
    MotorOn(0, 0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  }
  if ((encVal) < .8) {
    MotorOn(0, 0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  }
}

//Runs the motor by taking in the desired theta and using the PID function to calculate the effort
void runMotor(float theta) {
  cEffort = PID(theta);  // assign PID return value to cEffort
  if ((encVal) > 85) {
    cEffort = 0;
  }
  MotorControlEffort(cEffort, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
  printData(0, 1, 1, 1,1); 
}


//Uses the value created from the stance phase of stairs to run motor at specific torque value
void runMotorTorque(float angleTorque) {
  contEffort = angleTorque;

  if ((encVal) > 85) {
    contEffort = 0;
  }
  // motorReset(contEffort); 
  printData(0, 1, 1, 1,1); 
   MotorControlEffort(contEffort, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
  //delay(100000);
}
