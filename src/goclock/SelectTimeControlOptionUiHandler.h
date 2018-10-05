
#ifndef __SelectTimeControlOptionUiHandler_h__
#define __SelectTimeControlOptionUiHandler_h__

#include "UiHandler.h"
#include "GameClock.h"
#include "GameClockLcd.h"
#include "GameUiHandler.h"
#include "GameButtonGestures.h"

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern GameClock gameClock;

extern GameUiHandler gameUiHandler;
extern UiHandler    *currentUiHandler;

const char selectTimeControlOptionUiHandlerBack[] PROGMEM = "^ back ^";

class SelectTimeControlOptionUiHandler : public UiHandler {
	UiHandler *previousHandler;
	TimeControlUi *timeControlUi;
	int16_t currentOption;

public:

	SelectTimeControlOptionUiHandler() {
		timeControlUi = NULL;
		currentOption = 0;
	}

	virtual ~SelectTimeControlOptionUiHandler() {}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);

		int  travel       = buttonGestures.getNavigationTravel();
		bool buttonPushed = buttonGestures.wasPushButtonPushed();

		if (buttonPushed) {
			if (isBackOption()) {
				currentUiHandler = previousHandler;
			} else {
				gameClock.setup(clock, timeControlUi->create(currentOption));
				gameUiHandler.setTimeControlUi(timeControlUi);
				currentUiHandler = &gameUiHandler;
			}
			beep();

			return;
		}

		if (travel != 0) {
			int optionCount = (timeControlUi->getNumberOfOptions() + 1);
			currentOption = (currentOption + travel) % optionCount;

			if (currentOption < 0) {
				currentOption = optionCount + currentOption;
			}
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);

		if (isBackOption()) {
			lcd2.printBottomCenter(selectTimeControlOptionUiHandlerBack);
		} else {
			lcd2.printWholeScreen(timeControlUi->getOption(currentOption));
		}

		lcd2.endRender();
	}

	void setTimeControlUi(TimeControlUi *timeControlUi) {
		if (this->timeControlUi != timeControlUi) {
			currentOption = 0;
		}
		this->timeControlUi = timeControlUi;
	}

	void wire(UiHandler *previousHandler) {
		this->previousHandler = previousHandler;
	}

private:

	bool isBackOption() {
		return currentOption == timeControlUi->getNumberOfOptions();
	}
};

#endif // ifndef __SelectTimeControlOptionUiHandler_h__
