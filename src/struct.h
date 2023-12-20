// struct.h structure for Orbion_display.h

#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
  uint32_t color1;
  uint32_t color2;
  uint16_t timeout;
  uint8_t Mode;
  int8_t Encoder;  
  uint8_t led_mode;
  uint8_t led_color_mode;
  uint8_t contrast;
  
} MenuConfig;



#endif