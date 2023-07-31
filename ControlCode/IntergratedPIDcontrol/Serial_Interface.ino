
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
      gain = Serial.parseFloat(); //assigns the user input to the variable "gain"
      Serial.print("You entered: "); Serial.println(gain, 3); // prints what the user typed so the user can visually validate they types in what they wanted
      Serial.println();
      check = 0;
    }
  }
  clearBuffer();
  return gain;
}

void printMenu() {
  Serial.println(); Serial.println();
  Serial.println("Press p to change Kp. Press i to change Ki. Press d to change Kd. Press t to change desired theta. Press a to change alpha. Press b to change weight factor. Press l to change length of swing for walking.");
  Serial.println("Press r to test step response. Press w to go into walking controller. Press s to go into stair controller. Press e to exit any of these controllers ");
  Serial.print("The current angle is "); Serial.println( ReadEncoder(ENC_CS) - offset);
  Serial.println();
}
