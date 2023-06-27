#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>
#include "string.h"
#include "leds.h"
#include "struct.h"  // project structs 

class Orbion_settings
    {
        protected:
            int8_t current_menu=1;
            int8_t current_item=0;
            bool show=false;
            bool toupdate=true;
            Adafruit_SH1106G * display;
            Orbion_Neopixel * _leds;
            uint8_t width;
            const char* const* datas;
            char list[20];
            char actions[20];
            char ee_address[20];
            char action_strings[20];
            
            uint8_t str_pos=0;
            
            uint8_t startpos;
            uint8_t nbitem;
            uint8_t startm=0;
            uint8_t actionmode=0;
            uint8_t jogxvalue=0;
            uint8_t jogyvalue =0;
            uint8_t actionvalue=0;         

        public:
            Orbion_settings(const char* const* strings):datas(strings){
                strcpy_P(list, (char*)pgm_read_word(&(datas[0])));
                strcpy_P(actions, (char*)pgm_read_word(&(datas[1])));
                strcpy_P(ee_address, (char*)pgm_read_word(&(datas[2])));
                strcpy_P(action_strings, (char*)pgm_read_word(&(datas[3])));

            };
            void setdisplay(Adafruit_SH1106G * disp){ display=disp;width=disp->width();toupdate=true;};
            void setleds(Orbion_Neopixel * leds){ 
                _leds = leds;
                loadConfig();
            };
            void update();
            void refresh();

            void back();
            void exitSettings();
            void startSettings();
            
            void scroll(int8_t inc);
            void jogx(int8_t inc);
            void jogy(int8_t inc);
            void enter();
            void select();

            void action();
            

            bool isRunning(){return show;};
            void loadConfig();
            void buttonKey(button_action * b, uint8_t id);

            void pgmString(uint8_t id);
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
            void print_center(const String &t,int8_t x, uint8_t w);
            void fillrect_center(const String &t);
            void _jog(int8_t inc, uint8_t * axevalue, uint64_t *timer );
    };


#endif
