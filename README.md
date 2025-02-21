# GlobeClock

The guide to the CAD used in this project can be found here - https://makerworld.com/en/models/1128817#profileId-1128671

![hippo](https://makerworld.bblmw.com/makerworld/model/USb237c9d6ccaea0/design/2025-02-21_d5daae956518f.gif?x-oss-process=image/resize,w_1000/format,webp)

  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  This project uses the following
    WiFi.h
    NTPClient.h
    WiFiUdp.h
    Stepper.h
    FastLED.h

  XIAO_Globe_Clock_01.ino
  - Defines all vars used
  - Setup pins
  - Main loop

  LEDS.ino 
  - Controls LED state in various colours using NEOPIXEL

  Stepper.ino 
  - Holds various functions to control the stepper
  - Works out expected position
  - Looks at current position
  - Movers stepper to correct position on update interval

  WebTime.ino
  - Just used to get the current time and format into integers

  WiFi.ino
  - Just used for wifi setup
