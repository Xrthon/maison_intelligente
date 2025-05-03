#include "Arduino.h"

#pragma once
class Buzzer {
public:
  enum State {
    OFF,
    ON
  };

  Buzzer(int BUZZER_PIN);
  void setup();
  void update();
  void onState();
  void offState();
  void setBuzzerState(bool state);
  void setFrequency(int toneFrequency);
  void setTone(int frequency);

  int getPINBuzzer() {
    return this->PIN_BUZZER;
  }
  int getFrequency() {
    return this->frequency;
  }
  bool getState(){
    return state;
  }
private:
  int PIN_BUZZER;
  unsigned int frequency;
  unsigned long currentTimer;
  State state;
};