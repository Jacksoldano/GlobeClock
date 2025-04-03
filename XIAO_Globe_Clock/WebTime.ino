/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-04-01
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock
  File Name:      WebTime.ino
  Version:        02

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  - Just used to get the current time and format into integers
*****************************************************************************/

void getTIME(){
  // Requests time from 'timeClient'
  // splits string into hour, seconds & minutes
  // Converts strings to integers
  // generates current seconds in the day based on hours, seconds & minutes and stores in 'currentTIMEseconds'
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
}
