#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include "ESP8266WiFi.h"

const char *ssid = "PLAY_Swiatlowodowy_9999";
const char *password = "HuxhCvy4W7";

void conectToWiFi(){

  WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  WiFi.begin(ssid, password);

  Serial.print("Connecting server ");
   
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  WiFi.setSleepMode(WIFI_MODEM_SLEEP); 

  Serial.println("");
  Serial.print("IP Adress: ");
  Serial.print(WiFi.localIP());
}

#endif