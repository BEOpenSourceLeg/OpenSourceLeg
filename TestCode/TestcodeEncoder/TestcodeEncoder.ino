#include <e_encoder.h>
#define ENC_CS 10 //Chip select for rotary encoder

void setup() {
  // put your setup code here, to run once:
  EncoderSetup(ENC_CS);

}

void loop() {
  // put your main code here, to run repeatedly:
  TestEncoder(ENC_CS);

}
