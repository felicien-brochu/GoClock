
#ifndef __SelectTimeControlOptionUiHandler_h__
#define __SelectTimeControlOptionUiHandler_h__

#include "UiHandler.h"
#include "GameClock.h"
#include "GameClockLcd.h"
#include "GameUiHandler.h"
#include "CustomSetupUiHandler.h"
#include "GameButtonGestures.h"

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern GameClock *gameClock;

extern GameUiHandler gameUiHandler;
extern CustomSetupUiHandler customSetupUiHandler;
extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

const char selectTimeControlOptionUiHandlerBack[] PROGMEM = "^ BACK ^";
const char selectTimeControlOptionUiHandlerCustom[] PROGMEM = "- Custom time -";

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

		if (buttonGestures.wasPushButtonLongPushed()) {
			currentUiHandler = startingHandler;
		}

		int  travel       = buttonGestures.getNavigationTravel();
		bool buttonPushed = buttonGestures.wasPushButtonPushed();

		if (buttonPushed) {
			if (isBackOption()) {
				currentUiHandler = previousHandler;
			} else if (isCustomOption()) {
				startCustomSetup();
			} else {
				gameUiHandler.setup(timeControlUi->getTimeControlType(),
				                    timeControlUi->getDefaultOptionValues(currentOption),
				                    clock);
				currentUiHandler = &gameUiHandler;
			}
			beep();
			currentOption = 0;

			return;
		}

		if (travel != 0) {
			int optionCount = (timeControlUi->getNumberOfOptions() + 2);
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
		} else if (isCustomOption()) {
			lcd2.printTopCenter(selectTimeControlOptionUiHandlerCustom);
		} else {
			lcd2.printWholeScreen(timeControlUi->getOption(currentOption));
		}

		lcd2.endRender();
	}

	void setup(TimeControlUi *timeControlUi) {
		if (this->timeControlUi != timeControlUi) {
			currentOption = 0;
		}
		this->timeControlUi = timeControlUi;
	}

	void wire(UiHandler *previousHandler) {
		this->previousHandler = previousHandler;
	}

private:

	void startCustomSetup() {
		customSetupUiHandler.init(timeControlUi);
		currentUiHandler = &customSetupUiHandler;
	}

	bool isBackOption() {
		return currentOption == timeControlUi->getNumberOfOptions() + 1;
	}

	bool isCustomOption() {
		return currentOption == timeControlUi->getNumberOfOptions();
	}
};

#endif // ifndef __SelectTimeControlOptionUiHandler_h__
