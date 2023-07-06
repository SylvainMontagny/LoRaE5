#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"

/* TODO */
// Changer le nom de la classe LORAE5 et de l'objet instancié lorae5 sinon on s'y perd.
// Mettre une saisie au clavier 't' permettant de declancher un Uplink
// Mettre une tempo de 3s plutot qu'une boucle infinie lors du test de la présence de la liaison serie au début du programme.
// Faire une fonction capable de récupérer les data en downlink. 
// config_app : Mettre ADR à false et SF à 7 par default
// Pour la récupération des données en uplink, ainsi que l'envoi, il serait plus simple de travailler avec des tableaux uint8_t plutot que des String.
// Il faut privilégier les fonctions de la class String pour étudier le contenu des chaines de caractère lors du dialogue avec le module.
// Donner l'info que d'autres downlink sont pending... si c'est le cas. A voir si on active nous même un nouveau uplink dans le cas ou il reste des downlinks sur le NS.
// Dans la méthode checkResponse, il faut vérfier uniquement le mot et non pas \r\n, c'est plus simple pour la suite. De plus, on pourra se limiter à une partie de la réponse uniquement au lieu de mettre la totalité.
// Il faut solutionner l'erreur qui apparait en ABP.
// Mettre de une doc des fonctions utilisée en entête de chaque fonction
// Mettre la librairie dans le répertoire des librairies arduino.


LoraE5 LoRaE5(devEUI, appEUI, appKey, devAddr, nwkSKey, appSKey); 

void setup() {
  LoRa_Serial.begin(9600);
  USB_Serial.begin(9600);
  while (!USB_Serial);
  USB_Serial.println("\r\n\n\n\n\n\n\n\n");
  USB_Serial.println("#######################################");
  USB_Serial.println("#### LoRaWAN Training Session #########");
  USB_Serial.println("#### Savoie Mont Blanc University #####\r\n");
  
  while(!LoRaE5.checkBoard());

  LoRaE5.setup(ACTIVATION_MODE, CLASS, SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT);
  LoRaE5.printInfo(SEND_BY_PUSH_BUTTON, FRAME_DELAY, LOW_POWER);


  if(ACTIVATION_MODE == OTAA){
    LoRaE5.setDevEUI(devEUI);
    LoRaE5.setAppEUI(appEUI);
    LoRaE5.setAppKey(appKey);
    USB_Serial.println("\r\nJoin Procedure in progress...");
    while(LoRaE5.join() == false);
    delay(3000);
   }
  
 if(ACTIVATION_MODE == ABP){
   LoRaE5.setDevAddr(devAddr);
   LoRaE5.setNwkSKey(nwkSKey);
   LoRaE5.setAppSKey(appSKey);
 }
}

void loop() {
  LoRaE5.sendMsg(_STRING, "HELLO");
  //LoRaE5.sendMsg(_HEXA, "AA22BB33");
  delay(FRAME_DELAY-10000);
}
