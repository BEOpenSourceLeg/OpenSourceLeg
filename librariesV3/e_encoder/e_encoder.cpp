#include "e_encoder.h"
#include <SPI.h>
#include <Wire.h>


#define ENC_ADDR 0x3FFF //Encoder register, also used in some dark magic in the encoder tab. Pulled this value from Justin Francis' code on the Github for the open source leg
float prevReading = 0;
unsigned long ENC_millis; 
unsigned long ENC_micros = 0; 

SPISettings EncSettings(4e6, MSBFIRST, SPI_MODE1);

void EncoderSetup(int encoderPin){
  Serial.begin(9600);
  pinMode(encoderPin, OUTPUT);
  SPI.begin();
  digitalWrite(encoderPin, LOW);
  delay(1000);
}

float ReadEncoder(int encoderPin) {

  //Begin the transaction with the SPI object
  SPI.beginTransaction(EncSettings);

  float reading = 0;
  float encoderAlpha = .98;
  unsigned long currentMicros = micros(); 

  float dt = currentMicros - ENC_micros;
 
  //Signal that we want to get a reading
  digitalWrite(encoderPin, LOW);

  //Get a reading
  //Extract position data from SPI signal received
  reading = ((float)(SPI.transfer16(ENC_ADDR) & (ENC_ADDR)));
  //Serial.print(reading); Serial.print('\t');
    //Stop reading
  digitalWrite(encoderPin, HIGH);


    reading = prevReading * encoderAlpha + reading*(1-encoderAlpha);
    //Serial.print(prevReading); Serial.print("\t"); 
    //Serial.print(reading); Serial.print("\t"); 
    //Serial.print(reading * (1-encoderAlpha)); Serial.print("\t");
    //ENC_micros = micros();  
    ENC_micros = currentMicros;  
    
    prevReading = reading;

  //Map to degrees, 16989 is the resolution of the encoder.
  return 189. - (reading) * 360.0 / 16989.0;

}

float ReadEncoderUnfiltered(int encoderPin){
  SPI.beginTransaction(EncSettings);
  float reading = 0;

  //Signal that we want to get a reading
  digitalWrite(encoderPin, LOW);

  //Get a reading
  //Extract position data from SPI signal received
  reading = ((float)(SPI.transfer16(ENC_ADDR) & (ENC_ADDR)));
  //Serial.print(reading); Serial.print('\t');
    //Stop reading
  digitalWrite(encoderPin, HIGH);

  //Map to degrees, 16989 is the resolution of the encoder.
  return 189. - (reading) * 360.0 / 16989.0;
}

void TestEncoder(int encoderPin){
  Serial.println(ReadEncoder(encoderPin));
}