# M-Xyz firmware for Orbion

First attempt to build an alternate firmware for Orbion
It is now compatible with 3Dconnexion drivers and mimic Spacemouse compact.

## first done : 

- Orbion_Neopixel library
- Rotary encoder library based on https://github.com/mathertel/RotaryEncoder
- Joystick library
- button library
- display Library (still to simplify)

## todo/improve

- Orbion_settings : a mixed library wich managed display and settings
- Strings management to improve

## Installation :

Download the current repo, 
Compile firmware with platformio in Vscode

## Usage :
At first run the firmware store a default config in EEPROM


| Mode   | actuators  |  function   | Notes |
|--------|----------|-------------| ---|
| Normal  | B1/B2 | Button Left / Button right ~~Selected Fn~~ |
|   | ~~rot_Knob~~ | ~~Zoom~~ |
|   | Joystick or rot knob | Tilt / Orbit | ~~When Joystick is triggered rot_knob is disabled~~| 
|   | B3 ~~B1 + B_Knob~~ | Enter settings |
|   |~~B_Knob + rot_Knob~~| ~~Quick mode change~~ |
|   | B_Knob + Joystick or rot Knob | Pan / Zoom | 
| Settings | B_Knob | Select |
|   | B3 | Return/Cancel |
|   | rot_Knob | scroll |
| Color settings  | rot_Knob | change Hue |
|   | Joystick X | change saturation |
|   | Joystick Y | change brigthness | 

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

## Inspiration sources : 
- https://github.com/jfedor2/spaceball-2003
- https://github.com/JakubAndrysek/PySpaceMouse