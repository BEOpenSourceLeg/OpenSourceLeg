
void runStepResponse(float theta) {
  unsigned long prevSerial = 0;
  unsigned long prevMotor = 0;
  const unsigned long serialDelta = 500;  // microseconds, = 10 hze
  const unsigned long motorDelta = 2000;     // microseconds, = 1000 hz

  while (cont) {
    currentMicros = micros();

    if ((currentMicros - prevSerial) > serialDelta) {
      prevSerial = currentMicros;
      //Serial.println("hotkey!");
      if (Serial.available() > 0) {
        int hotKey = Serial.read();
        if (hotKey == 'e') {
          MotorControlEffort(0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          iError = 0; 
          break;
        }
      }
    }
    if ((currentMicros - prevMotor) > motorDelta) {
      prevMotor = currentMicros;
      encValUnfiltered = ReadEncoderUnfiltered(ENC_CS) - offset;
      encVal = prevEncVal * encAlpha + encValUnfiltered * (1.0 - encAlpha);
      prevEncVal = encVal;

      runMotor(theta);  // calls motor function and pid in motor function
    }
  }
}
