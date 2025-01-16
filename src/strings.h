/// Strings.h
// strings and menu structure 
// GNU GPL v3.0 july 2023

#ifndef STRINGS_H
#define STRINGS_H

#include <avr/pgmspace.h>

// Define constants
#define FIRMWARE_NAME "Xyz Mouse v0.3.1"

#define PANTILT_AM    1
#define DZ_WIDTH_AM   2
#define KNOBDIR_AM    3
#define SENS_AM       4
#define COLOR_AM      5
#define MOTIONMOD_AM  6
#define COLORMODE_AM  7
#define TIMEOUT_AM    8
#define CONTRAST_AM   9

// _____________________ Menu strings _____________________ //

const char menu00[] PROGMEM = "_";

const char menu10[] PROGMEM = "SETTINGS";

const char menu110[] PROGMEM = "Knob dir";
const char menu111[] PROGMEM = "PAN / TILT";
const char menu112[] PROGMEM = "DZ width";
const char menu113[] PROGMEM = "Sensitivity";

const char menu43[] PROGMEM = "Color 1";
const char menu44[] PROGMEM = "Color 2";
const char menu41[] PROGMEM = "Led Motion";
const char menu42[] PROGMEM = "Led Colors";
const char menu46[] PROGMEM = "OLED Contrast";
const char menu45[] PROGMEM = "Timeout (sec)";

// arMenu 
// first byte : pos of menu00 in PROGMEM array
// other : offpos of menu in array + nb of items
const byte arMenu[] PROGMEM = {0x06, 0x01U, 0x11U, 0x2AU} ;
// actions
// Menu action function for each menu items (see Orbion_display::action)
const byte actions[] PROGMEM = {KNOBDIR_AM, PANTILT_AM, DZ_WIDTH_AM, SENS_AM, COLOR_AM, COLOR_AM, MOTIONMOD_AM, COLORMODE_AM, CONTRAST_AM, TIMEOUT_AM};
// Display item pos in array = f(actionMode) from 1st item
const byte itemPos[] PROGMEM = {1,255,3,255,255,5,8,255,255};
// EE_address
// EEPROM address for each settings (first one is a control byte for version change)
const byte EE_address[] PROGMEM = {2,11,39,6,8,46,41,54,57,13,4};
// first byte : pos of first item  in PROGMEM array
// other : length of array
const byte arAction[] PROGMEM = {0x12, 0x01, 0x13, 0x01, 0x04, 0xFFU, 0x2, 0x3, 0x3C, 0x04} ;


const char jm0[] PROGMEM = "Normal";
const char jm1[] PROGMEM = "Inverted";

const char normal[] PROGMEM = "L Hand";
const char reversed[] PROGMEM = "R Hand";

const char mmode0[] PROGMEM = "Fixed";
const char mmode1[] PROGMEM = "Running";
const char mmode2[] PROGMEM = "Chase";

const char cmode3[] PROGMEM = "Off";
const char cmode0[] PROGMEM = "Color 1";
const char cmode1[] PROGMEM = "Mixed";
const char cmode2[] PROGMEM = "Rainbow";

const byte defaultVal[] PROGMEM = {0x0,0x0,0x2D,0x7E,0x2,0x2,0x1E};


const char *const datas[] PROGMEM = {(const char *) arMenu,
                                        (const char *) actions,
                                        (const char *) itemPos,
                                        (const char *) EE_address,
                                        (const char *) arAction,
                                        (const char *) defaultVal,
                                        menu00,
                                        menu10,
                                        menu110,menu111,menu112,menu113,menu43,menu44,menu41,menu42,menu46,menu45,
                                        jm0, jm1, 
                                        normal,reversed,
                                        mmode0,mmode1,mmode2,
                                        cmode3,cmode0,cmode1,cmode2
                                        };


#endif