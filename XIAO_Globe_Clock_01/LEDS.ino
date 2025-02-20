void ledSETUP(){
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void ledDRIVEorange(){
  for(int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = CRGB::Orange;
      color = random(5, 35);
      //color = random(0, 255);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEgreen(){
  for(int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = CRGB::Orange;
      color = random(85, 110);
      //color = random(0, 255);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEblue(){
  for(int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = CRGB::Orange;
      color = random(150, 170);
      //color = random(0, 255);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

void ledDRIVEpurple(){
  for(int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = CRGB::Orange;
      color = random(190, 210);
      //color = random(0, 255);
      saturation = random(200, 255);
      brightness = random(50, 255);
      leds[i] = CHSV(color, saturation, brightness);
      FastLED.show();
      delay(58);
  }
}

