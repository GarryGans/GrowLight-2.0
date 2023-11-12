#include <Bright.h>

Bright::Bright()
{
}

Bright::~Bright()
{
}

// double Bright::myMap(double x, double in_min, double in_max, double out_min, double out_max) // 2 - 1 - 3 - 255 - 100  
// {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // 1 * - 155 / 2 + 255
// }

byte Bright::myMap(byte x, byte in_min, byte in_max, byte out_min, byte out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

void Bright::correctAllBright()
{
    for (byte i = 0; i < lampAmount; i++)
    {
        if (riseBright[i] < setBright[i])
        {
            riseBright[i] = setBright[i];
        }

        if (maxBright[i] < riseBright[i])
        {
            maxBright[i] = riseBright[i];
        }
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
                maxBright[i] = myMap(allBrigh, minAllBright, maxAllBright, riseBright[i], allMaxPWM);

                analogWrite(pin[i], (allMaxPWM - maxBright[i]));

                //   TO DISPLAY
                // brightDisplay[i] = map(bright[i], riseBright[i], maxBright[i], minAllBright, maxAllBright);
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
            for (byte i = 0; i < 2; i++)
            {
                maxBright[i] = myMap(allColor, minAllColor, maxAllColor, riseBright[i], allMaxPWM);

                analogWrite(pin[i], (allMaxPWM - maxBright[i]));
            }

            // maxBright[2] = map(allColor, minAllColor, maxAllColor, riseBright[2], allMaxPWM);

            analogWrite(pin[2], (allMaxPWM - maxBright[2]));

            for (byte i = 3; i < lampAmount; i++)
            {
                maxBright[i] = myMap(allColor, minAllColor, maxAllColor, allMaxPWM, riseBright[i]);

                analogWrite(pin[i], (allMaxPWM - maxBright[i]));
            }
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

    setAllBrigh(key);
    setAllColor(key);
}