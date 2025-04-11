#include "Arduino.h"
#include "HardwareSerial.h"
#include "Screen.hpp"

Screen::Screen() {
  this->lcd = new LCD_I2C(0x27, column, row);
}

Screen::~Screen() {
  delete lcd;
}
void Screen::setup() {
  lcd->begin();
  lcd->backlight();
  lcd->setCursor(0, 0);
  lcd->print(this->getDA());
  lcd->setCursor(0, 1);
  lcd->print("Labo 4B");
  Serial.println("Setup Complet [ LCD ]");
}
void Screen::setDA(unsigned long idNumber) {
  this->daNumber = idNumber;
}
void Screen::setStartText1(String starText) {
  if (this->starText1 == starText) return;
  this->starText1 = starText;
}


void Screen::setStartText2(String starText) {
  if (this->starText2 == starText) return;
  this->starText2 = starText;
}

void Screen::setFirstLine(String ctText1) {
  if (this->currentText1 == ctText1) return;
  this->currentText1 = ctText1;
}


void Screen::setSecLine(String ctText2) {
  if (this->currentText2 == ctText2) return;
  this->currentText2 = ctText2;
}

void Screen::printSecLine() {
  int espace = starText2.length() + 1;
  lcd->setCursor(0, 1);
  lcd->print(this->starText2);
  lcd->setCursor(espace, 1);
  lcd->print(this->currentText2);
}
void Screen::printFirstLine() {
  int espace = starText1.length() + 1;
  lcd->setCursor(0, 0);
  lcd->print(this->starText1);
  lcd->setCursor(espace, 0);
  lcd->print(this->currentText1);
}
void Screen::update() {
  static bool firsTime = true;
  static String lastTextPrint1 = this->currentText1;
  static String lastTextPrint2 = this->currentText2;

  this->_currentTime = millis();

  if (firsTime) {
    lcd->clear();
    this->_previousTime = this->_currentTime;
    printFirstLine();
    printSecLine();
    firsTime = false;
  }

  if (this->_currentTime - this->_previousTime < rate) return;


  if (lastTextPrint1 != this->currentText1 || lastTextPrint2 != this->currentText2)
    firsTime = true;
}
