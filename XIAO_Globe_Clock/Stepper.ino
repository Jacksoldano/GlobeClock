/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-04-01
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock
  File Name:      Stepper.ino
  Version:        02

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  - Holds various functions to control the stepper
  - Works out expected position
  - Looks at current position
  - Movers stepper to correct position on update interval
*****************************************************************************/

void disablesteppers(){
  // Used to disable all stepper drive pins
  // Stepper function leave pins active once drive is complete
  delay (250); 
  digitalWrite(21, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
}

void ZerorotationSTEPS(){
  // Used to rotate globe to fine zero position
  // Finds zero position by monitoring 'ZeroPosition' analog read while rotating
  // Once zero position is found 'currentPosition' is set to 0
  ZeroPosition = digitalRead(HallSensor);
  Serial.print("Hall Sensor = ");
  Serial.println (ZeroPosition);

  if (ZeroPosition == 0){
    Serial.println("Startup in zero region, need to rotate out of this...");
    delay(100);
    while(ZeroPosition == 0){
      myStepper.setSpeed(defaultStepperSpeed);
	    myStepper.step(2);
      ZeroPosition = digitalRead(HallSensor);
      Serial.print("Hall Sensor = ");
      Serial.println (ZeroPosition);
      currentPosition = currentPosition + 2;
    }
    myStepper.setSpeed(defaultStepperSpeed);
	  myStepper.step(2);
    currentPosition = currentPosition + 2;
    Serial.println("Now clear of Zero region, starting normal zero sequence");
  }

  while(ZeroPosition == 1){
    myStepper.setSpeed(defaultStepperSpeed);
	  myStepper.step(2);
    ZeroPosition = digitalRead(HallSensor);
    Serial.print("Hall Sensor = ");
    Serial.println (ZeroPosition);
    currentPosition = currentPosition + 2;
  }

  ZeroPosition = digitalRead(HallSensor);
  Serial.print("Hall Sensor = ");
  Serial.println (ZeroPosition);
  currentPosition = 0;
  Serial.print("currentPosition = ");
  Serial.println (currentPosition);
}

void getExpectedPosition(){
  // Used every time interval to get the current step position delta
  // Looks at current time, determines what step count that should be in 'expectedPosition'
  // Then calculates the step delta from 'expectedPosition' and 'currentPosition' stored in 'stepsRequired2get2expectedPosition'
  // If 'expectedPosition' is less than 'currentPosition' hard set step count
  expectedPositionF = currentTIMEseconds / daySeconds2steps;
  Serial.print("expectedPositionF: ");
  Serial.println(expectedPositionF);
  
  expectedPosition = ((int)expectedPositionF) +1;
  Serial.print("expectedPosition: ");
  Serial.println(expectedPosition);

  stepsRequired2get2expectedPosition = expectedPosition - currentPosition;
  Serial.print("stepsRequired2get2expectedPosition: ");
  Serial.println(stepsRequired2get2expectedPosition);

  if (stepsRequired2get2expectedPosition<=0){
    Serial.println("!! Position is negative day has changed, stepsRequired2get2expectedPosition: 5");
    stepsRequired2get2expectedPosition = 6; //Over to current time interval expected step size
  }
}

void checkCurrentPosition(){
  // Used once every time interval to check 'stepsRequired2get2expectedPosition' which stores step delta
  // Checks that at full rotation device is reading zero sensor if not assume skipped steps and move to zero position
  // Steps the number stored in 'stepsRequired2get2expectedPosition' then add 'stepsRequired2get2expectedPosition' to 'currentPosition'
  // If current position now exceeds 1 full rotation determine overflow and store in 'currentPositionOverflow'
    Serial.print("Not in correct position, moving ");
    Serial.print(stepsRequired2get2expectedPosition);
    Serial.println(" steps ...");
    myStepper.setSpeed(defaultStepperSpeed);
	  myStepper.step(stepsRequired2get2expectedPosition);

    Serial.print("Updating current position to ");
    currentPosition = currentPosition + stepsRequired2get2expectedPosition;
    Serial.println(currentPosition);

    if(currentPosition >= stepsPerRevolution){
      ZeroPosition = digitalRead(HallSensor);
      Serial.print("Hall Sensor = ");
      Serial.println (ZeroPosition);

      if(ZeroPosition != 0){
        Serial.println("Steps must have been skipped, current position should mean globe is at zero position...");
        Serial.println("Moving to zero position...");
        while(ZeroPosition == 1){
          myStepper.setSpeed(defaultStepperSpeed);
          myStepper.step(2);
          ZeroPosition = digitalRead(HallSensor);
          Serial.print("Hall Sensor = ");
          Serial.println (ZeroPosition);
          currentPosition = currentPosition + 2;
        }

        ZeroPosition = digitalRead(HallSensor);
        Serial.print("Hall Sensor = ");
        Serial.print (ZeroPosition);
        Serial.print(", now we can reset currentPosition");
        currentPosition = 0;
      }
      else{
        currentPositionOverflow = currentPosition - stepsPerRevolution;
        Serial.print("One full rotation complete, overflow of ");
        Serial.print(currentPositionOverflow);
        Serial.println(" steps...");
        currentPosition = currentPositionOverflow;
      }
    }  
    disablesteppers();
}
