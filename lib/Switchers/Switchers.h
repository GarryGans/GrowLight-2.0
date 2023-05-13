#ifndef SWITCHERS_H
#define SWITCHERS_H

#include <Arduino.h>

#include <Watch.h>
#include <Key.h>

#define OFF HIGH // реле: 1 - высокого уровня (или мосфет), 0 - низкого
#define ON LOW

class Switchers
{
private:
    byte lamp_pins[lampAmount];

public:
    Switchers();
    ~Switchers();

    void begin(byte &startPinLamp);
    void switcher(Watch &watch, Key &key);
};

#endif