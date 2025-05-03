#include "Capteur.hpp"
#pragma once

class CommunicationSerie {
private:
  Capteur* capteur;
  String commande = "";
  bool commandeComplete = false;
  Stat commandeInfo;
public:
  CommunicationSerie();
  bool lire();
  void setCapteur(Capteur* capteur);
  void traiterCommande();
  Stat getCommandState();
};
