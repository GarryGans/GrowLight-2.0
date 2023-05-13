#ifndef Voltage_H
#define Voltage_H

#include <Arduino.h>
#include <Data.h>

class Voltage
{
    friend class Screen;

private:
    byte volt[lampAmount];
    int ampere[lampAmount];

public:
    Voltage();
    ~Voltage();

    void begin(byte &startPin);
};

#endif