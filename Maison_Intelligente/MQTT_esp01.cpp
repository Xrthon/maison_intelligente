#include "pins_arduino.h"
#include "Arduino.h"
#include "RGBLight.hpp"

#include "HardwareSerial.h"
#include "MQTT_esp01.h"

static MQTT* instance = nullptr;  // pointeur vers l'instance actuelle

MQTT::MQTT(Capteur* capteur, Stepper* motor, RGBLight* light, Screen* LCDscreen, Potentiometre* pot)
  : client(espClient) {
  this->capteur = capteur;
  this->motor = motor;
  this->light = light;
  this->LCDscreen = LCDscreen;
  this->potent = pot;
  instance = this;
};

void MQTT::setup() {

  Serial1.begin(115200);
  WiFi.init(Serial1);
  int status = WiFi.begin("Hitron_43_c80", "Chw3ing-g4m($)");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println();
    Serial.println("La communication avec le module WiFi a échoué!");
    // ne pas continuer
    errorState(2, 1);
  }

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println("Hitron_43_c80");

  // Tentative de connexion au Wifi
  while (status != WL_CONNECTED) {
    Serial.print(".");
    status = WiFi.begin("Hitron_43_c80", "Chw3ing-g4m($)");
  }

  Serial.println("Vous êtes connecté au réseau");
  printWifiStatus();
  Serial.println();

  Serial.println("WiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  // Configuration du client au serveur MQTTç

  Serial.println(mqtt_server);
  client.setServer(mqtt_server, 1883);


  // Configuration de la fonction de rappel
  if (!client.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS)) {
    Serial.println("Incapable de se connecter sur le serveur MQTT");
    Serial.print("client.state : ");
    Serial.println(client.state());
  } else {
    Serial.println("Connecté sur le serveur MQTT");
    client.subscribe("etd/17");
    client.subscribe("etd/17/motor");
    client.subscribe("etd/17/color");
  }

  this->client.setCallback(mqttCallbackWrapper);
}

void MQTT::update() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  periodicTask();
}
void MQTT::mqttCallbackWrapper(char* topic, byte* payload, unsigned int length) {
  if (instance != nullptr) {
    instance->mqttEvent(topic, payload, length);  // appel vers la méthode non-static
  }
}

//event recu par mqtt server
void MQTT::mqttEvent(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu [");
  Serial.print(topic);
  Serial.print("] : ");

  // Conversion du message en chaîne C
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.println(message);

  // Vérifie le topic
  if (strcmp(topic, "etd/17/motor") == 0) {
    // Vérifie si le message contient "motor":0 ou "motor":1
    if (strstr(message, "\"motor\":0")) {
      Serial.println("MOTEUR ÉTEINT");
      motor->offState();
    }
    if (strstr(message, "\"motor\":1")) {
      Serial.println("MOTEUR ALLUMÉ");
      motor->onState();
    }
  }

  if (strcmp(topic, "etd/17/color") == 0) {
    // Cherche la chaîne "color":" puis extrait la couleur
    char* ptr = strstr(message, "\"color\":\"");
    if (ptr != NULL) {
      ptr += 9;  // Avance juste après "color":"
      char couleur[8] = { 0 };
      int i = 0;
      while (*ptr != '"' && *ptr != '\0' && i < 9) {
        couleur[i++] = *ptr++;
      }

      couleur[i] = '\0';
      light->SetRGBLedColour(couleur);
    }
  }
}
void MQTT::periodicTask() {
  static unsigned long lastTime = 0;
  static char message[600] = "";
  static char szTemp[6];
  static char szHum[6];
  const unsigned int rate = 100;


  if (millis() - lastTime < rate) return;

  lastTime = millis();

  sprintf(message,
          "{"
          "\"name\":\"%s\","
          "\"number\":%d,"
          "\"uptime\":%lu,"
          "\"dist\":%d,"
          "\"angle\":%d,"
          "\"line1\":\"%s\","
          "\"line2\":\"%s\", "
          "\"pot\": %d"
          "}",
          "etd",                                     // name
          5,                                        // number                // motor
          millis() / 1000,                           // uptime
          capteur->getDistance(),                    // dist
          motor->getPosition(),                      // angle
          LCDscreen->getLine1().c_str(),             // line1
          LCDscreen->getLine2().c_str(),             // line2
          map(analogRead(A1), 0, 1023, 0, 100)  // pot
  );
  // Serial.print("Envoie : ");
  //Serial.println(message);
  // On publie le message
  if (!client.publish("etd/17/data", message)) {
    Serial.println("Incapable d'envoyer le message!");
  }
}

void MQTT::printWifiStatus() {

  // imprime le SSID du réseau auquel vous êtes connecté:
  char ssid1[40];
  WiFi.SSID(ssid1);
  Serial.print("SSID: ");
  Serial.println(ssid1);

  // imprime le BSSID du réseau auquel vous êtes connecté:
  uint8_t bssid[10];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  uint8_t mac[10];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  // imprime l'adresse IP de votre carte:
  IPAddress ip = WiFi.localIP();
  Serial.print("Adresse IP: ");
  Serial.println(ip);

  // imprime la force du signal reçu:
  long rssi = WiFi.RSSI();
  Serial.print("force du signal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void MQTT::errorState(int codeA, int codeB) {
  const int rate = 100;
  const int pauseBetween = 500;
  const int pauseAfter = 1000;

  // On ne sort jamais de cette fonction
  while (true) {
    for (int i = 0; i < codeA; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(rate);
      digitalWrite(LED_BUILTIN, LOW);
      delay(rate);
    }
    delay(pauseBetween);
    for (int i = 0; i < codeB; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(rate);
      digitalWrite(LED_BUILTIN, LOW);
      delay(rate);
    }
    delay(pauseAfter);

    Serial.print("Erreur : ");
    Serial.print(codeA);
    Serial.print(".");
    Serial.println(codeB);
  }
}


void MQTT::reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connexion au broker MQTT...");
    if (client.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS)) {
      Serial.println("connecté !");
      client.subscribe("etd/17");        // Ton topic principal
      client.subscribe("etd/17/motor");  // Ton topic principal
      client.subscribe("etd/17/color");  // Ton topic principal
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void MQTT::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 10) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
