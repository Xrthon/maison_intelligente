#pragma once
class Light {
public:
  enum State {
    OFF,
    ON
  };
  Light(int PIN);
  void setup();

  void blinkState();
  void fadeState();

  void setBlinkRate(int rate);
  void setLightState(bool State);

  int getBlinkRate() {
    return this->blinkRate;
  }
  int getLEDPin() {
    return this->PIN;
  }
  bool getState() {
    return this->state;
  }
  unsigned long getTimer() {
    return _currenTimer;
  }
protected:
  int blinkRate = 100;

private:
  unsigned long _currenTimer;
  int PIN;
  State state = OFF;
};