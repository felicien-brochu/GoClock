
#ifndef __RotaryEncoder_h__
#define __RotaryEncoder_h__

#include <Arduino.h>

#include "base.h"
#include "SignalFilter.h"
#include "PushButton.h"

class RotaryEncoder {
  
  PushButton pushButton;
  uint8_t aPin;
  uint8_t bPin;
  int counter;
  int lastCounter;
  
  int aState;
  int aLastState;
  int interState;

public:

  RotaryEncoder(uint8_t aPin, uint8_t bPin, uint8_t pushButtonPin) : pushButton( pushButtonPin ) {
    this->aPin = aPin;
    this->bPin = bPin;
    counter = 0;
    lastCounter = 0;
    interState = 0;
  }
  
  void init() {
    this->pushButton.init();
    
    pinMode( aPin, INPUT );
    pinMode( bPin, INPUT );
    aLastState = digitalRead( aPin );
  }
  
  void tick(Clock *clock) {
    pushButton.tick( clock );

    aState = digitalRead( aPin );
    // If the previous and the current state of the aPin are different, that means a Pulse has occured
    if (aState != aLastState){   
      // If the bPin state is different to the aPin state, that means the encoder is rotating clockwise
      if (digitalRead( bPin ) != aState) {
        if (interState == 1) {
          counter ++;
          interState = 0;
        } else{
          interState ++;
        }
      } else {
        if (interState == -1) {
          counter --;
          interState = 0;
        } else {
          interState --;
        }
      }
    } 
    aLastState = aState; // Updates the previous state of the aPin with the current state
  }

  int getTravel() {
    int travel = counter - lastCounter;
    lastCounter = counter;
    return travel;
  }

  bool wasPushButtonLongPushed() {
    return pushButton.wasLongPushed();
  }
  
  bool wasPushButtonPushed() {
    return pushButton.wasPushed();
  }
};

#endif
