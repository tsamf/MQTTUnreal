#include "LED.h"
#include <Arduino.h>

LED::LED(uint8_t pin)
  : _pin(pin),
    _state(LOW) {
}

void LED::begin() {
  pinMode(_pin, OUTPUT);
}

bool LED::toggle() {
  if (_state == LOW) {
    digitalWrite(_pin, HIGH);
    _state = HIGH;
  } else {
    digitalWrite(_pin, LOW);
    _state = LOW;
  }

  return _state;
}

bool LED::getState(){
  return _state;
}


bool LED::setState(bool state){
  _state = state;
  digitalWrite(_pin, _state);
  return _state;
}


