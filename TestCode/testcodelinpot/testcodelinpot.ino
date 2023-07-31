#include<b_potentiometer.h>
#include<Wire.h>

//#define POT_PIN_1 A1
//#define POT_PIN_2 A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  PotSetup(A1, A2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  PotTest(A1, A2);

}
