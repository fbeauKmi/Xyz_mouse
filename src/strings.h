/// Strings.h
// strings and menu structure 
// GNU GPL v3.0 july 2023

#ifndef STRINGS_H
#define STRINGS_H

#include <avr/pgmspace.h>

// Define constants
#define FIRMWARE_NAME "Xyz Mouse v0.1"

#define PANTILT_AM   1
#define KNOBDIR_AM   3
#define COLOR_AM     5
#define MOTIONMOD_AM 6
#define COLORMODE_AM 7
#define TIMEOUT_AM   8

// _____________________ Menu strings _____________________ //

const char menu00[] PROGMEM = "_";

const char menu10[] PROGMEM = "SETTINGS";

const char menu110[] PROGMEM = "Knob dir";
const char menu111[] PROGMEM = "PAN / TILT";
const char menu43[] PROGMEM = "Color 1";
const char menu44[] PROGMEM = "Color 2";
const char menu41[] PROGMEM = "Led Motion";
const char menu42[] PROGMEM = "Led Colors";
const char menu45[] PROGMEM = "Timeout (sec)";

// arMenu 
// first byte : pos of menu00 in PROGMEM array
// other : offpos of menu in array + nb of items
const byte arMenu[] PROGMEM = {0x06, 0x01U, 0x11U, 0x27U} ;
// actions
// Menu action function for each menu items (see Orbion_display::action)
const byte actions[] PROGMEM = {KNOBDIR_AM, PANTILT_AM, COLOR_AM, COLOR_AM, MOTIONMOD_AM, COLORMODE_AM, TIMEOUT_AM};
// Display item pos in array = f(actionMode) from 1st item
const byte itemPos[] PROGMEM = {1,255,3,255,255,5,8,255};
// EE_address
// EEPROM address for each settings (last one is a control byte if )
const byte EE_address[] PROGMEM = {11,39,46,41,54,57,5,2};
// first byte : pos of first item  in PROGMEM array
// other : length of array
const byte arAction[] PROGMEM = {0x0F, 0x01, 0xFFU, 0x01, 0xFFU, 0xFFU, 0x2, 0x3, 0x60 } ;


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
                                        menu110,menu111,menu43,menu44,menu41,menu42,menu45,
                                        jm0, jm1, 
                                        normal,reversed,
                                        mmode0,mmode1,mmode2,
                                        cmode3,cmode0,cmode1,cmode2
                                        };


#endif