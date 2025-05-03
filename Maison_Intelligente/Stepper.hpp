#include "Capteur.hpp"
#include <avr/interrupt.h>
#include "Arduino.h"
#include <AccelStepper.h>
#pragma once



class Stepper {
public:

  enum State {
    OFF,
    ON,
  };
  Stepper(int Pin_1, int Pin_2, int Pin_3, int Pin_4);
  ~Stepper();

  void reachTargetRange();
  void dontNeedToMove();
  void update();
  void setup(int minDegre, int maxDegre);
  void setMaxDegre(int minDegree);
  void setMinDegre(int maxDegree);
  void setNewPosition();
  void setCapteur(Capteur* capteur);
  int getNewPosition() {
    return targetPosition;
  };
  int getMinDegre() {
    return minDegreRotation;
  };

  int getMaxDegre() {
    return maxDegreRotation;
  };
  int getMaxRange() {
    return maxRange;
  };
  int getMinRange() {
    return minRange;
  };
  String getPosition();


private:
  unsigned long _previousTime = 0,
                _currentTime = 0;
  int IN_1, IN_2, IN_3, IN_4;
  int MAX_RANGE_STEPPER = 2038, MIN_RANGE_STEPPER = 0;
  int currentSpeed = 1000,
      maxspeed = 1000,
      acceleration = 1000,
      minDegreRotation,
      maxDegreRotation,
      minRange,
      maxRange,
      targetPosition;


  AccelStepper* myStepper;
  Capteur* capteur;
  State stepperState = OFF;
};