#pragma once

class Potentiometre {
  int PIN;
  int value = 0;
public:
  Potentiometre(const int& PIN);
  void update();
  int getValue() {
    return analogRead(A1);
  };
};