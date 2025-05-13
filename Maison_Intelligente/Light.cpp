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

void Light::fadeState() {
  static unsigned long previousMillis = _currenTimer;  // Temps actuel
  static int  brightness =0, fadeAmount=1;
  _currenTimer = millis();
  // Vérifie si le temps écoulé depuis la dernière mise à jour dépasse l'intervalle
  if (_currenTimer - previousMillis >= 100) {
    previousMillis = _currenTimer;  // Met à jour le dernier temps enregistré

    analogWrite(10, 0);  // Écrit la nouvelle luminosité sur la LED (PWM)

    brightness += fadeAmount;  // Augmente ou diminue la luminosité

    // Si on atteint une des bornes (0 ou 255), on inverse la direction du fondu
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
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
