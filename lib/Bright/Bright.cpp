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
        bright[i] = minManualBright;
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
    bright = minManualBright;
    analogWrite(pin, (allMaxPWM - bright));
}

void Bright::autoChangeBright(Watch &watch, Key &key, byte i)
{
    if (watch.autoSwitch[i])
    {
        if (!watch.brightDown[i])
        {
            setMinBright(pin[i], bright[i], riseBright[i]);

            if (timer[0].wait(speed) && bright[i] < maxBright[i])
            {
                bright[i]++;
                analogWrite(pin[i], (allMaxPWM - bright[i]));
            }
        }

        if (watch.brightDown[i])
        {
            if (timer[1].wait(speed) && bright[i] > setBright[i])
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
    else if (watch.skip[i] && key.screen != key.manual)
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
    if (key.screen != key.manual || key.reSetting)
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

void Bright::changeMaxBright(byte &bright, byte pin, Key &key, Watch &watch, byte min, byte max)
{
    if (key.valChange(bright, min, max))
    {
        if (!watch.brightDown[key.id] && watch.autoSwitch[key.id])
        {
            this->bright[key.id] = bright;
            analogWrite(pin, (allMaxPWM - this->bright[key.id]));
        }

        else if (key.buttonSwitch[key.id])
        {
            analogWrite(pin, (allMaxPWM - bright));
        }
    }
}

void Bright::manualChangeBright(Watch &watch, Key &key)
{
    if (key.screen == key.manual || key.screen == key.voltage)
    {
        if (key.buttonSwitch[key.id])
        {
            // key.screen = key.voltage;

            changeMaxBright(bright[key.id], pin[key.id], key, watch, minManualBright, maxManualBright);
            maxBright[key.id] = bright[key.id];
        }

        else
        {
            key.screen = key.manual;
            resetBright(pin[key.id], bright[key.id]);
        }
    }
}

void Bright::setRiseSpeed(Key &key)
{
    if (key.screen == key.speed)
    {
        if (key.valChange())
        {
            key.act == key.MINUS ? speed-- : speed++;

            if (speed < 0)
                speed = 255;

            if (speed > 255)
                speed = 0;
        }
    }
}

void Bright::setSetBright(byte &bright, Watch &watch, Key &key, byte min, byte max)
{
    if (key.valChange(bright, min, max))
    {
        if (!watch.brightDown[key.id] && watch.autoSwitch[key.id])
        {
            this->bright[key.id] = bright;
            analogWrite(pin[key.id], (allMaxPWM - this->bright[key.id]));
        }

        else if (key.buttonSwitch[key.id])
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
        case key.maxBright:

            changeMaxBright(maxBright[key.id], pin[key.id], key, watch, riseBright[key.id], maxManualBright);
            break;

        case key.riseBright:

            setSetBright(riseBright[key.id], watch, key, minManualBright, maxBright[key.id]);
            break;

        case key.setBright:

            setSetBright(setBright[key.id], watch, key, minManualBright, maxBright[key.id]);
            break;

        default:
            break;
        }
    }
}

byte Bright::mapBright(byte allBrigh, byte setBright, byte maxBright, byte minAllBright, byte maxAllBright)
{
    byte bright;

    bright = ((maxBright - setBright) / maxAllBright) * allBrigh;

    return bright;
}

boolean Bright::setAllBrigh(Key &key)
{
    if (key.allBrigh())
    {
        if (key.valChange(allBrigh, minAllBright, maxAllBright))
        {
            for (byte i = 0; i < lampAmount; i++)
            {
                maxBright[i] = mapBright(allBrigh, setBright[i], allMaxPWM, minAllBright, maxAllBright);
                
                //   TO DISPLgarri

                // brightDisplay[i] = map(bright[i], setBright[i], maxBright[i], minAllBright, maxAllBright);

                // analogWrite(pin[i], (allMaxPWM - bright[i]));
            }

            return true;
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

void Bright::commands(Watch &watch, Key &key)
{
    autoBright(watch, key);
    manualChangeBright(watch, key);
    changeBright(key, watch);
    setRiseSpeed(key);
}