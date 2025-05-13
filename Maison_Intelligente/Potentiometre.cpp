#include "pins_arduino.h"
#include "Arduino.h"
#include "potentiometre.h"

Potentiometre::Potentiometre(const int& PIN) {
  pinMode(PIN, INPUT); 
}
void Potentiometre::update() {
  value = analogRead(A1);
}
