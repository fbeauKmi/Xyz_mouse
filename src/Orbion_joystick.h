// define a class for joystick

/*
  Joystick.h
  
  Modified version of the library of CADDECK
  
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
#ifndef ORBIONJOYSTICK_H
#define ORBIONJOYSTICK_H

#include "Arduino.h"
#include <stdint.h>


class Orbion_joystick {
   public:
    Orbion_joystick(uint8_t pin_x, uint8_t pin_y);
    ~Orbion_joystick();
    void Init();
    void Update();
    void CalibrateZero();
    void SetDeadzone(int16_t deadzone);
    int16_t GetDeadzone();
    
    bool isTriggered();
    void action(bool buttonPressed,  void (*send_cmd)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t));
    
    int16_t x();
    int16_t y();

   private:
    // joystick variables
    uint8_t gpio_pin_x;
    uint8_t gpio_pin_y;
    int16_t zero_x;
    int16_t zero_y;
    int16_t value_int_x;
    int16_t value_int_y;
    
    int8_t deadzone;
    int16_t axev(int16_t  value);
  
    bool _triggered;
    uint32_t _T;
    
};

#endif