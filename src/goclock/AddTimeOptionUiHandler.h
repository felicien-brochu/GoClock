
#ifndef __AddTimeOptionUiHandler_h__
#define __AddTimeOptionUiHandler_h__

#include "UiHandler.h"
#include "GameClock.h"
#include "GameClockLcd.h"
#include "GameButtonGestures.h"

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

#define ADD_TIME_OPTION_COUNT 4

#define ADD_TIME_OPTION_LEFT 0
#define ADD_TIME_OPTION_RIGHT 1
#define ADD_TIME_OPTION_BOTH 2
#define ADD_TIME_OPTION_BACK 3

const char addTimeOptionUiHandlerLeftPlayer[] PROGMEM  = "< Left          ";
const char addTimeOptionUiHandlerRigthPlayer[] PROGMEM = "         Right >";
const char addTimeOptionUiHandlerBothPlayer[] PROGMEM  = "    < Both >    ";
const char addTimeOptionUiHandlerBack[] PROGMEM        = "^ BACK ^";

const char *const addTimeOptionUiHandlerOptions[] PROGMEM = {
	addTimeOptionUiHandlerLeftPlayer,
	addTimeOptionUiHandlerRigthPlayer,
	addTimeOptionUiHandlerBothPlayer,
	addTimeOptionUiHandlerBack
};

class AddTimeOptionUiHandler : public UiHandler {
	UiHandler *previousHandler;
	TimeControlUi *timeControlUi;
	int16_t currentOption;

public:

	AddTimeOptionUiHandler() {
		timeControlUi = nullptr;
		currentOption = 0;
	}

	virtual ~AddTimeOptionUiHandler() {}

	void setup(TimeControlUi *timeControlUi) {
		currentOption       = 0;
		this->timeControlUi = timeControlUi;
	}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);

		if (buttonGestures.wasPushButtonLongPushed()) {
			currentUiHandler = startingHandler;
			return;
		}

		int  travel       = buttonGestures.getNavigationTravel();
		bool buttonPushed = buttonGestures.wasPushButtonPushed();

		if (buttonPushed) {
			if (isBackOption()) {
				currentUiHandler = previousHandler;
			} else {
				startAddingTime();
			}
			return;
		}

		if (travel != 0) {
			currentOption = (currentOption + travel) % ADD_TIME_OPTION_COUNT;

			if (currentOption < 0) {
				currentOption = ADD_TIME_OPTION_COUNT + currentOption;
			}
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);
		const char *message = PROGMEM_getAnything(&addTimeOptionUiHandlerOptions[currentOption]);

		if (isBackOption()) {
			lcd2.printBottomCenter(message);
		} else {
			lcd2.printTopCenter(message);
		}

		lcd2.endRender();
	}

	void wire(UiHandler *previousHandler) {
		this->previousHandler = previousHandler;
	}

private:

	void startAddingTime() {
		// customSetupUiHandler.setup(timeControlUi);
		// currentUiHandler = &customSetupUiHandler;
	}

	bool isBackOption() {
		return currentOption == ADD_TIME_OPTION_BACK;
	}
};

#endif // ifndef __AddTimeOptionUiHandler_h__
