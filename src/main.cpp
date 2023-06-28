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

#include "Oled.h"
#include "menu.h"

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
Orbion_settings men(menu_str);           //  


Orbion_button b1(button1);
Orbion_button b2(button2);
Orbion_button b3(button3);
Orbion_button b4(joyButt);
RotaryEncoder enc(DT,CLK);
Joystick joy(horzPin,vertPin);
 
uint32_t timeoff = 0;
bool screensave = true;



//// Update Buttons settings ////

void set_buttons(){
  b1.setKey(men.conf.b1);
  b2.setKey(men.conf.b2);
  b3.setKey(men.conf.b3);
  b4.setKey(men.conf.b0);
  return;
}

//// Update Leds config ////

void led_config()
{
  pixels.config(men.conf.led_mode,men.conf.led_color_mode,men.conf.color1,men.conf.color2);
}


//// Update Joystick config ////

void joy_config(){
 char cBuf[5];
  for(int i=0; i<5;i++)
  {
      cBuf[i]=pgm_read_byte_near(jog_actions + (men.conf.Mode * 5 + i));
  }
  joy.setFunction(men.conf.sensibility,cBuf);

}

/////////////////////////// Setup /////////////////////////////////

void setup() 
  {
    
    // init display & leds  //
    display.begin(0x3C, true); // Address 0x3C default     // comment if your oled is SSD1306
    display.setRotation(2);   // define screen orientation
    display.clearDisplay();
    display.display();

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED) 

    men.setdisplay(&display);
    men.setleds(&pixels);

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
      men.update();
      
      int8_t enc_dir = enc.getDirectionHalf();
//////////////////// 
  
    if(men.isRunning()){  ////  Settings mode
       
        if(enc_dir)
          {
          men.scroll(enc_dir * men.conf.Encoder);
          }
        if(b4.clicked()){
          men.enter();
        }
        if(b1.clicked() ){
          men.back();
          led_config();
          if(!men.isRunning()){
            set_buttons();
            joy_config();
          }
        }

        men.jogy(joy.y());
        men.jogx(joy.x());

    }else{      ////    SpaceMouce mode ////
      if(b4.isPressed() && b1.isPressed()){    // Enter settings mode by pressind B1 and Knob button
        b4.reset();b1.reset();
        Keyboard.releaseAll();
        Mouse.release(MOUSE_ALL);
        men.startSettings();
        return;
      }

      if(!joy.isTriggered() && !b4.isPressed()){                     // Disable encoder when joystick is triggered
        Mouse.move(0, 0, enc.getDirection() * men.conf.Encoder);
      } else if (b4.isPressed() && enc_dir){
        men.conf.Mode = min(9,max(0, men.conf.Mode + enc_dir * men.conf.Encoder));
        joy_config();
        men.refresh();
      }
      b1.action(); b2.action(); b3.action(); b4.action(); 
      joy.action(b4.isPressed());

    }

////    ScreenSaver   ///// 
    if(millis() - timeoff > max((uint16_t)3000,men.conf.timeout)){
      if(!screensave){
        screensave = !screensave;
        pixels.clear();
        pixels.show();
        display.clearDisplay();
        display.display();
        men.exitSettings();
        
      }
    }
    else
    {
    pixels.display();
    if(screensave){
        screensave = !screensave;
        led_config();
        men.refresh();
      }
    }
    if( b1.isPressed()||
      b2.isPressed() ||
      b3.isPressed() ||
      b4.isPressed() ||
      joy.x() || joy.y() || enc_dir)
    {
        pixels.knobInc(enc_dir, men.conf.Encoder * LED_CYCLE_DIR);
        timeoff=millis();
    }
                
 }

