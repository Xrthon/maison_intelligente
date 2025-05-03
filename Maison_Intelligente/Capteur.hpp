#include "WString.h"
#include <HCSR04.h>
#include <avr/interrupt.h>
#pragma once




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
  void setup(int minDist, int maxDist, int alarmDist);
  void setMaxDist(int maxDist);
  void setMinDist(int minDist);
  void setRate(int rate);
  int alarmStartDistance() {
    return alarmDist;
  }
  void setAlarmStartDist(int distAlarmStart);
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
    return (String(distance) + " cm");
  };
  State getState() {
    return state;
  }
  String getSensorState();



private:
  unsigned long _currentTime = 0;
  int distance, alarmDist;
  HCSR04* sensor;
  String sensorState;
  State state;
  int PIN_ECHO, PIN_TRIG, minDist, maxDist, rate = 50;
};