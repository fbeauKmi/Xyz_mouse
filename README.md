# Cad_mouse

First attempt to build an alternate firmware for Orbion

## first done : 

- Orbion_Neopixel library
- Rotary encoder library based on https://github.com/mathertel/RotaryEncoder
- Joystick library (to debug)
- button library

## todo/improve

- Orbion_settings : a mixed library wich managed display and settings
- Strings management to improve

## Usage :
At first flash firmware store a default config in EEPROM

Press B1 + knob button to enter menu and modify settings.

Menu mode : B1 = back,  Knob button = valid
Color menu : use encoder to select Hue, X axis of joystick to choose saturation , Y axis for brigthness.