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
        static byte a = 5;

        if (timer.ready(a, resetCounter))
        {
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

void Key::keyCommands()
{
    read();

    manualSwitchLight();
    autoScreenMove();
    manualChangeScreen();

    setSpeed();
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
    if (screen == lamp || screen == start)
    {
        static byte a = 5;

        if (autoMove && timer.ready(a, navigation()))
        {
            idChange();
        }

        if (!autoMove && timer.ready(a, navigation()))
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
        if (getNum == 8)
        {
            direction = FORWARD;
            return true;
        }

        else if (getNum == 12)
        {
            direction = BACK;
            return true;
        }
    }

    return false;
}

boolean Key::valChange()
{
    if (onHold() || justPressed())
    {
        if (getNum == 6)
        {
            act = MINUS;

            return true;
        }

        else if (getNum == 15)
        {
            act = PLUS;

            return true;
        }
    }

    return false;
}

template <typename T>
boolean Key::valChange(T &val, T min, T max)
{
    if (onHold() || justPressed())
    {
        if (getNum == 6 && val > min)
        {
            val--;

            return true;
        }

        else if (getNum == 15 && val < max)
        {
            val++;

            return true;
        }
    }

    return false;
}

boolean Key::ok()
{
    if (justPressed() && getNum == 9)
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

        case dayDuration:
            writeDay = true;
            correctDay = true;
            reDay = false;
            break;

        case manual:
            // resetManualPot = true;
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
            return false;

        return true;
    }

    return false;
}

void Key::setSpeed()
{
    if ((justPressed() && getNum == 10) || autoOk(speed) || autoOk(interval))
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
    if ((justPressed() && getNum == 3) || autoOk(watch))
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
    if ((justPressed() && getNum == 14) || autoOk(duration))
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
    if ((justPressed() && getNum == 7) || (autoOk(maxBright)) || (autoOk(riseBright)) || (autoOk(setBright)))
    {
        autoMove = false;

        if (chekSet(maxBright) || chekSet(riseBright) || chekSet(setBright))
        {
            screen = lamp;
        }
        else
        {
            screen = maxBright;
            reBright[id] = true;
        }
    }

    else if (navigation() && (screen == maxBright || screen == riseBright || screen == setBright))
    {
        if (direction == FORWARD)
        {
            if (changeScreen() > setBright)
            {
                screen = maxBright;
            }
        }
        if (direction == BACK)
        {
            if (changeScreen() < maxBright)
            {
                screen = setBright;
            }
        }
    }

    return reBright[id];
}

boolean Key::dayReduration()
{
    if ((justPressed() && getNum == 2) || autoOk(dayDuration))
    {
        autoMove = false;

        if (chekSet(dayDuration))
        {
            screen = lamp;
        }
        else
        {
            screen = dayDuration;
            reDay = true;
        }
    }

    return reDay;
}

boolean Key::setVoltage()
{
    if (justPressed() && getNum == 4)
    {
        screen = voltage;
        return true;
    }

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
    if (isHold() && getNum == 1)
    {
        if (chekSet(manual))
        {
            autoMove = true;
            screen = lamp;
        }

        else
        {
            autoMove = false;

            // resetManualPot = true;
            resetManualBright = true;

            screen = manual;
        }
    }

    if (screen == manual)
    {
        if (justPressed() && getNum == 1)
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

    if (screen == voltage && ok())
    {
        writeBright = true;
    }
}

boolean Key::allBrigh(byte &val, byte min, byte max)
{
    if (screen == lamp && valChange(val, min, max))
    {
        screen = bright;
        resetCounter = true;
    }

    if (autoOk(bright))
    {
        writeAllBright = true;
        screen = lamp;
    }

    if (screen == bright)
    {
        if (valChange(val, min, max))
        {
            resetCounter = true;
        }
        else
        {
            resetCounter = false;
        }

        return true;
    }

    return false;
}
