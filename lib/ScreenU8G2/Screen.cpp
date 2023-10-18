#include "Screen.h"

Screen::Screen(String WavelengthSMD[], String lightColor[]) : EFX()
{
    for (byte i = 0; i < lampAmount; i++)
    {
        this->WavelengthSMD[i] = WavelengthSMD[i];
        this->lightColor[i] = lightColor[i];
    }
}

Screen::~Screen()
{
}

void Screen::printDig(byte value)
{
    if (value < 10)
    {
        print("0");
    }

    print(value);
}

void Screen::printTime(byte hh, byte mm)
{
    printDig(hh);

    print(":");

    printDig(mm);
}

void Screen::printWatch(byte hh, byte mm, byte ss)
{
    printDig(hh);

    print(":");

    printDig(mm);

    print(":");

    printDig(ss);
}

void Screen::printDate(byte day, byte month, int year)
{
    printDig(day);

    print("/");

    printDig(month);

    print("/");

    print(year);
}

void Screen::printSpecTime(Watch &watch, byte id)
{
    printTime(watch.startHour[id], watch.startMinute[id]);

    print("-");

    printTime(watch.finishHour[id], watch.finishMinute[id]);
}

void Screen::iGorLogo()
{
    firstPage();
    do
    {
        setHeight(u8g2_font_crox4h_tf);
        textAlign("Smart Garden", PosX::center, PosY::up);
        textAlign("iGor_2019", PosX::center, PosY::center);

    } while (nextPage());
}

void Screen::brightInfo(Bright &bright, Key &key)
{
    setHeight(u8g2_font_pressstart2p_8f);

    digAlign(bright.bright[key.id], PosX::rightHalf, PosY::downSpace);
}

void Screen::bottomLine(Watch &watch, Key &key, Bright &bright)
{
    if (watch.skip[key.id] && key.screen != key.manual)
    {
        setHeight(u8g2_font_crox4h_tf);

        moveString("SKIP", PosX::center, PosY::downSpace, 10);
    }

    else if (key.screen == key.manual)
    {
        setHeight(u8g2_font_crox4h_tf);

        textAlign("MANUAL", PosX::leftSpace, PosY::downSpace);
        brightInfo(bright, key);
    }

    else
    {
        setHeight(u8g2_font_courB08_tn);
        setPosition("00:00-00:00", PosX::leftSpace, PosY::downSpace);

        printSpecTime(watch, key.id);
        brightInfo(bright, key);
    }
}

void Screen::lampInfo(Watch &watch, Key &key)
{
    setHeight(u8g2_font_courB08_tf);

    // char string[12];
    // String(WavelengthSMD[key.id]).toCharArray(string, 12);

    moveString(WavelengthSMD[key.id], PosX::center, PosY::upHalf);

    setHeight(u8g2_font_crox5tb_tf);

    stringAlign(lightColor[key.id], PosX::leftHalf, PosY::center);

    stringAlign(state[watch.autoSwitch[key.id] || key.buttonSwitch[key.id]], PosX::rightHalf, PosY::center);
}

void Screen::headerTime(Watch &watch)
{
    setHeight(u8g2_font_courB08_tn);

    setPosition("00:00:00", PosX::rightSpace, PosY::upSpace);

    Time now = watch.time();

    printWatch(now.hour(), now.minute(), now.second());
}

void Screen::headerDate(Watch &watch)
{
    setHeight(u8g2_font_courB08_tf);

    setPosition("00/00/0000", PosX::leftHalf, PosY::upSpace);

    Date now = watch.date();

    printDate(now.day(), now.month(), now.year());

    stringAlign(daysOfTheWeek[now.dayOfTheWeek()], PosX::leftHalf, PosY::upHalf);
}

void Screen::setScreen(Bright &brigth, Key &key)
{
    if (key.screen == key.setBright)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Set Set", PosX::center, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], PosX::leftHalf, PosY::center);

            digAlign(brigth.setBright[key.id], PosX::rightHalf, PosY::center);

            blinkFrame(brigth.setBright[key.id], PosX::rightHalf, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::riseScreen(Bright &brigth, Key &key)
{
    if (key.screen == key.riseBright)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Set Rise", PosX::center, PosY::upSpace, 10);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], PosX::leftHalf, PosY::center);

            digAlign(brigth.riseBright[key.id], PosX::rightHalf, PosY::center);

            blinkFrame(brigth.riseBright[key.id], PosX::rightHalf, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::maxBrightScreen(Bright &bright, Key &key)
{
    if (key.screen == key.maxBright)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Set MaxBright", PosX::center, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], PosX::leftHalf, PosY::center);

            digAlign(bright.maxBright[key.id], PosX::rightHalf, PosY::center);

            blinkFrame(bright.maxBright[key.id], PosX::rightHalf, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::brightScreen(Bright &bright, Key &key)
{
    maxBrightScreen(bright, key);

    riseScreen(bright, key);

    setScreen(bright, key);
}

void Screen::timerScreen(Watch &watch, Key &key)
{
    if (key.screen == key.duration)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], PosX::leftHalf, PosY::center);

            setHeight(u8g2_font_profont22_tn);

            setPosition("00:00", PosX::rightHalf, PosY::upHalf);
            printTime(watch.startHour[key.id], watch.startMinute[key.id]);

            setPosition("00:00", PosX::rightHalf, PosY::downHalf);
            printTime(watch.finishHour[key.id], watch.finishMinute[key.id]);

            switch (watch.cursorSpectrum)
            {
            case 0:
                blinkFrame("00:00", 2, PosX::rightHalf, PosY::upFrameHalf, key.valChange());
                break;

            case 1:
                blinkFrame("00:00", 2, PosX::rightFrameHalfSide, PosY::upFrameHalf, key.valChange());
                break;

            case 2:
                blinkFrame("00:00", 2, PosX::rightHalf, PosY::downFrameHalf, key.valChange());
                break;

            case 3:
                blinkFrame("00:00", 2, PosX::rightFrameHalfSide, PosY::downFrameHalf, key.valChange());
                break;

            default:
                break;
            }

        } while (nextPage());
    }
}

void Screen::lampScreen(Watch &watch, Switchers &switchers, Key &key, Bright &bright)
{
    if (key.screen == key.lamp || key.screen == key.manual)
    {
        firstPage();
        do
        {
            headerTime(watch);
            lampInfo(watch, key);
            bottomLine(watch, key, bright);
        } while (nextPage());
    }
}

void Screen::blinkTime(Key &key, Watch &watch)
{
    setHeight(u8g2_font_pressstart2p_8f);

    setPosition("00:00:00", PosX::center, PosY::downHalf);

    printWatch(watch.hour, watch.min, watch.sec);

    switch (watch.cursorDateTime)
    {
    case 3:
        blinkFrame("00:00:00", 2, PosX::center, PosY::downFrameHalf, key.valChange());
        break;

    case 4:
        blinkFrame("00:00:00", 2, PosX::centerFrame, PosY::downFrameHalf, key.valChange());
        break;

    case 5:
        blinkFrame("00:00:00", 2, PosX::rightFrameSide, PosY::downFrameHalf, key.valChange());
        break;

    default:
        break;
    }
}

void Screen::blinkDate(Key &key, Watch &watch)
{
    setHeight(u8g2_font_pressstart2p_8f);

    setPosition("00/00/0000", PosX::center, PosY::upHalf);

    printDate(watch.day, watch.month, watch.year);

    // time_t rawTime;

    // Date now = watch.date();

    // textAlign(daysOfTheWeek[now.dayOfTheWeek()], PosX::center, PosY::center);

    switch (watch.cursorDateTime)
    {
    case 0:
        blinkFrame("00/00/0000", 2, PosX::center, PosY::upFrameHalf, key.valChange());
        break;

    case 1:
        blinkFrame("00/00/0000", 2, PosX::centerFrame, PosY::upFrameHalf, key.valChange());
        break;

    case 2:
        blinkFrame("00/00/0000", 4, PosX::rightFrameSide, PosY::upFrameHalf, key.valChange());
        break;

    default:
        break;
    }
}

void Screen::setWatchScreen(Watch &watch, Key &key)
{
    if (key.screen == key.watch)
    {
        firstPage();
        do
        {
            blinkDate(key, watch);
            blinkTime(key, watch);

        } while (nextPage());
    }
}

void Screen::showSunTime(Watch &watch)
{
    setHeight(u8g2_font_9x18_tn);

    setPosition("00:00", PosX::rightSpace, PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    setPosition("00:00", PosX::rightSpace, PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);
}

void Screen::blinkSunTime(Key &key, Watch &watch)
{
    setHeight(u8g2_font_profont22_tn);

    setPosition("00:00", PosX::center, PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    setPosition("00:00", PosX::center, PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);

    switch (watch.cursorDay)
    {
    case 0:
        blinkFrame("00:00", 2, PosX::center, PosY::centerFrame, key.valChange());
        break;

    case 1:
        blinkFrame("00:00", 2, PosX::rightFrameSide, PosY::centerFrame, key.valChange());
        break;

    case 2:
        blinkFrame("00:00", 2, PosX::center, PosY::downFrameSpace, key.valChange());
        break;

    case 3:
        blinkFrame("00:00", 2, PosX::rightFrameSide, PosY::downFrameSpace, key.valChange());
        break;

    default:
        break;
    }
}

void Screen::intervalScreen(Watch &watch, Key &key)
{
    if (key.screen == key.interval)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Interval", PosX::center, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digStringAlign(watch.interval, " m", PosX::center, PosY::center);

            blinkFrame(watch.interval, PosX::customFrame, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::riseSpeedScreen(Bright &bright, Key &key)
{
    if (key.screen == key.speed)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Sun Speed", PosX::center, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digStringAlign(bright.speed, " bp", PosX::center, PosY::center);

            blinkFrame(bright.speed, PosX::customFrame, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::sunTimeScreen(Watch &watch, Key &key)
{
    if (key.screen == key.sunDuration)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("Set SunTime", PosX::center, PosY::upSpace);
            blinkSunTime(key, watch);
        } while (nextPage());
    }
}

void Screen::startScreen(Watch &watch, Key &key)
{
    if (key.screen == key.start)
    {
        firstPage();
        do
        {
            headerDate(watch);
            headerTime(watch);

            showSunTime(watch);

        } while (nextPage());

        // static Timer timer;

        static byte a = 5;

        if (timer.ready(a))
        {
            key.screen = key.lamp;
        }
    }
}

void Screen::allBrightScreen(Bright &bright, Key &key)
{
    if (bright.setAllBrigh(key))
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("BRIGHT", PosX::leftSpace, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.allBrigh, PosX::leftHalf, PosY::center);

            blinkFrame(bright.allBrigh, PosX::leftHalf, PosY::centerFrame);

            for (byte i = 0; i < lampAmount; i++)
            {
                setHeight(u8g2_font_courB08_tf);

                customY(nextY(lampAmount, i));
                strDigAlign(lightColor[i], bright.maxBright[i], PosX::rightHalf, PosY::custom);
            }

        } while (nextPage());
    }
}

void Screen::allColorScreen(Bright &bright, Key &key)
{
    if (bright.setAllColor(key))
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("COLOR", PosX::leftSpace, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.allColor, PosX::leftHalf, PosY::center);

            blinkFrame(bright.allColor, PosX::leftHalf, PosY::centerFrame);

            for (byte i = 0; i < lampAmount; i++)
            {
                setHeight(u8g2_font_courB08_tf);

                customY(nextY(lampAmount, i));
                strDigAlign(lightColor[i], bright.maxBright[i], PosX::rightHalf, PosY::custom);
            }

        } while (nextPage());
    }
}

void Screen::voltageScreen(Bright &bright, Voltage &voltage, Key &key)
{
    if (key.screen == key.voltage)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveString("PWM", PosX::leftSpace, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.maxBright[key.id], PosX::leftHalf, PosY::center);

            blinkFrame(bright.maxBright[key.id], PosX::leftHalf, PosY::centerFrame, key.valChange());

            setHeight(u8g2_font_pressstart2p_8f);

            moveString("MW", PosX::rightSpace, PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(voltage.ampere[key.id], PosX::rightHalf, PosY::center);

            blinkFrame(voltage.ampere[key.id], PosX::rightHalf, PosY::centerFrame, key.valChange());

        } while (nextPage());
    }
}

void Screen::screens(Watch &watch, Switchers &switchers, Key &key, Bright &bright)
{
    startScreen(watch, key);

    setWatchScreen(watch, key);

    lampScreen(watch, switchers, key, bright);

    timerScreen(watch, key);
    brightScreen(bright, key);

    riseSpeedScreen(bright, key);
    intervalScreen(watch, key);

    sunTimeScreen(watch, key);
    allBrightScreen(bright, key);
    allColorScreen(bright, key);
}
