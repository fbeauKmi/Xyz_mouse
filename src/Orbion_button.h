#pragma once
#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define DEBOUNCE_DELAY   15

class Orbion_button
    {
    public:

    // Constructor
      Orbion_button(int pin);
      ~Orbion_button();
      
      
      void update();                 // Update button state
      bool isPressed();              // return True if button pressed
      void reset();                  // reset button state 
      bool clicked();                // return True once if button pressed
    
    private:
      int _pin;                       // button pin
      uint32_t lastCount;             // 
      uint32_t lastDebounceTime;
      bool ispressed = false;
      bool isreset = true;
      uint8_t lastState;
      bool _triggered;     

    };

#endif