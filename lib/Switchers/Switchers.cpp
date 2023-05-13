#include "Switchers.h"

Switchers::Switchers()
{
}

Switchers::~Switchers()
{
}

void Switchers::begin(byte &startPinLamp)
{
    for (byte i = 0; i < lampAmount; i++)
    {
        lamp_pins[i] = startPinLamp + i;
        pinMode(lamp_pins[i], OUTPUT);
        digitalWrite(lamp_pins[i], OFF);
    }
}

void Switchers::switcher(Watch &watch, Key &key)
{
    for (byte i = 0; i < lampAmount; i++)
    {
        if (watch.autoSwitch[i] || key.buttonSwitch[i])
        {
            digitalWrite(lamp_pins[i], ON);
        }
        else
        {
            digitalWrite(lamp_pins[i], OFF);
        }
    }
}
