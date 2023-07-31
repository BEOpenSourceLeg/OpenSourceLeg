#ifndef B_POTENTIOMETER
#define B_POTENTIOMETER

#include <arduino.h>

int ReadPot(int potPin);
bool StanceDetect(int thresh1, int thresh2, int pin_1, int pin_2);
void PotTest(int pin_1, int pin_2);
void PotSetup(int pin_1, int pin_2);

#endif