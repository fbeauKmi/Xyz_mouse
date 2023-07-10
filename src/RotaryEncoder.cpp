// -----
// RotaryEncoder.cpp - ligthweight library for using rotary encoders.
//
// Usage : Create object RotaryEncoder YourEnc(DT,CLK);
// Use YourEnc.getDirection(); // possible values -1, 0, 1.

#include "RotaryEncoder.h"
#include "Arduino.h"

// positions: [3] 1 [0] 2 [3] 1 [0] 2 [3]
// [0] and [3] initialize signal trigger then 1 and 2 define direct
// only [3] can be catched _direction_half
// ==> clockwise, count up
// <== counterclockwise,  count down


// ----- Initialization and Default Values -----

RotaryEncoder::RotaryEncoder(int pin1, int pin2)
{
  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins and turn on pullup resistor
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  
  // Store direction result,
  _direction = 0;       // [0] and  [3]
  _direction_half = 0;  // [3]
} // RotaryEncoder()

RotaryEncoder::~RotaryEncoder()
{
  
}

// [0] and [3] return 1 if clockwise -1 if counterclockwire, 0 if no changes
int8_t RotaryEncoder::getDirection()
{
  return _direction;
}

// [3] return 1 if clockwise -1 if counterclockwire, 0 if no changes
int8_t RotaryEncoder::getDirectionHalf()
{
  return _direction_half;
}

// update direction variables from actual state debouncetime 5ms
void RotaryEncoder::update(void)
{
  bool sig1 = digitalRead(_pin1);
  bool sig2 = digitalRead(_pin2);
  static unsigned long hS=0;
  static uint8_t _oldState = sig1 | (sig2 << 1);
  uint8_t thisState = sig1 | (sig2 << 1);
  
  _direction = 0;
  _direction_half = 0;

  if (millis()-hS>5){
    hS=millis();
    
    if (_oldState!=thisState){
      
      if(_oldState == 3 ){
        _direction = (sig1 ? 1 : -1);
        _direction_half = _direction;
      }
      if(_oldState == 0){
        _direction = (sig2 ? 1 : -1);
      }
      
      _oldState = thisState;
      return;
    }
  } 

} // update()


// Send command to the interface trying to get motion as smooth as possible
// S
void RotaryEncoder::action(bool buttonPressed, int8_t dir, void (*send_cmd)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t))
{
  static int8_t lastDirection; 
  static uint8_t buttonState;
  static unsigned long hS;
  static unsigned long hidDebounce;
  
  // Compute increment value (0 - 500)
  // add 32 each time Encoder is triggered
  // subtract 2 every 6ms
  if(getDirection()){
    _increment = min(500,_increment+32);
    lastDirection = getDirection();
  } else {
    if(millis() - hS > 6){
      hS = millis();
      _increment = max(0,_increment-2);
    }
  }
  
  // report HID every 10 ms if needed
  if (millis() - hidDebounce > 10 && _increment){
    hidDebounce=millis();
      if(buttonState != buttonPressed){
        _increment=0;
      }
      
      if (buttonPressed){
          (*send_cmd)(0,0,0,0,0,_increment * lastDirection * dir);
      }else{
          (*send_cmd)(0,0,_increment * lastDirection * dir,0,0,0);
      }
      buttonState = buttonPressed;
  }
} // action()