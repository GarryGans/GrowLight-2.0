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
        read(setBright_addr[id], bright.setBright[id], bright.minManualBright, bright.maxManualBright);
        read(riseBright_addr[id], bright.riseBright[id], bright.setBright[id], bright.maxManualBright);
        read(maxBright_addr[id], bright.maxBright[id], bright.setBright[id], bright.maxManualBright);
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

void Memory::writeBright(Bright &bright, byte id)
{
    EEPROM.put(setBright_addr[id], bright.setBright[id]);
    EEPROM.put(riseBright_addr[id], bright.riseBright[id]);
    EEPROM.put(maxBright_addr[id], bright.maxBright[id]);
}

void Memory::writeEachBright(Bright &bright)
{
    for (byte id = 0; id < lampAmount; id++)
    {
        writeBright(bright, id);
    }
}

void Memory::writeChanges(Watch &watch, Bright &bright, Key &key)
{
    if (key.writeTime)
    {
        writeTime(watch, key.id);

        key.writeTime = false;
    }

    else if (key.writeDay)
    {
        writeEachTime(watch);

        key.writeDay = false;
    }

    else if (key.writeBright)
    {
        writeBright(bright, key.id);

        key.writeBright = false;
    }

    else if (key.writeSkip)
    {
        EEPROM.put(skip_addr[key.id], watch.skip[key.id]);

        key.writeSkip = false;
    }

    else if (key.writeInterval && key.writeSpeed)
    {
        EEPROM.put(interval_addr, watch.interval);

        key.writeInterval = false;

        EEPROM.put(speed_addr, bright.speed);
        key.writeSpeed = false;
    }

    else if (key.writeAllBright)
    {
        EEPROM.put(allBright_addr, bright.allBrigh);
        key.writeAllBright = false;
    }
}

void Memory::begin(Watch &watch, Bright &bright)
{
    readEachBright(bright);
    readEachTime(watch);
    readEachSkip(watch);
    byte max = 255;
    // read(speed_addr, bright.speed, zero, max);
    read(interval_addr, watch.interval, int(zero), int(max));
    read(allBright_addr, bright.allBrigh, zero, bright.maxAllBright);
}