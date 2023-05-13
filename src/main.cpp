#include <main.h>

Key key(keyPin);
Watch watch;
Switchers switchers;
Bright bright;
Memory memory;
Screen screen(WavelengthSMD, lightColor);

void setup()
{
    Serial.begin(9600);

    key.begin(KB4x4, holdDelay, holdSpeed);
    delay(100);

    screen.begin();

    screen.iGorLogo();
    delay(100);

    switchers.begin(startPinLamp);
    delay(100);

    watch.begin();
    // watch.adjust(DateTime(F(__DATE__), F(__TIME__)));
    delay(100);

    bright.begin(startPinBright);
    delay(100);

    memory.begin(watch, bright);
    delay(100);
}

void loop()
{
    key.keyCommands();

    watch.commands(key);

    switchers.switcher(watch, key);

    bright.commands(watch, key);

    memory.writeChanges(watch, bright, key);

    screen.screens(watch, switchers, key, bright);
}
