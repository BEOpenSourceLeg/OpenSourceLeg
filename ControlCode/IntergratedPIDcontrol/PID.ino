
//declare variables - these are used across function calls so they are presistentn.
int ePrev = 0;        // previous error
float error;          // difference between desired angle and current angle
float iError;         // intergral error
float dError = 0;     // derivative error
unsigned long lastDerivative;

//run through PID equations to take a desired theta and return a effort value to give to the motor
int PID(float theta) {
  ePrev = error; // updates the previous error
  float deprev = dError; // updates the previous derivative error
  float maxIntergral = 100 / Ki; //Contrains the intergral variable to a maximum
  float dt = 1;         // change in time between each time the code is ran

  dt = ((micros() - lastDerivative) / 1000000.0); // calculates change in time between execution

  error = theta - (ReadEncoder(ENC_CS) - offset); // desired angle minus current error with regard to the offset
  iError += error * dt;     // calculates intergral error
  iError = constrain(iError, -maxIntergral, maxIntergral); // constrains intergral error to be between tghe positive and negative value of max intergral
  dError = dError * alpha + (error - ePrev) / dt * (1 - alpha); // calculates derivative error

  lastDerivative = micros();

  int controlEffort = Kp * error + Ki * iError + Kd * dError; // calculates the effort the motor should run at
  controlEffort = constrain (controlEffort, - 100, 100);  //constrains these values between pos/neg 100

  return controlEffort; // returns an effort for the motor
}

void runStepResponse(float theta) {
  int hotKey = 'b';
  while (hotKey != 'e') {
    if (Serial.available() > 0) {
      hotKey = Serial.read();
    } else {
      runMotor(theta);
    }
  }
}
