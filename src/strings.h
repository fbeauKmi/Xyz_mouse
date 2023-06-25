#ifndef STRINGS_H
#define STRINGS_H

#include <avr/pgmspace.h>

char strbuff[20];

// _____________________ Menu strings _____________________ //

const char menu00[] PROGMEM = "SETTINGS";

const char menu10[] PROGMEM = "Joystick";
const char menu20[] PROGMEM = "Knob";
const char menu30[] PROGMEM = "Buttons";
const char menu40[] PROGMEM = "Leds";

const char menu11[] PROGMEM = "Mode";
const char menu12[] PROGMEM = "Sensibility";

const char menu21[] PROGMEM = "Direction";
const char menu22[] PROGMEM = "Button";

const char menu31[] PROGMEM = "Button 1";
const char menu32[] PROGMEM = "Button 2";
const char menu33[] PROGMEM = "Button 3";

const char menu43[] PROGMEM = "Color 1";
const char menu44[] PROGMEM = "Color 2";
const char menu41[] PROGMEM = "Motion Mode";
const char menu42[] PROGMEM = "Color Mode";
const char menu45[] PROGMEM = "Timeout (sec)";


const byte arMenu[7] PROGMEM = {0x06, 0x01U, 0x14U, 0x52U, 0x72U, 0x93U, 0xC5U} ;
const byte actions[12] PROGMEM = {1,2,3,4,4,4,4,5,5,6,7,8};
const byte EE_address[13] PROGMEM = {11,1,39,36,21,24,28,46,41,54,57,5,2};
const byte arAction[9] PROGMEM = {0x17, 0x09, 0x04, 0x01, 0x2D, 0xFFU, 0x2, 0x3, 0x60 } ;
const byte Keys[] PROGMEM = {0x20,0x80,0x81,0x82,0xB3,0xB1,0x0,0x83,0x80};

const char jm0[] PROGMEM = "Mouse";
const char jm1[] PROGMEM = "Fusion360";
const char jm2[] PROGMEM = "Solid W/E";
const char jm3[] PROGMEM = "Blender";
const char jm4[] PROGMEM = "Adobe";
const char jm5[] PROGMEM = "Freecad (CAD)";
const char jm6[] PROGMEM = "Rhino";
const char jm7[] PROGMEM = "ZBrush";
const char jm8[] PROGMEM = "ANSA";
const char jm9[] PROGMEM = "OnShape";

const char chr0_35[36] PROGMEM = {'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m','1','2','3','4','5','6','7','8','9','0'};
const char chr36[] PROGMEM = "Space";
const char chr37[] PROGMEM = "CTRL";
const char chr38[] PROGMEM = "Shift";
const char chr39[] PROGMEM = "Alt";
const char chr40[] PROGMEM = "Tab";
const char chr41[] PROGMEM = "Esc";
const char chr42[] PROGMEM = "Del";
const char chr43[] PROGMEM = "Scroll";
const char chr44[] PROGMEM = "Solid W/E";
const char chr45[] PROGMEM = "ANSA";

const char normal[] PROGMEM = "L Hand";
const char reversed[] PROGMEM = "R Hand";

const char mmode0[] PROGMEM = "Fixed";
const char mmode1[] PROGMEM = "Running";
const char mmode2[] PROGMEM = "Chase";

const char cmode3[] PROGMEM = "Off";
const char cmode0[] PROGMEM = "Color 1";
const char cmode1[] PROGMEM = "Mixed";
const char cmode2[] PROGMEM = "Rainbow";

const byte defaultVal[] PROGMEM = {0x0,0x2,0x0,0x2D,0x2D,0x2D,0x0,0x7E,0x2,0x2,0x1E};

const char *const menu_str[] PROGMEM = {(const char *) arMenu,
                                        (const char *) actions,
                                        (const char *) EE_address,
                                        (const char *) arAction,
                                        (const char *) Keys,
                                        (const char *) defaultVal,
                                        menu00,
                                        menu10, menu20,menu30,menu40,
                                        menu11,menu12,
                                        menu21,menu22,
                                        menu31,menu32,menu33,
                                        menu43,menu44,menu41,menu42,menu45,
                                        jm0, jm1, jm2, jm3, jm4, jm5, jm6, jm7, jm8, jm9,
                                        chr0_35,chr36,chr37,chr38,chr39,chr40,chr41,chr42,chr43,chr44,chr45,
                                        normal,reversed,
                                        mmode0,mmode1,mmode2,
                                        cmode3,cmode0,cmode1,cmode2
                                        };

const byte jog_actions[] PROGMEM = {0x0,  0x0,  0x0, 0x0, 0x0,
                                    0x81, 0x0,  0x3, 0x4, 0x0,
                                    0x0,  0x0,  0x0, 0x4, 0x0,
                                    0x81, 0x82, 0x0, 0x4, 0x0,
                                    0x20, 0x0,  0x0, 0x1, 0x0,
                                    0x0,  0x0,  0x0, 0x4, 0x2,
                                    0x0,  0x0,  0x0, 0x2, 0x0,
                                    0x0,  0x0,  0x0, 0x1, 0x0,
                                    0x81, 0x0,  0x0, 0x1, 0x0,
                                    0x0,  0x0,  0x0, 0x2, 0x0
                                    };
#endif