
#include "Capteur.hpp"
#include "Stepper.hpp"
#include "RGBLight.hpp"
#include "Screen.hpp"
#include "potentiometre.h"

#pragma once

#include <WiFiEspAT.h>
#include <PubSubClient.h>

class MQTT {
private:
  const char* ssid = "Hitron_43_c80";
  const char* password = "Chw3ing-g4m($)";

  const char* mqtt_server = "216.128.180.194";
  const int mqtt_port = 1883;
  const char* MQTT_USER = "etdshawi";
  const char* MQTT_PASS = "shawi123";
  const char* DEVICE_NAME = "etd/17";
  Capteur* capteur;
  Stepper* motor;
  RGBLight* light;
  Screen* LCDscreen;
  Potentiometre* potent;
  WiFiClient espClient;
  PubSubClient client;

public:
  MQTT(Capteur* capteur, Stepper* motor, RGBLight* light, Screen* LCDscreen, Potentiometre* potentiometre);
  void setup();
  void update();
  void mqttEvent(char* topic, byte* payload, unsigned int length);
  void reconnectMQTT();
  void periodicTask();
  void printWifiStatus();
  static void mqttCallbackWrapper(char* topic, byte* payload, unsigned int length);
  void printMacAddress(byte mac[]);
  void errorState(int codeA, int codeB);
};
