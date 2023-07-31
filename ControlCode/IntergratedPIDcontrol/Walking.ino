
//declare variables
double traj[] = { 1.327, 6.858, 14.472, 24.658, 34.394, 43.502, 50.942,
                  57.031, 61.604, 63.349, 62.728, 61.199, 59.986, 58.768, 56.342, 52.163, 47.059, 41.355, 36.538, 31.751, 26.735, 21.642, 16.999, 13.377, 11.296, 9.068,
                  7.059, 4.914, 3.016, 1.895, 1.402, 0.918, 0.212, 0.237, 0.25, 0.247, 0.25, 0.251
                }; // trajectory determined by studies on how people walk

// State machine that determines if in the stance or swing phase of walking and sets the motor to a certain effort
void walking(int POTPIN1, int POTPIN2, int thresh1, int thresh2) {
  unsigned long swingStartTime; // keeps track of when swing starts
  float dtTraj = period / 38; // time period of swing divided by how many numbers in the trajectory

  int hotKey = 'b';
  while (hotKey != 'e') {
    if (Serial.available() > 0) {
      hotKey = Serial.read();
    }
    stanceState = StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2); //StanceDetect is a function in the motor library, measure if the linear potentiometers pass a certain threshhold
    int t = micros();
    if (stanceState || swingFinished == 0) { //if stance detect is true, they are in stance
      if (stanceState) {
        swingFinished = 1;
      }
      theta = 0; // set the desired angle to be fully extended
      Serial.println( "STANCE" );

      runMotor(theta); //use the runMotor function to determine how the motor should achieve the desired angle

    }
    else {
      Serial.print("SWING");
      swingStartTime = t;
      counter = 0; //need to reset the counter every time this loop is run
      unsigned long currentTime = micros();
      theta = traj[counter]; //theta will be assigned values from this array by counting through the array
      while ((counter < 38) && !StanceDetect(thresh1, thresh2, POTPIN1, POTPIN2) && (hotKey != 'e') && swingFinished == 1) { //while the counter is less than or equal to the number of items in the array and stance detect is not true run this code
        if ((micros() - currentTime) > dtTraj) { //only update to the next point in the trajectory once the time period alloted for each point has passed
          counter = counter + 1; //get the new angle value
          theta = traj[counter]; //assign the new angle value
          currentTime = micros(); //document the currect time since the program startd
        }

        runMotor(theta);
        if (micros() > (dtTraj * 38) + t) {
          //delay(1000);
          swingFinished = 0;
        }
        if (Serial.available() > 0) {
          hotKey = Serial.read();
        }
      }
    }
  }
}

//used in the main routine cases to run the walking controller
void walkingCase() {
  if (Serial.available()) {
    char inChar = Serial.read();
    if (inChar == 'e') {
      exit(1);
    }
  }
  else {
    walking(POTPIN1, POTPIN2, thresh1, thresh2);
    PotTest(POTPIN1, POTPIN2);
  }
}
