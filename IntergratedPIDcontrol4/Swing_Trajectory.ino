 void swing() {
  bool walkCont = true;
  unsigned long prevSerial = 0;
  unsigned long prevWalk = 0;
  unsigned long previousTime = 0;
  const unsigned long serialDelta = 500;  // microseconds, = 10 hz
  const unsigned long walkDelta = 500;      // microseconds, = 500Hz
  unsigned long stanceTime = 0; 
  int hotKey;
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
          theta = 0;
          walkCounter = 0;
          walkCont = 0;

          break;
        }
      }
    }

    if ((currentMicros - prevWalk) > walkDelta) {
      prevWalk = currentMicros;
    
    //   stanceState = StanceDetect(thresh2, thresh2, POTPIN2, POTPIN2);  //StanceDetect is a function in the motor library, measure if the linear potentiometers pass a certain threshhold
    //   //float pot1 = ReadPot(POTPIN1); //Calls on previously made function to read first potentiometer
    //  // float pot2 = ReadPot(POTPIN2); //Reads second potentiometer
    //  // Serial.print(pot1); //Print data from potentiometers to serial monitor
    //   //Serial.print("\t"); 
    //  // Serial.print(pot2);
    //  // Serial.print("\t");

    //   if (stanceState) {  //if stance detect is true, they are in stance
    //     theta = 0;        // set the desired angle to be fully extended
    //     walkCounter = 0;
    //     Ki = 0; 
    //     Serial.println("STANCE");

    //   }
    //   else if (!stanceState) {
    //     if (walkCounter == 37){ 
    //       Ki = 0; 
    //     }
    //     else {
    //       Ki = swingKi;
    //     }  
    //     if (walkCounter == 0) {
    //       Serial.println("SWING"); 
    //     }
        if (((currentMicros - previousTime) > walkPeriod ) && (walkCounter <= 36)) {  //only update to the next point in the trajectory once the time period alloted for each point has passed
          
          walkCounter = walkCounter + 1; 
          previousTime = currentMicros;
        }
         if (((currentMicros - previousTime)> 0) && (walkCounter <= 36)) {
          theta1 = traj[walkCounter]; 
          theta2 = traj[walkCounter+1]; 
          phase = (currentMicros - previousTime) / walkPeriod;
          
          deltaTraj = (theta2-theta1);  
          theta = deltaTraj * phase + theta1; 
           
               
        
      } 
    }
    encValUnfiltered = ReadEncoderUnfiltered(ENC_CS) - offset;
    encVal = prevEncVal * encAlpha + encValUnfiltered * (1.0 - encAlpha);
    prevEncVal = encVal;
    stanceTime = currentMicros; 

    runMotor(theta);

    if (walkCounter == 37) { 
      MotorControlEffort(0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
      theta = 0;
      walkCounter = 0;
      swingKi = 0; 
      walkCont = 0;

      break;
    }
    else {
      swingKi = Ki; 
    }
  }
}


//used in the main routine cases to run the walking controller
void swingCase() {
  int hotkey;
  if (Serial.available()) {
    hotkey = Serial.read();
    if (hotkey == 'e') {
      exit(1);
    }
  } else {
    swing();
  }
}
