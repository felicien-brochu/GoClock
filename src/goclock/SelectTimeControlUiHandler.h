
#ifndef __SelectTimeControlUiHandler_h__
#define __SelectTimeControlUiHandler_h__

#include "UiHandler.h"
#include "TimeControlUi.h"
#include "GameButtonGestures.h"
#include "SelectTimeControlOptionUiHandler.h"

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern TimeControlUi *timeControls[];

extern SelectTimeControlOptionUiHandler selectTimeControlOptionUiHandler;
extern UiHandler *currentUiHandler;

class SelectTimeControlUiHandler : public UiHandler {
  
  int currentTimeControlUi;
  
public:

  SelectTimeControlUiHandler() {
    currentTimeControlUi = 0;
  }
  
  virtual ~SelectTimeControlUiHandler() {
  }

  virtual void tick(Clock *clock) {
    buttonGestures.tick( clock );

    int travel = buttonGestures.getNavigationTravel();
    bool buttonPushed = buttonGestures.wasPushButtonPushed();

    if (travel != 0) {
      int optionCount = getNumberOfOptions();
      currentTimeControlUi = (currentTimeControlUi + travel) % optionCount;
      if (currentTimeControlUi < 0) {
        currentTimeControlUi = optionCount + currentTimeControlUi;
      }
    }

    if (buttonPushed) {
      selectTimeControlOptionUiHandler.setTimeControlUi( timeControls[ currentTimeControlUi ] );
      currentUiHandler = &selectTimeControlOptionUiHandler;
      beep();
    }

  }
  
  virtual void render(Clock *clock){
    lcd2.beginRender( clock );
    
    TimeControlUi *tc = timeControls[ currentTimeControlUi ];
    lcd2.printTopCenter( tc->getName() );

    lcd2.endRender();
  }

private:
  int getNumberOfOptions() {
    int optionCount = 0;
    while (timeControls[ optionCount ] != NULL) {
      optionCount++;
    }
    return optionCount;
  }
  
};

#endif
