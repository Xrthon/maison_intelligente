#include "HardwareSerial.h"
#include "WString.h"
#include <avr/interrupt.h>
#include "Capteur.hpp"
#include "Stepper.hpp"
#include "Arduino.h"
#include "String.h"
#include <LCD_I2C.h>
#pragma once



class Screen {
public:
  Screen();
  ~Screen();

  void setup(Stepper* stepper, Capteur* capteur);
  void update();

  void setStartText1(const String Start);
  void setStartText2(const String Start);
  void setSecLine(String texteLine2);
  void setFirstLine(String texteLine1);
  void printFirstLine();
  void printSecLine();
  void setDA(unsigned long idNumber);
  String getDA() {
    return String(daNumber);
  };
  String getLine1() {
    return starText1+currentText1;
  };
  String getLine2() {
    return starText2 + currentText2;
  };

private:
  unsigned long _currentTime = millis(), _previousTime = 0, daNumber = 1993855;
  uint8_t column = 16, row = 2;
  int rate = 100;
  LCD_I2C* lcd;
  Stepper* stepper;
  Capteur* capteur;

  bool needUpdate = false;
  String *ptr_text1= nullptr;
  String starText1, starText2, currentText1, currentText2;
  uint8_t lastNumber_55[8] = {
    0b11100,
    0b10000,
    0b11111,
    0b00100,
    0b11111,
    0b00001,
    0b00111,
    0b00000
  };
};
