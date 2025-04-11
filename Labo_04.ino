
#include "Stepper.hpp"
#include "Capteur.hpp"
#include "Screen.hpp"
#include "Buzzer.hpp"
#include "Light.hpp"
#include "RGBLight.hpp"


#define MIN_ROTATION 10
#define MAX_ROTATION 170
#define MAX_SENSOR_DISTANCE 60
#define MIN_SENSOR_DISTANCE 30
#define START_BUZZER_DIST 15


#define PIN_STEP_1 9
#define PIN_STEP_2 10
#define PIN_STEP_3 11
#define PIN_STEP_4 12

#define PIN_ECHO 2
#define PIN_TRIGG 3

#define PIN_RED_LED 7
#define PIN_GREEN_LED 6
#define PIN_BLUE_LED 5

#define PIN_BUZZER 8


unsigned long currenTime = 0;
unsigned int frequency_Buzzer = 1000;
int delayTurnOffAlarm = 3000;
Stepper* stepper = new Stepper(PIN_STEP_1, PIN_STEP_2, PIN_STEP_3, PIN_STEP_4);
Capteur* capteur = new Capteur(PIN_TRIGG, PIN_ECHO);
Screen* screen = new Screen();
Buzzer* buzzer = new Buzzer(PIN_BUZZER);
RGBLight* rgdLight = new RGBLight(PIN_RED_LED, PIN_GREEN_LED, PIN_BLUE_LED);

#pragma region setup - loop

void setup() {
  Serial.begin(115200);

  rgdLight->setup();
  buzzer->setup(frequency_Buzzer);
  screen->setup();
  capteur->setup(MIN_SENSOR_DISTANCE, MAX_SENSOR_DISTANCE);
  stepper->setup(MIN_ROTATION, MAX_ROTATION);
}

void loop() {
  currenTime = millis();
  capteur->update();
  printLCD();
}

void printLCD() {
  static unsigned long previousTime = currenTime;
  static bool firsTime = true;
  static String lastCapteurDistance = capteur->getDistanceStr();
  static String lasStepperState = stepper->getPosition();

  if (firsTime) {
    screen->setStartText1("Dist :");
    screen->setStartText2("Deg :");
    screen->setFirstLine(capteur->getDistanceStr() + " cm");
    screen->setSecLine(stepper->getPosition());
    firsTime = false;
  }
  if (lastCapteurDistance == capteur->getDistanceStr() && lasStepperState == stepper->getPosition()) return;

  screen->setFirstLine(capteur->getDistanceStr() + " cm");
  printSensorAndStepperinformation();



  stepper->update();
  screen->update();
  rgdLight->update();
  buzzer->update();
}
void printSensorAndStepperinformation() {
  static unsigned long previousTimer = 0;
  static bool alarmeState = false;



  if (distanceInGoodRange()) {
    stepper->setNewPosition(capteur->getDistance(), MIN_SENSOR_DISTANCE, MAX_SENSOR_DISTANCE);
    screen->setSecLine(stepper->getPosition() + " deg");
   

  } else {
    screen->setSecLine(capteur->getSensorState());
    if (distanceLowerThanMin()) {
      previousTimer = currenTime;
      startAlarm();
    } 
  }

  
   if (currenTime - previousTimer < delayTurnOffAlarm) return;
    previousTimer = currenTime;
    offAlarm();
}
bool distanceInGoodRange() {
  return (capteur->getDistance() <= MAX_SENSOR_DISTANCE && capteur->getDistance() >= MIN_SENSOR_DISTANCE);
}
bool distanceLowerThanMin() {
  return (capteur->getDistance() <= START_BUZZER_DIST);
}

void startAlarm() {
  buzzer->onState();
  rgdLight->blinkState();
}
void offAlarm() {

  buzzer->offState();
  rgdLight->offState();
}
#pragma endregion
