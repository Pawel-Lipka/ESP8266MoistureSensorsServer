#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#define maxDevices 20
#define analog_Pin 0
#define digitalPowerPinForSensor 15

#include "ESP8266WebServer.h" 
#include "DeviceData.h"
#include "MoistureSensor.h"
#include "TimeServer.h"

TimeServer timeServer("pool.ntp.org", 3600, 0);
ESP8266WebServer server(80);
MoistureSensor moistureSensor(analog_Pin,digitalPowerPinForSensor,700,200);
int deviceCount = 1;
DeviceData devices[maxDevices] = { { String(ESP.getChipId()), "Device Name",}};


int findDeviceIndex(const String id) {
  for (int i = 0; i < deviceCount; i++) {
    if (devices[i].id == id) return i;
  }
  return -1;
}

void handleData(){
  if (!server.hasArg("id") || !server.hasArg("raw")) {
    server.send(400, "text/plain", "Brak danych");
    return;
  }

  String id = server.arg("id");
  int rawValue = server.arg("raw").toInt();
  int percentValue = server.arg("perc").toInt();
  int idx = findDeviceIndex(id);

  if (idx == -1 && deviceCount < maxDevices) {
    
    devices[deviceCount].id = id;
    devices[deviceCount].name = "new sensor";
    devices[deviceCount].rawValue = rawValue;
    devices[deviceCount].percentValue = percentValue;
    devices[deviceCount].lastUpdate = timeServer.currentTime();
    deviceCount++;

  } else if (idx >= 0) {
    devices[idx].rawValue = rawValue;
    devices[idx].percentValue = percentValue;
    devices[idx].lastUpdate = timeServer.currentTime();;
  }

  server.send(200, "text/plain", "OK");
}



void mainPage(){

  // turn on power and wait until read wiil be possible 
  moistureSensor.turnOnPower();
  delay(500);

//devices[deviceCount].id = id;
//devices[deviceCount].name = "new sensor";
  devices[0].rawValue = moistureSensor.readRaw();
  devices[0].percentValue = moistureSensor.getMoisturePercent();
  devices[0].lastUpdate = timeServer.currentTime();

 
  moistureSensor.turnOffPower();

String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Kwiatki</title>
      <style>
        body {
          font-family: "Segoe UI", sans-serif;
          margin: 0;
          padding: 20px;
          background: #f2f2f2;
        }
        h1 {
          color: #333;
          font-size: 1.5em;
        }
        table {
          width: 100%;
          max-width: 600px;
          margin-top: 20px;
          border-collapse: collapse;
          background: white;
          box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        th, td {
          padding: 10px 15px;
          border-bottom: 1px solid #ddd;
          text-align: left;
        }
        th {
          background-color: #f9f9f9;
          font-weight: 600;
        }
        tr:hover {
          background-color: #f1f1f1;
        }
        footer {
          margin-top: 30px;
          font-size: 0.9em;
          color: #777;
        }
      </style>
    </head>
    <body>
      <h1>Moje kwiatki</h1>
      <table>
        <tr><th>ID</th><th>Nazwa</th><th>Wilgotność (%)</th><th>Aktualizacja</th><th>Zmień Nazwę</th><th>Usuń</th></tr>
  )rawliteral";

    html += "<tr><td>" + devices[0].id + "</td>";
    html += "<td>" + String(devices[0].name) + "</td>";
    html += "<td>" + String(devices[0].percentValue) + "</td>";
    html += "<td>" + devices[0].lastUpdate + "</td>";
    html += "<td><button id = " + String(0) + " type='button' onclick='changeName(\"" + String(0) + "\")'>Zmień Nazwę</button></td></tr>";

   for (int i = 1; i < deviceCount; ++i) {
    html += "<tr><td>" + devices[i].id + "</td>";
    html += "<td>" + String(devices[i].name) + "</td>";
    html += "<td>" + String(devices[i].percentValue) + "</td>";
    html += "<td>" + devices[i].lastUpdate + "</td>";
    html += "<td><button id = " + String(i) + " type='button' onclick='changeName(\"" + String(i) + "\")'>Zmień Nazwę</button></td>";
    html += "<td><button id = " + String(i) + " type='button' onclick='removeDevice(\"" + String(i) + "\")'>Usuń</button></td></tr>";
  }

  html += R"rawliteral(</table><footer>Pozdrawiam Asiunie</footer>)rawliteral";


  html += R"rawliteral(
      <script> 
        function changeName(id) {
          var newName = prompt("Podaj nową nazwę:"); 
          if (newName !== null && newName.trim() !== "") {
            window.location.href = "/setName?id=" + encodeURIComponent(id) + "&name=" + encodeURIComponent(newName);
          }
        }

        function removeDevice(id) {
          if (confirm("Czy na pewno chcesz usunąć ten wpis?")) {
            window.location.href = "/deleteDevice?id=" + encodeURIComponent(id);
          }
        }
      </script>
   
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleNotFound() { 
   String message = "Brak strony na serwerze...";
   server.send(404, "text/plain", message);
}

// change name of devices based on GET parameters
void setName() { 
  String name = server.arg("name");
  int id = server.arg("id").toInt();
  devices[id].name = name;

  server.sendHeader("Location", "/");
  server.send(303);
}

void deleteDevice(){
  int id = server.arg("id").toInt();
  
  for (int i = id; i < deviceCount; i++ ){
    devices[i] = devices[i + 1];
  }
  deviceCount --;

  server.sendHeader("Location", "/");
  server.send(303);
}

void routing(){

  server.on("/", mainPage);
  server.on("/data", handleData);
  server.on("/setName", setName);
  server.onNotFound(handleNotFound);
  server.on("/deleteDevice", deleteDevice);
  server.begin();
}
#endif