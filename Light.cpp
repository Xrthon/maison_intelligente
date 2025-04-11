#include <avr/interrupt.h>
#include "Arduino.h"

#include "Light.hpp"

Light::Light(int PIN) {
  this->PIN = PIN;
}

void Light::setBlinkRate(int rate) {
  blinkRate = rate;
}

void Light::setLightState(bool state) {
  this->state = (state) ? ON : OFF;
  digitalWrite(getLEDPin(), getState());
}
void Light::setup(bool state) {
  pinMode(getLEDPin(), OUTPUT);
  setLightState(state);
  Serial.println("Setup complet [ LED ]");
}


void Light::blinkState() {
  static bool firsTime = false;
  static unsigned long previousTime = 0;
  _currenTimer = millis();
  if (firsTime) {
    this->setLightState(ON);
    this->setBlinkRate(blinkRate);
    previousTime = this->getTimer();
    firsTime = false;
  }

  if (_currenTimer - previousTime < getBlinkRate()) return;
  previousTime = _currenTimer;

  if (this->getState()) {
    this->setLightState(OFF);
  };
  if (!this->getState()) {
    this->setLightState(ON);
  }
}




void Light::duoLightBlinkState(Light seclight) {
  static bool firsTime = true;
  static unsigned long previousTime = 0;

  if (firsTime) {
    previousTime = getTimer();
    seclight.setBlinkRate(getBlinkRate()+getBlinkRate());
    firsTime = false;
  }

  if (getTimer() - previousTime >= this->getBlinkRate()) {
    this->blinkState();
  }
  if (seclight.getTimer() - previousTime >= seclight.getBlinkRate()) {
    seclight.blinkState();
  }
}
