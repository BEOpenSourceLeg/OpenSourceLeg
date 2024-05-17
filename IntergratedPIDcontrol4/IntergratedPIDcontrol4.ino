/* TO CHANGE PARAMETERS AND SWITCH BETWEEN WALKING AND STAIRS
   m - print the control menu
   P - changes Kp value
   i - changes Ki intergral value
   d - changes Kd derivative value
   a - changes the alpha / filter value
   l - changes the length of the period for walking
   b - changes the weight factor for stance in stairs
   t - changes the desired theta / angle
   w - switches to walking controllerw
   s - switches to stairs controller
   e - exits the contollers 
*/


//include libraries
#include <e_encoder.h>
#include <d_motor.h>
#include <b_potentiometer.h>

//define pins
#define MTR_DIR_PIN 12  // motor direction
#define MTR_EN_PIN 13   // motor enable+
#define MTR_DAC_PIN A0  // motor digital to analog converter, controls speed
#define POTPIN1 A1      // logic pin for fist potentiometer
#define POTPIN2 A2      // logic pin for second potentiometer
#define ENC_CS 10       // chip select for encoder

#define offset 51
// RED KNEE OFFSET: 57.6
// BLACK KNEE OFFSET: 96.9
//changes system to system, a number that will make the encoder read 0 when the leg is straight

// ADJUST WITH KNEE
int thresh1 = 600;  // linear pot threshhold for first pot BLACK KNEE
int thresh2 = 520;  // linear pot threshhold for second pot
//int thresh1 = 520; // RED KNEE
//int thresh2 = 630;

int hotKey;
int weightFactor = 40;  //adjusts the power going to the knee for stance in the stair controller depending on how much a person weights
int counter;            // counts through the numbers in the trajectories
int motorDir;


// PID VARIABLES
float gain;    //used in getGain function to store a number so it can be assined to one of the gains variables
float Kp = 0;  // position PID constant
float Ki = 0;  // integral PID constant
float Kd = 0;  // derivative pid constant

float ePrev = 0;   // previous error
float error = 0;   // difference between desired angle and current angle
float iError = 0;  // intergral error
float dError = 0;  // derivative error
float cEffort;     // controller effort

// ENCODER VARIABLES
float encValUnfiltered = 0;  // encoder value before filter
float encVal = 0;            // enc val after filter
float prevEncVal = 0;        // prev enc val for filter
float encAlpha = 0.7;        // alpha value for enocoder filtering

unsigned long lastDerivative;
unsigned long currentMicros;

bool cont = true;
bool menu = false;  // used to print menu or not
bool stanceState;   // true/false whether the leg is loaded

float theta;              // desired knee angle
float alpha = .9;         // low pass filter constant
float walkPeriod = 5e6 / 38;  //this is for testing purposes the actual period of swing should be calulated by stance time
float stairPeriod = 2e6 / 34; 

// Walking Trajectory
double traj[] = { 1.327, 6.858, 14.472, 24.658, 34.394, 43.502, 50.942, 57.031, 61.604, 63.349, 62.728,
                  61.199, 59.986, 58.768, 56.342, 52.163, 47.059, 41.355, 36.538, 31.751, 26.735, 21.642, 16.999, 13.377, 11.296, 9.068,
                  7.059, 4.914, 3.016, 1.895, 1.402, 0.918, 0.212, 0.237, 0.25, 0.247, 0.25, 0.251 };
// trajectory determined by studies on how people walk
float swingKi = 0;

// Stair Trajectory
double stairSwingTraj[] = { 0.0732, 0.1252, 1.2574, 5.1179, 12.8734, 23.4831, 37.2224, 53.8639, 73.2341, 77.0469,
                            80.3527, 79.2153, 79.6884, 77.0876, 76.8827, 74.295, 72.0177, 69.9621, 67.9742, 66.7435, 65.9836, 65.6217, 66.1194,
                            66.5125, 66.8728, 66.4739, 64.6795, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 };

float angleTorque;
float contEffort;

float pin1Unf;
float pin2Unf;
float pin1;
float pin2;
float prevPin1;
float prevPin2;
float linPotAlpha = 0.9;

//setup function
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MotorSetup(MTR_DIR_PIN, MTR_EN_PIN);  //sets up motor pins, function from the motor library
  EncoderSetup(ENC_CS);                 //sets up encoder pins, function in the encoder library
  PotSetup(POTPIN1, POTPIN2);           //sets up potentiometer pins, function in the potentiometer library
}


void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    int inByte = Serial.read();
    clearBuffer();
    switch (inByte) {
      case 'p':  // changes Kp value
        Serial.println("enter a number to change Kp");
        Kp = getGain();
        break;

      case 'i':  // changes Ki value
        Serial.println("enter a number to change Ki");
        Ki = getGain();
        break;

      case 'w':  //switches into the walking controller
        Serial.println("enter e to exit");
        walkingCase();
        menu = true;
        break;

      case 's':
        Serial.println("enter e to exit");
        stairsCase();
        menu = true;
        break;

      case 'c':
        Serial.println("enter e to exit");
        swingCase();
        menu = true;
        break;

      case 'l':  // changes to the length of the period for walking
        Serial.println("Enter a number to change the walking swing period");
        walkPeriod = (getGain() * 1000000) / 38;
        break;

      case 'g':  // changes to the length of the period for walking
        Serial.println("Enter a number to change the walking swing period");
        stairPeriod = (getGain() * 1000000) / 34;
        break;

      case 'd':  // changes the Kd value
        Serial.println("enter a number to change Kd");
        Kd = getGain();
        break;

      case 'a':  // changes the alpha or filter value for the PID controller
        Serial.println("enter a number to change alpha");
        alpha = getGain();
        break;

      case 'b':  // changes the weight factor for stair stance
        Serial.println("enter a number to change the weight factor");
        weightFactor = getGain();
        break;

      case 't':  // changes the desired theta
        Serial.println("enter a number to change desired theta");
        theta = getGain();
        break;

      case '1':  // changes the desired theta
        Serial.println("enter a number to change desired first threshold");
        thresh1 = getGain();
        break;
      case '2':  // changes the desired theta
        Serial.println("enter a number to change desired second threshold");
        thresh2 = getGain();
        break;

      case 'r':  // run step response
        Serial.println("enter e to exit");
        runStepResponse(theta);
        menu = true;
        break;

      case 'm':  // print menu
        printMenu();
        break;

      default:
        Serial.println("You entered an incorrect value");
        break;
    }  // end swith case

    if (menu) {  // reprint menu after running a test
      // Print Description of Key strokes
      printMenu();
      menu = false;
    }

    clearBuffer();
  }  // end check serial buffer
}  // end loop
