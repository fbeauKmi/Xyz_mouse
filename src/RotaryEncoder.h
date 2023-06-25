// -----
// RotaryEncoder.h - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
//
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
//
// This work is licensed under a BSD 3-Clause style license,
// https://www.mathertel.de/License.aspx.
//
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 16.06.2019 pin initialization using INPUT_PULLUP
// 10.11.2020 Added the ability to obtain the encoder RPM
// 29.01.2021 Options for using rotary encoders with 2 state changes per latch.
// -----

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Arduino.h"

class RotaryEncoder
{
public:
  // ----- Constructor -----
  RotaryEncoder(int pin1, int pin2);
  ~RotaryEncoder();

  // simple retrieve of the direction the knob was rotated last time. 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
  int8_t getDirection();
  int8_t getDirectionHalf();

  // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  void update(void);


private:
  int _pin1, _pin2; // Arduino pins used for the encoder.

  int8_t _direction;  // direction of the knob catch by [0] and [3]
  int8_t _direction_half;  // direction of the knob catch by [3]
};

#endif

// End