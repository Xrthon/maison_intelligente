#include "Capteur.hpp"
#include "WString.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include <U8g2lib.h>
#include "Communication.hpp"
#include "TypeDefs.h"
#pragma once




class LEDMatrix {


public:

  LEDMatrix(int CLK_PIN, int CS_PIN, int DIN_PIN);
  void setup();
  void update();
  void setCapteur(Capteur *capteur);
  void configAlarmDist();
  void printError();
  void configLimitInfOrSup();
  void printCaptDist();
  void dessiner_message_a_position(const char *texte, uint16_t position_bits);
  void setMessage(char message);
private:
  int CLK_PIN, DIN_PIN, CS_PIN;
  char *message;
  String command;
  Stat ledState;
  short LARGEUR_POLICE = 4;
  short NOMBRE_MATRICES = 1;
  short TAILLE_TAMPON = 12;
  const uint8_t largeur_affichage = NOMBRE_MATRICES * 8;
  uint16_t position_bits = 0;
  uint16_t longueur_bits;
  Capteur *capteur;
  U8G2_MAX7219_8X8_F_4W_SW_SPI *u8g2;
};