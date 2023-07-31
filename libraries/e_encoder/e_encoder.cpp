#include "e_encoder.h"
#include <SPI.h>
#include <Wire.h>


#define ENC_ADDR 0x3FFF //Encoder register, also used in some dark magic in the encoder tab. Pulled this value from Justin Francis' code on the Github for the open source leg


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

  int reading;

  //Signal that we want to get a reading
  digitalWrite(encoderPin, LOW);

  //Get a reading
  reading = SPI.transfer16(ENC_ADDR);

  //Stop reading
  digitalWrite(encoderPin, HIGH);

  //Extract position data from SPI signal received
  reading = (reading & (ENC_ADDR));

  //Map to degrees, 16989 is the resolution of the encoder.
  return 189. - ((float)reading) * 360.0 / 16989.0;

}

void TestEncoder(int encoderPin){
  Serial.println(ReadEncoder(encoderPin));
}
