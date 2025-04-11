#include "HardwareSerial.h"
#include "Light.hpp"
#include "Arduino.h"
#include "RGBLight.hpp"

RGBLight::RGBLight(int redPin, int greenPin, int bluePin)
  : Light(redPin) {
  PIN_RED = redPin;
  PIN_GREEN = greenPin;
  PIN_BLUE = bluePin;
}
void RGBLight::setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}
void RGBLight::offState() {
  state = OFF;
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
}
void RGBLight::setRGBState(int redVal, int greenVal, int blueVal) {
  redValue = redVal;
  greenValue = greenVal;
  blueValue = blueVal;
  analogWrite(PIN_RED, redValue);
  analogWrite(PIN_GREEN, greenValue);
  analogWrite(PIN_BLUE, blueValue);
}
void RGBLight::setBlinkState(bool blinkState) {
  state = (blinkState) ? BLINK : OFF;
}
void RGBLight::setBlinkRate(int blinkRate) {
  this->blinkRate = blinkRate;
}
void RGBLight::blinkState() {
  static bool firsTime = false;
  static unsigned long previousTime = 0;
  _currenTimer = millis();
  if (firsTime) {
    state = BLINK;
    rgbState = RED;
    previousTime = _currenTimer;
    firsTime = false;
  }

  if (_currenTimer - previousTime < blinkRate) return;
  previousTime = _currenTimer;

  switch (rgbState) {
    case RED:
      setRGBState(255, 0, 0);
      rgbState = GREEN;
      break;
    case GREEN:
      setRGBState(255, 255, 255);
      rgbState = BLUE;
      break;
    case BLUE:
      setRGBState(0, 0, 255);
      rgbState = RED;
      break;
  }
  bool transition = (state != BLINK);
  if (transition) {
    state = OFF;
    firsTime = true;
  }
}
void RGBLight::fade(int rate){

}
void RGBLight::update() {
  switch (state) {
    case OFF:
      offState();
      break;
    case BLINK:
      blinkState();
      break;
    case FADE:
      break;
    case ON:
      break;
  }
}
