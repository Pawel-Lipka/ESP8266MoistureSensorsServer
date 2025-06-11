#include <Arduino.h> 
#include  "WebServer.h"
#include "LocalMDNS.h"
#include "MoistureSensor.h"
#include "WiFiConnection.h"


void setup() {

  Serial.begin(9600);
  conectToWiFi();// WiFIConnection.h
  startMDNS();// LocalMDNS,h
  routing();// WebServer.h
  
}

void loop() {
  
  MDNS.update();
  server.handleClient();
  delay(50); // lower power consumption
}
