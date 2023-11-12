#include <Bright.h>

Bright::Bright()
{
}

Bright::~Bright()
{
}

void Bright::begin(byte startBrightPin)
{
    for (byte i = 0; i < lampAmount; i++)
    {
        this->pin[i] = startBrightPin + i;
        pinMode(pin[i], OUTPUT);
        bright[i] = allMinPWM;
        analogWrite(pin[i], (allMaxPWM - bright[i]));
    }
}

void Bright::setMinBright(byte pin, byte &bright, byte brightRise)
{
    if (bright < brightRise)
    {
        bright = brightRise;
        analogWrite(pin, (allMaxPWM - bright));
    }
}

void Bright::resetBright(byte pin, byte &bright)
{
    bright = allMinPWM;
    analogWrite(pin, (allMaxPWM - bright));
}

void Bright::autoChangeBright(Watch &watch, Key &key, byte i)
{
    if (watch.autoSwitch[i])
    {
        if (!watch.brightDown[i])
        {
            setMinBright(pin[i], bright[i], riseBright[i]);

            if (timer[i].wait(waitTime[i]) && bright[i] < maxBright[i])
            {
                bright[i]++;
                analogWrite(pin[i], (allMaxPWM - bright[i]));
            }
        }

        if (watch.brightDown[i])
        {
            if (timer[i].wait(waitTime[i]) && bright[i] > setBright[i])
            {
                bright[i]--;
                analogWrite(pin[i], (allMaxPWM - bright[i]));
            }

            if (bright[i] == setBright[i])
            {
                resetBright(pin[i], bright[i]);
                watch.autoSwitch[i] = false;
                watch.brightDown[i] = false;
            }
        }
    }

    else if (watch.skip[i] && bright[i] > 0)
    {
        resetBright(pin[i], bright[i]);
    }
}

void Bright::resetAllBrights()
{
    for (byte i = 0; i < lampAmount; i++)
    {
        resetBright(pin[i], bright[i]);
    }
}

void Bright::autoBright(Watch &watch, Key &key)
{
    if (key.screen != key.manual && !key.reSetting)
    {
        for (byte i = 0; i < lampAmount; i++)
        {
            autoChangeBright(watch, key, i);
        }
    }

    if (key.resetManualBright)
    {
        resetAllBrights();
        key.resetManualBright = false;
    }
}

void Bright::changeMaxBright(byte &bright, Key &key, Watch &watch, byte min, byte max)
{
    if (key.valChange(bright, min, max))
    {
        if (watch.autoSwitch[key.id])
        {
            analogWrite(pin[key.id], (allMaxPWM - bright));
        }
    }
}

void Bright::changeBright(Key &key, Watch &watch)
{
    if (key.changeBright())
    {
        switch (key.screen)
        {
        case key.riseBright:

            changeMaxBright(riseBright[key.id], key, watch, allMinPWM, maxBright[key.id]);
            break;

        case key.maxBright:

            changeMaxBright(maxBright[key.id], key, watch, riseBright[key.id], allMaxPWM);
            break;

        case key.setBright:

            changeMaxBright(setBright[key.id], key, watch, allMinPWM, maxBright[key.id]);
            break;

        default:
            break;
        }
    }
}

void Bright::manualChangeBright(Watch &watch, Key &key)
{
    if (key.screen == key.manual)
    {
        if (key.buttonSwitch[key.id])
        {
            if (key.valChange(manualBright[key.id], allMinPWM, allMaxPWM))
            {
                analogWrite(pin[key.id], (allMaxPWM - manualBright[key.id]));
            }
        }

        else
        {
            resetBright(pin[key.id], manualBright[key.id]);
        }
    }
}

boolean Bright::setAllBrigh(Key &key)
{
    if (key.allBrigh())
    {
        if (key.valChange(allBrigh, minAllBright, maxAllBright))
        {
            for (byte i = 0; i < lampAmount; i++)
            {
                maxBright[i] = map(allBrigh, minAllBright, maxAllBright, setBright[i], allMaxPWM);

                analogWrite(pin[i], (allMaxPWM - maxBright[i]));

                //   TO DISPLAY
                // brightDisplay[i] = map(bright[i], setBright[i], maxBright[i], minAllBright, maxAllBright);
            }
        }

        return true;
    }

    return false;
}

boolean Bright::setAllColor(Key &key)
{
    if (key.allColor())
    {
        if (key.valChange(allColor, minAllColor, maxAllColor))
        {
            return true;
        }

        return true;
    }

    return false;
}

void Bright::setRiseSpeed(Key &key)
{
    if (key.screen == key.speed)
    {
        key.valChange(waitTime[key.id], 0, 99, true);
    }
}

void Bright::commands(Watch &watch, Key &key)
{
    autoBright(watch, key);
    changeBright(key, watch);
    manualChangeBright(watch, key);
    setRiseSpeed(key);
}