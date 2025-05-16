#include "orbion_display.h"
#include <EEPROM.h>

#ifdef SSD1306
Orbion_display::Orbion_display() : Adafruit_SSD1306(128, 64, &Wire, 4)
#else
Orbion_display::Orbion_display() : Adafruit_SH1106G(128, 64, &Wire, 4)
#endif
{
    strcpy_P(list, (char *)pgm_read_word(&datas[0]));
    memcpy_P(items_type, pgm_read_word(&datas[1]), _MENU_LENGTH);
    strcpy_P(itemPos, (char *)pgm_read_word(&datas[2]));
    strcpy_P(ee_address, (char *)pgm_read_word(&datas[3]));
    memcpy_P(max_vals, pgm_read_word(&datas[4]), _MENU_LENGTH);
    memcpy_P(default_vals, pgm_read_word(&datas[5]), _MENU_LENGTH);
    memcpy_P(min_vals, pgm_read_word(&datas[6]), _MENU_LENGTH);
    ee_reset = default_vals[0];
    actions_startpos = list[0] + (list[1] & 0x0F) + (list[2] & 0x0F);
};

void Orbion_display::init()
{
#ifdef SSD1306
    begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else
    begin(0x3C, true); // Address 0x3C default
#endif
    setRotation(2); // define screen orientation
    clear();        // Clear screen at startup
};

#ifdef SSD1306
void Orbion_display::setContrast(uint8_t contrast)
{
}
#endif

void Orbion_display::update()
{
    static uint32_t timer = 0;

    if (isTimeout(&timer, 150))
    {

        if (toupdate)
        {
            toupdate ^= true;

            clearDisplay();
            setContrast(conf.contrast << 5);
            setTextColor(SH110X_WHITE);
            setTextSize(1);

            if (showSettings)
            {

                startpos = list[0] + 1;
                nbitem = (list[2] & 0x0F);

                drawLine(15, 19, width() - 15, 19, SH110X_WHITE),
                    setCursor(0, 9);
                pgmString(list[0]);
                print_center(buf);

                if (!actionmode)
                {
                    select();
                }
                else
                {
                    action();
                }
            }
            else
            {
                // Logo
                drawBitmap(44, 6, logo, logo_BMPWIDTH, logo_BMPHEIGHT, SH110X_WHITE);
                // mode icon
                drawIcon(10, 10, pantilt_mode);
                setCursor(12, 45);
                print_center(FIRMWARE_NAME);
            }
            display();
        }
    }
}

/// @brief Allow display to be refresh()
void Orbion_display::refresh()
{
    toupdate = true;
    return;
}

/// @brief Allow display to be refresh() with a mode
void Orbion_display::refresh(bool mode)
{
    if (mode != pantilt_mode)
    {
        pantilt_mode = mode;
        toupdate = true;
    }
    return;
}

/// @brief Clear display and update
void Orbion_display::clear()
{
    clearDisplay();
    display();
}

/// @brief Exit Settings Mode
void Orbion_display::exitSettings()
{
    showSettings = false;
}

/// @brief Enter Settings Mode
void Orbion_display::startSettings()
{
    showSettings = true;
    toupdate = true;
    current_item = 0;
    actionmode = false;
}

/// @brief
void Orbion_display::select()
{
    uint8_t _textcolor;
    static uint8_t startm = 0;

    // Only 3 item can be displayed  // Choose first item to display  // scroll trick
    if (current_item > 2 + startm)
    {
        startm = current_item - 2;
    }
    else if (current_item < startm)
    {
        startm = current_item;
    }

    // display menu items
    for (uint8_t a = 0; a < 3; a++)
    {
        setCursor(31, 24 + a * _LETTER_SIZE);
        pgmString(startpos + a + startm);
        // Hightlight current item
        if (current_item == a + startm)
        {
            fillrect_center(buf);
            _textcolor = SH110X_BLACK;
        }
        else
        {
            _textcolor = SH110X_WHITE;
        }
        setTextColor(_textcolor);
        print_center(buf);
    }
    scrollBar(width() - _LETTER_SIZE, 23, 31, false, 0, nbitem, current_item);
}

void Orbion_display::action()
{
    if (actionmode)
    {
        setCursor(0, 23);

        pgmString(startpos + current_item);
        print_center(buf);
        setCursor(60, 38);

        switch (itemtype)
        {
        case PANTILT_AM:
        case KNOBDIR_AM:
        case MOTIONMOD_AM:
        case COLORMODE_AM:
        case RESET_AM:
            pgmString(actions_startpos + str_pos + actionvalue);
            fillrect_center(buf);
            setTextColor(SH110X_BLACK);
            print_center(buf);
            break;
        case COLOR_AM:
        {
            uint32_t c = _leds->Color(actionvalue, jogxvalue, jogyvalue);
            setCursor(20, 35);

            print("HSV ");

            print(actionvalue);
            write(44);
            print(jogxvalue);
            write(44);
            print(jogyvalue);
            setCursor(20, 47);
            print("RGB #");
            print(_leds->getColor(), HEX);

            _leds->display(0, 1, c, 0);
        }
        break;
        case CONTRAST_AM:
            setContrast((actionvalue + 1) << 5);
        default:
            setTextColor(SH110X_WHITE);
            print(actionvalue + 1);
            break;
        }
        if (itemtype != COLOR_AM)
        {
            scrollBar(26, 52, width() - 52, true, min_vals[itemtype], max_vals[itemtype], actionvalue);
        }
    }
}

/// @brief Way back in setting menu
void Orbion_display::back()
{
    toupdate = true;

    if (actionmode)
    {
        actionmode = false;
    }
    else
    {
        exitSettings();
    }
}

void Orbion_display::scroll(int8_t inc)
{
    static uint32_t timer;
    int8_t multiplier = 8;

    toupdate = true;
    if (!actionmode)
    {
        current_item = inRange(0, nbitem, current_item - inc);
    }
    else
    {

        if (isTimeout(&timer, 100))
        {
            multiplier = 1;
        }
        inc *= multiplier;
        inc += actionvalue;
        actionvalue = inRange(min_vals[itemtype], max_vals[itemtype], inc);
    }
}

void Orbion_display::jogx(int8_t inc)
{
    static uint32_t timer = 0;
    _jog(inc, &jogxvalue, &timer);
}
void Orbion_display::jogy(int8_t inc)
{
    static uint32_t timer = 0;
    _jog(inc, &jogyvalue, &timer);
}

void Orbion_display::_jog(int8_t inc, uint8_t *axevalue, uint32_t *timer)
{
    if (itemtype == COLOR_AM)
    {
        if (isTimeout(timer, 100))
        {
            inc = (inc + 1) >> 2;

            *axevalue = inRange(0, 0xFF, *axevalue - inc); // = max(16,min(0,jogyvalue+inc));
            toupdate = true;
        }
    }
}

void Orbion_display::enter()
{
    uint8_t id = current_item + 1;
    if (actionmode)
    {
        if (itemtype == RESET_AM)
        {
            ee_reset = (actionvalue == 0 ? default_vals[0] : 0xFF); // reset control byte to 0xFF
        }
        else
        { // Update EEPROM
            EE_write(id, actionvalue);
            if (itemtype == COLOR_AM)
            {
                EE_write(id, jogxvalue, 1);
                EE_write(id, jogyvalue, 2);
            }
        }
        loadConfig();
        back();
    }
    else
    { // Read EEPROM
        actionmode = true;
        itemtype = items_type[current_item];
        actionvalue = inRange(min_vals[itemtype], max_vals[itemtype], EE_read(id));
        actionvalue = (itemtype == RESET_AM ? 0 : actionvalue);
        str_pos = itemPos[itemtype];

        if (itemtype == COLOR_AM)
        {
            jogxvalue = EE_read(id, 1);
            jogyvalue = EE_read(id, 2);
        }
        toupdate = true;
    }
}

////  Display functions

/// @brief print a single char centered
/// @param t
void Orbion_display::print_center(const char &t)
{
    uint8_t x = width() / 2 - 3;
    setCursor(x, getCursorY());
    print(t);
}

/// @brief Print a String centered
/// @param t
void Orbion_display::print_center(const String &t)
{
    uint8_t x = width() / 2 - 3 * t.length();
    setCursor(x, getCursorY());
    print(t);
}

/// @brief print a string centered between x and x+w
/// @param t
/// @param x
/// @param w
void Orbion_display::print_center(const String &t, int8_t x, uint8_t w)
{
    uint8_t xs = x + w / 2 - 3 * t.length();
    setCursor(xs, getCursorY());
    print(t);
}

/// @brief draw a filled rectangle on the string t
/// @param t
void Orbion_display::fillrect_center(const String &t)
{
    uint8_t x1 = width() / 2 - 3 * t.length() - 5;
    fillRect(x1, getCursorY() - 2, t.length() * 6 + 10, 11, SH110X_WHITE);
}

/// @brief  read settings from PROGMEM store it to buffer
/// @param id
void Orbion_display::pgmString(uint8_t id)
{
    strcpy_P(buf, (char *)pgm_read_word(&datas[id]));
}

///
void Orbion_display::drawIcon(uint8_t x, uint8_t y, byte id)
{
    const uint8_t *icon_bitmap = (const uint8_t *)pgm_read_word(&icons[id]);
    drawBitmap(x, y, icon_bitmap, icon_SIZE, icon_SIZE, SH110X_WHITE);
}

/// @brief  draw a scroll bar on the display
/// @param x
/// @param y
/// @param l  scroll bar length in pixels
/// @param h  horizontal or vertical
/// @param nb_items  number of items on the scroll bar
/// @param current_item  current item on the scroll bar
void Orbion_display::scrollBar(uint8_t x, uint8_t y, uint8_t l, boolean horizontal, uint8_t min_val, uint8_t max_val, uint8_t val)
{
    uint8_t w;
    uint8_t h;
    uint8_t x3;
    uint8_t y3;

    val -= min_val;
    max_val -= min_val;
    uint8_t item_pos = val * l / max_val - 1; // position of the item

    if (horizontal)
    {
        w = l;
        h = 1;
        x3 = item_pos + x; // start of the item
        y3 = y - 1;        // start of the item
    }
    else
    {
        w = 1;
        h = l;
        x3 = x - 1;        // start of the item
        y3 = item_pos + y; // start of the item
    }
    fillRect(x, y, w, h, SH110X_WHITE);
    fillRect(x3, y3, 3, 3, SH110X_WHITE);
}

/// @brief
/// @param minval
/// @param maxval
/// @param val
/// @return
int16_t Orbion_display::inRange(int16_t minval, int16_t maxval, int16_t val)
{
    return min(maxval, max(minval, val));
}

//// EEPROM Read/write functions   ////

/// @brief Get EEPROM address from item ID
/// @param id
/// @return
uint8_t Orbion_display::EE_addr(uint8_t id)
{
    int ee_adr = (int)ee_address[id];
    return ee_adr;
}

/// @brief Write EEPROM value
/// @param id
/// @param value
void Orbion_display::EE_write(uint8_t id, byte value)
{
    EE_write(id, value, 0U);
}
void Orbion_display::EE_write(uint8_t id, byte value, uint8_t offset)
{
    int ee_adr = EE_addr(id);
    EEPROM.update(ee_adr + offset, value);
}

/// @brief Read EEPROM value, return default if value is invalid.
/// @param id
/// @return
uint8_t Orbion_display::EE_read(uint8_t id)
{
    byte val = EE_read(id, 0U);
    if (id > 0)
    {
        uint8_t amode = items_type[id - 1];
        return (val < min_vals[amode] || val > max_vals[amode] ? default_vals[id] : val);
    }
    return val;
}
uint8_t Orbion_display::EE_read(uint8_t id, uint8_t offset)
{
    int ee_adr = EE_addr(id);
    return EEPROM.read(ee_adr + offset);
}

//// Load config
void Orbion_display::loadConfig()
{
    if (EE_read(0) == ee_reset)
    { /// Check if EEPROM stores the config and read it

        conf.Encoder = EE_read(1) ? -1 : 1;
        conf.Mode = EE_read(2);

        conf.dz = EE_read(3); /// limit DZ and sensitivity
        conf.sensitivity = EE_read(4);

        conf.color1 = _leds->Color(EE_read(5), EE_read(5, 1), EE_read(5, 2));
        conf.color2 = _leds->Color(EE_read(6), EE_read(6, 1), EE_read(6, 2));

        conf.led_mode = EE_read(7);
        conf.led_color_mode = EE_read(8);
        conf.contrast = EE_read(9);
        conf.timeout = EE_read(10) * 1000;
    }
    else
    {
        // EEPROM is empty or version changed
        // first byte is a control byte for version change
        // 0x01 = 1st version, 0x02 = 2nd version, etc..
        // 0xFF = no settings stored

        ee_reset = default_vals[0]; // reset control byte

        for (uint8_t i = 0; i < 11; i++)
        {
            EE_write(i, default_vals[i]);
        }
        EE_write(5, 0x7D, 1);
        EE_write(5, 0x7D, 2);
        EE_write(6, 0x7D, 1);
        EE_write(6, 0x7D, 2);

        loadConfig();
    }
}
