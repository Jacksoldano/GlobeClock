void step(){
  // Rotate CCW quickly at 10 RPM
	myStepper.setSpeed(10);
	myStepper.step(-stepsPerRevolution);
	delay(1000);
}

void getExpectedPosition(){
  //expectedPositionF = currentTIMEseconds / daySeconds2steps;
  expectedPositionF = currentTIMEsecondsminusZero / daySeconds2steps;
  Serial.print("expectedPositionF: ");
  Serial.println(expectedPositionF);
  
  int expectedPosition = ((int)expectedPositionF) +1;
  Serial.print("expectedPosition: ");
  Serial.println(expectedPosition);

  stepsRequired2get2expectedPosition = expectedPosition - currentPosition;
  Serial.print("stepsRequired2get2expectedPosition: ");
  Serial.println(stepsRequired2get2expectedPosition);
}

void checkCurrentPosition(){
  if (stepsRequired2get2expectedPosition != 0){
    Serial.print("Not in correct position, moving ");
    Serial.print(stepsRequired2get2expectedPosition);
    Serial.println(" steps ...");
    myStepper.setSpeed(4);
	  myStepper.step(stepsRequired2get2expectedPosition);

    Serial.print("Updating current position to ");
    currentPosition = currentPosition + stepsRequired2get2expectedPosition;
    Serial.println(currentPosition);

    digitalWrite(21, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
  }
}


void testSTEPS(){
  Serial.print("Test x steps");
  Serial.print("10");
  Serial.println(" steps ...");
  myStepper.setSpeed(1);
	myStepper.step(10);

  digitalWrite(21, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
}

void fullrotationSTEPS(){
  Serial.print("Test full rotation");
  Serial.print(stepsPerRevolution);
  Serial.println(" steps ...");
  myStepper.setSpeed(4);
  myStepper.step(stepsPerRevolution);

  digitalWrite(21, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
}


void ZerorotationSTEPS(){
  ZeroPosition = digitalRead(HallSensor);
  Serial.print("Hall Sensor = ");
  Serial.println (ZeroPosition);

  if (ZeroPosition == 0){
    Serial.println("Startup in zero region, need to rotate out of this...");
    delay(100);
    while(ZeroPosition == 0){
      myStepper.setSpeed(4);
	    myStepper.step(5);
      ZeroPosition = digitalRead(HallSensor);
      Serial.print("Hall Sensor = ");
      Serial.println (ZeroPosition);
      currentPosition = currentPosition + 5;
    }
    myStepper.setSpeed(4);
	  myStepper.step(10);
    currentPosition = currentPosition + 10;
    Serial.println("Now clear of Zero region, starting normal zero sequence");
  }

  while(ZeroPosition == 1){
    myStepper.setSpeed(4);
	  myStepper.step(5);
    ZeroPosition = digitalRead(HallSensor);
    Serial.print("Hall Sensor = ");
    Serial.println (ZeroPosition);
    currentPosition = currentPosition + 5;
  }

  ZeroPosition = digitalRead(HallSensor);
  Serial.print("Hall Sensor = ");
  Serial.println (ZeroPosition);
  currentPosition = 0;
  Serial.print("currentPosition = ");
  Serial.println (currentPosition);
}
