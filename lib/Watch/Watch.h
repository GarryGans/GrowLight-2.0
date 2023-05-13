#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Wire.h>
#include <Key.h>
#include <Timer.h>

// class Watch : public RTC_ext_1307
class Watch : public RTC_ext_3231
{
    friend class Memory;
    friend class Screen;
    friend class Switchers;
    friend class Pot;
    friend class Bright;

private:
    
    byte cursorDateTime;
    byte cursorSpectrum;
    byte cursorDay;

    boolean leapYear;

    int year;
    byte month;
    byte day;
    byte hour;
    byte min;
    byte sec;
    byte dow;

    int start[lampAmount];
    int finish[lampAmount];

    int daySTD = 12 * 60;
    int totalDayLenght = daySTD;

    const int midNightBefore = 23 * 60 + 59;
    const int midNightAfter = 0;

    boolean autoSwitch[lampAmount];
    boolean skip[lampAmount];

    boolean brightDown[lampAmount];

    int intervalDefault = 0;
     int interval;

    byte startHour[lampAmount];
    byte startMinute[lampAmount];
    byte finishHour[lampAmount];
    byte finishMinute[lampAmount];

    byte RiseHour;
    byte RiseMin;
    byte SetHour;
    byte SetMin;

public:
    Watch();
    ~Watch();

    void setInterval(Key &key);

    void timeFromMinute(int time, byte &hour, byte &minute);
    int nowTime();
    int timeToMinute(byte setHour, byte setMinute);
    void autoSwitchLight(int start, int finish, boolean &autoSwitch, boolean &brightDown);
    void autoSwitcher(Key &key);

    void cursorChange(Key &key, byte &cursor);
    void hmsChange(Key &key, byte &hms, byte &cursor);

    void spectrumReDuration(Key &key);
    void dayReduration(Key &key);

    void dtCursor(Key &key);
    void timeChange(byte &time, Key &key);
    void yearChange(int &year, Key &key);
    void monthChange(byte &month, Key &key);
    void dayChange(byte &day, Key &key);
    void leapYearDay();
    void setWatch(Key &key);

    void commands(Key &key);
};

#endif