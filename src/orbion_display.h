#pragma once
#ifndef ORBIONDISP_H
#define ORBIONDISP_H

#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>
#include "string.h"
#include "Orbion_neopixel.h"
#include "struct.h"  // project structs 
#include "logo.h"
#include "strings.h"
#include <Wire.h>

#define _ENTRY_MODE 2

#define SSD1306 LOW
#define WHITE SH110X_WHITE

class Orbion_display : public Adafruit_SH1106G
    {
        protected:
            int8_t current_menu=1;
            int8_t current_item=0;
            bool show=false;
            bool toupdate=true;
    
            Orbion_Neopixel * _leds;
            
            char list[20];
            char actions[20];
            char itemPos[20];
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
            Orbion_display();
                 
            void setleds(Orbion_Neopixel * leds){ 
                _leds = leds;
                loadConfig();
            };
            void init();
            void update();
            void refresh();
            void clear();


            void back();
            void exitSettings();
            void startSettings();
            
            void scroll(int8_t inc);
            void jogx(int8_t inc);
            void jogy(int8_t inc);
            void enter();
            void select();

            void action();
            

            bool settingMode(){return show;};
            void loadConfig();

            void pgmString(uint8_t id);
            void pgmUintArray(uint8_t id, uint8_t nbval);
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
