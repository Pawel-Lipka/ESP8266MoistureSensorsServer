#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H


class MoistureSensor {
private:
    int pin;
    int powerPin;
    int dryValue;
    int wetValue;


public:
    MoistureSensor(int analogPin, int PowerPin = 4, int dry = 1023, int wet = 0);

    int readRaw() const;
    int getMoisturePercent() const;
    void turnOnPower() const;
    void turnOffPower() const;
};

#endif