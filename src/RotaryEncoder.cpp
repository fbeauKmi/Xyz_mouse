// -----
// RotaryEncoder.cpp - ligthweight library for using rotary encoders.
//
// Usage : Create object RotaryEncoder YourEnc(DT,CLK);
// Use YourEnc.getDirection(); // possible values -1, 0, 1.

#include "RotaryEncoder.h"

// positions: [3] 1 [0] 2 [3] 1 [0] 2 [3]
// [0] and [3] initialize signal trigger then 1 and 2 define direct
// only [3] can be catched _direction_half
// ==> clockwise, count up
// <== counterclockwise,  count down

// ----- Initialization and Default Values -----

/// @brief
/// @param pin1
/// @param pin2
RotaryEncoder::RotaryEncoder(int pin1, int pin2)
{
  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins and turn on pullup resistor
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  // Store direction result,
  _direction = 0;      // [0] and  [3]
  _direction_half = 0; // [3]
} // RotaryEncoder()

/// @brief
RotaryEncoder::~RotaryEncoder()
{
}

/// @brief [0] and [3] return 1 if clockwise -1 if counterclockwire, 0 if no changes
/// @return
int8_t RotaryEncoder::getDirection()
{
  return _direction;
}

/// @brief [3] return 1 if clockwise -1 if counterclockwire, 0 if no changes
/// @return
int8_t RotaryEncoder::getDirectionHalf()
{
  return _direction_half;
}

/// @brief update direction variables
/// @param
void RotaryEncoder::update(void)
{
  bool sig1 = digitalRead(_pin1);
  bool sig2 = digitalRead(_pin2);
  static uint32_t lastDebounceTime;
  uint8_t thisState = sig1 | (sig2 << 1);
  static uint8_t _oldState = thisState;

  _direction = 0;
  _direction_half = 0;
  // Check if the state has changed
  if (isTimeout(&lastDebounceTime, ROTARYENCODER_DEBOUNCE_DELAY) && _oldState != thisState)
  {
    if (_oldState == 3)
    {
      _direction = (sig1 ? 1 : -1);
      _direction_half = _direction;
    }
    if (_oldState == 0)
    {
      _direction = (sig2 ? 1 : -1);
    }

    _oldState = thisState;
  }

} // update()

/// @brief Return values for HID,emulate jogZ;
/// @return
Axes RotaryEncoder::returnValue()
{
  static int8_t lastDirection;
  static uint32_t hS;

  // Compute increment value (0 - 500)
  // add 32 each time Encoder is triggered
  // subtract 2 every 6ms

  if (getDirection())
  {
    _increment = min(0xFFF, _increment + 0x1C);
    lastDirection = getDirection();
  }
  else
  {
    if (isTimeout(&hS, 6))
    {
      _increment = max(0, _increment - 2);
    }
  }

  return {0, 0, (_increment * lastDirection) >> 2};

} // returnValue()