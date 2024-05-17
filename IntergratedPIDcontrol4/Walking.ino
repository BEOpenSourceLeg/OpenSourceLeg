
// State machine that determines if in the stance or swing phase of walking and sets the motor to a certain effort
void walking(int POTPIN1, int POTPIN2, int thresh1, int thresh2) {
  bool walkCont = true;
  unsigned long prevSerial = 0;
  unsigned long prevWalk = 0;
  unsigned long previousTime = 0;
  unsigned long stanceTime = 0; 
  const unsigned long serialDelta = 500;  // microseconds, = 10 hz
  const unsigned long walkDelta = 500;      // microseconds, = 500Hz
  int walkCounter = 0;
  float deltaTraj; 
  float phase; 
  float theta2;
  float theta1; 

  while (walkCont) {
    currentMicros = micros();
    if ((currentMicros - prevSerial) > serialDelta) {
      prevSerial = currentMicros;
      // Serial.println("hotkey!");
      if (Serial.available() > 0) {
        hotKey = Serial.read();
        if (hotKey == 'e') {
          MotorControlEffort(0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          Ki = 0; 
          swingKi = 0;
          theta = 0;
          walkCounter = 0;
          walkCont = 0;

          break;
        }
      }
    }

    if ((currentMicros - prevWalk) > walkDelta) {
      prevWalk = currentMicros;
      //stanceState = StanceDetect(thresh2, thresh2, POTPIN2, POTPIN2);  //StanceDetect is a function in the potentiometer library, measure if the linear potentiometers pass a certain threshhold
      pin1Unf = ReadPot(POTPIN1); 
      pin2Unf = ReadPot(POTPIN2); 
      pin1 = prevPin1 * linPotAlpha + pin1Unf * (1.0 - linPotAlpha);
      prevPin1 = pin1;
      pin2 = prevPin2 * linPotAlpha + pin2Unf * (1.0 - linPotAlpha);
      prevPin2 = pin2;

      stanceState = (pin1 > thresh1) || (pin2 > thresh2);    
      if ((stanceState == true) && (encVal < 10)) {  //if stance detect is true, they are in stance
        theta = 0;        // set the desired angle to be fully extended
        walkCounter = 0;
        swingKi = 0; 
        Serial.println("STANCE");
        stanceTime = micros(); 
      }

      else if (!stanceState) {
        if (walkCounter == 37){ 
          swingKi = 0; 
      }
        else {
          swingKi = Ki;
        }  
        if (walkCounter == 0) {
          Serial.println("SWING"); 
          theta = 0; 
        }
        if (((currentMicros - previousTime) > walkPeriod ) && (walkCounter <= 36)) {  //only update to the next point in the trajectory once the time period alloted for each point has passed 
          theta = traj[walkCounter];
          walkCounter = walkCounter + 1; 
          previousTime = currentMicros;
        } 

         if (((currentMicros - previousTime)> 0) && (walkCounter <= 36)){

          theta1 = traj[walkCounter-1]; 
          theta2 = traj[walkCounter]; 
          phase = (currentMicros - previousTime) / walkPeriod;
          
          deltaTraj = (theta2-theta1);  
          theta = deltaTraj * phase + theta1;  

         }    
        
      } 
    }
    encValUnfiltered = ReadEncoderUnfiltered(ENC_CS) - offset;
    encVal = prevEncVal * encAlpha + encValUnfiltered * (1.0 - encAlpha);
    prevEncVal = encVal;
    runMotor(theta);   
  }
}


//used in the main routine cases to run the walking controller
void walkingCase() {
  int hotkey;
  if (Serial.available()) {
    hotkey = Serial.read();
    if (hotkey == 'e') {
      exit(1);
    }
  } else {
    walking(POTPIN1, POTPIN2, thresh1, thresh2);
  }
}
