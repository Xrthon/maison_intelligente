
#include "Stepper.hpp"
#include "Capteur.hpp"
#include "Screen.hpp"
#include "Alarm.hpp"
#include "LEDMatrix.hpp"
#include "MQTT_esp01.h"
#include "Potentiometre.h"

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
RGBLight* ledRGB = new RGBLight(PIN_RED_LED, PIN_GREEN_LED, PIN_BLUE_LED);
Alarme* alarme = new Alarme(PIN_BUZZER, ledRGB);
LEDMatrix* ledMatrix = new LEDMatrix(CLK_PIN, CS_PIN, DIN_PIN);
Potentiometre* potentiometre = new Potentiometre(A1);
MQTT* wifiModule = new MQTT(capteur, stepper, ledRGB, screen, potentiometre);




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

  while (!Serial)
    ;

  wifiModule->setup();
}

void loop() {
  capteur->update();
  stepper->update();
  alarme->update();
  screen->update();
  potentiometre->update();

  wifiModule->update();
  ledMatrix->update();
}
