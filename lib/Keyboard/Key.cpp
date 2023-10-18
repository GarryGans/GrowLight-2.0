#include "Key.h"

Timer timer[3];

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
        static byte a = 5;

        if (timer[0].ready(a, resetCounter))
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
    Serial.println(getNum);
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
        static byte b = 3;

        if (autoMove && timer[1].ready(b, navigation()))
        {
            idChange();
        }

        if (!autoMove && timer[2].ready(b, navigation()))
        {
            autoMove = true;
        }
    }
}

void Key::manualChangeScreen()
{
    if (screen == lamp || screen == manual)
    {
        if (navigation())
        {
            autoMove = false;

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

boolean Key::navigation()
{
    if (onHold() || justPressed())
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

boolean Key::valChange()
{
    if (onHold() || justPressed())
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
boolean Key::valChange(T &val, T min, T max)
{
    if (clickOrHold())
    {
        if (getNum == keyDown && val > min)
        {
            resetCounter = true;

            val--;

            return true;
        }

        else if (getNum == keyUp && val < max)
        {
            resetCounter = true;

            val++;

            return true;
        }
    }

    resetCounter = false;

    return false;
}

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
        return true;
    }
    return false;
}

boolean Key::chekSet(Screen screen)
{
    if (this->screen != lamp)
    {
        switch (this->screen)
        {
        case watch:
            setDateTime = true;
            break;

        case duration:
            writeTime = true;
            reduration[id] = false;
            break;

        case maxBright:
            writeBright = true;
            reBright[id] = false;
            break;

        case riseBright:
            writeBright = true;
            reBright[id] = false;
            break;

        case setBright:
            writeBright = true;
            reBright[id] = false;
            break;

        case sunDuration:
            writeDay = true;
            correctDay = true;
            reDay = false;
            break;

        case sunBright:
            break;

        case sunColor:
            break;

        case manual:
            resetManualBright = true;

            for (byte i = 0; i < lampAmount; i++)
                buttonSwitch[i] = 0;
            break;

        case interval:
            writeSpeed = true;
            writeInterval = true;
            break;

        case speed:
            writeSpeed = true;
            writeInterval = true;
            break;

        default:
            break;
        }

        if (this->screen != screen)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void Key::setSpeed()
{
    if (click(keySpeed))
    {
        autoMove = false;

        if (chekSet(speed) || chekSet(interval))
        {
            screen = lamp;
        }

        else
        {
            screen = speed;
        }
    }

    if (screen == speed || screen == interval)
    {
        menuScreen(speed, interval);
    }
}

boolean Key::setWatch()
{
    if (click(keyWatch))
    {
        autoMove = false;

        if (chekSet(watch))
        {
            screen = lamp;
        }
        else
        {
            autoMove = false;
            screen = watch;
            return true;
        }
    }

    return false;
}

boolean Key::spectrumReDuration()
{
    if (click(keyTime))

    {
        autoMove = false;

        if (chekSet(duration))
        {
            screen = lamp;
        }

        else
        {
            screen = duration;
            reduration[id] = true;
        }
    }

    return reduration[id];
}

boolean Key::changeBright()
{
    if (click(keyBright))

    {
        autoMove = false;

        if (chekSet(riseBright) || chekSet(maxBright) || chekSet(setBright))
        {
            screen = lamp;
        }
        else
        {
            screen = riseBright;
            reBright[id] = true;
        }
    }

    else if (screen == maxBright || screen == riseBright || screen == setBright)
    {
        menuScreen(riseBright, setBright);
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
    if (screen == lamp && ok())

    {
        autoMove = false;

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
        if (chekSet(manual))
        {
            autoMove = true;
            screen = lamp;
        }

        else
        {
            autoMove = false;

            resetManualBright = true;

            screen = manual;
        }
    }

    if (screen == manual)
    {
        if (ok())
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

    // if (screen == voltage && ok())
    // {
    //     writeBright = true;
    // }
}

boolean Key::dayReduration()
{
    if (click(keySunTime) && screen == lamp)
    {
        autoMove = false;
        screen = sunDuration;
        reDay = true;
        nextScreen = true;
    }

    else if (reDay && escape())
    {
        reDay = false;
        screen = lamp;
    }

    if (chekSet(sunDuration) && ok())
    {
        screen = lamp;
    }

    return reDay;
}

boolean Key::allBrigh(byte &val, byte min, byte max)
{
    if (click(keySunTime)  && screen == sunDuration)
    {
        screen = sunBright;
    }

    if (ok())
    {
        writeAllBright = true;
    }

    if (chekSet(sunBright) && ok())
    {
        screen = lamp;
    }

    if (screen == sunBright)
    {
        return true;
    }

    return false;
}

boolean Key::allColor(byte &val, byte min, byte max)
{
    if (screen == sunBright  && click(keySunTime))
    {
        screen = sunColor;
    }

    if (ok())
    {
        writeAllColor = true;
    }

    if (chekSet(sunColor) && ok())
    {
        screen = lamp;
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

    autoScreenMove();
    manualChangeScreen();

    manualSwitchLight();

    dayReduration();

    setSpeed();
}