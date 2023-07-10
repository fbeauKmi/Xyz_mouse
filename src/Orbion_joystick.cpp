/*
  Orbion_joystick.cpp

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

#include "Orbion_joystick.h"

Orbion_joystick::Orbion_joystick(
    uint8_t pin_x,
    uint8_t pin_y)
{
    gpio_pin_x = pin_x;
    gpio_pin_y = pin_y;
    zero_x = 0;
    zero_y = 0;
    value_int_x = 0;
    value_int_y = 0;
    deadzone = 5;
    
    _T=millis();
    _triggered=false;
}

Orbion_joystick::~Orbion_joystick()
{
}

void Orbion_joystick::Init()
{
    
    pinMode(gpio_pin_x, INPUT);
    pinMode(gpio_pin_y, INPUT);

    CalibrateZero();
}

void Orbion_joystick::SetDeadzone(int16_t dz)
{
    deadzone = dz;
};

int16_t Orbion_joystick::GetDeadzone()
{
    return deadzone;
};

void Orbion_joystick::Update()
{
    // read the Orbion_joystick
    value_int_y = analogRead(gpio_pin_y);
    value_int_x = analogRead(gpio_pin_x);

    // triggered state
    _triggered = abs((value_int_x - zero_x) | (value_int_y - zero_y)) > deadzone ;
}


void Orbion_joystick::CalibrateZero()
{
    // calibrate the Orbion_joystick mid point
    int32_t in_x = 0;
    int32_t in_y = 0;
    
    for (int i = 0; i < 16; i++) {
        Update();
        in_x += value_int_x;
        in_y += value_int_y;
        delay(20);
    }

    zero_x = in_x >> 4 ;
    zero_y = in_y >> 4 ;
}

int16_t Orbion_joystick::x()
{
    int16_t value = (value_int_x - zero_x);
    return axev(value) >> 1;
};

int16_t Orbion_joystick::y()
{
    int16_t value = ((uint16_t)value_int_y - zero_y);
    return axev(value) >> 1;
};

int16_t Orbion_joystick::axev(int16_t value)
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

bool Orbion_joystick::isTriggered(){
    return _triggered;
}

void Orbion_joystick::action(bool buttonPressed, void (*send_cmd)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t) )
{
        if(millis()-_T> 10 ){
            _T=millis();
            if (buttonPressed){
                (*send_cmd)(0,0,0,-1*y(),x(),0);
            }else{
                (*send_cmd)(x(),y(),0,0,0,0);
            }
        }
}