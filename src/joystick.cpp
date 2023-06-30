/*
  Joystick.cpp

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

#include "Joystick.h"
#include <Keyboard.h>
#include <Mouse.h>

Joystick::Joystick(
    uint8_t pin_x,
    uint8_t pin_y)
{
    gpio_pin_x = pin_x;
    gpio_pin_y = pin_y;
    zero_x = 0;
    zero_y = 0;
    value_int_x = 0;
    value_int_y = 0;
    deadzone = 0;
    
    _smooth = 5;
    _T=millis();
    _triggered=false;
    jSens = 2;
}

Joystick::~Joystick()
{
}

void Joystick::Init()
{
    
    pinMode(gpio_pin_x, INPUT);
    pinMode(gpio_pin_y, INPUT);

   // analogReadResolution(10);
    CalibrateZero();
}

void Joystick::SetDeadzone(int8_t dz)
{
    deadzone = dz;
};

int8_t Joystick::GetDeadzone()
{
    return deadzone;
};

void Joystick::Update()
{
    // read the joystick
    value_int_y = (analogRead(gpio_pin_y)) >> 3;
    value_int_x = (analogRead(gpio_pin_x)) >> 3;
}


void Joystick::CalibrateZero()
{
    // calibrate the joystick mid point
    int64_t in_x = 0;
    int64_t in_y = 0;
    
    for (int i = 0; i < 16; i++) {
        Update();
        in_x += value_int_x;
        in_y += value_int_y;
        delay(20);
    }

    zero_x = in_x >> 4 ;
    zero_y = in_y >> 4 ;
}

int8_t Joystick::x()
{
    int8_t value = (value_int_x - zero_x);
    return axev(value) >> 2;
};

int8_t Joystick::y()
{
    int16_t value = ((uint16_t)value_int_y - zero_y);
    return axev(value) >> 2;
};

int8_t Joystick::axev(int16_t value)
{
    if (value > deadzone) {
        return max(0, value - deadzone);
    }
    else if (value < (- deadzone)) {
        return min(0, value + deadzone);
    }
    else {
        return 0;
    }
}

bool Joystick::isTriggered(){
    return _triggered;
}


void Joystick::action(bool buttonPressed)
{
    jog_action acts = (buttonPressed ? arAction_pan : (jog_action) arAction_rot);
    if(x() || y()){
        if(!_triggered){
            _triggered=!_triggered;
            if(acts.k1){Keyboard.press(acts.k1);}
            if(acts.k2){Keyboard.press(acts.k2);}
            if(acts.d){delay(acts.d);}
            if(acts.m1){Mouse.press(acts.m1);}
            if(acts.m2){Mouse.press(acts.m2);}
        }
        if(millis()-_T>(_smooth*jSens)){
            _T=millis();
            Mouse.move(-1 * x(), y(),0);
            _Trelease=millis();
        }
        
    } else if(_triggered){
        if(millis()-_Trelease>200){
            _Trelease=millis();
            _triggered=!_triggered;
            Keyboard.releaseAll();
            Mouse.release(MOUSE_ALL);
        }
    }
}