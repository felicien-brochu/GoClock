
#ifndef __AddTimeUiHandler_h__
#define __AddTimeUiHandler_h__

#include "UiHandler.h"
#include "SelectTimeControlUiHandler.h"
#include "GameUiHandler.h"
#include "GameButtonGestures.h"

#include "TimeControlUi.h"

#define MAX_ADD_VALUES 5

extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;
extern GameClock   *gameClock;

extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

class AddTimeUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;
	UiHandler *previousHandler;

	uint8_t addTimeType;
	uint8_t valueIndex;
	CustomValue currentCustomValue;
	long currentValue;
	uint8_t subValueIndex;
	long values[MAX_ADD_VALUES];

public:

	AddTimeUiHandler() {}

	void setup(TimeControlUi *timeControlUi, uint8_t addTimeType) {
		this->timeControlUi = timeControlUi;
		this->addTimeType   = addTimeType;
		valueIndex          = 0;
		nextCustomValueHandler();
	}

	virtual ~AddTimeUiHandler() {}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);

		if (buttonGestures.wasPushButtonLongPushed()) {
			currentUiHandler = startingHandler;
			return;
		}

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

		if (valueIndex >= this->timeControlUi->getAddTimeLength()) {
			addTime(clock);
		} else {
			nextCustomValueHandler();
		}
	}

	void wire(UiHandler *previousHandler) {
		this->previousHandler = previousHandler;
	}

private:

	void addTime(Clock *clock) {
		TimeControl *timeControl = gameClock->getTimeControl();

		timeControl->addTime(values, addTimeType, clock);
		currentUiHandler = this->previousHandler;
	}

	void nextCustomValueHandler() {
		currentCustomValue = this->timeControlUi->getAddTimeValue(valueIndex);
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

#endif // ifndef __AddTimeUiHandler_h__
