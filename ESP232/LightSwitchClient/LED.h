#ifndef LED_h
#define LED_h
#include "Arduino.h"

class LED
{
  public:
    LED(uint8_t pin);
    void begin();
    bool toggle();
    bool getState();
    bool setState(bool state);
  private:
    uint8_t  _pin;
    bool _state = LOW;
};

#endif