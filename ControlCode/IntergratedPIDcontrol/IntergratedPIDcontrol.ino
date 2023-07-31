/* TO CHANGE PARAMETERS AND SWITCH BETWEEN WALKING AND STAIRS
   m - print the control menu
   P - changes Kp value
   i - changes Ki intergral value
   d - changes Kd derivative value
   a - changes the alpha / filter value
   l - changes the length of the period for walking
   b - changes the weight factor for stance in stairs
   t - changes the desired theta / angle
   w - switches to walking controller
   s - switches to stairs controller
   e - exits the contollers 
*/




//include libraries
#include <e_encoder.h>
#include <d_motor.h>
#include <b_potentiometer.h>

//define pins
#define MTR_DIR_PIN 12 // motor direction
#define MTR_EN_PIN 13  // motor enable
#define MTR_DAC_PIN A0 // motor digital to analog converter, controls speed
#define POTPIN1 A1     // logic pin for fist potentiometer
#define POTPIN2 A2     // logic pin for second potentiometer
#define ENC_CS 10      // chip select for encoder

#define offset -29
//changes system to system, a number that will make the encoder read 0 when the leg is straight

// varibales
float gain;  //used in getGain function to store a number so it can be assined to one of the gains variables
float Kp = 0;       // position PID constant
float Ki = 0;      // integral PID constant
float Kd = 0;         // derivative pid constant
float theta;          // desired knee angle
float alpha = .9;     // low pass filter constant
int swingFinished;
bool stanceState;     // true/fasle whether the leg is loaded
int thresh1 = 500 ;   // linear pot threshhold for first pot
int thresh2 = 600;    // linear pot threshhold for second pot
float period = .9e6;  //this is for testing purposes the actual period of swing should be calulated by stance time
int weightFactor = 40;//adjusts the power going to the knee for stance in the stair controller depending on how much a person weights
int counter;          // counts through the numbers in the trajectories

boolean menu = false;

//kp 2.1, ki .005


//setup function
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  MotorSetup(MTR_DIR_PIN, MTR_EN_PIN); //sets up motor pins, function from the motor library
  EncoderSetup(ENC_CS); //sets up encoder pins, function in the encoder library
  PotSetup(POTPIN1, POTPIN2); //sets up potentiometer pins, function in the potentiometer library

}


void loop() {
  // put your main code here, to run repeatedly:

  if ( Serial.available() > 0) {
    int inByte = Serial.read();
    clearBuffer();
    switch (inByte) {
      case 'p': // changes Kp value
        Serial.println("enter a number to change Kp");
        Kp = getGain();
        break;

      case 'i': // changes Ki value
        Serial.println("enter a number to change Ki");
        Ki = getGain();
        break;

      case 'w': //switches into the walking controller
        Serial.println("enter e to exit");
        walkingCase();
        menu = true;
        break;

      case 's':
        Serial.println("enter e to exit");
        stairsCase();
        menu = true;
        break;

      case 'l': // changes to the length of the period for walking
        Serial.println("Enter a number to change the period");
        period = getGain() * 1000000;
        break;

      case 'd': // changes the Kd value
        Serial.println("enter a number to change Kd");
        Kd = getGain();
        break;

      case 'a': // changes the alpha or filter value for the PID controller
        Serial.println("enter a number to change alpha");
        alpha = getGain();
        break;

      case 'b': // changes the weight factor for stair stance
        Serial.println("enter a number to change the weight factor");
        weightFactor = getGain();
        break;

      case 't': // changes the desired theta
        Serial.println("enter a number to change desired theta");
        theta = getGain();
        break;

      case 'r': // run step response
        Serial.println("enter e to exit");
        runStepResponse(theta);
        menu = true;
        break;

      case 'm': // print menu
        printMenu();
        break;

      default:
        Serial.println("You entered an incorrect value");
        break;
    } // end swith case

    if (menu) { // reprint menu after running a test
      // Print Description of Key strokes
      printMenu();
      menu = false;
    }
    
    clearBuffer();
  } // end check serial buffer
} // end loop
