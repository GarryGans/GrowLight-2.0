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
    Timer timer[2];

    byte speed[lampAmount];

    byte pin[lampAmount];
    byte bright[lampAmount];

    byte riseBright[lampAmount]; // 197(340mA) sunRise 216(120mA) sunSet   min 196(350mA) max 22(2700ma)
    byte setBright[lampAmount];

    byte maxBright[lampAmount];

    byte minManualBright = minManual;
    byte maxManualBright = maxManual;

    byte brightLevel;
    byte lowLevel = 0;
    byte maxLevel[lampAmount];

    byte allBrigh;
    byte maxAllBright = 99;

    byte maxPWM = 255;

public:
    Bright();
    ~Bright();

    void brightLevelCount();

    void setRiseSpeed(Key &key);

    void begin(byte startBrightPin);

    void setMinBright(byte pin, byte &bright, byte brightRise);
    void resetBright(byte pin, byte &bright);

    void autoChangeBright(Watch &watch, Key &key, byte i);
    void autoBright(Watch &watch, Key &key);
    void manualChangeBright(Watch &watch, Key &key);
    void resetAllBrights();
    void changeBright(Key &key, Watch &watch);

    void setSetBright(byte &bright, Key &key, byte min, byte max);
    void setRiseBright(byte &brightRise, Key &key, byte min, byte max);
    void changeMaxBright(byte &bright, byte pin, Key &key, Watch &watch, byte min, byte max);

    boolean setAllBrigh(Key &key);

    void commands(Watch &watch, Key &key);
};

#endif