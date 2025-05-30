///////////////////////////////////////////////////////
//////////  M-Xyz  V4.0  ////////////////////////////
//////////  an Alternate firmware for Orbion //////////
//////////  by @fboc#1751 /////////////////////////////
//////////  Licence GNU GPL V3.0  /////////////////////
///////////////////////////////////////////////////////

// Current firmware uses the same Pins than Original
// firmware. But the behavior is different
// Instead of mimic Mouse and Keyboard gesture, current
// firmware attempt to mimic a 3Dconnexion SpaceMouse
// and can be used with 3DxWare
//
// Usage :
//   TILT : Joystick & encoder
//   PAN : Joystick & encoder + Knob button
//   Left Button : Button 1
//   Right Button : Button 2
//
//   Enter settings : Button 3
//     In settings Menu, B3 = Back, K Button = Validate
//  Settings:
//    Knob Dir : Left hand / Right Hand
//    rot/Pan  : knob button behavior
//    DZ Width : Joystick Deadzone
//    Sensitivity : Joystick Sensitivity
//    Color1  :  First Led Color
//    Color2  :   "
//    Motion_mode : Leds Motion mode
//    color_mode : Leds Color mode
//    Contrast : OLED Contrast
//    Timeout  : OLED Timeout
//    Reset config : Reset to default config

#include <Arduino.h>
#include "settings.h"
#include "hid_descriptor.h"
#include "Orbion_joystick.h"
#include "Orbion_button.h"
#include "RotaryEncoder.h"

#include "Orbion_neopixel.h"
#ifndef NO_DISPLAY
#include "orbion_display.h"
#endif
#include "timer.h"

/// Load Control Objects
Orbion_joystick Joystick(AxisX, AxisY);
Orbion_button knobButton(joyButt);
Orbion_button b1(button1);
Orbion_button b2(button2);
Orbion_button b3(button3);
RotaryEncoder Encoder(DT, CLK);

// Load Display Objects
Orbion_Neopixel leds(NUMPIXELS, LED);

#ifndef NO_DISPLAY
Orbion_display display;
#endif

/// @brief Set Neopixel config from EEprom datas
void led_config()
{
  leds.setConfig(display.conf.led_mode, display.conf.led_color_mode, display.conf.color1, display.conf.color2);
}

/// @brief Turn Off display and led when no action is done Timeout is done by config
/// @param
void screensaver(int8_t enc_dir)
{

  static uint32_t timeoff = 0;
  uint32_t _currentMillis;

  static bool screensave = false;

  ////    ScreenSaver   /////
  if (isTimeout(&timeoff, &_currentMillis, display.conf.timeout, false))
  {
    // Enter Screensave mode
    if (!screensave)
    {
      screensave ^= true;
      leds.clear();
      display.exitSettings();
      display.clear();
      leds.show();
    }
  }
  else
  {
    // Leave Screen save mode
    leds.display();
    if (screensave)
    {
      screensave ^= true;
      display.refresh();
    }
  }

  // trigger control init timer
  if (b1.isPressed() ||
      b2.isPressed() ||
      b3.isPressed() ||
      knobButton.isPressed() ||
      Joystick.isTriggered()
#ifndef AxisZ
      || Encoder.getDirection()
#endif
  )
  {
    if (enc_dir)
    {
      leds.knobInc(enc_dir * LED_DIR);
    }
    timeoff = _currentMillis;
  }
}

/////////////////////////////////  SETUP ///////////////////////////

void setup()
{
  // init HID
  XYZmouse().begin();

  // init display, leds
  display.init(&leds);
  //display.setleds(); // link leds to display

  // init and calibrate joystick
  Joystick.Init();
  Joystick.SetDeadzone(display.conf.dz);

  leds.begin();
  led_config();
}

/////////////////////////////////  LOOP  ////////////////////////////
void loop()
{
  static bool pantilt_mode = false;
  // update controls
  Joystick.Update();
  knobButton.update();
  b1.update();
  b2.update();
  b3.update();
  Encoder.update();
  display.update();

// get Axes values
#ifndef AxisZ

  Axes axes = Joystick.returnValue() + Encoder.returnValue();

  int8_t enc_dir = Encoder.getDirectionHalf();
#else

  Axes axes = Joystick.returnValue();
  int8_t enc_dir = Joystick.getDirectionHalf();
#endif

  if (display.settingMode())
  { ////  Settings mode

    if (enc_dir)
    {
      display.scroll(enc_dir * display.conf.Encoder);
    }
    if (knobButton.clicked())
    {
      display.enter();
    }
    if (b3.clicked())
    {
      display.back();
      led_config();
      Joystick.SetDeadzone(display.conf.dz);
    }

    display.jog(axes);
  }
  else
  {
    // enter settings mode
    if (b3.clicked())
    {
      display.startSettings();
      return;
    }

    // get pan/tilt mode against config and knob button
    if (display.conf.Mode == 2)
    {
      pantilt_mode ^= knobButton.clicked();
    }
    else
    {
      pantilt_mode = knobButton.isPressed() ^ display.conf.Mode;
    }
    display.refresh(pantilt_mode);

    // adjust sensitivity
    axes = axes << display.conf.sensitivity;

    // report HID state
    XYZmouse().send_command(pantilt_mode ? TRANS_ID : ROT_ID, axes);
    // send buttons state
    XYZmouse().send_buttons(b1.isPressed(), b2.isPressed());
  }

  // led / display / screensaver
  screensaver(enc_dir);
}