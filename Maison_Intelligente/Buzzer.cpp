#include "Arduino.h"
#include "Buzzer.hpp"

Buzzer::Buzzer(int BUZZER_PIN) {
  this->PIN_BUZZER = BUZZER_PIN;
}


void Buzzer::setup() {
  this->frequency = 100;
  pinMode(getPINBuzzer(), OUTPUT);
  Serial.println("Setup complet [ BUZZER ]");
}

void Buzzer::setFrequency(int toneFrequency) {
  this->frequency = toneFrequency;
}
void Buzzer::setBuzzerState(bool state) {
  this->state = (state) ? ON : OFF;
}
void Buzzer::onState() {
  static int liveFreq = 0;
  static bool montant = true;
  setBuzzerState(ON);

  if (montant) {
    if (liveFreq >= getFrequency()) {
      montant = false;
    } else {
      liveFreq++;
    }
  } else if (!montant) {
    if (liveFreq <= 0) {
      montant = true;
    } else {
      liveFreq--;
    }
  }

  tone(getPINBuzzer(), liveFreq);
}
void Buzzer::offState() {
  setBuzzerState(OFF);
  noTone(getPINBuzzer());
}

void Buzzer::update() {
  if (getState()) {
    onState();
  } else {
    offState();
  }
}