#pragma once
#include "Arduino.h"
#include "String.h"
#include <LCD_I2C.h>


class Screen {
public:
  Screen();
  ~Screen();

  void setup();
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


private:
  unsigned long _currentTime = millis(), _previousTime = 0, daNumber = 1993855;
  uint8_t column = 16, row = 2;
  int rate = 100;
  LCD_I2C* lcd;


  bool needUpdate = false;
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
