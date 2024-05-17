void motorReset(float torq) {
  //float torq = 0;
  float velocity = 0;
  float prevVel = 0;
  float dt = 1;
  float dirThresh = 1;
  float negTimeThresh = 11000;
  float zerTimeThresh = 11000;
  float gain;
  float pi = 3.14159265359;
  float velocityUnf;
  float fc = 10;
  float fs = 25;

  unsigned long prevDtMicros;
  unsigned long prevStopMicros;
  unsigned long motorOffStartTime;
  unsigned long nearZeroStartTime;
  unsigned long prevVelMicros;
  const unsigned long eStopDelta = 100000;
  const unsigned long velDelta = 1000000 / fs;

  int dirState;
  int timeState;

  bool resetCont = true;
  bool prevNegPower = false;
  bool prevNearZero = false;

  //while (cont) {
  if(resetCont) {
    currentMicros = micros();
    if ((currentMicros - prevVelMicros) > velDelta) {
      dt = 1;
      prevVelMicros = currentMicros;
      dt = (currentMicros - prevDtMicros) / 1000000.0;
      encVal = ReadEncoderUnfiltered(ENC_CS) - offset;
      float velDeltaSec = float(velDelta) / (pow(10.0, 6));
      alpha = (2 * pi * velDeltaSec * fc) / (2 * pi * velDeltaSec * fc + 1);

      // use velDelta or velFreq?
      velocityUnf = (encVal - prevEncVal) / dt;
      velocity = velocityUnf;
      velocity = velocity * alpha + (prevVel) * (1.0 - alpha);
      prevEncVal = encVal;
      prevDtMicros = currentMicros;
      prevVel = velocity;

      // Serial.print(encVal);
      //Serial.print("\t");
      // Serial.print(velocity);
      // Serial.print("\t");
      // Serial.print(velocityUnf);
      // Serial.print("\t");

      // see if velocity and torq are going in same direction
      if ((abs(velocity) < dirThresh) || (encVal < 2)) {
        if (!prevNearZero) {
          prevNearZero = true;
          nearZeroStartTime = currentMicros;
          dirState = 30;
        }
        dirState = 30;
        prevNegPower = false;
        Serial.print(F("Near Zero"));
        Serial.print("\t");

      } else if ((torq * velocity) > 0) {
        dirState = 10;
        prevNegPower = false;
        prevNearZero = false;
        Serial.print(F("Positive Power"));
        Serial.print("\t");

      } else if ((torq * velocity) < 0) {
        if (!prevNegPower) {
          prevNegPower = true;
          motorOffStartTime = currentMicros;
          dirState = 20;
        }
        dirState = 20;
        prevNearZero = true;
        Serial.print(F("Negative Power"));
        Serial.print("\t");
      }
      // See if time is near zero or not
      if ((currentMicros - motorOffStartTime) < negTimeThresh) {
        Serial.print(F("Onset"));
        Serial.print("\t");
        timeState = 0;
      } else if ((currentMicros - nearZeroStartTime) < zerTimeThresh) {
        Serial.print(F("Onset"));
        Serial.print("\t");
        timeState = 0;
      } else {
        Serial.print(F("Delay"));
        Serial.print("\t");
        timeState = 1;
      }

      //Switch on each state
      int state = timeState + dirState;
      // Serial.print(state);
      // Serial.print("\t");
      switch (state) {
        case 10:
        case 11:
          MotorControlEffort(torq, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          Serial.print(F("MOTOR ON"));
          break;

        case 20:
          MotorEStop();
          Serial.print(F("MOTOR OFF"));
          break;

        case 21:
          MotorControlEffort(torq, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          Serial.print(F("MOTOR ON"));
          prevNegPower = false;
          break;

        case 30:
          MotorEStop();
          Serial.print("MOTOR OFF");
          break;

        case 31:
          MotorControlEffort(torq, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          Serial.print(F("MOTOR ON"));
          prevNearZero = false;
          break;
      }
      //Serial.println("");
     // resetCont = false;
    }
    if ((currentMicros - prevStopMicros) > eStopDelta) {
      prevStopMicros = currentMicros;
      if (Serial.available()) {
        int hotkey = Serial.read();
        if (hotkey == 'e') {
          MotorControlEffort(0, MTR_DIR_PIN, MTR_DAC_PIN, MTR_EN_PIN, 0);
          torq = 0;
          resetCont = false;
          exit(1);
        }
      }
    }
  }
}

void MotorEStop() {
  digitalWrite(MTR_EN_PIN, LOW);
}
