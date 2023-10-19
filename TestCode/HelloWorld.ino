#include <e_encoder.h>
#include <d_motor.h>
#include<b_potentiometer.h>

#define led 13

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // turns LED off
  digitalWrite(led, HIGH);
  // prints text to serial monitor 
  Serial.println("LED is on");
  // delays 5 seconds
  delay(5000);
  
  // turns LED off
  digitalWrite(led, LOW);
  // prints text to serial monitor 
  Serial.println("LED is off");
  // delays 2 seconds
  delay(2000);
}
