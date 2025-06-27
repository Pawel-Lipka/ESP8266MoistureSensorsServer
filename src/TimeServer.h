#ifndef TIME_SERVER_H
#define TIME_SERVER_H

class TimeServer{
private:

    const char* ntpServer;
    const long  gmtOffset_sec;
    const int   daylightOffset_sec;

public:
    TimeServer(const char* ntpServer = "pool.ntp.org", const long  gmtOffset_sec = 3600, const int daylightOffset_sec = 3600);
    String currentTime();
};


#endif