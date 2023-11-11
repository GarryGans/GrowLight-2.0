#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory()
{
}

template <typename type>
void Memory::read(int &addr, type &var, type minValue, type maxValue)
{
    addr = startAddr;

    EEPROM.get(addr, var);

    var = constrain(var, minValue, maxValue);

    startAddr = addr + sizeof(var);
}

void Memory::readEachSkip(Watch &watch)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        read(skip_addr[id], watch.skip[id], false, true);
    }
}

void Memory::readEachTime(Watch &watch)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        byte zero = 0,
             min = 59,
             hour = 23;

        read(startHour_addr[id], watch.startHour[id], zero, hour);
        read(startMinute_addr[id], watch.startMinute[id], zero, min);
        read(finishHour_addr[id], watch.finishHour[id], zero, hour);
        read(finishMinute_addr[id], watch.finishMinute[id], zero, min);
    }
}

void Memory::readEachBright(Bright &bright)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        read(setBright_addr[id], bright.setBright[id], bright.allMinPWM, bright.allMaxPWM);
        read(riseBright_addr[id], bright.riseBright[id], bright.setBright[id], bright.allMaxPWM);
        read(maxBright_addr[id], bright.maxBright[id], bright.setBright[id], bright.allMaxPWM);
    }
}

void Memory::writeEachSkip(Watch &watch)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        EEPROM.put(skip_addr[id], watch.skip[id]);
    }
}

void Memory::writeTime(Watch &watch, byte id)
{
    EEPROM.put(startHour_addr[id], watch.startHour[id]);
    EEPROM.put(startMinute_addr[id], watch.startMinute[id]);
    EEPROM.put(finishHour_addr[id], watch.finishHour[id]);
    EEPROM.put(finishMinute_addr[id], watch.finishMinute[id]);
}

void Memory::writeEachTime(Watch &watch)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        writeTime(watch, id);
    }
}

void Memory::writeBright(Bright &bright, Key &key)
{
    if (key.writeSetBright)
    {
        EEPROM.put(setBright_addr[key.id], bright.setBright[key.id]);
        key.writeSetBright = false;
    }
    if (key.writeRiseBright)
    {
        EEPROM.put(riseBright_addr[key.id], bright.riseBright[key.id]);
        key.writeRiseBright = false;
    }
    if (key.writeMaxBright)
    {
        EEPROM.put(maxBright_addr[key.id], bright.maxBright[key.id]);
        key.writeMaxBright = false;
    }
}

void Memory::writeEachBright(Bright &bright, Key &key)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        key.id = id;
        writeBright(bright, key);
    }
}

void Memory::writeChanges(Watch &watch, Bright &bright, Key &key)
{
    if (key.writeTime)
    {
        writeTime(watch, key.id);

        key.writeTime = false;
    }

    if (key.writeDay)
    {
        writeEachTime(watch);

        key.writeDay = false;

        Serial.println("writeDay");
    }

    if (key.writeBright)
    {
        writeBright(bright, key);

        key.writeBright = false;
    }

    if (key.writeSkip)
    {
        EEPROM.put(skip_addr[key.id], watch.skip[key.id]);

        key.writeSkip = false;
    }

    if (key.writeInterval && key.writeSpeed)
    {
        EEPROM.put(interval_addr, watch.interval);

        key.writeInterval = false;

        EEPROM.put(speed_addr, bright.waitTime);

        key.writeSpeed = false;
    }

    if (key.writeAllBright)
    {
        EEPROM.put(allBright_addr, bright.allBrigh);
        key.writeAllBright = false;

        Serial.println("writeAllBright");
    }

    if (key.writeAllColor)
    {
        EEPROM.put(allColor_addr, bright.allColor);
        key.writeAllColor = false;

        Serial.println("writeAllColor");
    }
}

void Memory::begin(Watch &watch, Bright &bright)
{
    readEachBright(bright);
    readEachTime(watch);
    readEachSkip(watch);

    read(speed_addr, bright.waitTime, zero, max);
    read(interval_addr, watch.interval, zero, max);
    read(allBright_addr, bright.allBrigh, byte(zero), bright.maxAllBright);
    read(allColor_addr, bright.allColor, byte(zero), bright.maxAllColor);
}