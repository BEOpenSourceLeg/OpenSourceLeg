#include "b_potentiometer.h"

int ReadPot(int potPin) {

  //Read the linear potentiometer pin

  return analogRead(potPin);

}
bool StanceDetect(int thresh1, int thresh2, int pin_1, int pin_2) {

  //Detect if the user is in stance based on potentiometer threshold. User must calibrate the potentiometer manually.
  return ReadPot(pin_1) > thresh1 || ReadPot(pin_2) > thresh2;

}
// Test code to ensure the potentiometers are working
void PotTest(int pin_1, int pin_2){
  float pot1 = ReadPot(pin_1); //Calls on previously made function to read first potentiometer
  float pot2 = ReadPot(pin_2); //Reads second potentiometer
  Serial.print(pot1); //Print data from potentiometers to serial monitor
  Serial.print("\t"); 
  Serial.println(pot2);
  //Serial.print("\t");

}
// Run in the void setup. Etablished what pins you are using to recive data from potentiometers
void PotSetup(int pin_1, int pin_2){
  pinMode(pin_1, INPUT);
  pinMode(pin_2, INPUT);

}
