#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>

class display_ : public Adafruit_SSD1306
{
    private :
       
    public :
        display_() : Adafruit_SSD1306(4){};
        void display_init(){
            begin(SSD1306_SWITCHCAPVCC, 0x3C);
        };
};

#endif