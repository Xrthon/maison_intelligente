#include "HardwareSerial.h"
#include "Communication.hpp"

CommunicationSerie::CommunicationSerie() {
  lire();
}

bool CommunicationSerie::lire() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      traiterCommande();
      return true;
    } else {
      commande += c;
    }
  }

  return false;
}

void CommunicationSerie::setCapteur(Capteur *capteur) {
  this->capteur = capteur;
}
void CommunicationSerie::traiterCommande() {
  String config;
  int separation = 0;

  if (commande == "g_dist" || commande == "gDist") {
    Serial.println(capteur->getDistance());
    commandeInfo = Stat::GOODCMD;
  }

  for (int i = 0; i < commande.length(); i++) {
    char caractere = commande[i];

    if (caractere == ';') {
      switch (separation) {
        case 0:
          if (config == "cfg") {
            config = "";
            separation++;
          } else {
            commandeInfo = Stat::UNKNOWN;
          }
          break;

        case 1:
          {
            String number = "";
            for (int j = i + 1; j < commande.length(); j++) {
              number += commande[j];
            }
            int valeur = number.toInt();

            if (number.length() > 0 && number.length() < 4 && valeur > 0 && valeur < 400) {
              if (config == "lim_inf") {
                if (valeur > capteur->getMaxDist()) {
                  Serial.println("Erreur - Limite inférieure plus grande que limite supérieure");
                  commandeInfo = Stat::BADCMD;
                }
                capteur->setMinDist(valeur);
                commandeInfo = Stat::GOODCMD;
              }

              if (config == "lim_sup") {
                if (valeur < capteur->getMinDist()) {
                  Serial.println("Erreur - Limite supérieure plus petite que limite inférieure");
                  commandeInfo = Stat::BADCMD;
                }
                capteur->setMaxDist(valeur);
                commandeInfo = Stat::GOODCMD;
              }

              if (config == "alm") {
                capteur->setAlarmStartDist(valeur);
                commandeInfo = Stat::GOODCMD;
              }

              commandeInfo = Stat::UNKNOWN;
            }

            commandeInfo = Stat::BADCMD;
          }

        default:
          commandeInfo = Stat::UNKNOWN;
      }
    } else {
      config += caractere;
    }
  }
  commandeInfo = Stat::UNKNOWN;
}









Stat CommunicationSerie::getCommandState() {
  commande = "";

  return commandeInfo;
}
