//Control for determining  how to run the motor when climbing stairs
void stairs(int POTPIN1, int POTPIN2, int thresh1, int thresh2, float theta, int weightFactor) {
  int hotKey = 'b';  //assigns the hotkey value to an arbitrary letter. This varible will be used to stop the procedure when a person types 'e'
  bool noSwing = 0;  //varible to keep track of if a person has been in stance or not
  bool stairCont = true;
  unsigned long prevSerial = 0;
  unsigned long prevWalk = 0;
  unsigned long previousTime = 0;
  unsigned long stanceTime = 0;
  const unsigned long serialDelta = 500;  // microseconds, = 10 hz
  const unsigned long walkDelta = 500;    // microseconds
  int walkCounter = 0;
  float deltaTraj;
  float phase;
  float theta2;
  float theta1;
  unsigned long swingStartTime;  // keeps track of when swing starts
  // float stairSwingPeriod = 2e6;
  // float dtstairSwingTraj = stairSwingPeriod / 34;

  while (stairCont) {
      //printData(0, 1, 1, 1,1); 
    currentMicros = micros();
    if ((currentMicros - prevSerial) > serialDelta) {
      prevSerial = currentMicros;

      if (Serial.available() > 0) {
        hotKey = Serial.read();
        if (hotKey == 'e') {
          MotorControlEffort(0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          Ki = 0;
          swingKi = 0;
          walkCounter = 0;
          stairCont = 0;

          break;
        }
      }
    }

    if ((currentMicros - prevWalk) > walkDelta) {
      prevWalk = currentMicros;
      // READ STANCE BEHAVIOR
      pin1Unf = ReadPot(POTPIN1);
      pin2Unf = ReadPot(POTPIN2);
      pin1 = prevPin1 * linPotAlpha + pin1Unf * (1.0 - linPotAlpha);
      prevPin1 = pin1;
      pin2 = prevPin2 * linPotAlpha + pin2Unf * (1.0 - linPotAlpha);
      prevPin2 = pin2;
      stanceState = (pin1 > thresh1) || (pin2 > thresh2);
      // stanceState = StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2);  //StanceDetect is a function in the motor library, measure if the linear potentiometers pass a certain threshhold
      // int t = micros();                                                //counts micro seconds since the board began running the current program

      // State 1 - STANCE - run stance whenever you are in stance
      if (stanceState) {  //if stance detect is true, they are in stance
        Serial.print("STANCE: \t");
        // set swing blocker (i.e. noSwing) to 0 so swing can run. you have to have gone into stance before you can initate swing again.
        noSwing = 0;

        //put code in here to run motor according to torque
        angleTorque = ((1.628e-5 * pow(encVal, 4)) - (.00158 * pow(encVal, 3)) + (.0591 * pow(encVal, 2)) - (1.4914 * encVal) - .2187) / 24.7729;  //used biomechanics data to find a function that detmines joint torqu at different angles

        // calcualte effort with weight factor
        angleTorque = angleTorque * weightFactor;  // weight factor sets the torque proportional to what someone weighs since they will need more torque.

        // if angle torque is positive (i.e flexion) set to 0 effort.
        if (angleTorque > 0) {
          angleTorque = 0;
        }

        // Simple fix for zero effort/flexion at theta >65 or so....
        // if (encVal > 65) {
        //   angleTorque = -10;
        // }
        walkCounter = 0;
        theta = encVal;
        runMotorTorque(angleTorque);  // this function runs the motor according to a torque we provided.
      }

      // State 2 - TERMINAL SWING -  hold last swing position
      if (noSwing && !stanceState) {  // if the person has not put their foot down it hold the leg to this angle
        Serial.print("HOLD: \t");
        theta = 64.0;
        runMotor(theta);
      }

      // State 3 - SWING - run swing when noSwing is false (i.e. started at least one stance phase) and not in stance
      if (!noSwing && !stanceState) {
      Serial.print("SWING: \t");
      
      if (((currentMicros - previousTime) > stairPeriod) && (walkCounter <= 35)) {  //only update to the next point in the trajectory once the time period alloted for each point has passed
        theta = stairSwingTraj[walkCounter];
        walkCounter = walkCounter + 1;
        previousTime = currentMicros;
      }

      if (((currentMicros - previousTime) > 0) && (walkCounter <= 35)) {

        theta1 = stairSwingTraj[walkCounter - 1];
        theta2 = stairSwingTraj[walkCounter];
        phase = (currentMicros - previousTime) / stairPeriod;

        deltaTraj = (theta2 - theta1);
        theta = deltaTraj * phase + theta1;
      }

      if (walkCounter > 35) {
        noSwing = 1;
      }

      runMotor(theta);
      // unsigned long currentTime = micros();
      // theta = stairSwingTraj[counter];                                                                                  //theta will be assigned values from this array by counting through the array
      // while ((counter < 34) && !StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2) && (hotKey != 'e') && noSwing == 0) {  //while the counter is less than or equal to the number of items in the array and stance detect is not true run this code
      //   if ((micros() - currentTime) > dtstairSwingTraj) {                                                              //only update to the next point in the trajectory once the time period alloted for each point has passed
      //     counter = counter + 1;                                                                                        //get the new angle value
      //     theta = stairSwingTraj[counter];                                                                              //assign the new angle value
      //     currentTime = micros();                                                                                       //document the currect time since the program startd
      //   }
      //   runMotor(theta);
      //   // once swing is finished, don't go into swing until you start stance
      //   if (micros() > (dtstairSwingTraj * 34) + currentMicros) {
      //     noSwing = 1;
      //   }
      //   // if (Serial.available() > 0) {
      //   //   hotKey = Serial.read();
      //   // }
      // }
    }
  
  Serial.print(theta);
  Serial.print("\t");
  encValUnfiltered = ReadEncoderUnfiltered(ENC_CS) - offset;
  encVal = prevEncVal * encAlpha + encValUnfiltered * (1.0 - encAlpha);
  prevEncVal = encVal;
  }
}
}
//Used in the main routine cases to run the stairs controller
// void stairsCase() {
//   if (Serial.available()) {
//     char inChar = Serial.read();
//     if (inChar == 'e') {
//       exit(1);
//     }
//   } else {
//     stairs(POTPIN1, POTPIN2, thresh1, thresh2, theta, weightFactor);
//     PotTest(POTPIN1, POTPIN2);
//   }
// }

void stairsCase() {
  int hotkey;
  if (Serial.available()) {
    hotkey = Serial.read();
    if (hotkey == 'e') {
      exit(1);
    }
  } else {
    stairs(POTPIN1, POTPIN2, thresh1, thresh2, theta, weightFactor);
  }
}
