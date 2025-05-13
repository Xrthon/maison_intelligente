#include "Capteur.hpp"
#include "Arduino.h"
#include "Buzzer.hpp"
#include "RGBLight.hpp"
#pragma once

class Alarme {

public:
  Alarme(int BUZZER_PIN, RGBLight* ledRGB);
  void setup(int delayTurnOffAlarm);
  void update();
  void startAlarm();
  void offAlarm();
  void setSensor(Capteur* capteur);
  bool distanceLowerThanMin();

private:
  Buzzer* buzzer;
  RGBLight* rgdLight;
  Capteur* capteur;
  int delayTurnOffAlarm=0;
  bool alarmOff=false;
};