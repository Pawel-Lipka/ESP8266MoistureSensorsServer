#include "MoistureSensor.h"
#include <Arduino.h> 

MoistureSensor::MoistureSensor(int analogPin, int powerPin, int dry, int wet)
    : pin(analogPin), powerPin(powerPin), dryValue(dry), wetValue(wet) {

        pinMode(analogPin, INPUT);
        pinMode(powerPin,OUTPUT);
    }

int MoistureSensor::readRaw() const {
    return analogRead(pin);
}

int MoistureSensor::getMoisturePercent() const {
    
    int raw = readRaw();
    int percent = map(raw, dryValue, wetValue, 0, 100);
    //   int mappedValue = map(sensorValue,lowerBound,upperBound,100,0);
    return percent;
}

void MoistureSensor::turnOnPower() const {
    digitalWrite(powerPin,HIGH);
    Serial.println(powerPin);
}

void MoistureSensor::turnOffPower() const {
    digitalWrite(powerPin,LOW);
}
