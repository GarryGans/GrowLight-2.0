#include "Watch.h"

// Watch::Watch() : RTC_ext_1307()
Watch::Watch() : RTC_ext_3231()
{
}

Watch::~Watch()
{
}

void Watch::timeFromMinute(int time, byte &hour, byte &minute)
{
    hour = 0;

    while (time >= 60)
    {
        time -= 60;
        hour++;
    }

    minute = time;
}

int Watch::timeToMinute(byte setHour, byte setMinute)
{
    return (setHour * 60 + setMinute);
}

int Watch::nowTime()
{
    DateTime time = now();
    return (timeToMinute(time.hour(), time.minute()));
}

void Watch::autoSwitchLight(int start, int finish, boolean &autoSwitch, boolean &brightDown)
{
    if (start == finish)
    {
        autoSwitch = true;
        brightDown = false;
    }

    else if (start > finish)
    {
        if ((nowTime() >= start && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < finish))
        {
            autoSwitch = true;
            brightDown = false;
        }
        else
        {
            brightDown = true;
        }
    }

    else if (start < finish)
    {
        if ((nowTime() >= start && nowTime() < finish))
        {
            autoSwitch = true;
            brightDown = false;
        }
        else
        {
            brightDown = true;
        }
    }

    else
    {
        autoSwitch = false;
    }
}

void Watch::autoSwitcher(Key &key)
{
    key.skipEnable(skip[key.id]);

    if (key.screen != key.dayDuration)
    {
        RiseHour = startHour[0];
        RiseMin = startMinute[0];
        SetHour = finishHour[0];
        SetMin = finishMinute[0];
    }

    if (key.screen != key.manual)
    {
        for (byte i = 0; i < lampAmount; i++)
        {
            if (!skip[i] && !key.reduration[i])
            {
                start[i] = timeToMinute(startHour[i], startMinute[i]);
                finish[i] = timeToMinute(finishHour[i], finishMinute[i]);
                autoSwitchLight(start[i], finish[i], autoSwitch[i], brightDown[i]);
            }
            else if (skip[i])
            {
                autoSwitch[i] = false;
            }
        }
    }

    else
    {
        for (byte i = 0; i < lampAmount; i++)
        {
            autoSwitch[i] = false;
        }
    }
}

void Watch::cursorChange(Key &key, byte &cursor)
{
    if (key.navigation())
    {
        switch (key.direction)
        {
        case key.FORWARD:
            cursor++;
            if (cursor > 3)
            {
                cursor = 0;
            }
            break;
        case key.BACK:
            cursor--;
            cursor = constrain(cursor, 0, 3);
            break;

        default:
            break;
        }
    }
}

void Watch::hmsChange(Key &key, byte &hms, byte &cursor)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS)
        {
            hms--;
            if (cursor == 0 || cursor == 2)
            {
                hms = constrain(hms, 0, 23);
            }
            else
            {
                hms = constrain(hms, 0, 59);
            }
        }

        if (key.act == key.PLUS)
        {
            hms++;
            if ((cursor == 0 || cursor == 2) && hms > 23)
            {
                hms = 0;
            }
            if ((cursor == 1 || cursor == 3) && hms > 59)
            {
                hms = 0;
            }
        }
    }

    cursorChange(key, cursor);
}

void Watch::spectrumReDuration(Key &key)
{
    if (key.spectrumReDuration())
    {
        if (cursorSpectrum == 0)
        {
            hmsChange(key, startHour[key.id], cursorSpectrum);
        }
        else if (cursorSpectrum == 1)
        {
            hmsChange(key, startMinute[key.id], cursorSpectrum);
        }
        else if (cursorSpectrum == 2)
        {
            hmsChange(key, finishHour[key.id], cursorSpectrum);
        }
        else if (cursorSpectrum == 3)
        {
            hmsChange(key, finishMinute[key.id], cursorSpectrum);
        }
    }
}

void Watch::dayReduration(Key &key)
{
    if (key.dayReduration())
    {
        if (cursorDay == 0)
        {
            hmsChange(key, RiseHour, cursorDay);
        }
        else if (cursorDay == 1)
        {
            hmsChange(key, RiseMin, cursorDay);
        }
        else if (cursorDay == 2)
        {
            hmsChange(key, SetHour, cursorDay);
        }
        else if (cursorDay == 3)
        {
            hmsChange(key, SetMin, cursorDay);
        }
    }

    if (key.correctDay)
    {
        start[0] = timeToMinute(RiseHour, RiseMin);
        finish[0] = timeToMinute(SetHour, SetMin);

        for (byte id = 0; id < lampAmount; id++)
        {
            timeFromMinute(start[0] + intervalDefault, startHour[id], startMinute[id]);
            timeFromMinute(finish[0] - intervalDefault, finishHour[id], finishMinute[id]);
            intervalDefault += interval;
        }

        intervalDefault = 0;
        cursorDay = 0;

        key.correctDay = false;
    }
}

void Watch::dtCursor(Key &key)
{
    if (key.navigation())
    {
        switch (key.direction)
        {
        case key.FORWARD:
            cursorDateTime++;
            if (cursorDateTime > 5)
            {
                cursorDateTime = 0;
            }
            break;

        case key.BACK:
            cursorDateTime--;
            cursorDateTime = constrain(cursorDateTime, 0, 5);
            break;

        default:
            break;
        }
    }
}

void Watch::timeChange(byte &time, Key &key)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS)
        {
            time--;

            if (cursorDateTime == 3)
            {
                time = constrain(time, 0, 23);
            }

            if (cursorDateTime == 4 || cursorDateTime == 5)
            {
                time = constrain(time, 0, 59);
            }
        }

        if (key.act == key.PLUS)
        {
            time++;

            if (cursorDateTime == 3 && time > 23)
            {
                time = 0;
            }

            if ((cursorDateTime == 4 || cursorDateTime == 5) && time > 59)
            {
                time = 0;
            }
        }
    }
}

void Watch::yearChange(int &year, Key &key)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS)
        {
            year--;

            if (cursorDateTime == 2 && year < 2021)
            {
                year = 2021;
            }
        }

        if (key.act == key.PLUS)
        {
            year++;
        }
    }
}

void Watch::monthChange(byte &month, Key &key)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS)
        {
            month--;

            if (month < 1)
            {
                month = 12;
            }
        }

        if (key.act == key.PLUS)
        {
            month++;

            if (month > 12)
            {
                month = 1;
            }
        }
    }
}
void Watch::dayChange(byte &day, Key &key)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS)
        {
            day--;

            if (day < 1)
            {
                if (month == 4 || month == 6 || month == 9 || month == 11)
                {
                    day = 30;
                }

                else if (month == 2)
                {
                    if (leapYear)
                    {
                        day = 29;
                    }
                    else
                    {
                        day = 28;
                    }
                }

                else
                {
                    day = 31;
                }
            }
        }

        if (key.act == key.PLUS)
        {
            day++;

            if (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11))
            {
                day = 1;
            }

            else if (month == 2)
            {
                if ((day > 29 && leapYear) || (day > 28 && !leapYear))
                {
                    day = 1;
                }
            }

            else if (day > 31)
            {
                day = 1;
            }
        }
    }
}

void Watch::leapYearDay()
{
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
    {
        leapYear = true;
    }
    else
    {
        leapYear = false;
    }

    if (day > 29 && month == 2 && leapYear)
    {
        day = 29;
    }

    if (day > 28 && month == 2 && !leapYear)
    {
        day = 28;
    }
}

void Watch::setWatch(Key &key)
{
    if (key.setWatch())
    {
        cursorDateTime = 0;

        DateTime time = now();

        dow = time.dayOfTheWeek();

        if (time.year() < 2021)
        {
            year = 2021;
        }
        else
        {
            year = time.year();
        }

        month = time.month();
        day = time.day();
        hour = time.hour();
        min = time.minute();
        sec = time.second();
    }

    if (key.screen == key.watch)
    {
        dtCursor(key);

        if (cursorDateTime == 0)
        {
            dayChange(day, key);
        }
        else if (cursorDateTime == 1)
        {
            monthChange(month, key);
            leapYearDay();
        }
        else if (cursorDateTime == 2)
        {
            yearChange(year, key);
            leapYearDay();
        }
        else if (cursorDateTime == 3)
        {
            timeChange(hour, key);
        }
        else if (cursorDateTime == 4)
        {
            timeChange(min, key);
        }
        else if (cursorDateTime == 5)
        {
            timeChange(sec, key);
        }
    }

    if (key.setDateTime)
    {
        adjustDate(Date(year, month, day));
        adjustTime(Time(hour, min, sec));

        key.setDateTime = false;
    }
}

void Watch::setInterval(Key &key)
{
    if (key.screen == key.interval)
    {
        if (key.valChange())
        {
            key.act == key.MINUS ? interval-- : interval++;
            if (interval < 0)
                interval = 90;
            if (interval > 90)
                interval = 0;
        }
    }
}

void Watch::commands(Key &key)
{
    setWatch(key);
    dayReduration(key);
    spectrumReDuration(key);
    autoSwitcher(key);
    setInterval(key);
}