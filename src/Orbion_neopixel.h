#pragma once
#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


//// CLASS NEOPIXEL ////
// Mode 
#define MODE_FIXED  0
#define MODE_ONE    1
#define MODE_HALF   2
#define COLOR_OFF   0
#define COLOR_1     1
#define COLOR_MIXED 2
#define COLOR_RAINBOW 3


class Orbion_Neopixel : public Adafruit_NeoPixel {
    private:
    
        uint32_t _lastframe=0;
        byte _maxPos =0;
        uint32_t _color1 = 0;
        uint32_t _color2 = 0;
        byte _brightness = 0;
        byte _value      = 0;
        uint8_t n = 0;

        uint8_t r(uint32_t color);
        uint8_t g(uint32_t color);
        uint8_t b(uint32_t color);
        uint8_t LinearInterpolate2Channels(uint8_t channel1, uint8_t channel2, uint8_t frac2);
        uint32_t mix2Color(uint32_t color1, uint32_t color2, uint8_t frac2);
        uint32_t mixColorCycle(uint32_t color1, uint32_t color2, uint16_t frac2);
        
    public:
        Orbion_Neopixel(uint16_t n, uint16_t pin);

        byte _mode = 0;
        byte _color_mode = 0;
        byte pos = 0;
    
        
        void display(uint8_t mode, uint8_t color_mode, uint32_t color1, uint32_t color2);
        void display(void);
        void setConfig(uint8_t mode, uint8_t color_mode, uint32_t color1, uint32_t color2);
        uint32_t getColor();
        uint32_t colorHSV(uint8_t H, uint8_t S, uint8_t V);
        void knobInc(int8_t inc, int8_t dir);

// useful math function

        uint16_t fScale16(uint16_t i, uint16_t scale)   { return (uint64_t (i) << 16)/scale;}
        uint8_t fScale16to8(uint16_t i, uint16_t scale){ return fScale16(i,scale) >> 8;  }
        uint8_t fScale(uint8_t i){ return ((uint16_t)i << 8 | i ) / _brightness; }
};

#endif