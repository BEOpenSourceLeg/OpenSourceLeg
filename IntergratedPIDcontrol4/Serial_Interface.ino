// clears the serial monitor
void clearBuffer() {
  while (Serial.read() > -1) {
    // do nothing!
  }
}

//obtains a user input for different parameters and returns it
float getGain() {
  int check = 1;
  while (check) {
    if (Serial.available() > 0) {
      gain = Serial.parseFloat();  //assigns the user input to the variable "gain"
      Serial.print("You entered: ");
      Serial.println(gain, 5);  // prints what the user typed so the user can visually validate they types in what they wanted
      Serial.println();
      check = 0;
    }
  }
  clearBuffer();
  return gain;
}

void printMenu() {
  Serial.println();
  Serial.println();
  Serial.print("Press p to change Kp. The current Kp value is ");
  Serial.println(Kp, 5);
  Serial.print("Press d to change Kd. The current Kd value is ");
  Serial.println(Kd, 5);
  Serial.print("Press i to change Ki. The current Ki value is ");
  Serial.println(Ki, 5);
  Serial.print("The current angle is ");
  Serial.println(encVal);
  Serial.print("Press t to change desired theta. The current theta value is ");
  Serial.println(theta);
  Serial.print("Press a to change alpha. The current alpha value is ");
  Serial.println(alpha);
  Serial.print("Press b to change weight factor. The current weight value is ");
  Serial.println(weightFactor);
  Serial.print("Press l to change length of swing for walking. The current length value is ");
  Serial.println((walkPeriod * 38) / 1000000);
  Serial.print("Press g to change the length of swing for stairs. The current length value is "); 
  Serial.println((stairPeriod * 34)/ 1000000); 
  Serial.print("Press 1 to change the first threshhold. The current first thresh value is ");
  Serial.println(thresh1);
  Serial.print("Press 2 to change the second threshhold. The current second thresh value is ");
  Serial.println(thresh2);
  Serial.println();
  Serial.println();
}

void printData(bool timePrint, bool pidPrint, bool anglePrint, bool torqPrint, bool potPrint) {
  if (timePrint == true) {
    Serial.print(millis());
    Serial.print("\t");

  }
  if (anglePrint == true) {
    Serial.print(theta);
    Serial.print("\t");
    Serial.print(encVal);
    Serial.print("\t");
  }
  if (pidPrint == true) {
    Serial.print(cEffort);
    Serial.print("\t");
    // Serial.print(iError * Ki);
    // Serial.print("\t");
    // Serial.print(error * Kp);

    // Serial.print("\t");
    // Serial.print(dError * Kd);
    // Serial.print("\t");
  }
  if (torqPrint == true) {
    Serial.print(angleTorque);
    Serial.print("\t");
    // Serial.print(contEffort);
    // Serial.print("\t");
  }

  if (potPrint == true) {
   // float pot1 = ReadPot(POTPIN1);  //Calls on previously made function to read first potentiometer
   // float pot2 = ReadPot(POTPIN2);  //Reads second potentiometer
    //Serial.print(pin1Unf);  //Print data from potentiometers to serial monitor
    //Serial.print("\t");
    //Serial.print(pin2Unf);
    // Serial.print("\t");
    Serial.print(pin1); 
    Serial.print("\t"); 
    Serial.print(pin2); 

  }
  Serial.println();
}
