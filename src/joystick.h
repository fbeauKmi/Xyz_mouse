// define a new clasa for joystick

/*
  Joystick.h
  
  Copyright (c) 2022, Andrew Fernie

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "Arduino.h"
#include <stdint.h>
#include "struct.h"

class Joystick {
   public:
    Joystick(uint8_t pin_x, uint8_t pin_y);
    ~Joystick();
    void Init();
    void Update();
    void CalibrateZero();
    void SetDeadzone(int8_t deadzone);
    int8_t GetDeadzone();
    
    void setFunction(uint8_t _sensibility, char ar[10]){jSens=9-_sensibility*2;
    memcpy(&arAction_rot,&ar[0],sizeof(jog_action));
    memcpy(&arAction_pan,&ar[5],sizeof(jog_action));
    };
    bool isTriggered();
    void action(bool buttonPressed);
    
    int8_t x();
    int8_t y();

   private:
    // joystick variables
    uint8_t gpio_pin_x;
    uint8_t gpio_pin_y;
    int8_t zero_x;
    int8_t zero_y;
    int8_t value_int_x;
    int8_t value_int_y;
    
    int8_t deadzone;
    int8_t axev(int16_t  value);
    uint8_t jSens;
    jog_action arAction_rot;
    jog_action arAction_pan;
    bool _triggered;
    uint32_t _T;
    uint32_t _Trelease;
    uint8_t _smooth;
    
};
