#include <Arduino.h>
#include <Screen.h>
#include <Switchers.h>
#include <Timer.h>
#include <Watch.h>
#include <Key.h>
#include <Memory.h>
#include <Bright.h>

#define holdSpeed 50
#define holdDelay 500

// SINGLE VERSION
// byte startPinLamp = 22;
// byte startPinBright = 2; // 198 ~ 24 QuantumBoard 0.30 ~ 2.70

byte startPinLamp = 2;
byte startPinBright = 22; // 198 ~ 24 QuantumBoard 0.30 ~ 2.70

byte keyPin[] = {32, 34, 36, 38, 40, 42, 44, 46};

// ///SINGLE VERSION

// String WavelengthSMD[] = {"3500k+UV+RED+FR"};

// String lightColor[] = {"QB"};

/*!
     CREE Ultraviolet FarRed Red Blue
 */

String WavelengthSMD[lampAmount] = {
    "730nm",
    "660nm",
    "3000K",
    "440nm",
    "385nm"};

String lightColor[lampAmount] = {"FR", "RED", "CXB", "BL", "UV"};
