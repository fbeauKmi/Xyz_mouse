#pragma once
#ifndef ORBIONDISP_H
#define ORBIONDISP_H

#include <Arduino.h>
#include "Orbion_neopixel.h"
#include "struct.h" // project structs
#include "bitmaps.h"
#include "strings.h"
#include <Wire.h>

#define _ENTRY_MODE 2
#define _MENU_LENGTH 12
#define _LETTER_SIZE 12

#ifdef SSD1306
#define SH110X_WHITE WHITE
#define SH110X_BLACK BLACK
#include <Adafruit_SSD1306.h>
class Orbion_display : public Adafruit_SSD1306
#else
#include <Adafruit_SH110X.h>

class Orbion_display : public Adafruit_SH1106G
#endif
{
protected:
    int8_t current_menu = 1;
    int8_t current_item = 0;
    bool showSettings = false;
    bool toupdate = true;

    Orbion_Neopixel *_leds;

    char list[4];
    byte items_type[_MENU_LENGTH];
    char itemPos[_MENU_LENGTH];
    char ee_address[_MENU_LENGTH];
    byte max_vals[_MENU_LENGTH];
    byte default_vals[_MENU_LENGTH];
    byte min_vals[_MENU_LENGTH];

    uint8_t str_pos = 0;
    uint8_t actions_startpos;

    uint8_t startpos;
    uint8_t nbitem;

    uint8_t itemtype = 0;
    uint8_t jogxvalue = 0;
    uint8_t jogyvalue = 0;
    uint8_t actionvalue = 0;

    bool pantilt_mode = false;
    bool actionmode = false;

    uint8_t ee_reset = 0x01;

public:
    Orbion_display();
    void init(Orbion_Neopixel *leds);
    void update();
    void refresh();
    void refresh(bool mode);
    void clear();

    void back();
    void exitSettings();
    void startSettings();

    void scroll(int8_t inc);
    void jog(Axes axes);
    void enter();
    void select();

    void action();

    bool settingMode() { return showSettings; };
    void loadConfig();

    void pgmString(uint8_t id);
    size_t printRGB(uint32_t color);

    void drawIcon(uint8_t x, uint8_t y, uint8_t id);

    uint8_t EE_addr(uint8_t id);
    uint8_t EE_read(uint8_t id);
    uint8_t EE_read(uint8_t id, uint8_t offset);
    void EE_write(uint8_t id, byte value);
    void EE_write(uint8_t id, byte value, uint8_t offsetof);
    MenuConfig conf;

private:
    char buf[40];
    void print_center(const char &t);
    void print_center(const String &t);
    void print_center(const String &t, int8_t x, uint8_t w);
    void fillrect_center(const String &t);
    void scrollBar(uint8_t x, uint8_t y, uint8_t l, boolean horizontal, uint8_t min_val, uint8_t max_val, uint8_t val);
    void _jog(int8_t inc, uint8_t *axevalue, uint32_t *timer);
    int16_t inRange(int16_t minval, int16_t maxval, int16_t val);
#ifdef SSD1306
    void setContrast(uint8_t contrast);
#endif
};

#endif
