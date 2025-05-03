#include "Light.hpp"

#pragma once
class RGBLight : public Light {
public:
  enum RGBState {
    RED,
    GREEN,
    BLUE
  };
  enum State {
    OFF,
    BLINK,
    FADE,
    ON
  };
  RGBLight(int redPin, int greenPin, int bluePin);
  void update();
  void setup();
  void offState();
  void blinkState();
  void fade(int rate);
  void setRGBState(int r, int g, int b);

  void setBlinkState(bool blinkState);
  void setBlinkRate(int rate);
  int getRedValue() {
    return redValue;
  }
  int getGreenValue() {
    return greenValue;
  }
  int getBlueValue() {
    return blueValue;
  }

private:
  unsigned long _currenTimer;
  int PIN_RED, PIN_GREEN, PIN_BLUE;
  int redValue = 0, greenValue = 0, blueValue = 0;
  int blinkRate = 100;
  RGBState rgbState;
  State state;
};