#ifndef MY_MDNS_H
#define MY_MDNS_H

#include <ESP8266mDNS.h> 

const byte  DNS_PORT = 53; 

void startMDNS(){

if (!MDNS.begin("kwiatki")) {           
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
}

#endif