#include "HardwareSerial.h"
#include "Communication.hpp"
#include "LEDMatrix.hpp"
LEDMatrix::LEDMatrix(int CLK_PIN, int CS_PIN, int DIN_PIN) {
  this->u8g2 = new U8G2_MAX7219_8X8_F_4W_SW_SPI(
    U8G2_R0,             // rotation
    /* clock=*/CLK_PIN,  // pin Arduino reliée à CLK (horloge)
    /* data=*/DIN_PIN,   // pin Arduino reliée à DIN (données)
    /* cs=*/CS_PIN,      // pin Arduino reliée à CS (chip select)
    /* dc=*/U8X8_PIN_NONE,
    /* reset=*/U8X8_PIN_NONE);
}
void LEDMatrix::setCapteur(Capteur* capteur) {
  this->capteur = capteur;
}
void LEDMatrix::setup() {
  // Initialiser la bibliothèque
  u8g2->begin();


  // Ajuster la luminosité globale :
  //    0 = plus sombre, 255 = plus lumineux (selon la librairie, la limite peut être 0–255 ou 0–15).
  u8g2->setContrast(25);

  // Sélection de la police :
  // On peut utiliser "u8g2_font_6x10_tr" ou autre selon vos préférences
  u8g2->setFont(u8g2_font_4x6_tr);

  // Optionnel : effacer la matrice au démarrage
  //Serial.println("Setup complet [ u8g2 ]");
}
void LEDMatrix::setMessage(char message) {
  this->message = message;
}
void LEDMatrix::dessiner_message_a_position(const char* texte, uint16_t position_bits) {
  char tampon[TAILLE_TAMPON];                                 // Tampon pour contenir une portion du texte
  uint16_t debut_caractere = position_bits / LARGEUR_POLICE;  // Index du caractère de départ
  uint16_t i;

  // Copier une portion du texte dans le tampon
  for (i = 0; i < TAILLE_TAMPON - 1; i++) {
    tampon[i] = texte[debut_caractere + i];
    if (texte[debut_caractere + i] == '\0')
      break;
  }

  tampon[TAILLE_TAMPON - 1] = '\0';  // S'assurer que le tampon est bien terminé

  // Afficher le texte en tenant compte du décalage de bits (scroll horizontal)
  u8g2->drawStr(-(position_bits & (LARGEUR_POLICE - 1)), 7, tampon);
}



void LEDMatrix::configAlarmDist() {
  capteur->setAlarmStartDist(15);
}

void LEDMatrix::printError() {
  switch (ledState) {
    case Stat::BADCMD:
      u8g2->clearBuffer();
      this->u8g2->drawCircle(4, 4, 3);
      this->u8g2->drawLine(2, 2, 6, 6);
      u8g2->sendBuffer();
      break;
    case Stat::GOODCMD:
      u8g2->clearBuffer();
      this->u8g2->drawLine(1, 3, 5, 7);
      this->u8g2->drawLine(0, 2, 2, 0);
      u8g2->sendBuffer();
      break;
    case Stat::UNKNOWN:
      u8g2->clearBuffer();
      u8g2->drawLine(7, 0, 0, 7);
      u8g2->drawLine(0, 0, 7, 7);
      u8g2->sendBuffer();
      break;
  }
}


void LEDMatrix::update() {
  static unsigned long previousTime = 0;
  static unsigned long currenTimer = millis();
  static CommunicationSerie comm;
  static bool firstime = true;
  if (firstime) {
    comm.setCapteur(capteur);
    firstime = false;
  }
  if (comm.lire()) ledState = comm.getCommandState();
  currenTimer = millis();

  printError();
  if (currenTimer - previousTime <= 3000 && ledState != Stat::GOODCMD) return;
  ledState = Stat::GOODCMD;
  previousTime = currenTimer;
}