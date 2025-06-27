#include <Arduino.h> 
#include <time.h>
#include "TimeServer.h"

TimeServer::TimeServer(const char* ntpServer, const long  gmtOffset_sec, const int   daylightOffset_sec)
    :ntpServer(ntpServer), gmtOffset_sec(gmtOffset_sec), daylightOffset_sec(daylightOffset_sec)
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
}

String TimeServer::currentTime(){

    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    //String result = asctime(&timeinfo);

    char buffer[9]; // HH:MM:SS = 8 znaków + null
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    return String(buffer);
  
  //Serial.print("Current time: ");
  //Serial.print(asctime(&timeinfo)); // Prints date and time in a readable format

}