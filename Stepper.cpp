#include "WString.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "Stepper.hpp"



Stepper::Stepper(int Pin_1, int Pin_2, int Pin_3, int Pin_4) {
  this->IN_1 = Pin_1;
  this->IN_2 = Pin_2;
  this->IN_3 = Pin_3;
  this->IN_4 = Pin_4;
  this->myStepper = new AccelStepper(AccelStepper::FULL4WIRE, IN_1, IN_3, IN_2, IN_4);
  myStepper->run();
}
Stepper::~Stepper() {
  delete myStepper;
}
void Stepper::setup(int minDegree, int maxDegre) {
  this->minDegreRotation = minDegree;
  this->maxDegreRotation = maxDegre;
  myStepper->setMaxSpeed(this->maxspeed);
  myStepper->setAcceleration(this->acceleration);
  myStepper->setSpeed(this->currentSpeed);
  this->setMinDegre(this->minDegreRotation);
  this->setMaxDegre(this->maxDegreRotation);
  Serial.println("Setup complet [Stepper]");
}
void Stepper::setMinDegre(int minDegree) {
  this->minRange = minDegree * (this->MAX_RANGE_STEPPER / 360);
}
void Stepper::setMaxDegre(int maxDegree) {
  this->maxRange = maxDegree * (this->MAX_RANGE_STEPPER / 360);
}

void Stepper::reachTargetRange() {
  static bool firsTime = true;

  if (firsTime) {
    myStepper->run();
    myStepper->moveTo(getNewPosition());
    firsTime = false;
  }

  myStepper->run();
  myStepper->moveTo(getNewPosition());

  bool changeState = (myStepper->distanceToGo() == 0);

  if (changeState) {
    this->stepperState = OFF;
    myStepper->disableOutputs();
    firsTime = true;
  }
}
String Stepper::getPosition() {
  return String(map(myStepper->currentPosition(), this->getMinRange(), getMaxRange(), this->getMinDegre(), this->getMaxDegre()));
}
void Stepper::setNewPosition(int newTarget, int MIN_RANGE, int MAX_RANGE) {
  this->targetPosition = map(newTarget, MIN_RANGE, MAX_RANGE, this->minRange, this->maxRange);
}

void Stepper::dontNeedToMove() {
  static bool firsTime = true;
  static int lastPosition = this->targetPosition;
  static int rate = 0;

  this->_currentTime = millis();

  if (firsTime) {
    lastPosition = this->targetPosition;
    myStepper->disableOutputs();
    firsTime = false;
  }

  bool changeState = (lastPosition != this->targetPosition);

  if (changeState) {
    this->stepperState = ON;
    this->myStepper->run();
    firsTime = true;
  }
}
void Stepper::update() {
  switch (this->stepperState) {
    case OFF:
      this->dontNeedToMove();
      break;
    case ON:
      this->reachTargetRange();
      break;
  }
}