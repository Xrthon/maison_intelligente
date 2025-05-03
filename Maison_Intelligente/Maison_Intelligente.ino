
#include "Stepper.hpp"
#include "Capteur.hpp"
#include "Screen.hpp"
#include "Alarm.hpp"
#include "LEDMatrix.hpp"

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

#define CLK_PIN 22
#define CS_PIN 24
#define DIN_PIN 26

unsigned long currenTime = 0;
unsigned int frequency_Buzzer = 1000;
int delayTurnOffAlarm = 3000;
Stepper* stepper = new Stepper(PIN_STEP_1, PIN_STEP_2, PIN_STEP_3, PIN_STEP_4);
Capteur* capteur = new Capteur(PIN_TRIGG, PIN_ECHO);
Screen* screen = new Screen();
Alarme* alarme = new Alarme(PIN_BUZZER, PIN_RED_LED, PIN_GREEN_LED, PIN_BLUE_LED);
LEDMatrix* ledMatrix = new LEDMatrix(CLK_PIN, CS_PIN, DIN_PIN);
#pragma region setup - loop

void setup() {
  Serial.begin(115200);
  ledMatrix->setup();
  ledMatrix->setCapteur(capteur);

  alarme->setup(delayTurnOffAlarm);
  alarme->setSensor(capteur);

  capteur->setup(MIN_SENSOR_DISTANCE, MAX_SENSOR_DISTANCE, START_BUZZER_DIST);

  stepper->setCapteur(capteur);
  stepper->setup(MIN_ROTATION, MAX_ROTATION);
  
  screen->setup(stepper, capteur);
}

void loop() {
  currenTime = millis();

  capteur->update();
  stepper->update();
  alarme->update();
  screen->update();
  ledMatrix->update();
}

#pragma endregion
