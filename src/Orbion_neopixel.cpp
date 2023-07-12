// Extended library of Adafruit_Neopixel to match Orbion leds
// GNU GPL v3.0  July 2023
// @fboc#1751

#include "Orbion_neopixel.h"

Orbion_Neopixel::Orbion_Neopixel(uint16_t n, uint16_t pin) : Adafruit_NeoPixel(n, pin, NEO_GRB + NEO_KHZ800)
{
  _maxPos = 0xFF - (0xFF % n);
  _brightness = n > 6 ? 1500U / n : 255U;
  this->n = n;
}

void Orbion_Neopixel::setConfig(uint8_t mode, uint8_t color_mode, uint32_t color1, uint32_t color2){
      _color1 = colorHSV(r(color1), g(color1), b(color1) );
      _value = b(color1);
      _color2 = colorHSV(r(color2), g(color2), b(color2) );
      _color_mode = color_mode;
      _mode = mode;
      return;
}

void Orbion_Neopixel::display(uint8_t mode, uint8_t color_mode, uint32_t color1, uint32_t color2){
      setConfig(mode,color_mode,color1,color2);
      display();
      return;
}


/// @brief Set leds values 
/// @param  
// Mode : 0=fixed, 1= one led=f(pos), 3= half leds=f(pos)
// color_mode : 0 = off, 1= color1, 2= mix(color1, color2), 3=rainbow
void Orbion_Neopixel::display(void){    
    uint32_t color = _color1;

    knobInc(0,1);
    if(millis()-_lastframe > 150)
      {
      _lastframe=millis();
      
      clear();
      uint8_t first = (_mode ? (pos % n) : 0);
      uint8_t end = (_mode ? first + ( _mode == MODE_ONE ? 1 : n / 2) : n);
      
      if(_color_mode == COLOR_MIXED){
          color = mixColorCycle(_color1,_color2, _lastframe << 3 );
      } else if(_color_mode == COLOR_RAINBOW) {
          color = colorHSV((_lastframe >> 5) ,0xFFU, _value);
      }
      
      if(_color_mode){
      for(byte a=first; a < min(end,n); a++)
        {
        setPixelColor(a, color); 
        }
      if(end > n)
        {
        for(byte a=0; a<end-n; a++)
          {
          setPixelColor(a, color);
          }
        }
      }
      show();
      }
      return;
}

uint32_t Orbion_Neopixel::mix2Color(uint32_t color1, uint32_t color2, uint8_t frac2){
  
  if(!frac2) { return color1;}
  if (frac2 == 0xFF) {return color2;}
  return Color(
      LinearInterpolate2Channels(r(color1),r(color2),frac2),
      LinearInterpolate2Channels(g(color1),g(color2),frac2),
      LinearInterpolate2Channels(b(color1),b(color2),frac2));

}

uint32_t Orbion_Neopixel::mixColorCycle(uint32_t color1, uint32_t color2, uint16_t frac2){
  
  if(frac2 < 0x7FFF){
    return mix2Color(color1,color2, (uint8_t)(frac2*2 >> 8) & 0xFF);
  } 
  return mix2Color(color2,color1, (uint8_t)(frac2*2 >> 8 ) & 0xFF) ;
}

/// @brief getColor RGB from color1
/// @return  packed 32 bits RGB
uint32_t Orbion_Neopixel::getColor()
{
    return Color(fScale(r(_color1)),
                 fScale(g(_color1)),
                 fScale(b(_color1)));
}


uint32_t Orbion_Neopixel::colorHSV(uint8_t H, uint8_t S, uint8_t V)
{
    return ColorHSV( (H << 8) | H, S, ((uint16_t) V * _brightness) >> 8);
}


void Orbion_Neopixel::knobInc(int8_t inc, int8_t dir){
    pos = pos == _maxPos ? 0 : ( pos > _maxPos - 1 ? _maxPos - 1 : pos - dir * inc);
}

uint8_t Orbion_Neopixel::LinearInterpolate2Channels(uint8_t channel1, uint8_t channel2, uint8_t frac2){
     return uint8_t ((((channel2-channel1)*frac2) + (channel1 << 8)) >> 8) & 0xFF;
}

uint8_t Orbion_Neopixel::r(uint32_t color){
  return ( color >> 16 ) & 0xFF;
}
uint8_t Orbion_Neopixel::g(uint32_t color){
  return ( color >> 8 ) & 0xFF;
}
uint8_t Orbion_Neopixel::b(uint32_t color){
  return ( color ) & 0xFF;
}


