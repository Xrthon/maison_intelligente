#include "HardwareSerial.h"
#include "Capteur.hpp"

Capteur::Capteur(int Trigger, int Echo) {
  this->PIN_TRIG = Trigger;
  this->PIN_ECHO = Echo;
  this->sensor = new HCSR04(this->PIN_TRIG, this->PIN_ECHO);
}
Capteur::~Capteur() {
  delete this->sensor;
}
String Capteur::getSensorState() {
  state = (getDistance() < getMinDist()) ? TO_CLOSE : (getDistance() > this->getMaxDist()) ? TO_FAR
                                                                                           : GOOD;
  sensorState = (state == GOOD) ? "deg" : (state == TO_FAR) ? "Trop loin"
                                                             : "Trop pret";
  return sensorState;
}


void Capteur::setup(int minDistance, int maxDistance, int alarmDist) {
  this->minDist = minDistance;
  this->maxDist = maxDistance;
  this->alarmDist = alarmDist;
  pinMode(this->PIN_TRIG, OUTPUT);
  pinMode(this->PIN_ECHO, INPUT);
  this->setMinDist(this->minDist);
  this->setMaxDist(this->maxDist);
  Serial.println("Setup complet [ Capteur ]");
}
void Capteur::setAlarmStartDist(int distAlarmStart) {
  alarmDist = distAlarmStart;
}
void Capteur::setMaxDist(int maxDist) {
  if (maxDist < minDist) return;

  this->maxDist = maxDist;
}
void Capteur::setMinDist(int minDist) {
  if (minDist > maxDist) return;
  

  this->minDist = minDist;
}

void Capteur::update() {
  static unsigned long _previousTime = -rate;
  static int lastDistance = 0;
  static bool firsTime = true;
  this->_currentTime = millis();


  if (_currentTime - _previousTime < rate) return;
  _previousTime = _currentTime;

  this->distance = this->sensor->dist();
  if (this->distance != 0) {
    lastDistance = this->distance;
  } else {
    this->distance = lastDistance;
  }
}