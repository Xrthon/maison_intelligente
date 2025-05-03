#include "HardwareSerial.h"
#include "RGBLight.hpp"
#include "Light.hpp"
#include "Alarm.hpp"

Alarme::Alarme(int BUZZER_PIN, int LIGHT_PIN1, int LIGHT_PIN2, int LIGHT_PIN3) {
  buzzer = new Buzzer(BUZZER_PIN);
  rgdLight = new RGBLight(LIGHT_PIN1, LIGHT_PIN2, LIGHT_PIN3);
}
void Alarme::setup(int delayTurnOffAlarm) {
  this->delayTurnOffAlarm = delayTurnOffAlarm;
  buzzer->setup();
  rgdLight->setup();
}
void Alarme::update() {
  static unsigned long previousTimer = 0, currenTime = millis();
  currenTime = millis();
  if (this->distanceLowerThanMin()) {
    previousTimer = currenTime;
    this->startAlarm();
  }

  buzzer->update();
  rgdLight->update();

  if (currenTime - previousTimer < delayTurnOffAlarm) return;
  previousTimer = currenTime;
  this->offAlarm();
}

void Alarme::setSensor(Capteur* capteur) {
  this->capteur = capteur;
}
bool Alarme::distanceLowerThanMin() {
  return (capteur->getDistance() <= this->capteur->alarmStartDistance());
}

void Alarme::offAlarm() {
  buzzer->offState();
  rgdLight->offState();
}
void Alarme::startAlarm() {
  buzzer->onState();
  rgdLight->blinkState();
}