#include "Screen.h"

Screen::Screen(String WavelengthSMD[], String lightColor[]) 
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

void Screen::emptyDisplay(Key &key)
{
    if (key.screen == key.lamp && key.justPressed())
    {
        efx.firstPage();
        do
        {
            efx.textAlign("READY", EFX::PosX::center, EFX::PosY::center);
        } while (efx.nextPage());
    }
}

void Screen::printDig(byte value)
{
    if (value < 10)
    {
        efx.print("0");
    }

    efx.print(value);
}

void Screen::printTime(byte hh, byte mm)
{
    printDig(hh);

    efx.print(":");

    printDig(mm);
}

void Screen::printWatch(byte hh, byte mm, byte ss)
{
    printDig(hh);

    efx.print(":");

    printDig(mm);

    efx.print(":");

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
        textAlign("Smart Garden", EFX::PosX::center, EFX::PosY::up);
        textAlign("iGor_2019", EFX::PosX::center, EFX::PosY::center);

    } while (nextPage());
}

void Screen::brightInfo(Bright &bright, Key &key)
{
    setHeight(u8g2_font_pressstart2p_8f);

    digAlign(key.screen == key.manual ? bright.manualBright[key.id] : bright.bright[key.id], EFX::PosX::rightHalf, EFX::PosY::downSpace);
}

void Screen::bottomLine(Watch &watch, Key &key, Bright &bright)
{
    if (watch.skip[key.id] && key.screen != key.manual)
    {
        setHeight(u8g2_font_crox4h_tf);

        moveStringDeep("SKIP", EFX::PosX::center, EFX::PosY::downSpace, false, moveSpeedSkip);
    }

    else if (key.screen == key.manual)
    {
        setHeight(u8g2_font_crox4h_tf);

        textAlign("MANUAL", EFX::PosX::leftSpace, EFX::PosY::downSpace);
        brightInfo(bright, key);

        // escapeBar(key.resetCounter, escConter, key.escFrScreen, true, escSpeed);
    }

    else
    {
        setHeight(u8g2_font_courB08_tn);
        setPosition("00:00-00:00", EFX::PosX::leftSpace, EFX::PosY::downSpace);

        printSpecTime(watch, key.id);
        brightInfo(bright, key);
    }
}

void Screen::lampInfo(Watch &watch, Key &key)
{
    setHeight(u8g2_font_courB08_tf);

    char string[12];
    String(WavelengthSMD[key.id]).toCharArray(string, 12);

    // textAlign(string, EFX::PosX::center, EFX::PosY::upHalf);

    moveStringDeep(string, EFX::PosX::center, EFX::PosY::upHalf, deepX_SMD, moveSpeedSMD);

    setHeight(u8g2_font_crox5tb_tf);

    stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

    stringAlign(state[watch.autoSwitch[key.id] || key.buttonSwitch[key.id]], EFX::PosX::rightHalf, EFX::PosY::center);
}

void Screen::headerTime(Watch &watch)
{
    setHeight(u8g2_font_courB08_tn);

    setPosition("00:00:00", EFX::PosX::rightSpace, EFX::PosY::upSpace);

    Time now = watch.time();

    printWatch(now.hour(), now.minute(), now.second());
}

void Screen::headerDate(Watch &watch)
{
    setHeight(u8g2_font_courB08_tf);

    setPosition("00/00/0000", EFX::PosX::leftHalf, EFX::PosY::upSpace);

    Date now = watch.date();

    printDate(now.day(), now.month(), now.year());

    stringAlign(daysOfTheWeek[now.dayOfTheWeek()], EFX::PosX::leftHalf, EFX::PosY::upHalf);
}

void Screen::setScreen(Bright &brigth, Key &key)
{
    if (key.screen == key.setBright)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveStringDeep("Set Set", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            digAlign(brigth.setBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            blinkFrame(brigth.setBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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

            moveStringDeep("Set Rise", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            digAlign(brigth.riseBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            blinkFrame(brigth.riseBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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

            moveStringDeep("Set MaxBright", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            digAlign(bright.maxBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            blinkFrame(bright.maxBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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
            setHeight(u8g2_font_courB08_tn);

            setPosition("00:00:00", EFX::PosX::leftSpace, EFX::PosY::upSpace);

            Time now = watch.time();

            printWatch(now.hour(), now.minute(), now.second());

            setHeight(u8g2_font_ncenB18_tf);

            stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            setHeight(u8g2_font_profont22_tn);

            setPosition("00:00", EFX::PosX::rightHalf, EFX::PosY::upHalf);
            printTime(watch.startHour[key.id], watch.startMinute[key.id]);

            setPosition("00:00", EFX::PosX::rightHalf, EFX::PosY::downHalf);
            printTime(watch.finishHour[key.id], watch.finishMinute[key.id]);

            switch (watch.cursorSpectrum)
            {
            case 0:
                blinkFrame("00:00", 2, EFX::PosX::rightHalf, EFX::PosY::upFrameHalf, key.resetCounter);
                break;

            case 1:
                blinkFrame("00:00", 2, EFX::PosX::rightFrameHalfSide, EFX::PosY::upFrameHalf, key.resetCounter);
                break;

            case 2:
                blinkFrame("00:00", 2, EFX::PosX::rightHalf, EFX::PosY::downFrameHalf, key.resetCounter);
                break;

            case 3:
                blinkFrame("00:00", 2, EFX::PosX::rightFrameHalfSide, EFX::PosY::downFrameHalf, key.resetCounter);
                break;

            default:
                break;
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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

    setPosition("00:00:00", EFX::PosX::center, EFX::PosY::downHalf);

    printWatch(watch.hour, watch.minute, watch.second);

    switch (watch.cursorDateTime)
    {
    case 3:
        blinkFrame("00:00:00", 2, EFX::PosX::center, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    case 4:
        blinkFrame("00:00:00", 2, EFX::PosX::centerFrame, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    case 5:
        blinkFrame("00:00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    default:
        break;
    }
}

void Screen::blinkDate(Key &key, Watch &watch)
{
    setHeight(u8g2_font_pressstart2p_8f);

    setPosition("00/00/0000", EFX::PosX::center, EFX::PosY::upHalf);

    printDate(watch.day, watch.month, watch.year);

    // time_t rawTime;

    // Date now = watch.date();

    // textAlign(daysOfTheWeek[now.dayOfTheWeek()], EFX::PosX::center, EFX::PosY::center);

    switch (watch.cursorDateTime)
    {
    case 0:
        blinkFrame("00/00/0000", 2, EFX::PosX::center, EFX::PosY::upFrameHalf, key.resetCounter);
        break;

    case 1:
        blinkFrame("00/00/0000", 2, EFX::PosX::centerFrame, EFX::PosY::upFrameHalf, key.resetCounter);
        break;

    case 2:
        blinkFrame("00/00/0000", 4, EFX::PosX::rightFrameSide, EFX::PosY::upFrameHalf, key.resetCounter);
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

            // escapeBar(key.);

        } while (nextPage());
    }
}

void Screen::showSunTime(Watch &watch)
{
    setHeight(u8g2_font_9x18_tn);

    setPosition("00:00", EFX::PosX::rightSpace, EFX::PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    setPosition("00:00", EFX::PosX::rightSpace, EFX::PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);
}

void Screen::blinkSunTime(Key &key, Watch &watch)
{
    setHeight(u8g2_font_profont22_tn);

    setPosition("00:00", EFX::PosX::center, EFX::PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    setPosition("00:00", EFX::PosX::center, EFX::PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);

    switch (watch.cursorDay)
    {
    case 0:
        blinkFrame("00:00", 2, EFX::PosX::center, EFX::PosY::centerFrame, key.resetCounter);
        break;

    case 1:
        blinkFrame("00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::centerFrame, key.resetCounter);
        break;

    case 2:
        blinkFrame("00:00", 2, EFX::PosX::center, EFX::PosY::downFrameSpace, key.resetCounter);
        break;

    case 3:
        blinkFrame("00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::downFrameSpace, key.resetCounter);
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

            moveStringDeep("Interval", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            setHeight(u8g2_font_ncenB18_tf);

            digStringAlign(watch.interval, " m", EFX::PosX::center, EFX::PosY::center);

            blinkFrame(watch.interval, EFX::PosX::customFrame, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);
            
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

            moveStringDeep("Sun Speed", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            setHeight(u8g2_font_ncenB18_tf);

            digStringAlign(bright.waitTime, " bp", EFX::PosX::center, EFX::PosY::center);

            blinkFrame(bright.waitTime, EFX::PosX::customFrame, EFX::PosY::centerFrame, key.resetCounter);

            escapeBar(key.awCount, key.click(keySpeed));

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

            moveStringDeep("Set SunTime", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);
            blinkSunTime(key, watch);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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
            // escapeBar(key.resetCounter, escConter, key.escFrScreen, true, escSpeed);

        } while (nextPage());
    }
}

void Screen::sleep(Key &key)
{
    if (key.screen == key.lamp && key.justPressed() && empty)
    {
        empty = false;

        firstPage();
        do
        {
            setHeight(u8g2_font_ncenB18_tf);
            textAlign("READY", EFX::PosX::center, EFX::PosY::center);

        } while (nextPage());

        if (key.ok())
        {
            ready = true;
        }
    }

    else if (key.screen == key.lamp && ready)
    {
        // lampScreen(watch, switchers, key, bright);
    }

    else if (key.screen == key.lamp && !ready)
    {
        empty = true;

        firstPage();
        do
        {

        } while (nextPage());
    }
}

void Screen::allBrightScreen(Bright &bright, Key &key)
{
    if (key.screen == key.sunBright)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveStringDeep("BRIGHT", EFX::PosX::leftSpace, EFX::PosY::upSpace, false, moveSpeedShot);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.allBrigh, EFX::PosX::leftHalf, EFX::PosY::center);

            blinkFrame(bright.allBrigh, EFX::PosX::leftHalf, EFX::PosY::centerFrame);

            for (byte i = 0; i < lampAmount; i++)
            {
                setHeight(u8g2_font_courB08_tf);

                customY(nextY(lampAmount, i));
                strDigAlign(lightColor[i], bright.maxBright[i], EFX::PosX::rightHalf, EFX::PosY::custom);
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (nextPage());
    }
}

void Screen::allColorScreen(Bright &bright, Key &key)
{
    if (key.screen == key.sunColor)
    {
        firstPage();
        do
        {
            setHeight(u8g2_font_pressstart2p_8f);

            moveStringDeep("COLOR", EFX::PosX::leftSpace, EFX::PosY::upSpace, false, moveSpeedShot);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.allColor, EFX::PosX::leftHalf, EFX::PosY::center);

            blinkFrame(bright.allColor, EFX::PosX::leftHalf, EFX::PosY::centerFrame);

            for (byte i = 0; i < lampAmount; i++)
            {
                setHeight(u8g2_font_courB08_tf);

                customY(nextY(lampAmount, i));
                strDigAlign(lightColor[i], bright.maxBright[i], EFX::PosX::rightHalf, EFX::PosY::custom);
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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

            moveStringDeep("PWM", EFX::PosX::leftSpace, EFX::PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(bright.maxBright[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            blinkFrame(bright.maxBright[key.id], EFX::PosX::leftHalf, EFX::PosY::centerFrame, key.resetCounter);

            setHeight(u8g2_font_pressstart2p_8f);

            moveStringDeep("MW", EFX::PosX::rightSpace, EFX::PosY::upSpace);

            setHeight(u8g2_font_ncenB18_tf);

            digAlign(voltage.ampere[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            blinkFrame(voltage.ampere[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

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

    // sleep(key);
}
