#ifndef Key_H
#define Key_H

#include <Arduino.h>
#include <AmperkaKB.h>
#include <Timer.h>
#include <Data.h>

#define keyTime 14
#define keyBright 7
#define keySpeed 4

#define keyManual 11

#define keySunTime 2
#define keySunBright 3
#define keySunColor 1
#define keyInterval 5

#define keyWatch 10

#define keyEscape 0
#define keyOk 9

#define keyUp 15
#define keyDown 6
#define keyForward 8
#define keyBack 12

#define keySkip 13



#define autoEsc 4

class Key : public AmperkaKB
{
    friend class Screen;
    friend class Switchers;
    friend class Watch;
    friend class Memory;
    friend class Bright;

private:
    Timer timer_0;
    Timer timer_1;
    Timer timer_2;
    Timer timer_3;

    byte awCount = autoEsc;
    
    enum Screen
    {
        start,

        watch,

        lamp, // Show each Spectr info

        manual, // Manual use each lamp for test, on_off bright_level

        duration, // Each  Spectr duration time by watch

        riseBright,
        maxBright,
        setBright,

        interval, // ManualInterval or AutoInterval or SpecBySpec  or TugetherGroup or Together(NO interval is OFF)

        sunDuration,
        // In  Future realise with encoder
        sunBright,
        sunColor,
        
        speed, // Manual Sun Speed Set  or (Always At Start  Only Sun Rise) or No Speed is OFF

        voltage

    } screen = start;

    enum Direction
    {
        BACK,
        FORWARD
    } direction;

    // enum Act
    // {
    //     PLUS,
    //     MINUS
    // } act;

    boolean buttonSwitch[lampAmount];
    boolean reduration[lampAmount];
    boolean reDay;

    boolean resetCounter;

    boolean resetManualBright;

    boolean writeBright;
    boolean writeTime;
    boolean writeDay;
    boolean writeSkip;
    boolean writeInterval;
    boolean writeSpeed;
    boolean writeAllBright;
    boolean writeAllColor;

    boolean writeMaxBright;
    boolean writeRiseBright;
    boolean writeSetBright;

    boolean nextScreen;

    boolean reSetting;

    boolean reBright[lampAmount];

    boolean correctDay;

    boolean setDateTime;
    boolean setDate;
    boolean setTime;

    byte id;
    byte idFirst = 0;
    byte idLast = lampAmount - 1;

public:
    Key(byte pin[]);
    Key();
    ~Key();

    // void setPresets() // Vega, Bloom(time, bright, spectr)
    // Screen changeScreen();

    void checkKeyboard();

    Key::Screen menuScreen(Screen start, Screen end);

    boolean autoOk(Screen screen);
    boolean autoOk(byte count);

    // void setScreens();
    void keyCommands();
    void idChange();
    // void setSpec();

    void autoScreenMove();
    void manualChangeScreen();

    void skipEnable(boolean &skip);
    void manualSwitchLight();

    boolean setVoltage();

    boolean clickOrHold();
    boolean click(byte key);
    boolean escape();
    boolean ok();

    template <typename T>
    boolean valChange(T &val, T min, T max, boolean twoDirection = false);

    void cursor(byte &cursor, byte min, byte max);
    boolean navigation();

    void progressBar();
    boolean spectrumReDuration();
    boolean changeBright();
    boolean setWatch();
    void setSpeed();

    void resetSunSetting();
    void writeSunSetting();

    boolean dayReduration();
    boolean allBrigh();
    boolean allColor();
    boolean setInterval();
};

#endif