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
        Serial.println("ok");
        return true;
    }
    return false;
}

void Key::setSpeed()
{
    if (click(keySpeed))
    {
        autoMove = false;

        if (screen == speed)
        {
            Serial.println("case speed");
            writeSpeed = true;
            screen = lamp;
        }
        else if (screen == interval)
        {
            Serial.println("case interval");
            writeInterval = true;
            screen = lamp;
        }

        else if (screen == lamp)
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

        if (screen == watch)
        {
            Serial.println("case watch");
            setDateTime = true;
            screen = lamp;
        }
        else if (screen == lamp)
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

        if (screen == duration)
        {
            Serial.println("case duration");
            writeTime = true;
            reSetting = false;

            reduration[id] = false;
            screen = lamp;
        }

        else if (screen == lamp)
        {
            screen = duration;
            reSetting = true;

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

        if (screen == maxBright)
        {
            Serial.println("case maxBright");
            writeBright = true;
            // reBright[id] = false;
            // screen = lamp;
        }
        else if (screen == riseBright)
        {
            Serial.println("case riseBright");
            writeBright = true;
            // reBright[id] = false;
            // screen = lamp;
        }
        else if (screen == setBright)
        {
            Serial.println("case setBright");
            writeBright = true;
            // reBright[id] = false;
            // screen = lamp;
        }

        else if (screen == lamp)
        {
            screen = riseBright;
            reBright[id] = true;
            reSetting = true;
        }

        if (writeBright)
        {
            writeMaxBright = true;
            writeSetBright = true;
            writeRiseBright = true;
            reBright[id] = false;
            reSetting = false;

            screen = lamp;
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
    if (screen == lamp && click(keySkip))

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
        if (screen == manual)
        {
            Serial.println("case manual");
            resetManualBright = true;

            for (byte i = 0; i < lampAmount; i++)
                buttonSwitch[i] = 0;
            autoMove = true;
            screen = lamp;
        }

        else if (screen == lamp)
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
            Serial.println("sw");
            if (!buttonSwitch[id])
            {
                buttonSwitch[id] = true;
                Serial.println("ON");
            }

            else
            {
                Serial.println("OFF");
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
    if (click(keySunTime) && nextScreen && screen == sunColor)
    {
        nextScreen = false;
    }

    else if (click(keySunTime) && (screen == lamp || screen == sunColor) && !nextScreen)
    {
        autoMove = false;
        reDay = true;

        screen = sunDuration;
        nextScreen = true;
    }

    if (reDay && escape())
    {
        reDay = false;
        nextScreen = false;
        screen = lamp;
    }

    if (screen == sunDuration && ok())
    {
        Serial.println("case sunDuration");
        writeDay = true;
        correctDay = true;

        reDay = false;
        nextScreen = false;
        screen = lamp;
    }

    if (screen == sunDuration)
    {
        return true;
    }

    return false;
}

boolean Key::allBrigh(byte &val, byte min, byte max)
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
        reDay = false;
        nextScreen = false;
        screen = lamp;
    }

    if (screen == sunBright && ok())
    {
        Serial.println("case sunBright");
        writeAllBright = true;

        reDay = false;
        nextScreen = false;
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
        reDay = false;
        nextScreen = false;
        screen = lamp;
    }

    if (screen == sunColor && ok())
    {
        Serial.println("case sunColor");
        writeAllColor = true;

        reDay = false;
        nextScreen = false;
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

    setSpeed();
}