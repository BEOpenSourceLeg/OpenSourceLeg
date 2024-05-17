//run through PID equations to take a desired theta and return a effort value to give to the motor
float PID(float theta) {
  
  currentMicros = micros(); 
  float dt = 1;

  dt = (currentMicros - lastDerivative) / 1000000.0;  // calculates change in time between execution
  error = theta - (encVal);       // desired angle minus current error with regard to the offset
  iError += error * dt;// calculates intergral error

 if (Ki != 0){ 
  if (iError >= (5/Ki)) { 
      iError = 5/Ki; 
  }
  else if (iError <= (-5/Ki)){ 
    iError = -5/Ki; 
  }
 }
  
  dError = dError * alpha + (error - ePrev) / dt * (1.0 - alpha);  // calculates derivative error

  ePrev = error;  // updates the previous error
  lastDerivative = currentMicros;

  float controlEffort = Kp * error + Ki * iError + Kd * dError;  // calculates the effort the motor should run at
  controlEffort = constrain(controlEffort, -100, 100);           //constrains these values between pos/neg 100

  return controlEffort;  // returns an effort for the motor
}