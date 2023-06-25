#ifndef CONFIG_H
#define CONFIG_H

typedef struct 
{
  uint8_t K;
  uint8_t M;
}  button_action;

typedef struct
{
  uint8_t k1;
  uint8_t k2;
  uint8_t d;
  uint8_t m1;
  uint8_t m2;
} jog_action;


typedef struct
{
  uint32_t color1;
  uint32_t color2;
  uint16_t timeout;
  uint8_t Mode;
  uint8_t sensibility;
  int8_t Encoder;
  button_action b0;
  button_action b1;
  button_action b2;
  button_action b3;
  
  uint8_t led_mode;
  uint8_t led_color_mode;
  
} MenuConfig;



#endif