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

void Screen::begin()
{
    efx.begin();
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

    efx.print("/");

    printDig(month);

    efx.print("/");

    efx.print(year);
}

void Screen::printSpecTime(Watch &watch, byte id)
{
    printTime(watch.startHour[id], watch.startMinute[id]);

    efx.print("-");

    printTime(watch.finishHour[id], watch.finishMinute[id]);
}

void Screen::iGorLogo()
{
    efx.firstPage();
    do
    {
        efx.setHeight(u8g2_font_crox4h_tf);
        efx.textAlign("Smart Garden", EFX::PosX::center, EFX::PosY::up);
        efx.textAlign("iGor_2019", EFX::PosX::center, EFX::PosY::center);

    } while (efx.nextPage());
}

void Screen::brightInfo(Bright &bright, Key &key)
{
    efx.setHeight(u8g2_font_t0_12b_tf);

    efx.digAlign(bright.bright[key.id], EFX::PosX::rightHalf, EFX::PosY::downSpace);
}

void Screen::timerLine(Watch &watch, Key &key, Bright &bright)
{
    if (watch.skip[key.id] && key.screen != key.manual)
    {
        efx.setHeight(u8g2_font_pixelmordred_tf);

        efx.moveStringPad("SKIP", EFX::PosX::center, EFX::PosY::downSpace, paddingSkip, moveSpeedSkip);
    }

    else if (key.screen == key.manual)
    {
        efx.setHeight(u8g2_font_pressstart2p_8f);

        efx.textAlign("MANUAL", EFX::PosX::center, EFX::PosY::upHalf);

        efx.setHeight(u8g2_font_HelvetiPixelOutline_tr);

        // efx.textAlign("PWM: ", EFX::PosX::center, EFX::PosY::downSpace);

        // efx.digAlign(bright.manualBright[key.id], EFX::PosX::rightHalf, EFX::PosY::downSpace);

        efx.strDigAlign("PWM: ", bright.manualBright[key.id], EFX::PosX::center, EFX::PosY::downSpace);
    }

    else
    {
        efx.setHeight(u8g2_font_pressstart2p_8f);
        efx.setPosition("00:00-00:00", EFX::PosX::center, EFX::PosY::upHalf);

        printSpecTime(watch, key.id);

        brightInfo(bright, key);
    }
}

void Screen::lampInfo(Watch &watch, Key &key)
{
    efx.setHeight(u8g2_font_courB08_tf);

    char string[12];
    String(WavelengthSMD[key.id]).toCharArray(string, 12);

    // efx.textAlign(string, EFX::PosX::center, EFX::PosY::upHalf);

    efx.moveStringPad(string, EFX::PosX::leftHalf, EFX::PosY::upSpace, paddingSMD, moveSpeedSMD);

    efx.setHeight(u8g2_font_crox5tb_tf);

    efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

    efx.stringAlign(state[watch.autoSwitch[key.id] || key.buttonSwitch[key.id]], EFX::PosX::rightHalf, EFX::PosY::center);
}

void Screen::headerTime(Watch &watch)
{
    efx.setHeight(u8g2_font_courB08_tn);

    efx.setPosition("00:00:00", EFX::PosX::rightSpace, EFX::PosY::upSpace);

    Time now = watch.time();

    printWatch(now.hour(), now.minute(), now.second());
}

void Screen::headerDate(Watch &watch)
{
    efx.setHeight(u8g2_font_courB08_tf);

    efx.setPosition("00/00/0000", EFX::PosX::leftHalf, EFX::PosY::upSpace);

    Date now = watch.date();

    printDate(now.day(), now.month(), now.year());

    efx.stringAlign(daysOfTheWeek[now.dayOfTheWeek()], EFX::PosX::leftHalf, EFX::PosY::upHalf);
}

void Screen::setScreen(Bright &brigth, Key &key)
{
    if (key.screen == key.setBright)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Set Set", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.digAlign(brigth.setBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            efx.blinkFrame(brigth.setBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::riseScreen(Bright &brigth, Key &key)
{
    if (key.screen == key.riseBright)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Set Rise", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.digAlign(brigth.riseBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            efx.blinkFrame(brigth.riseBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::maxBrightScreen(Bright &bright, Key &key)
{
    if (key.screen == key.maxBright)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Set MaxBright", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.digAlign(bright.maxBright[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            efx.blinkFrame(bright.maxBright[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
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
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_courB08_tn);

            efx.setPosition("00:00:00", EFX::PosX::leftSpace, EFX::PosY::upSpace);

            Time now = watch.time();

            printWatch(now.hour(), now.minute(), now.second());

            efx.setHeight(u8g2_font_inb16_mf);

            efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.setHeight(u8g2_font_profont22_tn);

            efx.setPosition("00:00", EFX::PosX::rightHalf, EFX::PosY::upHalf);
            printTime(watch.startHour[key.id], watch.startMinute[key.id]);

            efx.setPosition("00:00", EFX::PosX::rightHalf, EFX::PosY::downHalf);
            printTime(watch.finishHour[key.id], watch.finishMinute[key.id]);

            switch (watch.cursorSpectrum)
            {
            case 0:
                efx.blinkFrame("00:00", 2, EFX::PosX::rightHalf, EFX::PosY::upFrameHalf, key.resetCounter);
                break;

            case 1:
                efx.blinkFrame("00:00", 2, EFX::PosX::rightFrameHalfSide, EFX::PosY::upFrameHalf, key.resetCounter);
                break;

            case 2:
                efx.blinkFrame("00:00", 2, EFX::PosX::rightHalf, EFX::PosY::downFrameHalf, key.resetCounter);
                break;

            case 3:
                efx.blinkFrame("00:00", 2, EFX::PosX::rightFrameHalfSide, EFX::PosY::downFrameHalf, key.resetCounter);
                break;

            default:
                break;
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::lampScreen(Watch &watch, Switchers &switchers, Key &key, Bright &bright)
{
    if (key.screen == key.lamp || key.screen == key.manual)
    {
        efx.firstPage();
        do
        {
            headerTime(watch);
            lampInfo(watch, key);
            timerLine(watch, key, bright);
        } while (efx.nextPage());
    }
}

void Screen::blinkTime(Key &key, Watch &watch)
{
    efx.setHeight(u8g2_font_pressstart2p_8f);

    efx.setPosition("00:00:00", EFX::PosX::center, EFX::PosY::downHalf);

    printWatch(watch.hour, watch.minute, watch.second);

    switch (watch.cursorDateTime)
    {
    case 3:
        efx.blinkFrame("00:00:00", 2, EFX::PosX::center, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    case 4:
        efx.blinkFrame("00:00:00", 2, EFX::PosX::centerFrame, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    case 5:
        efx.blinkFrame("00:00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::downFrameHalf, key.resetCounter);
        break;

    default:
        break;
    }
}

void Screen::blinkDate(Key &key, Watch &watch)
{
    efx.setHeight(u8g2_font_pressstart2p_8f);

    efx.setPosition("00/00/0000", EFX::PosX::center, EFX::PosY::upHalf);

    printDate(watch.day, watch.month, watch.year);

    // time_t rawTime;

    // Date now = watch.date();

    // efx.textAlign(daysOfTheWeek[now.dayOfTheWeek()], EFX::PosX::center, EFX::PosY::center);

    switch (watch.cursorDateTime)
    {
    case 0:
        efx.blinkFrame("00/00/0000", 2, EFX::PosX::center, EFX::PosY::upFrameHalf, key.resetCounter);
        break;

    case 1:
        efx.blinkFrame("00/00/0000", 2, EFX::PosX::centerFrame, EFX::PosY::upFrameHalf, key.resetCounter);
        break;

    case 2:
        efx.blinkFrame("00/00/0000", 4, EFX::PosX::rightFrameSide, EFX::PosY::upFrameHalf, key.resetCounter);
        break;

    default:
        break;
    }
}

void Screen::setWatchScreen(Watch &watch, Key &key)
{
    if (key.screen == key.watch)
    {
        efx.firstPage();
        do
        {
            blinkDate(key, watch);
            blinkTime(key, watch);

            // escapeBar(key.);

        } while (efx.nextPage());
    }
}

void Screen::showSunTime(Watch &watch)
{
    efx.setHeight(u8g2_font_9x18_tn);

    efx.setPosition("00:00", EFX::PosX::rightSpace, EFX::PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    efx.setPosition("00:00", EFX::PosX::rightSpace, EFX::PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);
}

void Screen::blinkSunTime(Key &key, Watch &watch)
{
    efx.setHeight(u8g2_font_profont22_tn);

    efx.setPosition("00:00", EFX::PosX::center, EFX::PosY::center);
    printTime(watch.RiseHour, watch.RiseMin);

    efx.setPosition("00:00", EFX::PosX::center, EFX::PosY::downSpace);
    printTime(watch.SetHour, watch.SetMin);

    switch (watch.cursorDay)
    {
    case 0:
        efx.blinkFrame("00:00", 2, EFX::PosX::center, EFX::PosY::centerFrame, key.resetCounter);
        break;

    case 1:
        efx.blinkFrame("00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::centerFrame, key.resetCounter);
        break;

    case 2:
        efx.blinkFrame("00:00", 2, EFX::PosX::center, EFX::PosY::downFrameSpace, key.resetCounter);
        break;

    case 3:
        efx.blinkFrame("00:00", 2, EFX::PosX::rightFrameSide, EFX::PosY::downFrameSpace, key.resetCounter);
        break;

    default:
        break;
    }
}

void Screen::intervalScreen(Watch &watch, Key &key)
{
    if (key.screen == key.interval)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Interval", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.digStringAlign(watch.interval, " m", EFX::PosX::center, EFX::PosY::center);

            efx.blinkFrame(watch.interval, EFX::PosX::customFrame, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::riseSpeedScreen(Bright &bright, Key &key)
{
    if (key.screen == key.speed)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Bright Speed", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);

            efx.setHeight(u8g2_font_crox5tb_tf);

            efx.stringAlign(lightColor[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.digStringAlign(bright.waitTime[key.id], " ms", EFX::PosX::rightHalf, EFX::PosY::center);

            efx.blinkFrame(bright.waitTime[key.id], EFX::PosX::customFrame, EFX::PosY::centerFrame, key.resetCounter);

            efx.escapeBar(key.awCount, key.click(keySpeed) || key.resetCounter);

        } while (efx.nextPage());
    }
}

void Screen::sunTimeScreen(Watch &watch, Key &key)
{
    if (key.screen == key.sunDuration)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("Set SunTime", EFX::PosX::center, EFX::PosY::upSpace, false, moveSpeed);
            blinkSunTime(key, watch);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::startScreen(Watch &watch, Key &key)
{
    if (key.screen == key.start)
    {
        efx.firstPage();
        do
        {
            headerDate(watch);
            headerTime(watch);

            showSunTime(watch);
            // escapeBar(key.resetCounter, escConter, key.escFrScreen, true, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::sleep(Key &key)
{
    if (key.screen == key.lamp && key.justPressed() && empty)
    {
        empty = false;

        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_ncenB18_tf);
            efx.textAlign("READY", EFX::PosX::center, EFX::PosY::center);

        } while (efx.nextPage());

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

        efx.firstPage();
        do
        {

        } while (efx.nextPage());
    }
}

void Screen::allBrightScreen(Bright &bright, Key &key)
{
    if (key.screen == key.sunBright)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("BRIGHT", EFX::PosX::leftHalf, EFX::PosY::upSpace, paddingShot, moveSpeedShot);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.digAlign(bright.allBrigh, EFX::PosX::leftHalf, EFX::PosY::center);

            efx.blinkFrame(bright.allBrigh, EFX::PosX::leftHalf, EFX::PosY::centerFrame, key.resetCounter);

            for (byte i = 0; i < lampAmount; i++)
            {
                efx.setHeight(u8g2_font_courB08_tf);

                efx.customY(efx.nextY(lampAmount, i));
                efx.strDigAlign(lightColor[i], bright.maxBright[i], EFX::PosX::rightHalf, EFX::PosY::custom);
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::allColorScreen(Bright &bright, Key &key)
{
    if (key.screen == key.sunColor)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("COLOR", EFX::PosX::leftHalf, EFX::PosY::upSpace, false, moveSpeedShot);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.digAlign(bright.allColor, EFX::PosX::leftHalf, EFX::PosY::center);

            efx.blinkFrame(bright.allColor, EFX::PosX::leftHalf, EFX::PosY::centerFrame, key.resetCounter);

            for (byte i = 0; i < lampAmount; i++)
            {
                efx.setHeight(u8g2_font_courB08_tf);

                efx.customY(efx.nextY(lampAmount, i));
                efx.strDigAlign(lightColor[i], bright.maxBright[i], EFX::PosX::rightHalf, EFX::PosY::custom);
            }

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
    }
}

void Screen::voltageScreen(Bright &bright, Voltage &voltage, Key &key)
{
    if (key.screen == key.voltage)
    {
        efx.firstPage();
        do
        {
            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("PWM", EFX::PosX::leftHalf, EFX::PosY::upSpace);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.digAlign(bright.maxBright[key.id], EFX::PosX::leftHalf, EFX::PosY::center);

            efx.blinkFrame(bright.maxBright[key.id], EFX::PosX::leftHalf, EFX::PosY::centerFrame, key.resetCounter);

            efx.setHeight(u8g2_font_pressstart2p_8f);

            efx.moveStringPad("MW", EFX::PosX::rightHalf, EFX::PosY::upSpace);

            efx.setHeight(u8g2_font_ncenB18_tf);

            efx.digAlign(voltage.ampere[key.id], EFX::PosX::rightHalf, EFX::PosY::center);

            efx.blinkFrame(voltage.ampere[key.id], EFX::PosX::rightHalf, EFX::PosY::centerFrame, key.resetCounter);

            // escapeBar(key.resetCounter, escConter, key.escFrScreen, false, escSpeed);

        } while (efx.nextPage());
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

    sunTimeScreen(watch, key);
    allBrightScreen(bright, key);
    allColorScreen(bright, key);
    intervalScreen(watch, key);

    // sleep(key);
}
