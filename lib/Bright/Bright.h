#ifndef Brigh_H
#define Brigh_H

#include <Arduino.h>
#include <Timer.h>
#include <Key.h>
#include <Watch.h>

class Bright
{
    friend class Memory;
    friend class Screen;

private:
    Timer timer[lampAmount];

    int minWaitTime = 0;
    int maxWaitTime = 90;

    int waitTime[lampAmount];

    byte pin[lampAmount];

    byte bright[lampAmount];

    byte riseBright[lampAmount]; // 197(340mA)-sunRise    216(120mA)-sunSet     min 196(350mA) max 22(2700ma)
    byte setBright[lampAmount];
    byte maxBright[lampAmount];

    // byte minPWM[lampAmount]; // 197(340mA)-sunRise    216(120mA)-sunSet     min 196(350mA) max 22(2700ma)
    byte maxPWM[lampAmount];

    const byte minAllBright = 1;
    byte allBrigh;
    const byte maxAllBright = 100;

    const byte minAllColor = 0;
    byte allColor;
    const byte maxAllColor = 99;

    const byte allMinPWM = 0;
    const byte allMaxPWM = 255;

    byte manualBright[lampAmount];

public:
    Bright();
    ~Bright();

    void setRiseSpeed(Key &key);

    void begin(byte startBrightPin);

    void setMinBright(byte pin, byte &bright, byte brightRise);
    void resetBright(byte pin, byte &bright);

    void autoChangeBright(Watch &watch, Key &key, byte i);
    void autoBright(Watch &watch, Key &key);
    void manualChangeBright(Watch &watch, Key &key);
    void resetAllBrights();
    void changeBright(Key &key, Watch &watch);

    void changeMaxBright(byte &bright, Key &key, Watch &watch, byte min, byte max);

    byte mapBright(byte allBrigh, byte minAllBright, byte maxAllBright, byte setBright, byte maxBright);

    boolean setAllBrigh(Key &key);
    boolean setAllColor(Key &key);

    void commands(Watch &watch, Key &key);
};

#endif