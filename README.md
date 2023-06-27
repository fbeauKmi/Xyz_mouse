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
At first run the firmware store a default config in EEPROM


| Mode   | actuators  |  function   | Notes |
|--------|----------|-------------| ---|
| Normal  | B1/B2/B3 | Selected Fn |
|   | rot_Knob | Zoom |
|   | Joystick | Orbit | When Joystick is triggered rot_knob is disabled| 
|   | B1 + B_Knob | Enter settings |
|   | B_Knob + rot_Knob| Quick mode change |
|   | B_Knob + Joystick | Pan | (not implemented yet)
| Settings | B_Knob | Select |
|   | B1 | Return/Cancel |
|   | rot_Knob | scroll |
| Color settings  | rot_Knob | change Hue |
|   | Joystick X | change saturation |
|   | Joystick Y | change brigthness | 


##  Setttings Menu (not the current one)

- Settings
    - Joystick
        - Default Mode
        - Alternate mode 1
        - Alternate mode 2
        - Sensitivity
    - Knob
        - Direction
    - Neopixel
        - Color 1
        - Color 2
        - Color mode
        - Motion mode
        - Screensave timeout

