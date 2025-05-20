
![LOGO Xyz Mouse](./logo.png)
# Xyz Mouse firmware for Orbion

An alternate firmware for [Orbion spacemouse](https://github.com/FaqT0tum/Orbion_3D_Space_Mouse) compatible with 3Dconnexion drivers that mimic _Spacemouse compact_.

The firmware is licensed under GPL 3.0 . 

It provides 6 degrees of freedom (6DoF: 3DoF*2 ;-) ) functionality to your Orbion (see usage). The current firmware is designed for
Arduino Leonardo. Ports for more modern mcus and different sensors is planned.

## Compatibility :

### Hardware :

  Xyz Mouse firmware shares the material support of Orbion 3D mouse.
  - Arduino micro ATmega32U4,
  - Oled display 128x64 i2c,
  - XY joystick,
  - Encoder for knob,
  - 3 buttons + knob button,
  - Neopixel led strip.

### Firmware :

  The firmware has been reported to work on latest Windows and macOS with
  3dxware drivers. For Linux systems, SpaceNav project is required.
  Note that not all applications will work with the driver. It depends on the support
  of both the driver and the software.

## Installation :

It can be tricky to get the firmware to be recognized by the system. Follow this steps to get it work.
- Clone the current repo,
- Open the folder with platformio in Vscode
- Check `platformio.ini` and `src\settings.h`
- Compile and upload the firmware
- Unplug/plug the Orbion
- Install the [3DConnexion drivers](https://3dconnexion.com/us/drivers/) (Windows, macOS) or  [spacenav](https://spacenav.sourceforge.net/) (Linux)
- Restart your computer.

> The default display build is configured for sh110x, if you want to use ssd1306 open `platformio.ini` and change ``default_envs`` value to  ``ssd1306``

![Main display](./images/p1.png)

## Usage :
At first run the firmware store a default config in EEPROM


| Mode   | actuators  |  function   | Notes |
|--------|----------|-------------| ---|
| Normal  | B1/B2 | Button Left / Button right |
|   | Joystick or rot knob | Panning / Rotation || 
|   | B3  | Enter settings |
|   | B_Knob + Joystick or rot Knob | Panning / Rotation | 
| Settings | B_Knob | Select |
|   | B3 | Return/Cancel |
|   | rot_Knob | scroll |
| Color settings  | rot_Knob | change Hue | 16M color settings
|   | Joystick X | change saturation |
|   | Joystick Y | change brigthness | Brigthness of Color1 is used in Rainbow mode

##  Settings Menu

Press B3 button to enter menu

![Settings](./images/p2.png)

Use encoder to navigate, Knob button to enter item or validate, B3 button to cancel.

![Menu item](./images/p3.png)


### Settings

  - **Knob dir.**: _[Left hand, Right hand]_ (default: Left Hand)  
    Change the direction of the encoder.
  - **Rot / Pan**: _[Hold to Pan, Hold to Rot, Click to invert]_ (default: Click
    to invert)  
    Three modes: `Hold to Pan` (hold knob button to pan), `Hold to Rot` (hold knob
    button to rotate), `Click to change` (press knob button to switch between panning
    and rotation mode).
  - **DZ Width**: _[3 - 20]_ (default: 5). \
    Joystick deadzone setting
  - **Sensitivity**: _[1 - 5]_ (default: 5).\
    Joystick sensitivity
  - **1st Color**: _For LED effects_ (default: HSV(46,127,127)).
  - **2nd Color**: _Secondary color for LED effects_ (default: HSV(127,127,127)).
  - **Led Motion**: _[Running, Chase, Fixed]_ (default: Chase).

  - **Color Mode**: _[Off, 1st Color, Mixed, Rainbow]_ (default: Mixed)  
    Mixed creates a gradient between Color 1 and Color 2.
  - **OLED Contrast**: _[1 - 5]_ (default: 1). \
    Screen contrast setting _(only for SH110x display, it does nothing on SSD1306)_
  - **Timeout (sec)**: _[3 - 60]_ (default: 30s). \
    Time before screensaver activates
  - **Restore def.**: Reset all settings to default value.


All other settings (by app, buttons functions )can be done in 3DxWare on your computer.

## Troubleshooting
_**I'm not able to upload firmware on Arduino micro.**_
  
  - Press reset button twice quickly before upload.
  - See [Orbion How to flash firmware](https://github.com/FaqT0tum/Orbion_3D_Space_Mouse/blob/main/HowToFlashing.pdf) 

_**I cannot access the Settings menu pressing B3.**_

  - Check the wiring.
  - Check if settings.h follows your pinout.

_**Mouse is always/frequently disconnecting/ressetting / Neopixel blinks when I move axes.**_
  
  - Check your work, i2c / neopixel require good quality wiring.

_**Black screen at startup**_

  - Check your wiring.
  - Check if settings.h follows your pinout. 
  - Wrong display type, check if your hardware is ssd1306 or sh110x and 
  adjust `platformio.ini`

_**Mouse is not recognize as SpaceMouse.**_

  - Manually set driver to 3Dconnexion Spacemouse compact in Hardware manager
  - or Uninstall/reinstall 3Dconnexion drivers. Restart you computer.
  - or flash the firmware again.

_**Display only shows half of the content**_

![half display](./images/half_display.png)

  - You probably have a 128x32 display. Xyz Mouse is **only** compatible 
  with 128x64 OLED display

_**Mouse doesn't act on my software.**_

  - See [software support](https://3dconnexion.com/us/software/) on 3Dconnexion
   website.

_**I'm unable to change axis on my software but some other softwares work.**_

  - See [software support](https://3dconnexion.com/us/software/) on 3Dconnexion
   website
  - for [Superslicer](https://github.com/supermerill/SuperSlicer) , Activate _"Enable
  support for legacy 3Dconnexion devices"_ in _"Preferences/Camera"_ . Use `Crtl+M`
  to access to the settings tab for 3D mouse in Superslicer.
  - Alternativly, refer to the software editor to ask for Spacemouse compact support. 

## Questions ?

@fboc on Discord

## Inspiration sources : 
- https://github.com/jfedor2/spaceball-2003
- https://github.com/JakubAndrysek/PySpaceMouse