#include <StreamString.h>
#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H


struct DeviceData
{
    String id;
    String name;
    int rawValue;
    int percentValue;
    unsigned long lastUpdate;
};
#endif
