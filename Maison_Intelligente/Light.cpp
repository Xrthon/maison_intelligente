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
void Light::setup() {
  pinMode(getLEDPin(), OUTPUT);
  setLightState(LOW);
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


