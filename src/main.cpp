///////////////////////////////////////////////////////
//////////  M-Xyz  V0.1alpha  /////////////////////////
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
//    TILT/PAN  : behavior 
//    Color1  :  First Led Color
//    Color2  :   "
//    Motion_mode : Leds Motion mode
//    color_mode


#include <Arduino.h>
#include "settings.h"
#include "HID.h"
#include "hid_descriptor.h"
#include "Orbion_joystick.h"
#include "Orbion_button.h"
#include "RotaryEncoder.h"

#include "Orbion_neopixel.h"
#include "orbion_display.h"


/// Load Control Objects 
Orbion_joystick Joystick(AxisX,AxisY);
Orbion_button knobButton(joyButt);
Orbion_button b1(button1);
Orbion_button b2(button2);
Orbion_button b3(button3);
RotaryEncoder Encoder(DT,CLK);

// Load Display Objects
Orbion_Neopixel leds(NUMPIXELS,LED);
Orbion_display display; 
 

/// @brief Set Neopixel config from EEprom datas 
void led_config()
{
  leds.setConfig(display.conf.led_mode,display.conf.led_color_mode,display.conf.color1,display.conf.color2);
}

/// @brief Turn Off display and led when no action is done Timeout is done by config
/// @param  
void screensaver(void){
    static unsigned long timeoff;
    static bool screensave = false;
////    ScreenSaver   ///// 
    if(millis() - timeoff > max(3000,display.conf.timeout)){
      // Enter Screensave mode
      if(!screensave){
        screensave = !screensave;
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
      if(screensave){
        screensave = !screensave;
        display.refresh();
      }
    }

    // trigger control init timer
    if( b1.isPressed()||
      b2.isPressed() ||
      b3.isPressed() ||
      knobButton.isPressed() ||
      Joystick.isTriggered() || Encoder.getDirection())
    {
        leds.knobInc(Encoder.getDirectionHalf(), KNOB_DIR*display.conf.Encoder);
        timeoff=millis();
    }

}

/////////////////////////////////  SETUP ///////////////////////////

void setup() {
  // init and calibrate joystick
  Joystick.Init();
  
  // init display, leds
  display.init();
  display.setleds(&leds); // link leds to display

  leds.begin();
  led_config();

  // init HID
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

/////////////////////////////////  LOOP  ////////////////////////////

 
void loop() {
  // update controls
  Joystick.Update();
  knobButton.update();
  b1.update();
  b2.update();
  b3.update();
  Encoder.update();
  display.update();

  int8_t enc_dir = Encoder.getDirectionHalf();
  if(display.settingMode()){  ////  Settings mode
       
        if(enc_dir)
          {
          display.scroll(enc_dir * display.conf.Encoder);
          }
        if(knobButton.clicked()){
          display.enter();
        }
        if(b3.clicked() ){
          display.back();
          led_config();
        }

        display.jogy(Joystick.y()>>2);
        display.jogx(Joystick.x()>>2);

  } else {
    // enter settings mode
    if(b3.clicked()){
        display.startSettings();
        return;
    }

    // report HID state
    Joystick.action(knobButton.isPressed()^display.conf.Mode, send_command);
    Encoder.action(knobButton.isPressed()^display.conf.Mode, display.conf.Encoder, send_command);
    send_buttons(b1.isPressed(),b2.isPressed());

  }
  
  // led / display screensaver
  screensaver();
}