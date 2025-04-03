/*****************************************************************************
  Author:         Jack R Soldano
  Release Date:   2025-04-01
  Link:           Project details at https://github.com/Jacksoldano/GlobeClock
  File Name:      LEDS.ino
  Version:        02

  Description
  This Code is used in Globe Clock Project and uses an ESP32 XIAOC3
  It gets the current time from the web then calculates the orientation
  that the globe should be, effectively working out the current zenith of
  the sun relative to the systems "zero" position.

  - Controls LED state in various colours using NEOPIXEL
*****************************************************************************/

void ledSETUP(){
  // Setup fastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void ledDRIVEorange(){
  // Simple random loop to generate 'orange' hue colours at random brightness with a range defined by current night/day mode
  for(int i = 0; i < NUM_LEDS; i++) {
      color = random(5, 35);
      saturation = random(200, 255);
      brightness = random(CurrentMinBrightness, CurrentMaxBrightness);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEgreen(){
  // Simple random loop to generate 'green' hue colours at random brightness between 50 & 255
  for(int i = 0; i < NUM_LEDS; i++) {
      color = random(85, 110);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEblue(){
  // Simple random loop to generate 'blue' hue colours at random brightness between 50 & 255
  for(int i = 0; i < NUM_LEDS; i++) {
      color = random(150, 170);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEpurple(){
  // Simple random loop to generate 'purple' hue colours at random brightness between 50 & 255
  for(int i = 0; i < NUM_LEDS; i++) {
      color = random(190, 210);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}
