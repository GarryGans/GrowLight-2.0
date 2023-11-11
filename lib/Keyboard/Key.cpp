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

Key::Screen Key::changeScreen()
{
    if (direction == FORWARD)
    {
        screen = (Screen)(screen + 1);
    }

    else if (direction == BACK)
    {
        screen = (Screen)(Key::screen - 1);
    }

    return screen;
}

void Key::cursor(byte &cursor, byte min, byte max)
{
    if (navigation())
    {
        switch (direction)
        {
        case FORWARD:
            cursor++;

            if (cursor > max)
            {
                cursor = min;
            }

            break;

        case BACK:
            cursor--;

            cursor = constrain(cursor, min, max);

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

void Key::menuScreen(Screen start, Screen end)
{
    if (navigation())
    {
        if (direction == FORWARD)
        {
            if (changeScreen() > end)
            {
                screen = start;
            }
        }

        if (direction == BACK)
        {
            if (changeScreen() < start)
            {
                screen = end;
            }
        }
    }
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
        if (navigation())
        {
            if (direction == BACK)
            {
                id--;
                id = constrain(id, 0, idLast);
            }

            else if (direction == FORWARD)
            {
                id++;

                if (id > idLast)
                {
                    id = idFirst;
                }
            }
        }
    }
}

boolean Key::valChange()
{
    if (clickOrHold())
    {
        if (getNum == keyDown)
        {
            resetCounter = true;

            act = MINUS;

            return true;
        }

        else if (getNum == keyUp)
        {
            resetCounter = true;

            act = PLUS;

            return true;
        }
    }

    resetCounter = false;

    return false;
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

    if (screen == speed || screen == interval)
    {
        menuScreen(speed, interval);

        if (ok())
        {
            writeInterval = true;
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
        menuScreen(riseBright, setBright);

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
    if (click(keySunTime) && nextScreen && screen == sunColor)
    {
        screen = sunDuration;
        nextScreen = true;
    }

    else if (click(keySunTime) && screen == lamp && !nextScreen)
    {
        reDay = true;
        reSetting = true;

        screen = sunDuration;
        nextScreen = true;
    }

    if (reDay && escape())
    {
        resetSunSetting();
    }

    if (screen == sunDuration && ok())
    {
        writeSunSetting();

        resetSunSetting();
    }

    if (screen == sunDuration)
    {
        return true;
    }

    return false;
}

boolean Key::allBrigh()
{
    if (click(keySunTime) && nextScreen && screen == sunDuration)
    {
        nextScreen = false;
    }

    else if (click(keySunTime) && screen == sunDuration && !nextScreen)
    {
        screen = sunBright;
        nextScreen = true;
    }

    if (reDay && escape())
    {
        resetSunSetting();
    }

    if (screen == sunBright && ok())
    {
        writeSunSetting();

        resetSunSetting();
    }

    if (screen == sunBright)
    {
        return true;
    }

    return false;
}

boolean Key::allColor()
{
    if (click(keySunTime) && nextScreen && screen == sunBright)
    {
        nextScreen = false;
    }

    else if (click(keySunTime) && screen == sunBright && !nextScreen)
    {
        screen = sunColor;
        nextScreen = true;
    }

    if (reDay && escape())
    {
        resetSunSetting();
    }

    if (screen == sunColor && ok())
    {
        writeSunSetting();

        resetSunSetting();
    }

    if (screen == sunColor)
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
        if (timer_2.ready(3))
        {
            screen = lamp;
        }
    }

    autoScreenMove();
    manualChangeScreen();

    manualSwitchLight();

    setSpeed();
}