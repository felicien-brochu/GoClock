
#ifndef __CustomSetupUiHandler_h__
#define __CustomSetupUiHandler_h__

#include "UiHandler.h"
#include "GameUiHandler.h"
#include "GameButtonGestures.h"

#include "TimeControlUi.h"

// #include "CustomValueUiHandler.h"

#define MAX_CUSTOM_VALUES 5

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;
extern GameClock    gameClock;

extern GameUiHandler gameUiHandler;
extern UiHandler    *currentUiHandler;

class CustomSetupUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;

	uint8_t valueIndex;
	CustomValue currentCustomValue;
	long currentValue;
	uint8_t subValueIndex;
	long values[MAX_CUSTOM_VALUES];

public:

	CustomSetupUiHandler() {}

	void init(TimeControlUi *timeControlUi) {
		this->timeControlUi = timeControlUi;
		valueIndex          = 0;
		nextCustomValueHandler();
	}

	virtual ~CustomSetupUiHandler() {}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);

		int  travel       = buttonGestures.getNavigationTravel();
		bool buttonPushed = buttonGestures.wasPushButtonPushed();

		if (travel != 0) {
			lcd2.resetBlinking(clock);
		}
		applyTravel(travel);

		if (buttonPushed) {
			if (subValueIndex + 1 >= getSubValueCount()) {
				onCustomValueConfirmed(clock);
			} else {
				subValueIndex++;
			}
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);

		lcd2.printTopCenter(currentCustomValue.label);

		switch (currentCustomValue.type) {
		case CUSTOM_VALUE_TIME:
			lcd2.printBottomCustomValueTime(currentValue, subValueIndex);
			break;

		case CUSTOM_VALUE_MIN_SEC:
			lcd2.printBottomCustomValueMinSec(currentValue, subValueIndex);
			break;

		case CUSTOM_VALUE_INT:
			lcd2.printBottomCustomValueInt(currentValue);
			break;
		}

		lcd2.endRender();
	}

	void onCustomValueConfirmed(Clock *clock) {
		values[valueIndex] = currentValue;
		valueIndex++;

		if (valueIndex >= this->timeControlUi->getCustomSetupLength()) {
			startGame(clock);
		} else {
			nextCustomValueHandler();
		}
	}

private:

	void startGame(Clock *clock) {
		gameClock.setup(clock, timeControlUi->createCustom(values));
		gameUiHandler.setTimeControlUi(timeControlUi);
		currentUiHandler = &gameUiHandler;
	}

	void nextCustomValueHandler() {
		currentCustomValue = this->timeControlUi->getCustomSetupValue(valueIndex);
		currentValue       = currentCustomValue.def;
		subValueIndex      = 0;
	}

	uint8_t getSubValueCount() {
		switch (currentCustomValue.type) {
		case CUSTOM_VALUE_TIME:
		case CUSTOM_VALUE_MIN_SEC:
			return 2;

		case CUSTOM_VALUE_INT:
			return 1;

		default:
			return 1;
		}
	}

	void applyTravel(int travel) {
		uint8_t valueType = currentCustomValue.type;

		if (((valueType == CUSTOM_VALUE_INT) && (subValueIndex == 0)) ||
		    ((valueType == CUSTOM_VALUE_MIN_SEC) && (subValueIndex == 1)) ||
		    ((valueType == CUSTOM_VALUE_TIME) && (subValueIndex == 1))) {
			currentValue += travel;
		} else if (((valueType == CUSTOM_VALUE_MIN_SEC) && (subValueIndex == 0)) ||
		           ((valueType == CUSTOM_VALUE_TIME) && (subValueIndex == 0))) {
			currentValue += travel * 60L;
		}

		if (currentValue > currentCustomValue.max) {
			currentValue = currentCustomValue.max;
		} else if (currentValue < currentCustomValue.min) {
			currentValue = currentCustomValue.min;
		}
	}
};

#endif // ifndef __CustomSetupUiHandler_h__
