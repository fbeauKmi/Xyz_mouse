#include "Orbion_button.h"

Orbion_button::Orbion_button(int pin){
  _pin = pin;
  lastState = HIGH;
  _triggered = false;
  pinMode(_pin, INPUT_PULLUP);
  
}

Orbion_button::~Orbion_button()
{
}

void Orbion_button::update(){

  uint8_t reading = digitalRead(_pin);
  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > 500 && reading == HIGH){
    reset();
    lastDebounceTime = millis();

  }else
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (reading == LOW){
      if(!ispressed){
        if(!isreset){
          ispressed = true;
        }
        isreset=false;
        lastCount = millis();
      }
    }else{
      ispressed = false;
    } 
  }
  lastState=reading;
  return;
}

bool Orbion_button::isPressed(){
  return ispressed;
}

void Orbion_button::reset(){
  if(!isreset){
    isreset = true;
    return;
  }
}

bool Orbion_button::clicked(){
  if( ispressed && !isreset  && millis()- lastCount> 60 ){
    reset();
    return true;
  }
  return false; 
}

