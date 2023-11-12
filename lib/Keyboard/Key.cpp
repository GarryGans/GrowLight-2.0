#include "Key.h"

Key::Key(byte pin[]) : AmperkaKB(pin[0], pin[1], pin[2], pin[3], pin[4], pin[5], pin[6], pin[7])
{
}

Key::Key()
{
}

Key::~Key()
{
}

boolean Key::autoOk(Screen screen)
{
    if (this->screen == screen)
    {
        byte a = 5;

        if (timer_0.ready(a, resetCounter))
        {
            screen = lamp;
            return true;
        }
    }

    return false;
}

boolean Key::autoOk(byte count)
{
    awCount = timer_2.reduceCounter(count);

    if (awCount == 0)
    {
        // Serial.print("awCount FIN: ");
        // Serial.println(awCount);

        return true;
    }

    // Serial.print("awCount: ");
    // Serial.println(awCount);

    return false;
}

void Key::cursor(byte &cursor, byte min, byte max)
{
    if (navigation())
    {
        switch (direction)
        {
        case BACK:
            cursor--;

            cursor = constrain(cursor, min, max);

            break;

        case FORWARD:
            cursor++;

            if (cursor > max)
            {
                cursor = min;
            }

            break;

        default:
            break;
        }
    }
}

boolean Key::navigation()
{
    if (clickOrHold())
    {
        if (getNum == keyForward)
        {
            resetCounter = true;

            direction = FORWARD;
            return true;
        }

        else if (getNum == keyBack)
        {
            resetCounter = true;

            direction = BACK;
            return true;
        }
    }

    resetCounter = false;

    return false;
}

Key::Screen Key::menuScreen(Screen start, Screen end)
{
    if (navigation())
    {
        if (direction == BACK)
        {
            screen = (Screen)(screen - 1);

            if (screen < start)
            {
                screen = end;
            }
        }

        if (direction == FORWARD)
        {
            screen = (Screen)(screen + 1);

            if (screen > end)
            {
                screen = start;
            }
        }
    }

    return screen;
}

void Key::checkKeyboard()
{
    // Serial.println(getNum);
}

void Key::idChange()
{
    id++;

    if (id > idLast)
    {
        id = idFirst;
    }
}

void Key::autoScreenMove()
{
    if (screen == lamp)
    {
        byte b = 3;

        if (timer_1.ready(b, resetCounter))
        {
            idChange();
        }
    }
}

void Key::manualChangeScreen()
{
    if (screen == lamp || screen == manual)
    {
        cursor(id, idFirst, idLast);
    }
}

template <typename T>
boolean Key::valChange(T &val, T min, T max, boolean twoDirection)
{
    if (clickOrHold())
    {
        if (getNum == keyDown)
        {
            resetCounter = true;

            if (twoDirection)
            {
                val--;

                if (val < min)
                {
                    val = max;
                }

                val = constrain(val, min, max);
            }

            else if (!twoDirection && val > min)
            {
                val--;
            }

            return true;
        }

        else if (getNum == keyUp)
        {
            resetCounter = true;

            if (twoDirection)
            {
                val++;

                if (val > max)
                {
                    val = min;
                }
            }

            else if (!twoDirection && val < max)
            {
                val++;
            }

            return true;
        }
    }

    resetCounter = false;

    return false;
}

template boolean Key::valChange<byte>(byte &, byte, byte, boolean);

template boolean Key::valChange<int>(int &, int, int, boolean);

boolean Key::clickOrHold()
{
    return (justPressed() || onHold());
}

boolean Key::click(byte key)
{
    return justPressed() && getNum == key;
}

boolean Key::escape()
{
    if (click(keyEscape))
    {
        return true;
    }
    return false;
}

boolean Key::ok()
{
    if (click(keyOk))
    {
        // Serial.println("ok");
        return true;
    }
    return false;
}

void Key::setSpeed()
{
    if (click(keySpeed))
    {
        if (screen == lamp)
        {
            screen = speed;
        }
    }

    if (screen == speed)
    {
        if (ok())
        {
            writeSpeed = true;
            screen = lamp;
        }

        else if (escape())
        {
            screen = lamp;
        }
    }
}

boolean Key::setWatch()
{
    if (click(keyWatch))
    {
        if (screen == lamp)
        {
            screen = watch;
            return true;
        }
    }

    // if ((screen == watch && ok()) || autoOk(watch))
    if (screen == watch && ok())
    {
        setDateTime = true;
        screen = lamp;
    }

    return false;
}

void Key::progressBar()
{
}

boolean Key::spectrumReDuration()
{
    if (click(keyTime))
    {
        if (screen == lamp)
        {
            screen = duration;
            reduration[id] = true;
        }
    }

    if (screen == duration)
    {
        if (escape())
        {
            reduration[id] = false;
            screen = lamp;
        }

        if (ok())
        {
            writeTime = true;
            reduration[id] = false;
            screen = lamp;
        }
    }

    return reduration[id];
}

boolean Key::changeBright()
{
    if (click(keyBright))
    {
        if (screen == lamp)
        {
            screen = riseBright;
            reBright[id] = true;
        }
    }

    if (screen == maxBright || screen == riseBright || screen == setBright)
    {
        screen = menuScreen(riseBright, setBright);

        if (escape())
        {
            reBright[id] = false;
            screen = lamp;
        }

        if (ok())
        {
            writeBright = true;

            writeMaxBright = true;
            writeSetBright = true;
            writeRiseBright = true;

            reBright[id] = false;

            screen = lamp;
        }
    }

    return reBright[id];
}

boolean Key::setVoltage()
{
    // if (justPressed() && getNum == 4)
    // {
    //     screen = voltage;
    //     return true;
    // }

    return false;
}

void Key::skipEnable(boolean &skip)
{
    if (screen == lamp && click(keySkip))
    {
        if (!skip)
        {
            skip = true;
        }

        else
        {
            skip = false;
        }

        writeSkip = true;
    }
}

void Key::manualSwitchLight()
{
    if (click(keyManual))
    {
        if (screen == manual)
        {
            resetManualBright = true;

            for (byte i = 0; i < lampAmount; i++)
            {
                buttonSwitch[i] = 0;
            }

            screen = lamp;
        }

        else if (screen == lamp)
        {
            resetManualBright = true;

            screen = manual;
        }
    }

    if (screen == manual && ok())
    {

        if (!buttonSwitch[id])
        {
            buttonSwitch[id] = true;
        }

        else
        {
            buttonSwitch[id] = false;
        }
    }
}

void Key::resetSunSetting()
{
    reSetting = false;
    reDay = false;
    nextScreen = false;
    screen = lamp;
}

void Key::writeSunSetting()
{
    writeDay = true;
    writeAllBright = true;
    writeAllColor = true;
    correctDay = true;
}

boolean Key::dayReduration()
{
    if (click(keySunTime))
    {
        if (screen == lamp)
        {
            reDay = true;
            reSetting = true;

            screen = sunDuration;
        }
    }

    if (reDay && escape())
    {
        reSetting = false;
        reDay = false;

        screen = lamp;
    }

    if (screen == sunDuration && ok())
    {
        writeDay = true;

        correctDay = true;

        reSetting = false;

        reDay = false;

        screen = lamp;
    }

    if (screen == sunDuration)
    {
        return true;
    }

    return false;
}

boolean Key::allBrigh()
{
    if (click(keySunBright) && screen == lamp)
    {
        screen = sunBright;
    }

    if (screen == sunBright && escape())
    {
        reSetting = false;

        screen = lamp;
    }

    if (screen == sunBright && ok())
    {
        writeAllBright = true;

        correctDay = true;

        reSetting = false;

        screen = lamp;
    }

    if (screen == sunBright)
    {
        return true;
    }

    return false;
}

boolean Key::allColor()
{
    if (click(keySunColor) && screen == lamp)
    {
        screen = sunColor;
    }

    if (screen == sunColor && escape())
    {
        reSetting = false;

        screen = lamp;
    }

    if (screen == sunColor && ok())
    {
        writeAllColor = true;

        correctDay = true;

        reSetting = false;

        screen = lamp;
    }

    if (screen == sunColor)
    {
        return true;
    }

    return false;
}

boolean Key::setInterval()
{
    if (click(keyInterval))
    {
        if (screen == lamp)
        {
            screen = interval;
        }
    }

    if (screen == interval)
    {
        if (ok())
        {
            writeInterval = true;
            screen = lamp;
        }

        else if (escape())
        {
            screen = lamp;
        }
    }

    if (screen == interval)
    {
        return true;
    }

    return false;
}

void Key::keyCommands()
{  
    read();

    if (screen == start)
    {
        resetCounter = true;

        if (autoOk(4))
        {
            screen = lamp;
        }
    }

    // autoScreenMove();
    manualChangeScreen();

    manualSwitchLight();

    setSpeed();
}