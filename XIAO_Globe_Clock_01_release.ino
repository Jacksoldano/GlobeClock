/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-02-20
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  LEDS.ino Description
  - Controls LED state in various colours using NEOPIXEL

  Stepper.ino Description
  - Holds various functions to control the stepper
  - Works out expected position
  - Looks at current position
  - Movers stepper to correct position on update interval

  WebTime.ino
  - Just used to get the current time and format into integers

  WiFi.ino
  - Just used for wifi setup
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

//Define System
int SYSVersion = 1;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save time
String hourS, minuteS, secondS;
int hour, minute, second;
int hour2sec, minute2sec, second2second, currentTIMEseconds, currentTIMEsecondsminusZero;
int dayinSECONDS = 86400;
float daySeconds2steps = 42.39450442;
float expectedPositionF;
int expectedPosition, stepsRequired2get2expectedPosition;
int currentPosition = 0;

// Position
#define HallSensor 3
int ZeroPosition = 0;

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;
//Stepper myStepper = Stepper(stepsPerRevolution, 21, 7, 6, 5);
//Stepper myStepper = Stepper(stepsPerRevolution, 6, 5, 21, 7);
Stepper myStepper = Stepper(stepsPerRevolution, 5, 7, 6, 21);

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600, GMT +2 = 7,200, GMT -1 = -3600, GMT 0 = 0
//int Timeoffset = 0;  // Est 15:14 offset
int Timeoffset = -54840; // <--- 15.233333h offset

void setup() {

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


  ledSETUP();
  ledDRIVEorange();

  ZerorotationSTEPS();
  //fullrotationSTEPS();

  ledDRIVEblue();
  wifiSETUP();
  // Initialize a NTPClient to get time

  ledDRIVEpurple();
  timeClient.begin();
  timeClient.setTimeOffset(Timeoffset);
  // Get current time
  getTIME(); 
  getExpectedPosition();
  checkCurrentPosition();
}
void loop() {

  systemTIME = millis();
  ledDRIVEorange();

  //if (systemTIME - lastcheckTIME >= 42394){ // <---- Check every 1 step or so...
  //if (systemTIME - lastcheckTIME >= 1000){  // <---- Check every 1 second or so...
  if (systemTIME - lastcheckTIME >= 211970){  // <---- Check every 5 step or so...
  //if (systemTIME - lastcheckTIME >= 847880){  // <---- Check every 20 step or so...
  //if (systemTIME - lastcheckTIME >= 635910){  // <---- Check every 15 step or so...
    Serial.println("Its been 635 seconds, checking current time again...");
    getTIME(); 
    getExpectedPosition();
    checkCurrentPosition();  
    //testSTEPS(); // <--- test
    lastcheckTIME = systemTIME;
  }

  loopTIME = systemTIME - lastsystemTIME;
  Serial.print("Current systemTIME : ");
  Serial.print(systemTIME);
  Serial.print("ms, this loop took : ");
  Serial.print(loopTIME);
  Serial.print("ms, current step position : ");
  Serial.println(currentPosition);
  lastsystemTIME = systemTIME;

  
}