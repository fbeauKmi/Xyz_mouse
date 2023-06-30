/**********************************************************************************************/
/*                                  Orbion Alternate Firmware                                 */
/**********************************************************************************************/
/*                                        version 0.1                                         */
/*                                       Since: June 2023                                     */
/*                                Creator: fboc#1751                                          */
/**********************************************************************************************/
/*                                           LICENSE                                          */
/*                                                                                            */
/*                                      (GPL 3.0)                                             */
/**********************************************************************************************/
#include <Arduino.h>

#include "Settings.h"

#include "struct.h"
#include "strings.h"

#include "leds.h"
#include "buttons.h"
#include "RotaryEncoder.h"
#include "joystick.h"

//#include "Oled.h"
#include "orbion_display.h"

#include <Mouse.h>
#include <Keyboard.h>
// #include <TinyUSB_Mouse_and_Keyboard.h>
#include <Wire.h>
#include <SPI.h>


#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define VERSION "0.1.0beta"

Orbion_Neopixel pixels(NUMPIXELS, LED);  // Init LEDs
Orbion_display display(menu_str);           //  


Orbion_button b1(button1);
Orbion_button b2(button2);
Orbion_button b3(button3);
Orbion_button b4(joyButt);
RotaryEncoder enc(DT,CLK);
Joystick joy(horzPin,vertPin);
 
uint32_t timeoff = 0;
bool screensave = true;

int8_t current_mode;


//// Update Buttons settings ////

void set_buttons(){
  b1.setKey(display.conf.b1);
  b2.setKey(display.conf.b2);
  b3.setKey(display.conf.b3);
  b4.setKey(display.conf.b0);
  return;
}

//// Update Leds config ////

void led_config()
{
  pixels.config(display.conf.led_mode,display.conf.led_color_mode,display.conf.color1,display.conf.color2);
}


//// Update Joystick config ////

void joy_config(){
 char cBuf[10];
  for(int i=0; i<10;i++)
  {
      cBuf[i]=pgm_read_byte_near(jog_actions + (display.conf.Mode * 10 + i));
  }
  joy.setFunction(display.conf.sensibility,cBuf);

}

/////////////////////////// Setup /////////////////////////////////

void setup() 
  {
    
    // init display & leds  //
    display.init();

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED) 

    display.setleds(&pixels);

    // init controls
     set_buttons();
     joy_config();
    joy.Init();
    joy.SetDeadzone(3);
    Mouse.begin();      
    Keyboard.begin();

    timeoff=millis();  // Init Screensaver timer

    led_config();
  }

//////////////////////////////////////////////  LOOP /////////////////////////////////////

void loop() 
  {

//////////////////// Update controls ////////////////////////////////////
      b1.update();
      b2.update();
      b3.update();
      b4.update();
      enc.update(); 
      joy.Update();
      display.update();
      
      int8_t enc_dir = enc.getDirectionHalf();
//////////////////// 
  
    if(display.isRunning()){  ////  Settings mode
       
        if(enc_dir)
          {
          display.scroll(enc_dir * display.conf.Encoder);
          }
        if(b4.clicked()){
          display.enter();
        }
        if(b1.clicked() ){
          display.back();
          led_config();
          if(!display.isRunning()){
            set_buttons();
            joy_config();
          }
        }

        display.jogy(joy.y());
        display.jogx(joy.x());

    }else{      
      ////    SpaceMouce mode ////
      
      if(b4.isPressed() && b1.isPressed()){    // Enter settings mode by pressind B1 and Knob button
        b4.reset();b1.reset();
        Keyboard.releaseAll();
        Mouse.release(MOUSE_ALL);
        display.startSettings();
        return;
      }

      if(!joy.isTriggered() && !b4.isPressed()){                     // Disable encoder when joystick is triggered
        Mouse.move(0, 0, enc.getDirection() * display.conf.Encoder);
      } else if (b4.isPressed() && enc_dir){                         // Quick mode settings
        current_mode = max(0,min(2, current_mode + enc_dir * display.conf.Encoder ));
        display.conf.Mode = (current_mode == 0 ? display.conf.AltMode0: (current_mode== 1 ? display.conf.AltMode1: display.conf.AltMode2 ));
        joy_config();
        display.refresh();
      }
      b1.action(); b2.action(); b3.action(); b4.action(); 
      joy.action(b4.isPressed());
    }

////    ScreenSaver   ///// 
    if(millis() - timeoff > max((uint16_t)3000,display.conf.timeout)){
      if(!screensave){
        screensave = !screensave;
        pixels.clear();
        pixels.show();
        display.clearDisplay();
        display.display();
        display.exitSettings();
      }
    }
    else
    {
    pixels.display();
    if(screensave){
        screensave = !screensave;
        led_config();
        display.refresh();
      }
    }
    if( b1.isPressed()||
      b2.isPressed() ||
      b3.isPressed() ||
      b4.isPressed() ||
      joy.x() || joy.y() || enc_dir)
    {
        pixels.knobInc(enc_dir, display.conf.Encoder * LED_CYCLE_DIR);
        timeoff=millis();
    }
                
 }

