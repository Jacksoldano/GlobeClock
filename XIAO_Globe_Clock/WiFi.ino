/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-04-01
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock
  File Name:      WiFi.ino
  Version:        02

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  - Just used for wifi setup
*****************************************************************************/

void wifiSETUP(){
  // Generic wifi setup function
  // Prints IP address of device and starts web server
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
