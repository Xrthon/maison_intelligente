#pragma once
#include "Arduino.h"
class Buzzer {
public:
  enum State {
    OFF,
    ON
  };

  Buzzer(int BUZZER_PIN);
  void setup(int toneFrequency);
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