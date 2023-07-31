
#include <d_motor.h>

#define MTR_DIR_PIN 12
#define MTR_EN_PIN 13
#define MTR_DAC_PIN A0

// Pin A0 from Serial.println(A0) 

int a = 0;
int dir;
int ab;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MotorSetup(MTR_DIR_PIN, MTR_EN_PIN);
  Serial.println(A0);
}

void loop() {
  if( Serial.available( ) )  
  {  
  a = Serial.parseFloat();
  }
  if (a < 0){
    dir = 1;
    ab = a * -1;
  }
  else{
    dir = 0;
    ab = a;
  }
  MotorTest(dir, ab, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN);
  Serial.println(dir);
  //make sure to pass in a number between 30-90 in serial monitor
}
