
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SH110X.h>
#include <Wire.h>

class display_ : public Adafruit_SH1106G
{
    private :
        
    public :
        display_(): Adafruit_SH1106G(128, 64, &Wire, 4){}; 
        void display_init(){
             begin(0x3C, true); // Address 0x3C default
        };
};

#endif