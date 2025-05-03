
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
void Stepper::setCapteur(Capteur* capteur) {
  this->capteur = capteur;
}
void Stepper::reachTargetRange() {
  myStepper->run();
  myStepper->moveTo(getNewPosition());

  bool changeState = (myStepper->distanceToGo() == 0);

  if (changeState) {
    this->stepperState = OFF;
    myStepper->disableOutputs();
  }
}

String Stepper::getPosition() {
  return String(map(myStepper->currentPosition(), this->getMinRange(), getMaxRange(), this->getMinDegre(), this->getMaxDegre()));
}
void Stepper::setNewPosition() {
  static int lastDistance = 0;
   int minDistSensor = capteur->getMinDist();
  int maxDistSensor = capteur->getMaxDist();

if (lastDistance == capteur->getDistance()) return;
  if (capteur->getDistance() >= minDistSensor && capteur->getDistance() <= maxDistSensor) {
    this->targetPosition = map(capteur->getDistance(), minDistSensor, maxDistSensor, this->minRange, this->maxRange);
  }
  if (capteur->getDistance() <= minDistSensor) {
    this->targetPosition = this->minRange;
  }
  if (capteur->getDistance() >= maxDistSensor) {
    this->targetPosition =this->maxRange;
  }
  lastDistance = capteur->getDistance();
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
  setNewPosition();
  switch (this->stepperState) {
    case OFF:
      this->dontNeedToMove();
      break;
    case ON:
      this->reachTargetRange();
      break;
  }
}