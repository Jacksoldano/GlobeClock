void getTIME(){
  /*
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }*/

  timeClient.forceUpdate();

  hourS = timeClient.getHours();
  minuteS = timeClient.getMinutes();
  secondS = timeClient.getSeconds();
  Serial.print(hourS);
  Serial.print(":");
  Serial.print(minuteS);
  Serial.print(":");
  Serial.println(secondS);

  hour = hourS.toInt();
  minute = minuteS.toInt();
  second = secondS.toInt();
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  hour2sec = hour * 3600;
  minute2sec = minute * 60;
  second2second = second;
  Serial.print(hour2sec);
  Serial.print(":");
  Serial.print(minute2sec);
  Serial.print(":");
  Serial.println(second2second);

  currentTIMEseconds = hour2sec + minute2sec + second2second;
  Serial.print("currentTIMEseconds: ");
  Serial.println(currentTIMEseconds);

  currentTIMEsecondsminusZero = currentTIMEseconds;
  Serial.print("currentTIMEsecondsminusZero: ");
  Serial.println(currentTIMEsecondsminusZero);
}
