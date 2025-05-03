#include "Capteur.hpp"
#include "Arduino.h"
#include "Buzzer.hpp"
#include "RGBLight.hpp"
#pragma once

class Alarme {

public:
  Alarme(int BUZZER_PIN, int LIGHT_PIN1, int LIGHT_PIN2, int LIGHT_PIN3);
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
};