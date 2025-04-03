/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-04-01
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock
  File Name:      XIAO_Globe_Clock.ino
  Version:        02

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  - Defines libs required;
    - WiFi.h - https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/
    - NTPClient.h - https://github.com/arduino-libraries/NTPClient
    - WiFiUdp.h - https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/
    - Stepper.h - https://github.com/arduino-libraries/Stepper
    - FastLED.h - https://github.com/FastLED/FastLED
  - Define all values used
  - Setup Pins & functions
  - Main loop
*****************************************************************************/

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Stepper.h>
#include <FastLED.h>

// Replace with your network credentials
const char* ssid     = "<INSERT SSID HERE>";
const char* password = "<INSERT PASSWORD HERE>";

// Define LED config
#define NUM_LEDS 17
#define DATA_PIN 10
int color, saturation, brightness;
CRGB leds[NUM_LEDS];

unsigned long systemTIME, lastcheckTIME, loopTIME, lastsystemTIME;

// Define System version
int SYSVersion = 2;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save time
String hourS, minuteS, secondS;
int hour, minute, second;
int hour2sec, minute2sec, second2second, currentTIMEseconds, currentTIMEsecondsminusZero;
int dayinSECONDS = 86400;

// Position values
#define HallSensor 3
int ZeroPosition = 0;
int defaultStepperSpeed = 1;
float daySeconds2steps = 42.39450442;
float expectedPositionF;
int expectedPosition, stepsRequired2get2expectedPosition;
int currentPosition,currentPositionOverflow = 0;
const int stepsPerRevolution = 2038;
Stepper myStepper = Stepper(stepsPerRevolution, 5, 7, 6, 21);

//int GlobeUpdateIntervalms = 42394;  // <---- Check every 1 step or so (had issues with lost steps here)
//int GlobeUpdateIntervalms = 84788;  // <---- Check every 2 step or so (had issues with lost steps here)
//int GlobeUpdateIntervalms = 211970; // <---- Check every 5 step or so...
int GlobeUpdateIntervalms = 254364;    // <---- Check every 6 step or so...
//int GlobeUpdateIntervalms = 635910; // <---- Check every 15 step or so...
//int GlobeUpdateIntervalms = 847880; // <---- Check every 20 step or so...

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600, GMT +2 = 7,200, GMT -1 = -3600, GMT 0 = 0
int Timeoffset = -54840; // <--- 15.233333h offset

// Night mode is used to dull LED strip between 'NightmodestarttimeGMT' & 'NightmodeendtimeGMT' times
// Recommend starting device up and using serial monitor to understand current time readout with offset then adjusting these accordingly
int NightmodestarttimeGMT = 7; // 7 = 11pm GMT
int NightmodeendtimeGMT = 14; // 14 = 6am GMT

int DayMaxBrightness = 255;
int NightMaxBrightness = 55;
int DayMinBrightness = DayMaxBrightness*0.2;
int NightMinBrightness = NightMaxBrightness*0.2;
int CurrentMaxBrightness, CurrentMinBrightness = 255;

void setup() {

  // Setup Pins
  pinMode(21, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(HallSensor, INPUT);

  // Initialize Serial Monitor
  Serial.begin(115200);
    Serial.print("Welcome to Globe Clock Version: ");
    Serial.println(SYSVersion);

  // Setup LED Drivers
  ledSETUP();
  ledDRIVEgreen();

  // Get globe into Zero position
  ZerorotationSTEPS();
  ledDRIVEblue();

  // Get Wifi connection setup
  wifiSETUP();
  ledDRIVEpurple();

  // Setup time client
  timeClient.begin();
    timeClient.setTimeOffset(Timeoffset);

  // Get current time
  getTIME(); 

  // Work out expected position based on time
  getExpectedPosition();

  // Check current position against expected position
  checkCurrentPosition();
}

void loop() {

  systemTIME = millis();

  // Check time and define current LED brightness limits accordingly
  Serial.print("Hour : ");
  Serial.print(hour);
  if(hour >= NightmodestarttimeGMT && hour <= NightmodeendtimeGMT){
    CurrentMaxBrightness = NightMaxBrightness;
    CurrentMinBrightness = NightMinBrightness;
    Serial.print(", Mode : Night");
  }
  else{
    CurrentMaxBrightness = DayMaxBrightness;
    CurrentMinBrightness = DayMinBrightness;
    Serial.print(", Mode : Day");
  }
  ledDRIVEorange();

  // Once a time interval has been complete run through stepper position updates.
  if (systemTIME - lastcheckTIME >= GlobeUpdateIntervalms){   
    Serial.print("Its been ");
    Serial.print(GlobeUpdateIntervalms/1000);
    Serial.print(" seconds, checking current time again...");

    getTIME(); 
    getExpectedPosition();
    checkCurrentPosition();  
    lastcheckTIME = systemTIME;
  }

  // Extra debug to help understand loop
  loopTIME = systemTIME - lastsystemTIME;
  Serial.print(", systemTIME : ");
  Serial.print(systemTIME);
  Serial.print("ms, this loop took : ");
  Serial.print(loopTIME);
  Serial.print("ms, current step position : ");
  Serial.println(currentPosition);
  lastsystemTIME = systemTIME;
}
