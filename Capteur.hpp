#include "WString.h"
#pragma once

#include "Arduino.h"
#include <HCSR04.h>
#include <avr/interrupt.h>



class Capteur {
public:
  enum State {
    TO_CLOSE,
    GOOD,
    TO_FAR
  };
  Capteur(int Trigger, int Echo);
  ~Capteur();

  void update();
  void setup(int minDist, int maxDist);
  void setMaxDist(int maxDist);
  void setMinDist(int minDist);
  void setRate(int rate);
  int getPinEcho() {
    return PIN_ECHO;
  };
  int getPinTrig() {
    return PIN_TRIG;
  };

  int getMaxDist() {
    return maxDist;
  };
  int getMinDist() {
    return minDist;
  };
  int getDistance() {
    return distance;
  };
  String getDistanceStr() {
    return String(distance);
  };
  State Capteur::getState() {
    return state;
  }
  String getSensorState();


private:
  unsigned long _currentTime = 0;
  int distance;
  HCSR04* sensor;
  String sensorState;
  State state;
  int PIN_ECHO, PIN_TRIG, minDist, maxDist, rate = 50;
};