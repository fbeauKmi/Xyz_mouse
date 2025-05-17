/// Strings.h
// strings and menu structure 
// GNU GPL v3.0 2023-2025

#ifndef STRINGS_H
#define STRINGS_H

#include <avr/pgmspace.h>

// Define constants
#define FIRMWARE_NAME "Xyz Mouse v4.0"

#define PANTILT_AM    0
#define DZ_WIDTH_AM   1
#define KNOBDIR_AM    2
#define SENS_AM       3
#define COLOR_AM      4
#define MOTIONMOD_AM  5
#define COLORMODE_AM  6
#define TIMEOUT_AM    7
#define CONTRAST_AM   8
#define RESET_AM      9
#define ABOUT_AM     10

// _____________________ Menu strings _____________________ //

const char menu10[] PROGMEM = "SETTINGS    ";

const char menu110[] PROGMEM = "Knob dir.";
const char menu111[] PROGMEM = "Rot / Pan";
const char menu112[] PROGMEM = "DZ width";
const char menu113[] PROGMEM = "Sensitivity";

const char menu43[] PROGMEM = "1st Color";
const char menu44[] PROGMEM = "2nd Color";
const char menu41[] PROGMEM = "Led Motion";
const char menu42[] PROGMEM = "Color Mode";
const char menu46[] PROGMEM = "OLED Contrast";
const char menu45[] PROGMEM = "Timeout (sec)";
const char menu47[] PROGMEM = "Restore def.";


// arMenu 
// first byte : pos of menu10 string in PROGMEM array
// other : offpos of menu strings in array + last item id
const byte arMenu[] PROGMEM = {0x07, 0x01U, 0x0AU} ;
// actions
// Types for each menu items (see Orbion_display::action())
const byte itemsType[] PROGMEM = {KNOBDIR_AM, PANTILT_AM, DZ_WIDTH_AM, SENS_AM, COLOR_AM, COLOR_AM, MOTIONMOD_AM, COLORMODE_AM, CONTRAST_AM, TIMEOUT_AM, RESET_AM, ABOUT_AM} ;
// Display item pos in array = f(itemTypes) from 1st item
const byte itemPos[] PROGMEM = {1,255,4,255,255,6,9,255,255,13};

// min/max value for item types
const byte minVals[] PROGMEM = {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
const byte maxVals[] PROGMEM = {0x02, 0x13, 0x01, 0x06, 0xFFU, 0x02, 0x03, 0x3B, 0x04, 0x01, 0x00} ;

const char jm0[] PROGMEM = "Hold to Pan";
const char jm1[] PROGMEM = "Hold to rot";
const char jm2[] PROGMEM = "Click to change";

const char normal[] PROGMEM = "Left Hand";
const char reversed[] PROGMEM = "Right Hand";

const char mmode0[] PROGMEM = "Fixed";
const char mmode1[] PROGMEM = "Running";
const char mmode2[] PROGMEM = "Chase";

const char cmode3[] PROGMEM = "Off";
const char cmode0[] PROGMEM = "1st Color";
const char cmode1[] PROGMEM = "Mixed";
const char cmode2[] PROGMEM = "Rainbow";

const char reset1[] PROGMEM = "No";
const char reset2[] PROGMEM = "Yes";

// ________________ EEPROM settings ______________________ //
// EEPROM address for each settings (first one is a control byte for version change)
const byte EE_address[] PROGMEM = {1,10,38,5,7,45,40,53,56,12,3};

// ________________ Default EE values ______________________ //
// - ctrl byte: 0x01 ,
// - knobdir: 0x0 = Left Hand , 0x1 = Right Hand ,
// - Pan/Rot: 0x1 = Hold to Pan , 0x2 = Hold to Rot , 0x2 = Click to invert ,
// - DZ width: 0x4 ,
// - Sensitivity: 0x13,
// - Color 1: 0x2D ,
// - Color 2: 0x7E ,
// - Led Motion: 0x0 = Fixed , 0x1 = Running , 0x2 = Chase ,
// - Led Colors: 0x0 = Off , 0x1 = Color 1 , 0x2 = Mixed , 0x3 = Rainbow ,
// - OLED Contrast: 0x0 ,
// - Timeout: 0x1D = 30s.

const byte defaultVal[] PROGMEM = { 0x01, 0x00, 0x02, 0x04, 0x13, 0x2D, 0x7E, 0x02, 0x02, 0x00, 0x1D};


const char *const datas[] PROGMEM = {(const char *) arMenu,
                                        (const char *) itemsType,
                                        (const char *) itemPos,
                                        (const char *) EE_address,
                                        (const char *) maxVals,
                                        (const char *) defaultVal,
                                        (const char *) minVals,

                                        menu10,
                                        menu110,menu111,menu112,menu113,menu43,
                                        menu44,menu41,menu42,menu46,menu45,menu47,

                                        jm0, jm1, jm2,
                                        normal,reversed,
                                        mmode0,mmode1,mmode2,
                                        cmode3,cmode0,cmode1,cmode2,
                                        reset1,reset2
                                        };


#endif