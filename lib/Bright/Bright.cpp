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
        analogWrite(pin[i], (maxPWM - bright[i]));
    }
}

void Bright::brightLevelCount()
{
    for (byte i = 0; i < lampAmount; i++)
    {
        maxLevel[i] = maxBright[i] - setBright[i];
    }
}

void Bright::setMinBright(byte pin, byte &bright, byte brightRise)
{
    if (bright < brightRise)
    {
        bright = brightRise;
        analogWrite(pin, (maxPWM - bright));
    }
}

void Bright::resetBright(byte pin, byte &bright)
{
    bright = minManualBright;
    analogWrite(pin, (maxPWM - bright));
}

void Bright::autoChangeBright(Watch &watch, Key &key, byte i)
{
    if (watch.autoSwitch[i] && !key.reduration[i] && !key.reBright[i])
    {
        if (!watch.brightDown[i])
        {
            setMinBright(pin[i], bright[i], riseBright[i]);

            if (timer[0].wait(speed[i] * 10) && bright[i] < maxBright[i])
            {
                bright[i]++;
                analogWrite(pin[i], (maxPWM - bright[i]));
            }
        }

        if (watch.brightDown[i])
        {
            if (timer[1].wait(speed[i] * 10) && bright[i] > setBright[i])
            {
                bright[i]--;
                analogWrite(pin[i], (maxPWM - bright[i]));
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
    if (key.screen != key.manual || key.screen != key.maxBright)
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
    if (key.valChange())
    {
        if (key.act == key.MINUS && bright > min)
        {
            bright--;
        }

        else if (key.act == key.PLUS && bright < max)
        {
            bright++;
        }

        if (!watch.brightDown[key.id] && watch.autoSwitch[key.id])
        {
            this->bright[key.id] = bright;
            analogWrite(pin, (maxPWM - this->bright[key.id]));
        }

        else if (key.buttonSwitch[key.id])
        {
            analogWrite(pin, (maxPWM - bright));
        }
    }
}

void Bright::manualChangeBright(Watch &watch, Key &key)
{
    if (key.screen == key.manual || key.screen == key.voltage)
    {
        if (key.buttonSwitch[key.id])
        {
            key.screen = key.voltage;

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
            key.act == key.MINUS ? speed[key.id]-- : speed[key.id]++;

            if (speed[key.id] < 0)
                speed[key.id] = 255;

            if (speed[key.id] > 255)
                speed[key.id] = 0;
        }
    }
}

void Bright::setSetBright(byte &bright, Key &key, byte min, byte max)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS && bright > min)
        {
            bright--;
        }

        else if (key.act == key.PLUS && bright < max)
        {
            bright++;
        }

        bright = constrain(bright, min, max);
    }
}

void Bright::setRiseBright(byte &brightRise, Key &key, byte min, byte max)
{
    if (key.valChange())
    {
        if (key.act == key.MINUS && brightRise > min)
        {
            brightRise--;
        }

        else if (key.act == key.PLUS && brightRise < max)
        {
            brightRise++;
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

            setRiseBright(riseBright[key.id], key, minManualBright, maxBright[key.id]);
            break;

        case key.setBright:

            setSetBright(setBright[key.id], key, minManualBright, maxBright[key.id]);
            break;

        default:
            break;
        }
    }
}

boolean Bright::setAllBrigh(Key &key)
{
    return key.allBrigh(allBrigh, 0, 99);
}

void Bright::commands(Watch &watch, Key &key)
{
    autoBright(watch, key);
    manualChangeBright(watch, key);
    changeBright(key, watch);
    setRiseSpeed(key);
}