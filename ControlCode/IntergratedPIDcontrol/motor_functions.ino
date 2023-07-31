


// if the motor angle surpasses these two angles it should turn the motor off the prevent it from slamming into the end stops
void eStop() {
  if ((ReadEncoder(ENC_CS) - offset) > 80) {
    MotorOn(0, 0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  }
  if ((ReadEncoder(ENC_CS) - offset) < .8) {
    MotorOn(0, 0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  }
}

//Runs the motor by taking in the desired theta and using the PID function to calculate the effort
void runMotor(float theta) {
  int cEffort = PID(theta);  // assign PID return value to cEffort
  int motorDir;         // sets the motor direction
  
  if (cEffort  > 0) { // direction control
    motorDir = 1;
  }
  else {
    motorDir = 0;
  }
  cEffort = abs(cEffort);  // makes the control effort positive because the motor speed only takes positive values
  cEffort = map(cEffort, 0, 100, 26, 80); //maps the effort to a range the motor can handle

  if ((ReadEncoder(ENC_CS) - offset) > 85) {
    cEffort = 26;
  }
  /*if ((ReadEncoder(ENC_CS) - offset) < .8) {
    cEffort = 26;
  }*/
  MotorOn(motorDir, cEffort, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN); // runs motor by calling on function in the d_motor library

  // prints useful information
  Serial.print(cEffort);
  Serial.print("\t");
  Serial.print(ReadEncoder(ENC_CS) - offset);
  Serial.print("\t");
  Serial.print(theta);
  Serial.print("\t");
  Serial.println(motorDir);
}


//Uses the value created from the stance phase of stairs to run motor at specific torque value
void runMotorTorque(float angleTorque) {
  int contEffort = (angleTorque);
  int motorDir;         // sets the motor direction
  
  if (contEffort > 0) {
    motorDir = 1;
  }
  else {
    motorDir = 0;
  }
  contEffort = abs(contEffort);
  if (contEffort > 80) {
    contEffort = 80;
  }
  if ((ReadEncoder(ENC_CS) - offset) > 85) {
    contEffort = 26;
  }
  /*if ((ReadEncoder(ENC_CS) - offset) < .8) {
    contEffort = 26;
  }*/
  MotorOn(motorDir, contEffort, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  Serial.print(ReadEncoder(ENC_CS) - offset);
  Serial.print("\t");
  Serial.print(angleTorque);
  Serial.print("\t");
  Serial.print(contEffort);
  Serial.print("\t");
  Serial.println(motorDir);
}
