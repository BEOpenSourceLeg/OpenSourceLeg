
//declare variables
double stairSwingTraj[] = { 0.0732, 0.1252, 1.2574, 5.1179, 12.8734, 23.4831, 37.2224, 53.8639, 73.2341, 77.0469, 80.3527, 79.2153, 79.6884, 77.0876, 76.8827, 74.295, 72.0177, 69.9621, 67.9742, 66.7435, 65.9836, 65.6217, 66.1194, 66.5125, 66.8728, 66.4739, 64.6795, 64, 64, 64, 64, 64
                            , 64, 64, 64, 64, 64
                          };


//Control for determining  how to run the motor when climbing stairs
void stairs(int POTPIN1, int POTPIN2, int thresh1, int thresh2, float theta, int weightFactor) {
  int hotKey = 'b'; //assigns the hotkey value to an arbitrary letter. This varible will be used to stop the procedure when a person types 'e'
  boolean noSwing = 0; //varible to keep track of if a person has been in stance or not
  unsigned long swingStartTime; // keeps track of when swing starts
  float stairSwingPeriod = 4e6;
  float dtstairSwingTraj = stairSwingPeriod / 34;
  float angleTorque;
  
  while (hotKey != 'e') {// while the hotkey value does not equal 'e' the code will keep running
    if (Serial.available() > 0) {
      hotKey = Serial.read();
    }

    // READ STANCE BEHAVIOR
    stanceState = StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2); //StanceDetect is a function in the motor library, measure if the linear potentiometers pass a certain threshhold
    int t = micros(); //counts micro seconds since the board began running the current program

    // State 1 - STANCE - run stance whenever you are in stance
    if (stanceState) { //if stance detect is true, they are in stance
      // set swing blocker (i.e. noSwing) to 0 so swing can run. you have to have gone into stance before you can initate swing again.
      noSwing = 0;

      //put code in here to run motor according to torque
      float thetaRead = ReadEncoder(ENC_CS) - offset;
      float angleTorque = ((1.628e-5 * pow(thetaRead, 4)) - (.00158 * pow(thetaRead, 3)) + (.0591 * pow(thetaRead, 2)) - (1.4914 * thetaRead) - .2187 ) / 24.7729; //used biomechanics data to find a function that detmines joint torqu at different angles

      // if angle torque is positive (i.e flexion) set to 0 effort.
      if (angleTorque > 0) {
        angleTorque = 0;
      }

      // calcualte effort with weight factor
      angleTorque = angleTorque * weightFactor;// weight factor sets the torque proportional to what someone weighs since they will need more torque.

      // Simple fix for zero effort/flexion at theta >65 or so....
      if (thetaRead > 55) {
        angleTorque = -50;
      }
      Serial.print("Stance:\t");
      runMotorTorque(angleTorque); // this function runs the motor according to a torque we provided.
    }

    // State 2 - TERMINAL SWING -  hold last swing position
    if (noSwing && !stanceState) { // if the person has not put their foot down it hold the leg to this angle
      theta = 64;
      runMotor(theta);
    }

    // State 3 - SWING - run swing when noSwing is false (i.e. started at least one stance phase) and not in stance
    if (!noSwing && !stanceState) {
      Serial.print("SWING:\t");
      swingStartTime = t;
      counter = 0; //need to reset the counter every time this loop is run
      unsigned long currentTime = micros();
      theta = stairSwingTraj[counter]; //theta will be assigned values from this array by counting through the array
      while ((counter < 34) && !StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2) && (hotKey != 'e') && noSwing == 0) { //while the counter is less than or equal to the number of items in the array and stance detect is not true run this code
        if ((micros() - currentTime) > dtstairSwingTraj) { //only update to the next point in the trajectory once the time period alloted for each point has passed
          counter = counter + 1; //get the new angle value
          theta = stairSwingTraj[counter]; //assign the new angle value
          currentTime = micros(); //document the currect time since the program startd
        }
        runMotor(theta);
        // once swing is finished, don't go into swing until you start stance
        if (micros() > (dtstairSwingTraj * 34) + t) {
          noSwing = 1;
        }
        if (Serial.available() > 0) {
          hotKey = Serial.read();
        }
      }
    }


  }
}

//Used in the main routine cases to run the stairs controller
void stairsCase() {
  if (Serial.available()) {
    char inChar = Serial.read();
    if (inChar == 'e') {
      exit(1);
    }
  }
  else {
    stairs(POTPIN1, POTPIN2, thresh1, thresh2, theta, weightFactor);
    PotTest(POTPIN1, POTPIN2);
  }
}
