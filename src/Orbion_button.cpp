#include "Orbion_button.h"

Orbion_button::Orbion_button(int pin)
  : _pin(pin), lastState(HIGH), _triggered(false) {
  pinMode(_pin, INPUT_PULLUP);
}

Orbion_button::~Orbion_button()
{
}

void Orbion_button::update(){

  static uint32_t lastDebounceTime;
  uint32_t _currentMillis;

  uint8_t reading = digitalRead(_pin);
  
  if (reading != lastState) {
    // reset the debouncing timer
    isTimeout(&lastDebounceTime,0);
  }
  if( isTimeout(&lastDebounceTime, 500) && reading == HIGH){
    reset();

  }else
  if (isTimeout(&lastDebounceTime, &_currentMillis, DEBOUNCE_DELAY, false)) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (reading == LOW){
      if(!ispressed){
        if(!isreset){
          ispressed = true;
        }
        isreset=false;
        lastCount=_currentMillis;
      }
    }else{
      ispressed = false;
    } 
  }
  lastState=reading;
}

bool Orbion_button::isPressed(){
  return ispressed;
}

void Orbion_button::reset(){
  if(!isreset){
    isreset = true;
  }
}

bool Orbion_button::clicked(){
  uint32_t _dummy;
  if( ispressed && !isreset  && isTimeout(& lastCount, &_dummy, 60, false) ){
    reset();
    return true;
  }
  return false; 
}

