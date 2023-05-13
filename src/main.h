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

// byte startPinBright = 10; //198 ~ 24 QuantumBoard 0.30 ~ 2.70
// byte startPinLamp = 12;

// Mega Ver.
byte startPinBright = 2; // 198 ~ 24 QuantumBoard 0.30 ~ 2.70
byte startPinLamp = 22;
byte keyPin[] = {32, 34, 36, 38, 40, 42, 44, 46};

// SINGLE VERSION
//  String WavelengthSMD[] = {"3500k+UV+RED+FR"};

// String lightColor[] = {"QB"};

/*!
     CREE Ultraviolet FarRed Red Blue
 */

String WavelengthSMD[lampAmount] = {
    "3000K",
    "730nm",
    "660nm",
    "440nm",
    "365nm+385nm"};

String lightColor[lampAmount] = {"CXB", "FR", "RED", "BL", "UV"};
