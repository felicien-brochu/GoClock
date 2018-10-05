
#ifndef __ToggleButton_h__
#define __ToggleButton_h__

#include <Arduino.h>

#include "base.h"
#include "SignalFilter.h"

class ToggleButton {
  
  SignalFilter filter0;
  SignalFilter filter1;
  uint8_t pin0;
  uint8_t pin1;
  bool enabled;

public:

  ToggleButton(uint8_t pin0, uint8_t pin1) : filter0( 3 ), filter1( 3 ) {
    this->pin0 = pin0;
    this->pin1 = pin1;
    enabled = false;
  }
  
  void init() {
    pinMode( pin0, INPUT );
    pinMode( pin1, INPUT );
    digitalWrite( pin0, HIGH ); // http://sheepdogguides.com/arduino/aht0bounce1.htm
    digitalWrite( pin1, HIGH );
  }
  
  void tick(Clock *clock) {
    filter0.tick( clock, digitalRead( pin0 ) );
    filter1.tick( clock, digitalRead( pin1 ) );
    
    if( enabled && isDisabled() ) {
      enabled = false;
    } else if( !enabled && isEnabled() ) {
      enabled = true;
    }
  }
  
  bool isDisabled() {
    return !filter0.isOn() && filter1.isOn();
  }

  bool isEnabled() {
    return filter0.isOn() && !filter1.isOn();
  }

};

#endif
