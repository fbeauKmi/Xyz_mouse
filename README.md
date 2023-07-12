
![LOGO Xyz Mouse](./logo.png)
# Xyz Mouse firmware for Orbion

First attempt to build an alternate firmware for Orbion. 
It is now compatible with 3Dconnexion drivers and mimic Spacemouse compact.

It gives to your Orbion 6Dof (see usage). The current firmware is designed for
Arduino Leonardo. Ports for more modern mcus and different sensors is planned.

## First done : 

- Orbion_Neopixel library
- Rotary encoder library based on https://github.com/mathertel/RotaryEncoder
- Joystick library
- button library
- display Library (still to simplify)

## Todo/improve

- Orbion_settings : a mixed library wich managed display and settings
- Strings management to improve

## Installation :

Clone the current repo, 
Compile and upload the firmware with platformio in Vscode.
Install 3DConnexion drivers  

## Usage :
At first run the firmware store a default config in EEPROM


| Mode   | actuators  |  function   | Notes |
|--------|----------|-------------| ---|
| Normal  | B1/B2 | Button Left / Button right |
|   | Joystick or rot knob | Tilt / Orbit || 
|   | B3  | Enter settings |
|   | B_Knob + Joystick or rot Knob | Pan / Zoom | 
| Settings | B_Knob | Select |
|   | B3 | Return/Cancel |
|   | rot_Knob | scroll |
| Color settings  | rot_Knob | change Hue | 16M color settings
|   | Joystick X | change saturation |
|   | Joystick Y | change brigthness | Britthness of Color1 is used in Rainbow mode

##  Settings Menu

Press B3 button to enter menu
- Settings
    - Knob Direction (L hand / R hand)
    - PAN / TILT (reverser behavior when knob button is pressed)
    - Color 1 (For led Effects)
    - Color 2
    - Color mode (Color 1 , gradient(color 1, color 2), Rainbow) 
    - Motion mode (fixed, )
    - Screensave timeout

All other settings (by app, buttons functions )can be done in 3DxWare on your computer.

## Any questions

@fboc#1751 on Discord

## Inspiration sources : 
- https://github.com/jfedor2/spaceball-2003
- https://github.com/JakubAndrysek/PySpaceMouse