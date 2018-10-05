
#ifndef __GameButtonGestures_h__
#define __GameButtonGestures_h__

#include "RotaryEncoder.h"
#include "ToggleButton.h"

extern RotaryEncoder rotaryEncoder;
extern ToggleButton toggleButton;

class GameButtonGestures {
  
public:

  void tick(Clock *clock) {
    rotaryEncoder.tick( clock );
    toggleButton.tick( clock );
  }

  int getNavigationTravel() {
    return rotaryEncoder.getTravel();
  }
  
  bool wasPushButtonPushed() {
    return rotaryEncoder.wasPushButtonPushed();
  }

  bool wasPushButtonLongPushed() {
    return rotaryEncoder.wasPushButtonLongPushed();
  }

  bool isToggleButtonEnabled() {
    return toggleButton.isEnabled();
  }
  
  bool isToggleButtonDisabled() {
    return toggleButton.isDisabled();
  }

};

#endif
